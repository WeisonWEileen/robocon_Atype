/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       sd_card.c
 * @brief      this file contains sd card basic operating function
 * @author     weison_pan
 * @note
 * @Version    V1.0.0
 * @Date       Jan-30-2018
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */

#include "bsp_driver_sd.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "sdio.h"
#include "sd_card.h"
#include "main.h"
#include "bsp_log.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "struct_typedef.h"
#include "bsp_dwt.h"
#include "string.h"

// __attribute__((__used__)) __attribute__((section(".ccram"))) int ccramdata[10] = {0};

FATFS SDFatFs;					  /* file system object for SD card logical drive */
static uint8_t buffer[_MAX_SS];	  /* a work buffer for the f_mkfs() */
uint32_t byteswritten, bytesread; /* file write/read counts */
uint8_t rtext;					  /* file read buffer */
uint8_t err;
static char *f_name = "ares.txt";				 /* file name */
uint8_t wtext[48] = "pleaseasdfasdfddfsadfgo! "; /* file write buffer */

extern osThreadId_t sdioTaskHandle;

/**
 * @brief  read ok indicator
 * @param
 * @retval
 */
static void
read_ok(void)
{
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}

static uint8_t mount_flag = 0;
static uint8_t mount_state;
static volatile uint8_t open_state;
static uint8_t m_o_state; /*tell the GNU compiler to always place this variable into memory instead of
									 register (not to optimized this variable)
									so that different task can comunicate through this m_o_state*/
/**
 * @brief mount card record the state of mounting 
 */
void Mount_sdTask(void)
{
	mount_state = f_mount(&SDFatFs, (TCHAR const *)SDPath, 1);
	if (mount_state == FR_OK)
		LOGINFO("mount sd card ok!\n");
	else
		LOGERROR("f_mount error code is %d\n", mount_state);
	LOGINFO("mount to open...\n");

	// open_state = f_open(&SDFile, f_name, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);

	// if (open_state == FR_OK)
	// {
	// 	LOGINFO("open sd card ok!\n");
	// }
	// else
	// 	LOGERROR("open sd card error code is %d\n", open_state);

	// m_o_state = ((mount_state == FR_OK) && (open_state == FR_OK));
	if ((mount_state == FR_OK))
	{
		f_printf(&SDFile, "%s", "weison_pan_mount and open success"); /* succes log */
		LOGINFO("Begin sd card logging!\n");
	}
	else
		LOGERROR("sd card fatfs failed \n");
	return m_o_state;
}

/**
 * @brief  doing some tests to SD card, like mount, create file, open a text etc.
 * @param
 * @retval
 */

void SdioTask(void *argument)
{
	// static fp32 now_time, last_time, timecost_writing;
	while (1)
	{
		uint32_t num_unit_encoding;
		uint32_t now_time = DWT_GetTimeline_ms();
		if (mount_flag == 0)
		{
			Mount_sdTask();
			mount_flag = 1;
		}
		/* Create and Open a new text file object with write access */
		if (mount_state == FR_OK)
		{
			f_open(&SDFile, f_name, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			f_lseek(&SDFile, f_size(&SDFile));
			num_unit_encoding = f_printf(&SDFile, "Now the time for this loop is %d\n", now_time);
			f_close(&SDFile);
			if (num_unit_encoding >= 0)
				LOGINFO("%d of character encoding units written to the task\n", num_unit_encoding);
			else
				LOGWARNING("print_into error! please check in detailed!\n");
		}
		else
		{
			LOGERROR("sdio error! Deleted sd logging task!\n");
			vTaskDelete(sdioTaskHandle);
		}

		/* Unlink the SD disk I/O driver */
		// FATFS_UnLinkDriver(SDPath);
		vTaskDelay(5000);
	}
}

/**
 * @brief      show errors by blinking led
 * @param[in]  err: error id
 * @retval
 */
static void led_blinking(uint8_t num)
{
	uint8_t i;
	for (i = 0; i < num; i++)
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		osDelay(4000);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		osDelay(100);
	}
	osDelay(500);
}

/**
 * @brief      show errors by blinking led
 * @param[in]  err: error id
 * @retval
 */
void ErrorDetectTask(void *argument)
{
	while (1)
	{

		switch (err)
		{
		case ERR_MOUNT_MKFS:
		{
			led_blinking(ERR_MOUNT_MKFS);
			LOGERROR("Register file system error or create file system error!\n");
			osDelay(10);
		}
		break;

		case ERR_OPEN:
		{
			led_blinking(ERR_OPEN);
			LOGERROR("Open file error!\n");
			osDelay(10);
		}
		break;

		default:
		{
			LOGINFO("Write & Read sdcard ok\n");
			osDelay(10);
		}
		break;
		}
		osDelay(10000);
	}
}
