// #include "cmsis_os.h"
#include "chasis.h"
#include "main.h"
#include "pid.h"
#include "CAN_receive.h"
#include "sbus.h"

motor_run_data_t motor_3508[4]; // 电机驱动电机运动的数据
chassis_move_t chassis_vxyz;

/**
 * @brief          电机转动任务，freertos种调用
 * @param[in]      argument:不需要传参，canReceive任务传入的参数
 * @retval         none
 */
// void MotorTask(void const *argument)

void MotorTask(void)
{
    // HAL_Delay(100); // delay一下，让canReceive里面的数据先解算
    // motor_data_init(); //desireRpm = 0
    // motor_3508[0].ang_pid.PID_reset(&motor_3508[0].ang_pid, 0.08, 0, 2);
    // motor_3508[0].pid.PID_reset(&motor_3508[0].pid, 6, 0.01, 0.1);
    //防止开始前已经有can累计
    // motor_3508[0].accumAngle = 0;
    // while (1)
    // {
        sbus_to_chasisvxyz();
        chasisvxzy_to_desireRpm();

        for(int i=0 ; i<4 ; i++)
        {
            PID_Calculate(&motor_3508[i].pid, motor_3508[i].realRpm, motor_3508[i].desireRpm);
        }
        
        CAN_cmd_chassis(motor_3508[0].pid.Output, motor_3508[1].pid.Output, motor_3508[2].pid.Output, motor_3508[3].pid.Output);
        // vTaskDelay(1);
        // HAL_Delay(1);

        // // 双环pid，先算外环的角度的输出，@to do没调明白
        // PID_Calculate(&motor_3508[0].ang_pid, motor_3508[0].accumAngle, motor_3508[0].desireAngle);
        // // 再算内环的电流的输出
        // PID_Calculate(&motor_3508[0].pid, motor_3508[0].realRpm, motor_3508[0].ang_pid.Output);
        CAN_cmd_chassis(motor_3508[0].pid.Output, 0, 0, 0);
        // vTaskDelay(1);
    // }
}

/**
 * @brief          所有电机数据初始化，需要选择位置PID或者差分PID
 * @retval         none
 */
void motor_data_init(void)
{
    // 给电机3000的目标转速
    for (int i = 0; i < 4 ; i++)
    {
        motor_3508[i].desireRpm = motor_3508[i].realRpm = motor_3508[i].desireAngle = 0;
    }

    for (int i = 0; i < 4 ; i++)
    {
        // PID_Init(&motor_3508[i].ang_pid, 9600, 3000, 0.03, 1, 0, 0, 100, 100, 0.02, 0.02, NONE);
        PID_Init(&motor_3508[i].pid, 9600, 3000, 3, 6, 0, 0.1, 100, 100, 0.02, 0.02, NONE);
    }
}

void chasisvxzy_to_desireRpm(void)
{
    motor_3508[0].desireRpm = (-chassis_vxyz.vx + chassis_vxyz.vy + chassis_vxyz.wz ) * rpmCoeff;
    motor_3508[1].desireRpm = -(chassis_vxyz.vx + chassis_vxyz.vy - chassis_vxyz.wz ) * rpmCoeff;
    motor_3508[2].desireRpm = -(-chassis_vxyz.vx + chassis_vxyz.vy - chassis_vxyz.wz ) * rpmCoeff;
    motor_3508[3].desireRpm = (chassis_vxyz.vx + chassis_vxyz.vy + chassis_vxyz.wz ) * rpmCoeff;
}


/**
 * @brief          电机角度解算，包括角度和角速度
 * @param[in]      raw_data: 电机原始数据
 * @param[in]      motor_run: 电机运动数据
 * @retval         none

*/
void Angle_compute(motor_raw_measure_t *raw_data, motor_run_data_t *motor_run)
{
    __uint16_t delta_ecd = raw_data->ecd - raw_data->last_ecd;
    // ecd的测量值为(-4096,4096)，根据电机模型，前后差值的超过4096的话，其通过一次ecd为0的点，圈数可以+1
    if (delta_ecd > 4096)
        motor_run->accumAngle += 2*PI ;
    else if (delta_ecd < -4096)
        motor_run->accumAngle -= 2 * PI;

    // 1. 解算角度:（当前读取码盘值-初始的码盘值+圈数*8192）/ 8192 / 转速比 *2*PI
    //        2. 映射到（-pi，pi）区间
        motor_run->accumAngle += (float)(delta_ecd) / 8192 / 19.02f * 2 * PI;
}

fp32 norm_rad_format(fp32 angle)
{
    if (angle > PI)
    {
        while (angle > PI)
        {
            angle -= 2 * PI;
        }
    }
    else if (angle < -PI)
    {
        while (angle < -PI)
        {
            angle += 2 * PI;
        }
    }
    return angle;
}


// 防止频繁按键
// bool_t motor_flag = 1;
// int key_delay = 1000;

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     if (GPIO_Pin == KEY_interrupt_Pin) // 按一次按键，motor_flag置1，开始一个pi/2的任务
//     {
//         if (key_delay == 0)
//         {
//             motor_flag = 1;
//             key_delay = 1000;
//         }
//     }
// }

