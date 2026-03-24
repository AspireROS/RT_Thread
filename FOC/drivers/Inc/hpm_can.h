//
// Created by 21184 on 2026/3/17.
//

#ifndef RTTHREAD_HPM_CAN_H
#define RTTHREAD_HPM_CAN_H

#include <rtthread.h>
#include "board.h"
#include "hpm_mcan_drv.h"

/**
 * @brief Initialize MCAN0 and MCAN1 with given baudrate.
 * @param baudrate Standard CAN baudrate (e.g., 500000)
 */
void hpm_can_init(uint32_t baudrate);

/**
 * @brief Send a standard CAN frame.
 * @param ptr MCAN peripheral base
 * @param id CAN ID
 * @param data Data pointer
 * @param len Data length (0-8)
 * @return status_success or error code
 */
hpm_stat_t hpm_can_send_frame(MCAN_Type *ptr, uint32_t id, uint8_t *data, uint8_t len);

#endif //RTTHREAD_HPM_CAN_H
