//-----------------------------------------------------------------------------
#ifndef I2C_H
#define I2C_H
//-----------------------------------------------------------------------------
#include "config.h"
#include "stm32f2xx_hal.h"
//-----------------------------------------------------------------------------
extern I2C_HandleTypeDef hi2c1;
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
extern I2C_HandleTypeDef hi2c3;
#endif
//-----------------------------------------------------------------------------
void i2c_init(void);
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
uint8_t SetCurrent(int8_t iI2cWriteAxis, float fCurrent);
#else
uint8_t SetCurrent(int8_t iAxis, float fCurrent);
#endif
void driver_current_enable(float fXCurrent, float fYCurrent);
void driver_current_disable(void);
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
uint8_t judg_power(void);
#endif
//-----------------------------------------------------------------------------
#endif // I2C_H
//-----------------------------------------------------------------------------
