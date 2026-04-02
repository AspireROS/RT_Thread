//
// Created by 21184 on 2026/3/24.
// Modified for VCE2755Q
//

#ifndef RTTHREAD_HPM_ENCODER_H
#define RTTHREAD_HPM_ENCODER_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* VCE2755Q Registers & Commands */
#define VCE2755_CMD_READ_ANGLE  0x83    // 读命令(0x80) | 寄存器地址(0x03)
#define VCE2755_BIT_RES         18      // 原始角度数据位宽
#define VCE2755_MAX_VAL         262144  // 2^18

/* ABZ 编码器配置参数 (需与 VCE2755Q 内部 MTP 配置一致) */
#define VCE2755_CPR             1024    // 编码器线数 (配置为1024)
#define QEI_COUNTS_PER_REV      (VCE2755_CPR * 4) // QEI 硬件4倍频后的单圈脉冲数 (4096)

/* VCE2755Q 结构体，用于返回完整状态 */
typedef struct {
    uint32_t angle_18bit; // 18位绝对角度 (0 ~ 262143)
    uint8_t smf_alarm; // 弱磁场报警标志 (0:正常, 1:弱磁/掉落)
    uint8_t bte_flag; // 内部定义标志位
    uint8_t crc_val; // 接收到的 4-bit CRC
} vce2755_data_t;

void hpm_vce2755_init(void);

/* 读取 VCE2755 绝对角度与状态 (SPI) */
void hpm_vce2755_read_data(vce2755_data_t *data);

/* 获取当前 QEI 硬件计数值 */
uint32_t hpm_vce2755_get_qei_count(void);

/* 上电同步：用 SPI 读取绝对角度，并装载给 QEI 硬件寄存器 */
void hpm_vce2755_sync_qei(void);

#endif //RTTHREAD_HPM_ENCODER_H
