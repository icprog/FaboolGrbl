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
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define STEP_Y1         GPIO_NUM(PORTC, 4)
#define STEP_Y2         GPIO_NUM(PORTC, 10)
#else
#define STEP_Y          GPIO_NUM(PORTC, 4)
#endif

#define DIR_X          GPIO_NUM(PORTC, 1)
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define DIR_Y1         GPIO_NUM(PORTC, 5)
#define DIR_Y2         GPIO_NUM(PORTC, 11)
#else
#define DIR_Y          GPIO_NUM(PORTC, 5)
#endif

#define EN_X            GPIO_NUM(PORTC, 2)
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define EN_Y1           GPIO_NUM(PORTC, 6)
#define EN_Y2           GPIO_NUM(PORTC, 12)
#else
#define EN_Y            GPIO_NUM(PORTC, 6)
#endif

#define LIMIT_X1         GPIO_NUM(PORTA, 1)
#define LIMIT_Y1         GPIO_NUM(PORTA, 4)

#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS || GRBL_MODEL == FABOOL_LASER_CO2_DS
#define LIMIT_X2         GPIO_NUM(PORTA, 2)
#define LIMIT_Y2         GPIO_NUM(PORTA, 5)
#endif

#define LIMIT_D         GPIO_NUM(PORTA, 6)

#define LASER_OUT       GPIO_NUM(PORTC, 8)

#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS || GRBL_MODEL == FABOOL_LASER_CO2_DS
#define WATER_FLOW      GPIO_NUM(PORTB, 12)
#endif
//-----------------------------------------------------------------------------
void gpio_init(void);
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
//-----------------------------------------------------------------------------
extern GPIO_PinState    gpsLimitX1;
extern GPIO_PinState    gpsLimitX2;
extern GPIO_PinState    gpsLimitY1;
extern GPIO_PinState    gpsLimitY2;
extern GPIO_PinState    gpsLimitD;
#endif
//-----------------------------------------------------------------------------
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define STEP_MASK (GPIO_BIT(STEP_X) | GPIO_BIT(STEP_Y1) | GPIO_BIT(STEP_Y2))
#else
#define STEP_MASK (GPIO_BIT(STEP_X) | GPIO_BIT(STEP_Y))
#endif
static inline void step_wr(uint16_t x)
{
    uint32_t val = GPIO_BASE(STEP_X)->ODR;
    val &= ~STEP_MASK;
    GPIO_BASE(STEP_X)->ODR = (val | x);
}
//-----------------------------------------------------------------------------
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
#define DIR_MASK (GPIO_BIT(DIR_X) | GPIO_BIT(DIR_Y1) | GPIO_BIT(DIR_Y2))
#else
#define DIR_MASK (GPIO_BIT(DIR_X) | GPIO_BIT(DIR_Y))
#endif
static inline void dirn_wr(uint16_t x)
{
    uint32_t val = GPIO_BASE(DIR_X)->ODR;
    val &= ~DIR_MASK;
    GPIO_BASE(DIR_X)->ODR = (val | x);
}
//-----------------------------------------------------------------------------
static inline void stepper_motor_enable(void)
{
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    HAL_GPIO_WritePin(GPIO_BASE(EN_X), GPIO_BIT(EN_X), GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y1), GPIO_BIT(EN_Y1), GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y2), GPIO_BIT(EN_Y2), GPIO_PIN_SET);
#else
    HAL_GPIO_WritePin(GPIO_BASE(EN_X), GPIO_BIT(EN_X), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y), GPIO_BIT(EN_Y), GPIO_PIN_RESET);
#endif
}
static inline void stepper_motor_disable(void)
{
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    HAL_GPIO_WritePin(GPIO_BASE(EN_X), GPIO_BIT(EN_X), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y1), GPIO_BIT(EN_Y1), GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y2), GPIO_BIT(EN_Y2), GPIO_PIN_RESET);
#else
    HAL_GPIO_WritePin(GPIO_BASE(EN_X), GPIO_BIT(EN_X), GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_BASE(EN_Y), GPIO_BIT(EN_Y), GPIO_PIN_SET);
#endif
}
//-----------------------------------------------------------------------------
// The input gpios are spread out across several ports. We read and pack them into a
// single uint32_t and debounce them together.

#define X1_LIMIT_BIT 1
#define Y1_LIMIT_BIT 0
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS || GRBL_MODEL == FABOOL_LASER_CO2_DS
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

#elif GRBL_MODEL == FABOOL_LASER_CO2_DS
    return (gpsLimitX1 == GPIO_PIN_SET) << X1_LIMIT_BIT |
           (gpsLimitY1 == GPIO_PIN_SET) << Y1_LIMIT_BIT |
           (gpsLimitX2 == GPIO_PIN_SET) << X2_LIMIT_BIT |
           (gpsLimitY2 == GPIO_PIN_SET) << Y2_LIMIT_BIT;
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
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    return (gpsLimitD == GPIO_PIN_SET);
#else
    return (HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_D), GPIO_BIT(LIMIT_D)) == GPIO_PIN_RESET);
#endif
}

#if GRBL_MODEL == FABOOL_LASER_CO2_DS
//-----------------------------------------------------------------------------
static inline uint8_t switch_judge(void)
{
    if (gpsLimitX1 == GPIO_PIN_SET || gpsLimitY1 == GPIO_PIN_SET ||
        gpsLimitX2 == GPIO_PIN_SET || gpsLimitY2 == GPIO_PIN_SET ||
        gpsLimitD == GPIO_PIN_SET) {

        return 1;
    }

    return 0;
}
#endif
//-----------------------------------------------------------------------------
static inline void control_air_assist(bool enable) {
}

static inline void control_aux1_assist(bool enable) {
}
//-----------------------------------------------------------------------------

#endif // GPIO_H

//-----------------------------------------------------------------------------
