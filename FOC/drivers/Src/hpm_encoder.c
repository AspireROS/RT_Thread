//
// Created by 21184 on 2026/3/24.
// Modified for VCE2755Q
//

#include "../Inc/hpm_encoder.h"
#include "hpm_spi_drv.h"
#include "hpm_qeiv2_drv.h"
#include "hpm_ioc_regs.h"
#include "hpm_clock_drv.h"

#define VCE2755_SPI HPM_SPI1
#define VCE2755_QEI HPM_QEI1

static void hpm_vce2755_pinmux_init(void) {
    /* SPI1 Pins */
    HPM_IOC->PAD[IOC_PAD_PC12].FUNC_CTL = IOC_PC12_FUNC_CTL_SPI1_MISO;
    HPM_IOC->PAD[IOC_PAD_PC13].FUNC_CTL = IOC_PC13_FUNC_CTL_SPI1_MOSI;
    HPM_IOC->PAD[IOC_PAD_PC11].FUNC_CTL = IOC_PC11_FUNC_CTL_SPI1_CS_0;
    HPM_IOC->PAD[IOC_PAD_PC10].FUNC_CTL = IOC_PC10_FUNC_CTL_SPI1_SCLK | IOC_PAD_FUNC_CTL_LOOP_BACK_MASK;

    /* QEI1 Pins */
    HPM_IOC->PAD[IOC_PAD_PB31].FUNC_CTL = IOC_PB31_FUNC_CTL_QEI1_B;
    HPM_IOC->PAD[IOC_PAD_PB30].FUNC_CTL = IOC_PB30_FUNC_CTL_QEI1_A;
    HPM_IOC->PAD[IOC_PAD_PB29].FUNC_CTL = IOC_PB29_FUNC_CTL_QEI1_Z;
}

void hpm_vce2755_init(void) {
    hpm_vce2755_pinmux_init();

    /* SPI1 Clock & Init */
    clock_add_to_group(clock_spi1, 0);
    uint32_t spi_clk = board_init_spi_clock(VCE2755_SPI);

    spi_timing_config_t timing_config = {0};
    spi_master_get_default_timing_config(&timing_config);
    timing_config.master_config.clk_src_freq_in_hz = spi_clk;
    timing_config.master_config.sclk_freq_in_hz = 10000000; // 10MHz, VCE2755最高支持约16MHz
    spi_master_timing_init(VCE2755_SPI, &timing_config);

    spi_format_config_t format_config = {0};
    spi_master_get_default_format_config(&format_config);

    /* VCE2755Q 使用 SPI Mode 3: CPOL=1, CPHA=1 */
    format_config.common_config.data_len_in_bits = 8; // 数据帧长改为 8 bit
    format_config.common_config.mode = spi_master_mode;
    format_config.common_config.cpol = spi_sclk_high_idle; // 空闲高电平
    format_config.common_config.cpha = spi_sclk_sampling_even_clk_edges; // 第二个边沿(上升沿)采样
    spi_format_init(VCE2755_SPI, &format_config);

    /* QEI1 Clock & Init */
    clock_add_to_group(clock_qei1, 0);

    qeiv2_mode_config_t qei_mode_config = {0};
    qei_mode_config.work_mode = qeiv2_work_mode_abz;
    qei_mode_config.z_count_inc_mode = qeiv2_z_count_inc_on_z_input_assert;
    qei_mode_config.phcnt_max = 0xFFFFFFFF;

    qeiv2_config_mode(VCE2755_QEI, &qei_mode_config);
}

/**
 * 连读 VCE2755Q 角度与状态字 (SPI)
 */
void hpm_vce2755_read_data(vce2755_data_t *data) {
    /* * tx_buf: 1字节命令 + 3字节 Dummy 时钟用于接收
     * rx_buf: 第0字节无意义，后续依次为 Reg 0x03, 0x04, 0x05 的数据
     */
    uint8_t tx_buf[4] = {VCE2755_CMD_READ_ANGLE, 0x00, 0x00, 0x00};
    uint8_t rx_buf[4] = {0};

    spi_control_config_t control_config = {0};
    spi_master_get_default_control_config(&control_config);
    control_config.common_config.trans_mode = spi_trans_write_read_together;

    /* 发起 4 个字节长度的全双工传输 */
    spi_transfer(VCE2755_SPI, &control_config, NULL, NULL, tx_buf, 4, rx_buf, 4);

    /* 数据解析
     * rx_buf[1] -> Reg 0x03: ANGLE<17:10>
     * rx_buf[2] -> Reg 0x04: ANGLE<9:2>
     * rx_buf[3] -> Reg 0x05: ANGLE<1:0>[7:6] | SMF[5] | BTE[4] | CRC<3:0>[3:0]
     */
    if (data != NULL) {
        data->angle_18bit = ((uint32_t) rx_buf[1] << 10) |
                            ((uint32_t) rx_buf[2] << 2) |
                            ((rx_buf[3] >> 6) & 0x03);

        data->smf_alarm = (rx_buf[3] >> 5) & 0x01;
        data->bte_flag = (rx_buf[3] >> 4) & 0x01;
        data->crc_val = rx_buf[3] & 0x0F;
    }
}

/**
 * 获取 QEI 计数值
 */
uint32_t hpm_vce2755_get_qei_count(void) {
    /* 直接读取 QEIV2 的相位计数器寄存器 */
    return qeiv2_get_phase_cnt(VCE2755_QEI);
}

/**
 * 编码器同步：上电/空闲时调用，将 SPI 绝对角度装载给 QEI 硬件
 */
void hpm_vce2755_sync_qei(void) {
    vce2755_data_t vce_data;
    uint32_t initial_pulses;

    /* 1. 通过 SPI 获取静态绝对角度 */
    hpm_vce2755_read_data(&vce_data);

    /* 2. 角度等比例换算: QEI脉冲 = (当前18位角度 / 18位最大值) * QEI一圈总脉冲 */
    initial_pulses = (uint32_t) (((float) vce_data.angle_18bit / VCE2755_MAX_VAL) * QEI_COUNTS_PER_REV);

    /* 3. 强制写入先楫 MCU 的 QEIV2 硬件计数器 */
    /* 注: 若旧版本 SDK 无对应封装函数，可直接写寄存器: VCE2755_QEI->PHCNT = initial_pulses; */
    qeiv2_set_phase_cnt(VCE2755_QEI, initial_pulses);
}

/* 导出 MSH 命令用于测试与防呆检测 */
static void hpm_vce2755_test(void) {
    vce2755_data_t vce_data;

    /* 触发一次同步 (仅用于测试验证) */
    hpm_vce2755_sync_qei();

    /* 延迟一下，模拟系统运行 */
    rt_thread_mdelay(2);

    /* 读取最新状态 */
    hpm_vce2755_read_data(&vce_data);
    uint32_t qei_val = hpm_vce2755_get_qei_count();

    if (vce_data.smf_alarm) {
        rt_kprintf("\033[31mVCE2755 ALARM: Weak Magnet / Sensor detached!\033[0m\n");
    }

    rt_kprintf("VCE2755 - Raw Angle(18b): %u, SMF: %d, QEIV2 PhCnt: %u\n",
               vce_data.angle_18bit,
               vce_data.smf_alarm,
               qei_val);
}

MSH_CMD_EXPORT(hpm_vce2755_test, VCE2755Q dual - mode sensor test);
