//-----------------------------------------------------------------------------
#ifndef I2C_H
#define I2C_H
//-----------------------------------------------------------------------------
#include "config.h"
#include "stm32f2xx_hal.h"
//-----------------------------------------------------------------------------
extern I2C_HandleTypeDef hi2c1;
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
extern I2C_HandleTypeDef hi2c3;
#endif
//-----------------------------------------------------------------------------
void i2c_init(void);
uint8_t SetCurrent(int8_t iAxis, float fCurrent);
void driver_current_enable(float fXCurrent, float fYCurrent);
void driver_current_disable(void);
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
uint8_t SetAnalog(uint8_t intensity);
#endif
//-----------------------------------------------------------------------------
#endif // I2C_H
//-----------------------------------------------------------------------------
