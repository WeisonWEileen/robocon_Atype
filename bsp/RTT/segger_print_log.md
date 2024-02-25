# 使用了Jlink所在公司开发的segger rtt
- 一开始，直接使用`segger rtt`会在**gbd-server** `control block no found`的错误。首先，到`stm32F42xx`的`data_sheet`里面查询`stmF427`对应的RAM地址，查询得到是`0x20000000`,大小是192kb,到`launch.json`文件里面配置响应的设置,如下面所示
```            
"rttConfig": {
                "enabled": true,
                "address": "0x20000000",
                "searchSize": 196608,
                "rtt_start_retry": 10000,
                "searchId": "SEGGER RTT",
                "clearSearch": false,
                "polling_interval": 20,
                "decoders": [
                    {
                        "label": "",
                        "port": 0,
                        "type": "console"
                    }
                ]
            }
```
TODO
- 目前都是使用dap和stlink进行开发的，是不是使用jlink会更好？
# 利用`usart6`在`usart6.c`里面重写了`write()`函数，实现重定义print函数，输出打印信息到上位机，下面是重写的代码
注：(dma发送很可能把buffer塞爆，不用dma又非常占用cpu资源，不推荐使用)
```
#include <stdio.h>

#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
  HAL_UART_Transmit(&huart6, (uint8_t *)ptr, len, 0xFFFF);
  return len;
}
#endif
```
目前cpu尚可，发送时间可拉满到`0xFFFF`。慎用`HAL_UART_Transmit_DMA`，相邻时间过段调用，很可能导致后者发送失败


TODO
- 查看gcc关于`printf`的源代码，阅读看看其为什么重写的是`write`
- 解决打印浮点数失败的问题
- 使用 peripherals svd