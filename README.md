# tmc5160 固件库使用说明

本文为TMC5160固件库调试经验案例一篇。主要以经验案例的方式向软件库使用者描述芯片的主要功能、SPI通信方式特点、快速启动方法、函数功能清单。

## 0 TMC5160 简介

TMC5160 是带串行通信接口的高功率步进电机控制驱动芯片，将实现自动目标定位的灵活斜坡发生器和业界最先进的步进电机驱动器结合在一起。配合外置的功率晶体管，实现高动态、高扭矩电机驱动。TRINAMICs 先 进 的 SpreadCycle 和StealthChop 斩波器，驱动器可绝对无噪音的运行，并实现最大效率和最佳电机扭矩控制。高集成度、高能效和小外形尺寸使系统小型化和性能扩展可行，从而实现经济高效的解决方案。完整的解决方案在实现高性能基础上极大缩短学习时间。

## 1 SPI通信方式

TMC5160 采用 40 位 SPI (串行外围接口，SPI 是摩托罗拉的商标)数据报与微控制器 MCU 通信。带硬件 SPI 的微控制器 MCU 通常用 8 比特位的整数倍进行通信。设备的 NCS 信号必须在数据报传输的整个持续时间内保持有效(低)。发送到 TMC5160 的每个数据报由一个地址字节和四个数据字节组成，共计五个字节，每个8位，构成40位完整数据包。

注意：写数据一个40位数据即有效，但是读数据需要两个40位，第一个40收回的数据无效，第二个40位收回的数据有效。

## 2 快速启动方法

快速启动方法旨在使用最小的配置，完成启动，以验证硬件连接的可靠。按照如下方法进行配置：

以下 SPI 数据示例支持步进和方向模式， 12mhz 时钟，配置小于 30 rpm 运行 StealthChop 及大于 30rpm 运行 spreadCycle 的斩波器参数。

|      | SPI 发送     | 说明                                                         |
| ---- | ------------ | ------------------------------------------------------------ |
| 1    | 0xEC000100C3 | CHOPCONF: TOFF=3, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle) |
| 2    | 0x9000061F0A | IHOLD_IRUN: IHOLD=10, IRUN=31 (最大电流 ), IHOLDDELAY=6      |
| 3    | 0x910000000A | TPOWERDOWN=10: 电机静止到电流减小之间的延时                  |
| 4    | 0x8000000004 | EN_PWM_MODE=1 enables StealthChop (缺省 PWM_CONF 值)         |
| 5    | 0x93000001F4 | TPWM_THRS=500 对应切换速度 35000 = ca. 30RPM                 |

以下SPI 数据示例使能并初始化运动控制器，内部斜坡发生器控制电机运行一圈（51200微步），最后查询读取当前位置。

|      | SPI 发送     | 说明                                          |
| ---- | ------------ | --------------------------------------------- |
| 1    | 0xA4000003E8 | A1 = 1 000 第一阶段加速度                     |
| 2    | 0xA50000C350 | V1 = 50 000 加速度阈值速度 V1                 |
| 3    | 0xA6000001F4 | AMAX = 500 大于 V1 的加速度                   |
| 4    | 0xA700030D40 | VMAX = 200 000                                |
| 5    | 0xA8000002BC | DMAX = 700 大于 V1 的减速度                   |
| 6    | 0xAA00000578 | D1 = 1400 小于 V1 的减速度                    |
| 7    | 0xAB0000000A | VSTOP = 10 停止速度(接近于0)                  |
| 8    | 0xA000000000 | RAMPMODE = 0 (目标位置运动)                   |
| 9    | 0xADFFFF3800 | XTARGET = -51200 (向左运动一圈 (200*256 微步) |
| 10   | 0x2100000000 | 查询 XACTUAL – 下一个读操作返回 XACTUAL       |
| 11   | 0x2100000000 | 读XACTUAL                                     |

一共16步配置可以完成完整的一圈转动，验证硬件连接的可靠。

## 3 函数功能清单

完整版固件库功能清单

基础功能区：

| 序号 | 函数名称                                                 | 功能           |
| ---- | -------------------------------------------------------- | -------------- |
| 1    | uint32_t right(uint8_t motor, int32_t velocity);         | 正转           |
| 2    | uint32_t left(uint8_t motor, int32_t velocity);          | 反转           |
| 3    | uint32_t rotate(uint8_t motor, int32_t velocity);        | 旋转           |
| 4    | uint32_t start(uint8_t motor,TMC_START_PARA start_para); | 开始           |
| 5    | uint32_t stop(uint8_t motor);                            | 停止           |
| 6    | uint32_t moveTo(uint8_t motor, int32_t position);        | 移动到某个位置 |
| 7    | uint32_t moveBy(uint8_t motor, int32_t *ticks);          | 以某个速度移动 |

高级功能区：

| 序号 | 函数名称                                                     | 功能         |
| ---- | ------------------------------------------------------------ | ------------ |
| 1    | uint32_t GAP(uint8_t type, uint8_t motor, int32_t *value);   | 获取配置参数 |
| 2    | uint32_t SAP(uint8_t type, uint8_t motor, int32_t value);    | 设置配置参数 |
| 3    | void readRegister(uint8_t motor, uint8_t address, int32_t *value); | 读寄存器     |
| 4    | void writeRegister(uint8_t motor, uint8_t address, int32_t value); | 写寄存器     |

用户自定义区：

uint32_t userFunction(uint8_t type, uint8_t motor, int32_t *value);

内部使用接口：

| 序号 | 函数名称                                                     | 功能   |
| ---- | ------------------------------------------------------------ | ------ |
| 1    | void tmc5160_writeDatagram(uint8_t motor,uint8_t address, uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4); | 写数据 |
| 2    | void tmc5160_writeInt(uint8_t motor, uint8_t address, int value); | 写数据 |
| 3    | int tmc5160_readInt(uint8_t motor, uint8_t address);         | 读数据 |

## 4 高级功能区

基础功能区提供了基本的正、反转，停止、启动，以某个速度运行等指令，满足不了用户的高动态配置的需求。高级接口开放了这些功能，并统一做成了宏的形式供用户来调用。对高级功能区的两个函数做出详细介绍。

| 序号 | 函数名                                                     | 传入参数           |
| ---- | ---------------------------------------------------------- | ------------------ |
| 1    | uint32_t GAP(uint8_t type, uint8_t motor, int32_t *value); | 索引，电机，参数值 |
| 2    | uint32_t SAP(uint8_t type, uint8_t motor, int32_t value);  | 索引，电机，参数值 |

索引列表已全部宏定义，无需记忆，直接使用宏

| 参数索引 | 英文释义                                    | 中文释义                  |
| -------- | ------------------------------------------- | ------------------------- |
| 0        | Target position                             | 目标位置                  |
| 1        | Actual position                             | 实际位置                  |
| 2        | Target speed                                | 目标速度                  |
| 3        | Actual speed                                | 实际速度                  |
| 4        | Maximum speed                               | 最大速度                  |
| 5        | Maximum acceleration                        | 最大加速度                |
| 6        | Maximum current                             | 最大电流                  |
| 7        | Standby current                             | 待机电流                  |
| 8        | Position reached flag                       | 位置达到标志              |
| 10       | Right endstop                               | 右限位停止                |
| 11       | Left endstop                                | 左限位停止                |
| 12       | Automatic right stop                        | 自动右限位停止            |
| 13       | Automatic left stop                         | 自动左限位停止            |
| 14       | SW_MODE Register                            | 使用限位                  |
| 15       | Acceleration A1                             | 加速度A1                  |
| 16       | Velocity V1                                 | 速度V1                    |
| 17       | Maximum Deceleration                        | 最大减速                  |
| 18       | Deceleration D1                             | 减速D1                    |
| 19       | Velocity VSTART                             | 启动速度                  |
| 20       | Velocity VSTOP                              | 停止速度                  |
| 21       | Waiting time after ramp down                | 下匝道后的等待时间        |
| 23       | Speed threshold for high speed mode         | 高速模式的速度阈值        |
| 24       | Minimum speed for switching to dcStep       | 切换到dcStep的最小速率    |
| 27       | High speed chopper mode                     | 高速斩波方式              |
| 28       | High speed fullstep mode                    | 高速全步进模式            |
| 29       | Measured Speed                              | 测量速度                  |
| 33       | Analog I Scale                              | 模拟电流检测              |
| 34       | Internal RSense                             | 内部R阻值                 |
| 140      | Microstep Resolution                        | 细分数                    |
| 162      | Chopper blank time                          | 斩波的空白时间            |
| 163      | Constant TOff Mode                          | 恒定TOff模式              |
| 164      | Disable fast decay comparator               | 禁用快速衰减比较器        |
| 165      | Chopper hysteresis end / fast decay time    | 斩波滞后结束/快速衰减时间 |
| 166      | Chopper hysteresis start / sine wave offset | 斩波滞后启动/正弦波偏移   |
| 167      | Chopper off time                            | 斩波器关机时间            |
| 168      | smartEnergy current minimum (SEIMIN)        | 最小能量电流(SEIMIN)      |
| 169      | smartEnergy current down step               | smartEnergy电流下降速度   |
| 170      | smartEnergy hysteresis                      | smartEnergy滞后           |
| 171      | smartEnergy current up step                 | smartEnergy电流上升       |
| 172      | smartEnergy hysteresis start                | smartEnergy滞后开始       |
| 173      | stallGuard2 filter enable                   | stallGuard2过滤器使能     |
| 174      | stallGuard2 threshold                       | stallGuard2阈值           |
| 180      | smartEnergy actual current                  | smartEnergy实际电流       |
| 181      | smartEnergy stall velocity                  | smartEnergy失速速度       |
| 182      | smartEnergy threshold speed                 | smartEnergy阈值速度       |
| 184      | Random TOff mode                            | 随机TOff模式              |
| 185      | Chopper synchronization                     | 斩波器同步                |
| 186      | PWM threshold speed                         | PWM速度阈值               |
| 187      | PWM gradient                                | PWM梯度                   |
| 188      | PWM amplitude                               | PWM振幅                   |
| 191      | PWM frequency                               | PWM频率                   |
| 192      | PWM autoscale                               | PWM自动定标               |
| 204      | Freewheeling mode                           | 飞轮模式                  |
| 206      | Load value                                  | 负载值                    |
| 209      | Encoder position                            | 编码器的位置              |
| 210      | Encoder Resolution                          | 编码器的分辨率            |



## 5 详细文件清单

共包含8个文件，文件结构如下，总文件大小50K.

tmc5160

​    include

​        tmc5160_api_header.h

​        tmc5160_constants.h

​        tmc5160_fields.h

​        TMC5160_Register.h

​        tmc5160_type.h

​        tmc5160.h

​    tmc5160_api_header.c

​    tmc5160.c

2022.9.13
