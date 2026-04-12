/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-12    西柚嵌入式       the first version
 */
#include "./foc/Inc/hpm_clarke.h"

// 假设 1/sqrt(3) 的常量
#define ONE_BY_SQRT3 0.577350269f

/**
 * @brief Clarke 变换计算 (三相静止 -> 两相静止)
 * @param pHandle Clarke 结构体句柄
 * @note 假设 Ia + Ib + Ic = 0，因此只需 Ia 和 Ib 即可计算
 */
void hpm_clarke_calc(clarke_t *pHandle) {
    // Ialpha = Ia
    pHandle->Ialpha = pHandle->Ia;

    // Ibeta = (Ia + 2*Ib) / sqrt(3)
    pHandle->Ibeta = (pHandle->Ia + 2.0f * pHandle->Ib) * ONE_BY_SQRT3;
}
