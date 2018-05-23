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
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    {STEP_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {STEP_Y1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {STEP_Y2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_Y1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_Y2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {EN_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_RESET},
    {EN_Y1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_RESET},
    {EN_Y2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_RESET},
    {LIMIT_X1, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y1, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_X2, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y2, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_D, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, 0, 0, -1},
    {LASER_OUT, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, GPIO_AF2_TIM3, -1},
    {WATER_FLOW, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0, -1},
#elif GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2
    {STEP_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {STEP_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {EN_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {EN_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {LIMIT_X1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_X2, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y2, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_D, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LASER_OUT, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, GPIO_AF2_TIM3, -1},
    {WATER_FLOW, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0, -1},
#elif GRBL_MODEL == FABOOL_LASER_DS
    {STEP_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {STEP_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {EN_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {EN_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {LIMIT_X1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_X2, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y2, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_D, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LASER_OUT, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, GPIO_AF2_TIM3, -1},
    {WATER_FLOW, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
#else
    {STEP_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {STEP_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {DIR_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, 0, GPIO_PIN_RESET},
    {EN_X, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {EN_Y, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0, GPIO_PIN_SET},
    {LIMIT_X1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_Y1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LIMIT_D, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0, -1},
    {LASER_OUT, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_HIGH, GPIO_AF2_TIM3, -1},
#endif
};
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
//-----------------------------------------------------------------------------
GPIO_PinState   gpsLimitX1;
GPIO_PinState   gpsLimitX2;
GPIO_PinState   gpsLimitY1;
GPIO_PinState   gpsLimitY2;
GPIO_PinState   gpsLimitD;
#endif
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
#if GRBL_MODEL == FABOOL_LASER_CO2_DS
    // 割り込み設定
    HAL_NVIC_SetPriority(EXTI1_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    gpsLimitX1 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X1), GPIO_BIT(LIMIT_X1));
    gpsLimitX2 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X2), GPIO_BIT(LIMIT_X2));
    gpsLimitY1 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y1), GPIO_BIT(LIMIT_Y1));
    gpsLimitY2 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y2), GPIO_BIT(LIMIT_Y2));
    gpsLimitD = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_D), GPIO_BIT(LIMIT_D));
#endif
}

#if GRBL_MODEL == FABOOL_LASER_CO2_DS
//-----------------------------------------------------------------------------
void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_BIT(LIMIT_X1));
}
//-----------------------------------------------------------------------------
void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_BIT(LIMIT_X2));
}
//-----------------------------------------------------------------------------
void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_BIT(LIMIT_Y1));
}
//-----------------------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_BIT(LIMIT_Y2));
    HAL_GPIO_EXTI_IRQHandler(GPIO_BIT(LIMIT_D));
}
//-----------------------------------------------------------------------------
// 割り込みコールバック
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_BIT(LIMIT_X1)) {
        gpsLimitX1 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X1), GPIO_BIT(LIMIT_X1));
    }
    else if (GPIO_Pin == GPIO_BIT(LIMIT_X2)) {
        gpsLimitX2 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_X2), GPIO_BIT(LIMIT_X2));
    }
    else if (GPIO_Pin == GPIO_BIT(LIMIT_Y1)) {
        gpsLimitY1 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y1), GPIO_BIT(LIMIT_Y1));
    }
    else if (GPIO_Pin == GPIO_BIT(LIMIT_Y2)) {
        gpsLimitY2 = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_Y2), GPIO_BIT(LIMIT_Y2));
    }
    else if (GPIO_Pin == GPIO_BIT(LIMIT_D)) {
        gpsLimitD = HAL_GPIO_ReadPin(GPIO_BASE(LIMIT_D), GPIO_BIT(LIMIT_D));
    }
    else {
        return;
    }

    // スイッチがONになった場合、レーザーの出力をOFFにする
    control_laser_intensity(0, 1);
}
#endif
//-----------------------------------------------------------------------------
