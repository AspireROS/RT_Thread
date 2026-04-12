/*
* Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-12    西柚嵌入式       the first version
 */

#ifndef RTTHREAD_HPM_ADC_H
#define RTTHREAD_HPM_ADC_H

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------
 * 硬件采样参数配置 (需根据电机驱动板硬件原理图修改)
 * ------------------------------------------------------------------ */
#define ADC_VREF                (3.3f)      // ADC 参考电压 (通常为 3.3V)
#define ADC_RESOLUTION_MAX      (65536.0f)  // 16位 ADC 的最大值 (2^16)
#define SHUNT_RESISTOR          (0.01f)     // 采样电阻阻值 (例如 10mR = 0.01欧姆)
#define OPAMP_GAIN              (10.0f)     // 运放放大倍数 (例如 10倍)

/* 原始值转换为电流的预计算系数，优化 FOC 中断里的计算速度 */
#define ADC_TO_CURRENT_FACTOR   (ADC_VREF / ADC_RESOLUTION_MAX / (SHUNT_RESISTOR * OPAMP_GAIN))

/**
 * @brief 三相电流物理量结构体
 */
typedef struct {
    float Iu; /*!< U相实际电流 (安培 A) */
    float Iv; /*!< V相实际电流 (安培 A) */
    float Iw; /*!< W相实际电流 (安培 A) */
} foc_phase_current_t;

/* ------------------------------------------------------------------
 * 对外开放的 API
 * ------------------------------------------------------------------ */

/**
 * @brief 初始化 ADC PMT 抢占模式 (绑定到三相电流通道)
 * @param ch_u U相 ADC 通道号
 * @param ch_v V相 ADC 通道号
 * @param ch_w W相 ADC 通道号
 * @return 无
 */
void hpm_adc_init_for_foc(uint8_t ch_u, uint8_t ch_v, uint8_t ch_w);

/**
 * @brief 电流零点偏置标定
 * @note 必须在 PWM 开启但占空比为 50% (不导通)、电机静止时调用！
 * @return 无
 */
void hpm_adc_calibrate_offset(void);

/**
 * @brief 获取换算后的真实三相电流 (安培)
 * @param currents 用于存放三相电流结果的结构体指针
 * @return 无
 */
void hpm_adc_get_phase_currents(foc_phase_current_t *currents);

#ifdef __cplusplus
}
#endif

#endif /* RTTHREAD_HPM_ADC_H */
