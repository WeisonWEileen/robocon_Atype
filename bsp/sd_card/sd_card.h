/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       sd_card.h
 * @brief      this file contains the common defines and functions prototypes for 
 *             the sd_card.c driver
 * @note         
 * @Version    V1.0.0
 * @Date       Jan-30-2018      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */
 
#ifndef __SD_CARD_H__
#define __SD_CARD_H__

#include "sdio.h"
 
#define ERR_MOUNT_MKFS (1)
#define ERR_OPEN       (2)

#define MAX_MO_TIMES 3

extern uint8_t err;

void ErrorDetectTask(void *argument);
void SdioTask(void *argument);
void Mount_sdTask(void);

#endif

