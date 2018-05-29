//-----------------------------------------------------------------------------
#include "adc.h"
#include "config.h"
//-----------------------------------------------------------------------------
#define POWER_THRESHOLD 11.0
//-----------------------------------------------------------------------------
ADC_HandleTypeDef hadc1;
//-----------------------------------------------------------------------------
void adc_init(void)
{
    ADC_ChannelConfTypeDef sConfig;

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
#if GRBL_MODEL == SMART_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_CO2 || GRBL_MODEL == FABOOL_LASER_DS
//-----------------------------------------------------------------------------
uint8_t judg_power(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    double dVolt = HAL_ADC_GetValue(&hadc1) * 3.3 / 4096 / 0.39 * 1.89;
    HAL_ADC_Stop(&hadc1);

    if (POWER_THRESHOLD > dVolt) {
        return 1;
    }

    return 0;
}
#endif
//-----------------------------------------------------------------------------
