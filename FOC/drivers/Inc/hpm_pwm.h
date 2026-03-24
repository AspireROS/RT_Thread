//
// Created by 21184 on 2026/3/17.
//

#ifndef RTTHREAD_HPM_PWM_H
#define RTTHREAD_HPM_PWM_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define PWM_DEV0                   "pwm0"
#define PWM_DEV1                   "pwm1"
#define BOARD_PWM_CHANNEL1         (0U)
#define BOARD_PWM_CHANNEL2         (1U)
#define BOARD_PWM_CHANNEL3         (2U)
#define BOARD_PWM_CHANNEL4         (3U)
#define BOARD_PWM_CHANNEL5         (4U)
#define BOARD_PWM_CHANNEL6         (5U)
#define BOARD_PWM_CHANNEL7         (6U)
#define BOARD_PWM_CHANNEL8         (7U)

void hpm_pwm_complementary_init(void);

/**
 * Configure PWM valley trigger to TRGO_0.
 */
void hpm_pwm_set_valley_trigger(void);

#endif //RTTHREAD_HPM_PWM_H
