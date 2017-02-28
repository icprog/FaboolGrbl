//-----------------------------------------------------------------------------
#include "i2c.h"
#include "config.h"
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
#include "timers.h"
#endif
//-----------------------------------------------------------------------------
#define MCP4461_VW0 0x00
#define MCP4461_VW1 0x10
#define MCP4461_VW2 0x60
#define MCP4461_VW3 0x70
#define MCP4461_WRITE 0x0

#define SENSE_RESISTOR 0.1
#define MAX_STEPPER_DIGIPOT_VOLTAGE (3.3 * 2.5 / (2.7 + 2.5))
//-----------------------------------------------------------------------------
I2C_HandleTypeDef hi2c1;
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
I2C_HandleTypeDef hi2c3;
uint8_t save_intensity;
#endif
//-----------------------------------------------------------------------------
void i2c_init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    HAL_I2C_Init(&hi2c1);

#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
    hi2c3.Instance = I2C3;
    hi2c3.Init.ClockSpeed = 400000;
    hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c3.Init.OwnAddress1 = 0;
    hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2 = 0;
    hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c3);

    save_intensity = 1;
    SetAnalog(0);
#endif
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

    if (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0x2C << 1, (uint8_t *)data, 2, 1000) !=  HAL_OK)
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
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
uint8_t SetAnalog(uint8_t intensity)
{
    uint16_t iOutBit;
    uint8_t data[4];

    if (save_intensity == intensity) {
        return true;
    }
    save_intensity = intensity;

    iOutBit = (uint16_t)(4095.0 / LASER_TIMER_PERIOD * intensity);
    data[0] = 0x40;
    data[1] = (uint8_t)(iOutBit / 16);
    data[2] = (uint8_t)((iOutBit % 16) << 4);

    if (HAL_I2C_Master_Transmit(&hi2c3, (uint16_t)0x60 << 1, (uint8_t *)data, 3, 1000) !=  HAL_OK)
    {
        return false;
    }

    return true;
}
#endif
//-----------------------------------------------------------------------------
