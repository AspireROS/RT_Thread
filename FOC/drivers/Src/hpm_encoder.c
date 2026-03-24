//
// Created by 21184 on 2026/3/24.
//

#include "../Inc/hpm_encoder.h"
#include "hpm_spi_drv.h"
#include "hpm_qeiv2_drv.h"
#include "hpm_ioc_regs.h"
#include "hpm_clock_drv.h"

#define AS5047P_SPI HPM_SPI1
#define AS5047P_QEI HPM_QEI1

/* 计算 16 位数据的偶校验 (Even Parity) */
static uint16_t calculate_parity(uint16_t data) {
    uint16_t p = 0;
    uint16_t temp = data & 0x7FFF;
    while (temp) {
        p ^= (temp & 1);
        temp >>= 1;
    }
    return p; // 如果低15位有奇数个1，返回1，从而使总数变为偶数
}

static void hpm_as5047p_pinmux_init(void) {
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

void hpm_as5047p_init(void) {
    hpm_as5047p_pinmux_init();

    /* SPI1 Clock & Init */
    clock_add_to_group(clock_spi1, 0);
    uint32_t spi_clk = board_init_spi_clock(AS5047P_SPI);

    spi_timing_config_t timing_config = {0};
    spi_master_get_default_timing_config(&timing_config);
    timing_config.master_config.clk_src_freq_in_hz = spi_clk;
    timing_config.master_config.sclk_freq_in_hz = 10000000;
    spi_master_timing_init(AS5047P_SPI, &timing_config);

    spi_format_config_t format_config = {0};
    spi_master_get_default_format_config(&format_config);
    format_config.common_config.data_len_in_bits = 16;
    format_config.common_config.mode = spi_master_mode;
    format_config.common_config.cpol = spi_sclk_low_idle;
    format_config.common_config.cpha = spi_sclk_sampling_even_clk_edges;
    spi_format_init(AS5047P_SPI, &format_config);

    /* QEI1 Clock & Init */
    clock_add_to_group(clock_qei1, 0);

    qeiv2_mode_config_t qei_mode_config = {0};
    qei_mode_config.work_mode = qeiv2_work_mode_abz;
    qei_mode_config.z_count_inc_mode = qeiv2_z_count_inc_on_z_input_assert;
    qei_mode_config.phcnt_max = 0xFFFFFFFF;

    qeiv2_config_mode(AS5047P_QEI, &qei_mode_config);
}

/**
 * 读取绝对角度 (SPI)
 * @return 0-16383 (14-bit)
 */
uint16_t hpm_as5047p_read_angle(void) {
    uint16_t command = AS5047P_REG_ANGLECOM | (1 << 14); // Read ANGLECOM (0x3FFF), bit 14 is 'Read'
    if (calculate_parity(command)) {
        command |= (1 << 15); // Even Parity bit
    }

    uint16_t rx_data = 0;
    spi_control_config_t control_config = {0};
    spi_master_get_default_control_config(&control_config);

    /* AS5047P requires a second frame to get the result of the previous command */
    control_config.common_config.trans_mode = spi_trans_write_only;
    spi_transfer(AS5047P_SPI, &control_config, NULL, NULL, (uint8_t *) &command, 1, NULL, 0);

    control_config.common_config.trans_mode = spi_trans_write_read_together;
    spi_transfer(AS5047P_SPI, &control_config, NULL, NULL, (uint8_t *) &command, 1, (uint8_t *) &rx_data, 1);

    // Result frame: [15] Parity, [14] Error flag, [13:0] Data
    if (rx_data & (1 << 14)) {
        return 0xFFFF; // 返回 0xFFFF 代表读取出错
    }

    return rx_data & 0x3FFF; // 14-bit absolute position
}

/**
 * 获取 QEI 计数值
 */
uint32_t hpm_as5047p_get_qei_count(void) {
    /* Directly read Phase Counter register from QEIV2 */
    return qeiv2_get_phase_cnt(AS5047P_QEI);
}

/* 导出 MSH 命令用于测试 */
static void hpm_as5047p_test(void) {
    uint16_t angle = hpm_as5047p_read_angle();
    uint32_t qei = hpm_as5047p_get_qei_count();

    if (angle == 0xFFFF) {
        rt_kprintf("AS5047P - SPI Read Error!\n");
    } else {
        rt_kprintf("AS5047P - Angle: %d, QEIV2: %u\n", angle, qei);
    }
}

MSH_CMD_EXPORT(hpm_as5047p_test, AS5047P sensor test);
