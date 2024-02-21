#include "CAN_receive.h"
#include "can.h"
#include "chasis.h"

extern CAN_HandleTypeDef hcan1;   //can1句柄
extern motor_run_data_t motor_3508[4]; //底盘四个电机数据，声明在chasis.c里面
motor_raw_measure_t motor_chassis[4];  // 电机从can线读到的原始数据

static CAN_TxHeaderTypeDef chassis_tx_message;
static uint8_t chassis_can_send_data[8];  //定义电机发送的数据，仅限CAN_receive文件内使用

/**
 * @brief CAN interrupt receive function
 * @param None
 * @retval None
 * @date 2024/2/20
*/

/**
 * @brief          hal CAN fifo call back, receive motor data
 * @param[in]      hcan, the point to CAN handle
 * @retval         none
 */
/**
 * @brief          hal库CAN回调函数,接收电机数据
 * @param[in]      hcan:CAN句柄指针
 * @retval         none
 */

#define get_motor_measure(ptr, data)                                     \
    {                                                                    \
        (ptr)->last_ecd = (ptr)->ecd;                                    \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);           \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);     \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
        (ptr)->temp = (data)[6];                                    \
    }

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    __uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); /*接受CAN总线提供的函数*/

    if (hcan == &hcan1)
    {
        switch (rx_header.StdId)
        {
        case CAN_3508_M1_ID:
        {
            get_motor_measure(&motor_chassis[0], rx_data);
            motor_3508[0].realRpm = (float)motor_chassis[0].speed_rpm;
            // Angle_compute(&motor_chassis[0], &motor_3508[0]);
            break;
        }
        case CAN_3508_M2_ID:
        {
            get_motor_measure(&motor_chassis[1], rx_data);
            motor_3508[1].realRpm = (float)motor_chassis[1].speed_rpm;
            break;
        }
        case CAN_3508_M3_ID:
        {
            get_motor_measure(&motor_chassis[2], rx_data);
            motor_3508[2].realRpm = (float)motor_chassis[2].speed_rpm;
            break;
        }
        case CAN_3508_M4_ID:
        {
            get_motor_measure(&motor_chassis[3], rx_data);
            motor_3508[3].realRpm = (float)motor_chassis[3].speed_rpm;
            break;
        }

        default:
        {
            break;
        }
        }
    }
}

/**
 * @brief send chassis data
*/
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}
