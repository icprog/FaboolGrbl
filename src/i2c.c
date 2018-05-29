//-----------------------------------------------------------------------------
#include "i2c.h"
#include "config.h"
//-----------------------------------------------------------------------------
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define I2C_WRITE_X_AXIS    0
#define I2C_WRITE_Y1_AXIS   1
#define I2C_WRITE_Y2_AXIS   2
#endif
#define MCP4461_VW0 0x00
#define MCP4461_VW1 0x10
#define MCP4461_VW2 0x60
#define MCP4461_VW3 0x70
#define MCP4461_WRITE 0x0

#define SENSE_RESISTOR 0.1
#define MAX_STEPPER_DIGIPOT_VOLTAGE (3.3 * 2.5 / (2.7 + 2.5))

#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define POWER_THRESHOLD 11.0
#endif
//-----------------------------------------------------------------------------
I2C_HandleTypeDef hi2c1;
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
I2C_HandleTypeDef hi2c3;
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

#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    hi2c3.Instance = I2C3;
    hi2c3.Init.ClockSpeed = 100000;
    hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c3.Init.OwnAddress1 = 0;
    hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2 = 0;
    hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c3);
#endif
}
//-----------------------------------------------------------------------------
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
uint8_t SetCurrent(int8_t iI2cWriteAxis, float fCurrent)
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

    switch (iI2cWriteAxis)
    {
    case I2C_WRITE_X_AXIS:
        data[0] |= MCP4461_VW0;
        break;
    case I2C_WRITE_Y1_AXIS:
        data[0] |= MCP4461_VW1;
        break;
    case I2C_WRITE_Y2_AXIS:
        data[0] |= MCP4461_VW3;
        break;
    }

    data[0] |= MCP4461_WRITE;

    if (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0x2C << 1, (uint8_t *)data, 2, 100) !=  HAL_OK)
    {
        return false;
    }

    return true;
}
#else
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
#endif
//-----------------------------------------------------------------------------
void driver_current_enable(float fXCurrent, float fYCurrent)
{
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    SetCurrent(I2C_WRITE_X_AXIS, fXCurrent);
    SetCurrent(I2C_WRITE_Y1_AXIS, fYCurrent / 2.0);
    SetCurrent(I2C_WRITE_Y2_AXIS, fYCurrent / 2.0);
#else
    SetCurrent(X_AXIS, fXCurrent);
    SetCurrent(Y_AXIS, fYCurrent);
#endif
}
//-----------------------------------------------------------------------------
void driver_current_disable(void)
{
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    SetCurrent(I2C_WRITE_X_AXIS, 0);
    SetCurrent(I2C_WRITE_Y1_AXIS, 0);
    SetCurrent(I2C_WRITE_Y2_AXIS, 0);
#else
    SetCurrent(0, 0);
    SetCurrent(1, 0);
#endif
}
//-----------------------------------------------------------------------------
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
uint8_t judg_power(void)
{
    uint8_t tx_buf[3] = { 0x00 };
    uint8_t rx_buf[3] = { 0x00 };


    tx_buf[0] = 0x90;
    if (HAL_I2C_Master_Transmit(&hi2c3, (uint16_t)0x68 << 1, (uint8_t *)tx_buf, 1, 10) !=  HAL_OK)
    {
        return 1;
    }

    _delay_ms(10);

    if (HAL_I2C_Master_Receive(&hi2c3, (uint16_t)0x68 << 1, rx_buf, 2, 10) !=  HAL_OK)
    {
        return 1;
    }

    uint16_t ud = ((uint16_t)rx_buf[0] << 8) | (uint16_t)rx_buf[1];
    int16_t sd = (int16_t)ud;

    if (ud & 0x0800)  {
        sd = (int16_t)(ud | 0xf000);
    }
    else {
        sd = (int16_t)(ud & 0x0fff);
    }

    double dVolt = (double)sd / 1000.0 / 0.39 * 2.79;


    if (POWER_THRESHOLD > dVolt) {
        return 1;
    }

    return 0;
}
#endif
//-----------------------------------------------------------------------------
