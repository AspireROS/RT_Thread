//
// Created by 21184 on 2026/3/24.
// Modified for VCE2755Q (SPI + QEI Fusion)
//

#include "../Inc/hpm_foc.h"
#include <math.h>
#include "../../drivers/Inc/hpm_encoder.h"
#include "../../drivers/Inc/hpm_pwm.h"
#include <rtthread.h>

foc_controller_t foc_ctrl;

#ifndef PI
#define PI 3.14159265358979323846f
#endif

void hpm_foc_init(uint8_t pole_pairs, float vdc) {
    foc_ctrl.pole_pairs = pole_pairs;

    /* 初始化 PID 控制器参数
     * 此处的 Kp 和 Ki 为示例参数，实际需根据电机的电感(L)和电阻(R)进行整定
     */
    float max_out = vdc / 1.73205081f; // 最大输出相电压幅值 (约 0.577 * Vdc)
    hpm_pid_init(&foc_ctrl.pid_d, 0.1f, 0.01f, max_out);
    hpm_pid_init(&foc_ctrl.pid_q, 0.1f, 0.01f, max_out);

    foc_ctrl.svpwm.Vdc = vdc;
    foc_ctrl.svpwm.pwm_period_max = (float) hpm_pwm_get_period();

    foc_ctrl.target_Id = 0.0f;
    foc_ctrl.target_Iq = 0.0f; // 开环测试或初始状态先设为0
}

void hpm_foc_set_target(float id, float iq) {
    foc_ctrl.target_Id = id;
    foc_ctrl.target_Iq = iq;
}

void hpm_foc_step(float ia, float ib, float ic) {
    foc_ctrl.phase_a_current = ia;
    foc_ctrl.phase_b_current = ib;
    foc_ctrl.phase_c_current = ic;

    /* 1. 读取 VCE2755Q 的极低延迟 QEI 硬件脉冲计数值
     * (摒弃了之前耗时的 SPI 读取操作)
     */
    uint32_t qei_cnt = hpm_vce2755_get_qei_count();

    // 防止溢出，将计数值限制在一圈以内 (例如配置为1024线时，取模 4096)
    qei_cnt = qei_cnt % QEI_COUNTS_PER_REV;

    // 将 QEI 脉冲数转换为机械角度 (0 ~ 2PI)
    foc_ctrl.theta_mech = ((float) qei_cnt / (float) QEI_COUNTS_PER_REV) * 2.0f * PI;

    // 计算电角度
    foc_ctrl.theta_elec = foc_ctrl.theta_mech * foc_ctrl.pole_pairs;

    // 归一化电角度到 0 ~ 2PI
    foc_ctrl.theta_elec = fmodf(foc_ctrl.theta_elec, 2.0f * PI);
    if (foc_ctrl.theta_elec < 0.0f) {
        foc_ctrl.theta_elec += 2.0f * PI;
    }

    /* 2. Clarke 变换 (三相静止 -> 两相静止) */
    foc_ctrl.clarke.Ia = foc_ctrl.phase_a_current;
    foc_ctrl.clarke.Ib = foc_ctrl.phase_b_current;
    foc_ctrl.clarke.Ic = foc_ctrl.phase_c_current;
    hpm_clarke_calc(&foc_ctrl.clarke);

    /* 3. Park 变换 (两相静止 -> 两相旋转) */
    foc_ctrl.park.Ialpha = foc_ctrl.clarke.Ialpha;
    foc_ctrl.park.Ibeta = foc_ctrl.clarke.Ibeta;
    foc_ctrl.park.theta = foc_ctrl.theta_elec;
    hpm_park_calc(&foc_ctrl.park);

    /* 4. PID 控制计算 Vd, Vq (电流环) */
    foc_ctrl.inv_park.Vd = hpm_pid_calc(&foc_ctrl.pid_d, foc_ctrl.target_Id, foc_ctrl.park.Id);
    foc_ctrl.inv_park.Vq = hpm_pid_calc(&foc_ctrl.pid_q, foc_ctrl.target_Iq, foc_ctrl.park.Iq);

    /* 5. 反 Park 变换 (两相旋转 -> 两相静止) */
    foc_ctrl.inv_park.theta = foc_ctrl.theta_elec;
    hpm_inv_park_calc(&foc_ctrl.inv_park);

    /* 6. SVPWM 空间矢量调制 (计算各相比较寄存器值) */
    foc_ctrl.svpwm.Valpha = foc_ctrl.inv_park.Valpha;
    foc_ctrl.svpwm.Vbeta = foc_ctrl.inv_park.Vbeta;
    // 动态获取当前的 pwm 周期
    foc_ctrl.svpwm.pwm_period_max = (float) hpm_pwm_get_period();
    hpm_svpwm_calc(&foc_ctrl.svpwm);

    /* 7. 更新 PWM 占空比 (写入硬件寄存器) */
    hpm_pwm_set_duty(foc_ctrl.svpwm.cmp_a, foc_ctrl.svpwm.cmp_b, foc_ctrl.svpwm.cmp_c);
}
