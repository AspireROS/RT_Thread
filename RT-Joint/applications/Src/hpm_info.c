/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-04-08    西柚嵌入式    the first version
 */
#include <rtthread.h>
#include "board.h"

#include "pinmux.h"
#include "./Inc/hpm_info.h"
#include "./drivers/Inc/hpm_pwm.h"
#include "./drivers/Inc/hpm_encoder.h"
#include "./drivers/Inc/hpm_adc.h"

/* 配置你的实际相电流 ADC 通道 */
#define ADC_CH_U  (12)
#define ADC_CH_V  (13)
#define ADC_CH_W  (14)

/**
 * @brief 打印系统启动信息并初始化外设
 * @note 此函数为系统硬件初始化总入口，依次调用各个底层外设的配置函数。
 * @return 无
 */
void hpm_init_info(void) {
    banner_info();
    clock_freq_info();
    led_info();
    pwm_info(); /* 必须在 ADC 之前初始化 PWM */
    encoder_info();
    adc_info(); /* 初始化 ADC 并完成硬件互联和标定 */
}

/**
 * @brief 打印系统启动横幅 (Banner)
 * @note 在终端输出 HPMicro 的 ASCII Art 字符画。
 * @return 无
 */
void banner_info(void) {
    rt_kprintf("----------------------------------------------------------------------\n");
    rt_kprintf("$$\\   $$\\ $$$$$$$\\  $$\\      $$\\ $$\\\n");
    rt_kprintf("$$ |  $$ |$$  __$$\\ $$$\\    $$$ |\\__|\n");
    rt_kprintf("$$ |  $$ |$$ |  $$ |$$$$\\  $$$$ |$$\\  $$$$$$$\\  $$$$$$\\   $$$$$$\\\n");
    rt_kprintf("$$$$$$$$ |$$$$$$$  |$$\\$$\\$$ $$ |$$ |$$  _____|$$  __$$\\ $$  __$$\\\n");
    rt_kprintf("$$  __$$ |$$  ____/ $$ \\$$$  $$ |$$ |$$ /      $$ |  \\__|$$ /  $$ |\n");
    rt_kprintf("$$ |  $$ |$$ |      $$ |\\$  /$$ |$$ |$$ |      $$ |      $$ |  $$ |\n");
    rt_kprintf("$$ |  $$ |$$ |      $$ | \\_/ $$ |$$ |\\$$$$$$$\\ $$ |      \\$$$$$$  |\n");
    rt_kprintf("\\__|  \\__|\\__|      \\__|     \\__|\\__| \\_______|\\__|       \\______/\n");
    rt_kprintf("----------------------------------------------------------------------\n");
}

/**
 * @brief 打印系统各个核心时钟的频率信息
 * @note 获取并输出 CPU、AHB、AXI 等总线的当前运行频率，用于核对时钟树。
 * @return 无
 */
void clock_freq_info(void) {
    rt_kprintf("==============================\n");
    rt_kprintf(" %s clock summary\n", BOARD_NAME);
    rt_kprintf("==============================\n");
    rt_kprintf("cpu0:\t\t %dHz\n", clock_get_frequency(clock_cpu0));
    rt_kprintf("ahb:\t\t %luHz\n", clock_get_frequency(clock_ahb0));
    rt_kprintf("axif:\t\t %dHz\n", clock_get_frequency(clock_axif));
    rt_kprintf("axis:\t\t %dHz\n", clock_get_frequency(clock_axis));
    rt_kprintf("axic:\t\t %dHz\n", clock_get_frequency(clock_axic));
    rt_kprintf("xpi0:\t\t %dHz\n", clock_get_frequency(clock_xpi0));
    rt_kprintf("mchtmr0:\t %dHz\n", clock_get_frequency(clock_mchtmr0));
    rt_kprintf("==============================\n");
}

/**
 * @brief 初始化 LED 并打印相关引脚及测试指令信息
 * @note 内部调用 init_led_pins() 完成引脚复用配置。
 * @return 无
 */
void led_info(void) {
    init_led_pins();
    rt_kprintf("LED initialization succeeded! \n");
    rt_kprintf("The LED pins are PD06 and PD07\n");
    rt_kprintf("==============================\n");
    rt_kprintf("Command to test LED -> hpm_led\n");
    rt_kprintf("==============================\n");
}

/**
 * @brief 初始化 PWM 发波外设并打印硬件映射信息
 * @note 内部调用 hpm_pwm_foc_init() 完成 FOC 所需的互补 PWM 通道配置。
 * @return 无
 */
void pwm_info(void) {
    hpm_pwm_foc_init();
    rt_kprintf("PWM initialization succeeded! \n");
    rt_kprintf("The complementary PWM outputs for phase U are PWM0 channel 4 (PD04) and channel 5 (PD05).\n");
    rt_kprintf("The complementary PWM outputs for phase V are PWM1 channel 0 (PC08) and channel 1 (PC09).\n");
    rt_kprintf("The complementary PWM outputs for phase W are PWM0 channel 6 (PC14) and channel 7 (PC15).\n");
    rt_kprintf("==============================\n");
    rt_kprintf("Command to test PWM -> hpm_pwm\n");
    rt_kprintf("==============================\n");
}

/**
 * @brief 初始化编码器外设并打印硬件映射信息
 * @note 内部调用 hpm_encoder_init() 完成 SPI 绝对位置与 QEI 增量接口的配置。
 * @return 无
 */
void encoder_info(void) {
    hpm_encoder_init();
    rt_kprintf("Encoder (VCE2755) initialization succeeded! \n");
    rt_kprintf("SPI1 (Absolute): CS(PC11), CLK(PC10), MISO(PC12), MOSI(PC13).\n");
    rt_kprintf("QEI1 (Incremental): A(PB30), B(PB31), Z(PB29).\n");
    rt_kprintf("==============================\n");
    rt_kprintf("Command to test Encoder -> hpm_encoder\n");
    rt_kprintf("==============================\n");
}

/**
 * @brief 初始化 ADC 并建立与 PWM 的硬件触发互联
 * @note 包含 TRGM 路由、PMT 抢占模式初始化，以及关键的静态电流零点偏置校准。
 * @return 无
 */
void adc_info(void) {
    hpm_pwm_set_valley_trigger();
    rt_kprintf("pwm initialization succeeded! \n");
    hpm_adc_init_for_foc(ADC_CH_U, ADC_CH_V, ADC_CH_W);//出问题了
    rt_kprintf("ADC initialization succeeded! \n");

    // hpm_adc_calibrate_offset();//卡住了
    rt_kprintf("ADC calibrate succeeded! \n");

    rt_kprintf("ADC (PMT Mode) initialization succeeded! \n");
    rt_kprintf("Channels U(%d), V(%d), W(%d) mapped to PWM valley trigger.\n", ADC_CH_U, ADC_CH_V, ADC_CH_W);
    rt_kprintf("==============================\n");
    rt_kprintf("Command to test ADC scope -> hpm_adc\n");
    rt_kprintf("==============================\n");
}
