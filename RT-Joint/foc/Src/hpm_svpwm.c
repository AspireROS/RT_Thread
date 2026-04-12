/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-12    西柚嵌入式       the first version
 */
#include "./foc/Inc/hpm_svpwm.h"

#define SQRT3 1.73205081f

/**
 * @brief SVPWM 计算，生成三相占空比
 * @param pHandle SVPWM 结构体句柄
 * @note Valpha 和 Vbeta 需要根据母线电压 (Vbus) 归一化或缩放
 */
void hpm_svpwm_calc(svpwm_t *pHandle) {
    float Ua, Ub, Uc;
    float t1, t2, t0;
    uint8_t sector = 0;

    // 1. 计算虚拟参考电压 U1, U2, U3 用于判断扇区
    Ua = pHandle->Vbeta;
    Ub = (SQRT3 * pHandle->Valpha - pHandle->Vbeta) * 0.5f;
    Uc = (-SQRT3 * pHandle->Valpha - pHandle->Vbeta) * 0.5f;

    // 2. 判断扇区 (Sector 1-6)
    if (Ua > 0.0f) sector += 1;
    if (Ub > 0.0f) sector += 2;
    if (Uc > 0.0f) sector += 4;

    // 3. 计算相邻非零矢量的作用时间 (T1, T2)
    // 根据系统 PWM 的最大重载值 (PWM_PRD) 调整 T1 和 T2
    float K = pHandle->pwm_period_max / pHandle->Vdc;

    switch (sector) {
        case 3: t1 = Uc * K;
            t2 = Ua * K;
            break; // Sector 1
        case 1: t1 = Ub * K;
            t2 = Uc * K;
            break; // Sector 2
        case 5: t1 = Ua * K;
            t2 = Ub * K;
            break; // Sector 3
        case 4: t1 = -Uc * K;
            t2 = -Ua * K;
            break; // Sector 4
        case 6: t1 = -Ub * K;
            t2 = -Uc * K;
            break; // Sector 5
        case 2: t1 = -Ua * K;
            t2 = -Ub * K;
            break; // Sector 6
        default: t1 = 0;
            t2 = 0;
            break;
    }

    // 过调制处理：如果 T1 + T2 > PWM_PERIOD，按比例缩小
    if ((t1 + t2) > pHandle->pwm_period_max) {
        float scale = pHandle->pwm_period_max / (t1 + t2);
        t1 *= scale;
        t2 *= scale;
    }

    // 4. 计算零矢量时间，并分配给三相比较寄存器 (七段式)
    t0 = (pHandle->pwm_period_max - t1 - t2) * 0.5f;
    float ta, tb, tc;

    switch (sector) {
        case 3: ta = t1 + t2 + t0;
            tb = t2 + t0;
            tc = t0;
            break;
        case 1: ta = t1 + t0;
            tb = t1 + t2 + t0;
            tc = t0;
            break;
        case 5: ta = t0;
            tb = t1 + t2 + t0;
            tc = t2 + t0;
            break;
        case 4: ta = t0;
            tb = t1 + t0;
            tc = t1 + t2 + t0;
            break;
        case 6: ta = t2 + t0;
            tb = t0;
            tc = t1 + t2 + t0;
            break;
        case 2: ta = t1 + t2 + t0;
            tb = t0;
            tc = t1 + t0;
            break;
        default: ta = t0;
            tb = t0;
            tc = t0;
            break;
    }

    // 5. 保存结果，准备写入 PWM 寄存器
    pHandle->cmp_a = (uint32_t) ta;
    pHandle->cmp_b = (uint32_t) tb;
    pHandle->cmp_c = (uint32_t) tc;
}
