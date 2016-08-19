//-----------------------------------------------------------------------------
#include "i2c.h"
#include "config.h"
//-----------------------------------------------------------------------------
#define MCP4461_VW0 0x00
#define MCP4461_VW1 0x10
#define MCP4461_VW2 0x60
#define MCP4461_VW3 0x70
#define MCP4461_WRITE 0x0

#define SENSE_RESISTOR 0.1
#define MAX_STEPPER_DIGIPOT_VOLTAGE (3.3 * 2.5 / (2.7 + 2.5))
//-----------------------------------------------------------------------------
I2C_HandleTypeDef hi2c;
//-----------------------------------------------------------------------------
void i2c_init(void)
{
	hi2c.Instance = I2C1;
	hi2c.Init.ClockSpeed = 100000;
	hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c.Init.OwnAddress1 = 0;
	hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	hi2c.Init.OwnAddress2 = 0;
	hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	HAL_I2C_Init(&hi2c);
}
//-----------------------------------------------------------------------------
uint8_t SetCurrent(int8_t iAxis, float fCurrent)
{
	uint16_t pot = (uint16_t)((0.256 * fCurrent * 8.0 * SENSE_RESISTOR + MAX_STEPPER_DIGIPOT_VOLTAGE / 2) / MAX_STEPPER_DIGIPOT_VOLTAGE);

	uint8_t data[3];

	if (pot > 0xFF)
	{
		data[0] = 0x01;
		data[1] = (uint8_t)0x100;
	}
	else
	{
		data[0] = 0x00;
		data[1] = (uint8_t)pot;
	}

	switch (iAxis)
	{
	case X_AXIS:
		data[0] |= MCP4461_VW1;
		break;
	case Y_AXIS:
		data[0] |= MCP4461_VW3;
		break;
	}

	data[0] |= MCP4461_WRITE;

	if (HAL_I2C_Master_Transmit(&hi2c, (uint16_t)0x2C << 1, (uint8_t *)data, 2, 1000) !=  HAL_OK)
	{
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
void driver_current_enable(float fXCurrent, float fYCurrent)
{
	SetCurrent(X_AXIS, fXCurrent);
	SetCurrent(Y_AXIS, fYCurrent);
}
//-----------------------------------------------------------------------------
void driver_current_disable(void)
{
	SetCurrent(0, 0);
	SetCurrent(1, 0);
}
//-----------------------------------------------------------------------------