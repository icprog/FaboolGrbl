//-----------------------------------------------------------------------------
#include "usbd_cdc_if.h"

#include "stm32f2xx_hal.h"
#include "usbd_desc.h"
#include "timers.h"
#include "stepper.h"
#include "gcode.h"
#include "config.h"
//-----------------------------------------------------------------------------
extern USBD_HandleTypeDef hUsbDeviceFS;
//-----------------------------------------------------------------------------
#define TX_FIFO_SIZE 2048
static uint8_t tx_fifo[TX_FIFO_SIZE];
static uint32_t tx_wr;
static uint32_t tx_rd;

#define RX_FIFO_SIZE 512
static uint8_t rx_fifo[RX_FIFO_SIZE];
static uint32_t rx_wr;
static uint32_t rx_rd;

static uint8_t rx_buffer[CDC_DATA_FS_OUT_PACKET_SIZE];

/** protocol *************************************
* The sending app initiates any stream by        *
* requesting a ready byte. This serial code then *
* sends one as soon as there are RX_CHUNK_SIZE   *
* slots available in the rx buffer. The sending  *
* app can then send this amount of bytes.        *
* Thereafter it can again request a ready byte   *
* and apon receiving it send the next chunk.     *
*************************************************/
#define CHAR_READY '\x12'
#define CHAR_REQUEST_READY '\x14'
#define RX_CHUNK_SIZE 64
static uint8_t send_ready_flag;
static uint8_t request_ready_flag;
static uint32_t rx_buffer_open_slots;
static uint8_t tx_data[1];

static bool bConnected;
static bool bFirstConnected;
//-----------------------------------------------------------------------------
static USBD_CDC_LineCodingTypeDef LineCoding = {
    115200, // baud rate
    0x00,   // 1 stop bit
    0x00,   // no parity
    0x08    // 8 data bits
};
//-----------------------------------------------------------------------------
static int8_t CDC_Init_FS(void)
{
    tx_wr = 0;
    tx_rd = 0;
    rx_wr = 0;
    rx_rd = 0;

	send_ready_flag = 0;
	request_ready_flag = 0;
	rx_buffer_open_slots = RX_FIFO_SIZE - 1;

    bConnected = false;
    bFirstConnected = true;

    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, tx_fifo, 0);
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, rx_buffer);

    return USBD_OK;
}
//-----------------------------------------------------------------------------
static int8_t CDC_DeInit_FS(void)
{
    return USBD_OK;
}
//-----------------------------------------------------------------------------
static int8_t CDC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
	switch (cmd) {
	case CDC_SET_LINE_CODING:
		LineCoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
		LineCoding.format = pbuf[4];
		LineCoding.paritytype = pbuf[5];
		LineCoding.datatype = pbuf[6];

		bConnected = true;
		break;

	case CDC_GET_LINE_CODING:
		pbuf[0] = (uint8_t)(LineCoding.bitrate);
		pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
		pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
		pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
		pbuf[4] = LineCoding.format;
		pbuf[5] = LineCoding.paritytype;
		pbuf[6] = LineCoding.datatype;
		break;

	case CDC_SET_CONTROL_LINE_STATE:
		bConnected = false;
		break;

	case CDC_SEND_ENCAPSULATED_COMMAND:
	case CDC_GET_ENCAPSULATED_RESPONSE:
	case CDC_SET_COMM_FEATURE:
	case CDC_GET_COMM_FEATURE:
	case CDC_CLEAR_COMM_FEATURE:
	case CDC_SEND_BREAK:
		break;
	default:
		break;
	}

	return USBD_OK;
}
//-----------------------------------------------------------------------------
// read a character from the rx fifo ring buffer
uint8_t serial_read(void)
{
    uint8_t data;
    if (rx_wr != rx_rd) {
        data = rx_fifo[rx_rd];
        rx_rd = (rx_rd == (RX_FIFO_SIZE - 1)) ? 0 : rx_rd + 1;
    }
    else {
    	return SERIAL_NO_DATA;
	}

    __disable_irq();

    if (rx_buffer_open_slots == RX_CHUNK_SIZE) {  // enough slots opening up
      if (request_ready_flag) {
        send_ready_flag = 1;
        cdc_timer_start();
        request_ready_flag = 0;
      }
    }
    rx_buffer_open_slots++;

    __enable_irq();

    return data;
}
//-----------------------------------------------------------------------------
int serial_rx_hook(uint8_t data)
{
    int rc = 1;

    // Pick off runtime command characters directly from the serial stream. These characters are
    // not passed into the buffer, but these set system state flag bits for runtime execution.
    switch (data) {
        case CMD_FEED_HOLD: {
            stepper_request_stop(STATUS_SERIAL_STOP_REQUEST);
            break;
        }
        case CMD_CYCLE_START: {
            stepper_stop_resume();
            break;
        }
        case CHAR_REQUEST_READY: {
		    if (rx_buffer_open_slots > RX_CHUNK_SIZE) {
		      send_ready_flag = 1;
              cdc_timer_start();
		    } else {
		      // send ready when enough slots open up
		      request_ready_flag = 1;
		    }
            break;
        }
        default: {
            rc = 0;
            break;
        }
    }

    return rc;
}
//-----------------------------------------------------------------------------
static int8_t CDC_Receive_FS (uint8_t* pbuf, uint32_t *Len)
{
    uint32_t n = *Len;
    uint32_t i;

    // Write the received buffer to the Rx fifo.
    for (i = 0; i < n; i ++) {
        if (serial_rx_hook(pbuf[i])) {
            // the character has been absorbed by the rx hook
            continue;
        }
        uint32_t rx_wr_inc = (rx_wr == (RX_FIFO_SIZE - 1)) ? 0 : rx_wr + 1;
        if (rx_wr_inc != rx_rd) {
            rx_fifo[rx_wr] = pbuf[i];
            rx_wr = rx_wr_inc;
            rx_buffer_open_slots--;
        } else {
            stepper_request_stop(STATUS_RX_BUFFER_OVERFLOW);
        }
    }

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, rx_buffer);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return USBD_OK;
}
//-----------------------------------------------------------------------------
USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};
//-----------------------------------------------------------------------------
// write a character to the tx fifo ring buffer
void serial_write(uint8_t data)
{
    uint32_t tx_wr_inc = (tx_wr == (TX_FIFO_SIZE - 1)) ? 0 : tx_wr + 1;

    while (tx_wr_inc == tx_rd) {
    }

    tx_fifo[tx_wr] = data;
    tx_wr = tx_wr_inc;

    cdc_timer_start();

}
//-----------------------------------------------------------------------------
void cdc_timer_isr(void)
{
    uint32_t n;

    if (((USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData)->TxState) {
		if (ui_cdc_timer_count++ < CDC_TIMER_MAX) {
	    	return;
		}
		else {
			((USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData)->TxState = 0;
		}
    }
    cdc_timer_stop();
    if (send_ready_flag) {    // request another chunk of data
      tx_data[0] = CHAR_READY;
      USBD_CDC_SetTxBuffer(&hUsbDeviceFS, &tx_data[0], 1);
      USBD_CDC_TransmitPacket(&hUsbDeviceFS);
      send_ready_flag = 0;
    }
    else if(tx_wr != tx_rd) {
        if(tx_wr < tx_rd) {
            // The write index has wrapped around.
            // Tx from tx_rd to the end of fifo.
            n = TX_FIFO_SIZE - tx_rd;
        } else {
            // The write index is ahead of the read index
            // Tx from tx_rd to tx_wr - 1
            n = tx_wr - tx_rd;
        }

        USBD_CDC_SetTxBuffer(&hUsbDeviceFS, &tx_fifo[tx_rd], n);
        tx_rd += n;
        if (tx_rd == TX_FIFO_SIZE) {
            tx_rd = 0;
        }
    	USBD_CDC_TransmitPacket(&hUsbDeviceFS);
    }
}
//-----------------------------------------------------------------------------
// Reset and empty data in read buffer. Used by e-stop and reset.
void serial_reset_read_buffer(void)
{
    rx_wr = 0;
    rx_rd = 0;
}
//-----------------------------------------------------------------------------
void printString(const char *s)
{
	while (*s)
	{
		serial_write(*s++);
	}
}
//-----------------------------------------------------------------------------
void printPgmString(const char *s)
{
	while (*s)
	{
		serial_write(*s++);
	}
}
//-----------------------------------------------------------------------------
static void print_uint32_base10(unsigned long n)
{
  unsigned char buf[10];
  uint8_t i = 0;

  if (n == 0) {
    serial_write('0');
    return;
  }

  while (n > 0) {
    buf[i++] = n % 10 + '0';
    n /= 10;
  }

  for (; i > 0; i--)
    serial_write(buf[i-1]);
}
//-----------------------------------------------------------------------------
void printInteger(long n)
{
  if (n < 0) {
    serial_write('-');
    n = -n;
  }
  print_uint32_base10(n);
}
//-----------------------------------------------------------------------------
#define DEFAULT_DECIMAL_PLACES 3
void printFloat(float n)
{
  if (n < 0) {
    serial_write('-');
    n = -n;
  }

  uint8_t decimals = DEFAULT_DECIMAL_PLACES;
  while (decimals >= 2) { // Quickly convert values expected to be E0 to E-4.
    n *= 100;
    decimals -= 2;
  }
  if (decimals) { n *= 10; }
  n += 0.5; // Add rounding factor. Ensures carryover through entire value.

  // Generate digits backwards and store in string.
  unsigned char buf[10];
  uint8_t i = 0;
  uint32_t a = (long)n;
  buf[DEFAULT_DECIMAL_PLACES] = '.'; // Place decimal point, even if decimal places are zero.
  while(a > 0) {
    if (i == DEFAULT_DECIMAL_PLACES) { i++; } // Skip decimal point location
    buf[i++] = (a % 10) + '0'; // Get digit
    a /= 10;
  }
  while (i < DEFAULT_DECIMAL_PLACES) {
     buf[i++] = '0'; // Fill in zeros to decimal point for (n < 1)
  }
  if (i == DEFAULT_DECIMAL_PLACES) { // Fill in leading zero, if needed.
    i++;
    buf[i++] = '0';
  }

  // Print the generated string.
  for (; i > 0; i--)
    serial_write(buf[i-1]);
}
//-----------------------------------------------------------------------------
void printFirstConnectedMsg()
{
	if (bConnected == false)
	{
		return;
	}
	bConnected = false;
	if (bFirstConnected)
	{
		bFirstConnected = false;
		return;

	}
	_delay_ms(20);

	printPgmString(PSTR("# " GRBL_STR " " GRBL_VERSION));
	printPgmString(PSTR("\n"));
}
//-----------------------------------------------------------------------------
