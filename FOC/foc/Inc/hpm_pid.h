//
// Created by 21184 on 2026/3/24.
//

#ifndef RTTHREAD_HPM_PID_H
#define RTTHREAD_HPM_PID_H

#ifdef __cplusplus
extern "C" {



#endif

/**
 * @brief 带有积分抗饱和的位置式 PID 结构体
 */
typedef struct {
    /* 参数 */
    float Kp; // 比例系数
    float Ki; // 积分系数

    /* 状态与限制 */
    float integral; // 积分累加值
    float out_max; // 输出上限 (防积分饱和)
    float out_min; // 输出下限

    /* 最终输出 */
    float out; // 控制器计算结果
} foc_pid_t;

/**
 * @brief PID 控制器初始化
 */
void hpm_pid_init(foc_pid_t *pHandle, float kp, float ki, float max_out);

/**
 * @brief PID 计算 (带积分限幅)
 * @param pHandle PID 结构体句柄
 * @param ref 目标值 (Reference)
 * @param fdb 反馈值 (Feedback)
 * @return 控制器输出
 */
float hpm_pid_calc(foc_pid_t *pHandle, float ref, float fdb);

#ifdef __cplusplus
}
#endif

#endif //RTTHREAD_HPM_PID_H
