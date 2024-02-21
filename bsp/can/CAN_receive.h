#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "struct_typedef.h"

#define CHASSIS_CAN hcan1

/**
 * @file  CAN_receive.c/CAN_receive.h
 * @brief This file is for receiving CAN messages,
 * @note
 * @histrory
 * Date
 * 2024/2/20
 */

typedef struct
{
    uint16_t ecd;          // ecd value now
    uint16_t last_ecd;     // ecd value last time
    int16_t speed_rpm;     // the speed in rpm
    int16_t given_current; // the control current
    uint8_t temp;          // motor temperature
} motor_raw_measure_t;

typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

} can_msg_id_e;

/**
 * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
 * @param[in]      motor1: (0x201) 3508电机控制电流, 范围 [-16384,16384]
 * @param[in]      motor2: (0x202) 3508电机控制电流, 范围 [-16384,16384]
 * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
 * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
 * @retval         none
 */
extern void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

#endif