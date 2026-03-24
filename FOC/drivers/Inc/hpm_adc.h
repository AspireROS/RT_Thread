//
// Created by 21184 on 2026/3/17.
//

#ifndef RTTHREAD_HPM_ADC_H
#define RTTHREAD_HPM_ADC_H

#include <rtthread.h>
#include "board.h"
#include "hpm_adc16_drv.h"
#include "hpm_soc.h"

/**
 * @brief Initialize the ADC pins.
 */
void hpm_adc_gpio_init(void);

/**
 * @brief Configure PWM to trigger ADC sampling for FOC project.
 * @param adc_channel The ADC channel to be sampled.
 */
void foc_pwm_trigger_adc_init(uint8_t adc_channel);

/**
 * @brief Initialize ADC0 in Preemption Mode (PMT) triggered by PWM.
 * @param ch1 First channel to sample.
 * @param ch2 Second channel to sample.
 */
void hpm_adc_pmt_init(uint8_t ch1, uint8_t ch2);

extern adc16_pmt_dma_data_t pmt_result_buff[];

#endif //RTTHREAD_HPM_ADC_H
