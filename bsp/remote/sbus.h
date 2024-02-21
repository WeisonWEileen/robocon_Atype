#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "main.h"
#include "struct_typedef.h"


#define SBUS_RX_BUF_NUM 50u
#define RC_FRAME_LENGTH 25u

#define SBUS_VALUE_MAX ((fp32)671)
#define SBUS_VALUE_OFFSET ((fp32)1000)
#define SBUS2_DEFAULT ((fp32)1024)

extern fp32 SBUS_CHANNEL[10];

extern void sbus_init(void);
extern void sbus_disable(void);
extern void sbus_restart(void);
extern void sbus_hook(void);
extern void sbus_to_chasisvxyz(void);
#endif
