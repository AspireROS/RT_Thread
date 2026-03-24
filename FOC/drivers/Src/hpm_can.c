//
// Created by 21184 on 2026/3/17.
//

#include "../Inc/hpm_can.h"
#include "hpm_mcan_drv.h"
#include "hpm_soc.h"

static void can_pin_init(void) {
    /* MCAN1 Pins */
    HPM_IOC->PAD[IOC_PAD_PD04].FUNC_CTL = IOC_PD04_FUNC_CTL_MCAN1_RXD;
    HPM_IOC->PAD[IOC_PAD_PD05].FUNC_CTL = IOC_PD05_FUNC_CTL_MCAN1_TXD;
    HPM_IOC->PAD[IOC_PAD_PD03].FUNC_CTL = IOC_PD03_FUNC_CTL_MCAN1_STBY;

    /* MCAN0 Pins */
    HPM_IOC->PAD[IOC_PAD_PD01].FUNC_CTL = IOC_PD01_FUNC_CTL_MCAN0_RXD;
    HPM_IOC->PAD[IOC_PAD_PD00].FUNC_CTL = IOC_PD00_FUNC_CTL_MCAN0_TXD;
    HPM_IOC->PAD[IOC_PAD_PD02].FUNC_CTL = IOC_PD02_FUNC_CTL_MCAN0_STBY;
}

void hpm_can_init(uint32_t baudrate) {
    can_pin_init();

    /* Initialize MCAN0 */
    uint32_t can0_clk_freq = board_init_can_clock(HPM_MCAN0);
    mcan_config_t can0_cfg;
    mcan_get_default_config(HPM_MCAN0, &can0_cfg);
    can0_cfg.baudrate = baudrate;
    /* If you want to enable CANFD, set these:*/
    can0_cfg.enable_canfd = true;
    can0_cfg.baudrate_fd = 2000000;
    mcan_get_default_ram_config(HPM_MCAN0, &can0_cfg.ram_config, true);

    mcan_init(HPM_MCAN0, &can0_cfg, can0_clk_freq);

    /* Initialize MCAN1 */
    uint32_t can1_clk_freq = board_init_can_clock(HPM_MCAN1);
    mcan_config_t can1_cfg;
    mcan_get_default_config(HPM_MCAN1, &can1_cfg);
    can1_cfg.baudrate = baudrate;
    mcan_init(HPM_MCAN1, &can1_cfg, can1_clk_freq);
}

hpm_stat_t hpm_can_send_frame(MCAN_Type *ptr, uint32_t id, uint8_t *data, uint8_t len) {
    mcan_tx_frame_t tx_frame;
    memset(&tx_frame, 0, sizeof(mcan_tx_frame_t));

    tx_frame.std_id = id & 0x7FF;
    tx_frame.use_ext_id = false;
    tx_frame.dlc = (len > 8) ? 8 : len;
    memcpy(tx_frame.data_8, data, tx_frame.dlc);

    return mcan_transmit_blocking(ptr, &tx_frame);
}
