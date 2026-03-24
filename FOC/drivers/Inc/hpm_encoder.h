//
// Created by 21184 on 2026/3/24.
//

#ifndef RTTHREAD_HPM_ENCODER_H
#define RTTHREAD_HPM_ENCODER_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* AS5047P Registers */
#define AS5047P_REG_ERRFL       0x0001
#define AS5047P_REG_PROG        0x0003
#define AS5047P_REG_DIAAGC      0x3FFC
#define AS5047P_REG_MAG         0x3FFD
#define AS5047P_REG_ANGLE       0x3FFE
#define AS5047P_REG_ANGLECOM    0x3FFF

void hpm_as5047p_init(void);

uint16_t hpm_as5047p_read_angle(void);

uint32_t hpm_as5047p_get_qei_count(void);

#endif //RTTHREAD_HPM_ENCODER_H
