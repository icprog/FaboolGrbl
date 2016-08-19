//-----------------------------------------------------------------------------
#ifndef I2C_H
#define I2C_H
//-----------------------------------------------------------------------------
#include "stm32f2xx_hal.h"
//-----------------------------------------------------------------------------
extern I2C_HandleTypeDef hi2c;
//-----------------------------------------------------------------------------
void i2c_init(void);
uint8_t SetCurrent(int8_t iAxis, float fCurrent);
void driver_current_enable(float fXCurrent, float fYCurrent);
void driver_current_disable(void);
//-----------------------------------------------------------------------------
#endif // I2C_H
//-----------------------------------------------------------------------------