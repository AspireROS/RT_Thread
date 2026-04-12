/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-12    西柚嵌入式       the first version
 */
#ifndef RTTHREAD_HPM_CLARKE_H
#define RTTHREAD_HPM_CLARKE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Clarke 变换结构体
 */
typedef struct {
    /* 输入量 */
    float Ia; // A相电流
    float Ib; // B相电流
    float Ic; // C相电流 (如果三相平衡，通常不需要参与计算)

    /* 输出量 */
    float Ialpha; // alpha轴电流
    float Ibeta; // beta轴电流
} clarke_t;

/**
 * @brief Clarke 变换计算 (三相静止 -> 两相静止)
 * @param pHandle Clarke 结构体句柄
 */
void hpm_clarke_calc(clarke_t *pHandle);

#ifdef __cplusplus
}
#endif

#endif /* FOC_INC_HPM_CLARKE_H_ */
