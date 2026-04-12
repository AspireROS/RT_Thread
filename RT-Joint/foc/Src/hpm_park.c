/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-12    西柚嵌入式       the first version
 */
#include "./foc/Inc/hpm_park.h"

#include <math.h> // 需要用到 sinf 和 cosf，建议后续替换为查表法或 DSP 库的快速三角函数

/**
 * @brief Park 变换 (两相静止 -> 两相旋转)
 * @param pHandle Park 结构体句柄
 */
void hpm_park_calc(park_t *pHandle) {
    float sin_theta = sinf(pHandle->theta);
    float cos_theta = cosf(pHandle->theta);

    pHandle->Id = pHandle->Ialpha * cos_theta + pHandle->Ibeta * sin_theta;
    pHandle->Iq = -pHandle->Ialpha * sin_theta + pHandle->Ibeta * cos_theta;
}

/**
 * @brief 反 Park 变换 (两相旋转 -> 两相静止)
 * @param pHandle Park 结构体句柄
 */
void hpm_inv_park_calc(park_t *pHandle) {
    float sin_theta = sinf(pHandle->theta);
    float cos_theta = cosf(pHandle->theta);

    pHandle->Valpha = pHandle->Vd * cos_theta - pHandle->Vq * sin_theta;
    pHandle->Vbeta = pHandle->Vd * sin_theta + pHandle->Vq * cos_theta;
}
