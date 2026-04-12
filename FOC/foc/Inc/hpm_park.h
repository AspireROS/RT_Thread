//
// Created by 21184 on 2026/3/24.
//

#ifndef RTTHREAD_HPM_PARK_H
#define RTTHREAD_HPM_PARK_H

#ifdef __cplusplus
extern "C" {



#endif

/**
 * @brief Park 与反 Park 变换结构体
 */
typedef struct {
    /* 公共输入 */
    float theta; // 转子电角度 (弧度制, 0 ~ 2PI)

    /* Park 变换变量 (电流环反馈使用) */
    float Ialpha; // 输入: alpha轴电流
    float Ibeta; // 输入: beta轴电流
    float Id; // 输出: d轴电流
    float Iq; // 输出: q轴电流

    /* 反 Park 变换变量 (电流环输出使用) */
    float Vd; // 输入: d轴电压目标值
    float Vq; // 输入: q轴电压目标值
    float Valpha; // 输出: alpha轴电压
    float Vbeta; // 输出: beta轴电压
} park_t;

/**
 * @brief Park 变换 (两相静止 -> 两相旋转)
 */
void hpm_park_calc(park_t *pHandle);

/**
 * @brief 反 Park 变换 (两相旋转 -> 两相静止)
 */
void hpm_inv_park_calc(park_t *pHandle);

#ifdef __cplusplus
}
#endif

#endif //RTTHREAD_HPM_PARK_H
