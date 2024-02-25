## 读卡模式
**SDIO** + **FATFS** + **DMA** + **FreeRTOS**
> 关于总线的宽度的设置 (手动更改CubeMX的bug)
> + SD卡在刚上电的时寻卡初始化的过程中，时采用单总线模式，所以`MX_SDIO_SD_INIT`函数初始化需要把`BusWide`设置为`SDIO_BUS_WIDE_1B`

TODO
- 解决最高频率25Hz 50Hz的问题，cubeMX里面到底应该配置多少合适