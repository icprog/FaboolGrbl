//-----------------------------------------------------------------------------
#include "usb.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "config.h"
//-----------------------------------------------------------------------------
USBD_HandleTypeDef hUsbDeviceFS;
//-----------------------------------------------------------------------------
void usb_init(void)
{
	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);

	while (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) ;

}
//-----------------------------------------------------------------------------
