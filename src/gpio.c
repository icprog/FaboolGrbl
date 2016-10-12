//-----------------------------------------------------------------------------
#include "stm32f2xx_hal.h"
#include "gpio.h"
//-----------------------------------------------------------------------------

typedef struct gpio_info {
    uint32_t num;       // gpio number - as defined in gpio.h
    uint32_t mode;      // input, output, etc.
    uint32_t pull;      // pull up/down, etc.
    uint32_t speed;     // slew rate
    uint32_t alt;       // alternate pin functions
    int init;           // initial pin value

} GPIO_INFO;

//-----------------------------------------------------------------------------
static const GPIO_INFO gpio_info[] = {
    {STEP_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {STEP_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {EN_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {EN_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {LIMIT_X1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
#if SMART_LASER_CO2 == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2
    {LIMIT_X2, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y2, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
#endif
    {LIMIT_D, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LASER_OUT, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, GPIO_AF2_TIM3, -1},
#if SMART_LASER_CO2 == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2
    {WATER_FLOW, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0, -1},
#endif
};
//-----------------------------------------------------------------------------
void gpio_init(void)
{
    int i;

    for (i = 0; i < sizeof(gpio_info)/sizeof(GPIO_INFO); i ++) {
        const GPIO_INFO *gpio = &gpio_info[i];
        GPIO_InitTypeDef GPIO_InitStruct;
        // enable the peripheral clock: __GPIOx_CLK_ENABLE()
        RCC->AHB1ENR |= (1 << GPIO_PORT(gpio->num));
        // setup the gpio port/pin
        GPIO_InitStruct.Pin = GPIO_BIT(gpio->num);
        GPIO_InitStruct.Mode = gpio->mode;
        GPIO_InitStruct.Pull = gpio->pull;
        GPIO_InitStruct.Speed = gpio->speed;
        GPIO_InitStruct.Alternate = gpio->alt;
        HAL_GPIO_Init(GPIO_BASE(gpio->num), &GPIO_InitStruct);
        // set any initial value
        if (gpio->init >= 0) {
            HAL_GPIO_WritePin(GPIO_BASE(gpio->num), GPIO_BIT(gpio->num), gpio->init);
        }
    }
}

//-----------------------------------------------------------------------------
