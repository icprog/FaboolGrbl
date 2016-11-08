//-----------------------------------------------------------------------------
#ifndef GPIO_H
#define GPIO_H
//-----------------------------------------------------------------------------
#include "config.h"
#include "stm32f2xx_hal.h"
#include <stdbool.h>

//-----------------------------------------------------------------------------
// port numbers
#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4
#define PORTF 5
#define PORTG 6
#define PORTH 7
#define PORTI 8
//-----------------------------------------------------------------------------
// gpio macros
#define GPIO_NUM(port, pin) ((port << 4) | (pin))
#define GPIO_PORT(n) (n >> 4)
#define GPIO_PIN(n) (n & 0xf)
#define GPIO_BIT(n) (1 << GPIO_PIN(n))
#define GPIO_BASE(n) ((GPIO_TypeDef  *)(GPIOA_BASE + (GPIO_PORT(n) * 0x400)))
//-----------------------------------------------------------------------------
// gpio assignments

#define STEP_X          GPIO_NUM(PORTC, 0)
#define STEP_Y          GPIO_NUM(PORTC, 4)

#define DIR_X          GPIO_NUM(PORTC, 1)
#define DIR_Y          GPIO_NUM(PORTC, 5)

#define EN_X            GPIO_NUM(PORTC, 2)
#define EN_Y            GPIO_NUM(PORTC, 6)

#define LIMIT_X1         GPIO_NUM(PORTA, 1)
#define LIMIT_Y1         GPIO_NUM(PORTA, 4)

#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2
#define LIMIT_X2         GPIO_NUM(PORTA, 2)
#define LIMIT_Y2         GPIO_NUM(PORTA, 5)
#endif

#define LIMIT_D         GPIO_NUM(PORTA, 6)

#define LASER_OUT       GPIO_NUM(PORTC, 8)

#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2
#define WATER_FLOW      GPIO_NUM(PORTB, 12)
#endif
//-----------------------------------------------------------------------------
void gpio_init(void);
//-----------------------------------------------------------------------------
#define STEP_MASK (GPIO_BIT(STEP_X) | GPIO_BIT(STEP_Y))
static inline void step_wr(uint16_t x)
{
    uint32_t val = GPIO_BASE(STEP_X)->ODR;
    val &= ~STEP_MASK;
    GPIO_BASE(STEP_X)->ODR = (val | x);
}
//-----------------------------------------------------------------------------
#define DIR_MASK (GPIO_BIT(DIR_X) | GPIO_BIT(DIR_Y))
static inline void dirn_wr(uint16_t x)
{
    uint32_t val = GPIO_BASE(DIR_X)->ODR;
    val &= ~DIR_MASK;
    GPIO_BASE(DIR_X)->ODR = (val | x);
}
//-----------------------------------------------------------------------------
static inline void stepper_motor_enable(void)
{
    HAL_GPIO_WritePin(GPIO_BASE(EN_X), GPIO_BIT(EN_X), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y), GPIO_BIT(EN_Y), GPIO_PIN_RESET);
}
static inline void stepper_motor_disable(void)
{
    HAL_GPIO_WritePin(GPIO_BASE(EN_X), GPIO_BIT(EN_X), GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y), GPIO_BIT(EN_Y), GPIO_PIN_SET);
}
//-----------------------------------------------------------------------------
// The input gpios are spread out across several ports. We read and pack them into a
// single uint32_t and debounce them together.

#define X1_LIMIT_BIT 1
#define Y1_LIMIT_BIT 0
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2
    #define X2_LIMIT_BIT 3
    #define Y2_LIMIT_BIT 2
#endif

#if GRBL_MODEL == SMART_LASER_MINI || GRBL_MODEL == FABOOL_LASER_MINI
    #define LIMIT_MASK ((1 << X1_LIMIT_BIT) | (1 << Y1_LIMIT_BIT))
#else
    #define LIMIT_MASK ((1 << X1_LIMIT_BIT) | (1 << Y1_LIMIT_BIT) | (1 << X2_LIMIT_BIT) | (1 << Y2_LIMIT_BIT))
#endif

static inline uint16_t limit_input(void)
{
#if GRBL_MODEL == SMART_LASER_MINI || GRBL_MODEL == FABOOL_LASER_MINI
    return (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X1), GPIO_BIT(LIMIT_X1)) == GPIO_PIN_RESET) << X1_LIMIT_BIT |
            (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y1), GPIO_BIT(LIMIT_Y1)) == GPIO_PIN_RESET) << Y1_LIMIT_BIT;
#else
    return (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X1), GPIO_BIT(LIMIT_X1)) == GPIO_PIN_RESET) << X1_LIMIT_BIT |
           (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y1), GPIO_BIT(LIMIT_Y1)) == GPIO_PIN_RESET) << Y1_LIMIT_BIT |
           (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X2), GPIO_BIT(LIMIT_X2)) == GPIO_PIN_RESET) << X2_LIMIT_BIT |
           (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y2), GPIO_BIT(LIMIT_Y2)) == GPIO_PIN_RESET) << Y2_LIMIT_BIT;
#endif
}
//-----------------------------------------------------------------------------
static inline uint16_t door_input(void)
{
    return (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_D), GPIO_BIT(LIMIT_D)) == GPIO_PIN_RESET);
}

//-----------------------------------------------------------------------------
static inline void control_air_assist(bool enable) {
}

static inline void control_aux1_assist(bool enable) {
}
//-----------------------------------------------------------------------------

#endif // GPIO_H

//-----------------------------------------------------------------------------
