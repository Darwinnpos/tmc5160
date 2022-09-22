/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-26     Fpi_lc       the first version
 */
#ifndef APPLICATIONS_TMC5160_TMC5160_H_

#define APPLICATIONS_TMC5160_TMC5160_H_

#include "tmc5160_api_header.h"
#include "TMC5160_Register.h"
#include "TMC5160_Constants.h"
#include "tmc5160_fields.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
    uint32_t vstart;
    uint32_t a1;
    uint32_t v1;
    uint32_t amax;
    uint32_t vmax;
    uint32_t dmax;
    uint32_t d1;
    uint32_t vstop;
    uint32_t rampmode;
    int32_t  x_target;
    int32_t  x_actual;
}TMC_START_PARA;



//Migrate from board.h by fpi-lc on 2021.11.27

typedef enum {
    TMC_COMM_SPI,
    TMC_COMM_UART,
    TMC_COMM_WLAN
} TMC_Board_Comm_Mode;

typedef enum {
    DRIVER_DISABLE,
    DRIVER_ENABLE,
    DRIVER_USE_GLOBAL_ENABLE
} DriverState;


#define READ   0
#define WRITE  1

typedef enum {
    TMC_ERROR_TYPE = 0x04,
    TMC_ERROR_ADDRESS = 0x04,
    TMC_ERROR_NOT_DONE = 0x20
} EvalboardErrorBit;


//Migrate from board.h by fpi-lc on 2021.11.27

#define TMC5160_FIELD_READ(motor, address, mask, shift)           FIELD_READ(tmc5160_readInt, motor, address, mask, shift)
#define TMC5160_FIELD_WRITE(motor, address, mask, shift, value)   FIELD_WRITE(tmc5160_writeInt, motor, address, mask, shift, value)
#define TMC5160_FIELD_UPDATE(motor, address, mask, shift, value)  FIELD_UPDATE(tmc5160_readInt, tmc5160_writeInt, motor, address, mask, shift, value)

// Factor between 10ms units and internal units for 16MHz
//#define TPOWERDOWN_FACTOR (4.17792*100.0/255.0)
// TPOWERDOWN_FACTOR = k * 100 / 255 where k = 2^18 * 255 / fClk for fClk = 16000000)

typedef struct
{
    int velocity, oldX;
    uint32_t oldTick;
    int32_t registerResetState[TMC5160_REGISTER_COUNT];
    uint8_t registerAccess[TMC5160_REGISTER_COUNT];
    uint8_t channels[TMC5160_MOTORS];
} TMC5160TypeDef;

void tmc5160_initConfig(TMC5160TypeDef *tmc5160);
void tmc5160_periodicJob(uint8_t motor, uint32_t tick, TMC5160TypeDef *tmc5160, ConfigurationTypeDef *TMC5160_config);
uint8_t tmc5160_reset(ConfigurationTypeDef *TMC5160_config);
uint8_t tmc5160_restore(ConfigurationTypeDef *TMC5160_config);

//GAP SAP function TYPE_DEFINE AND VALUE_DEFINE
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-26     fpi-inc  lc     the first version
 */

#define TYPE_ID_TARGET_POSITION             0// min.: 0, max.: 1, default: 0
#define TYPE_ID_ACTUAL_POSITION             1// min.: 0, max.: 1, default: 0
#define TYPE_ID_TARGET_SPEED                2// min.: 0, max.: 1, default: 0
#define TYPE_ID_ACTUAL_SPEED                3// min.: 0, max.: 1, default: 0
#define TYPE_ID_MAX_SPEED                   4// min.: 0, max.: 1, default: 0
#define TYPE_ID_MAX_ACCERATION              5// min.: 0, max.: 1, default: 0
#define TYPE_ID_MAXMIUM_CURRENT             6// min.: 0, max.: 1, default: 0
#define TYPE_ID_STANBY_CURRENT              7// min.: 0, max.: 1, default: 0
#define TYPE_ID_POSITION_REACHED_FLAG       8// min.: 0, max.: 1, default: 0
#define TYPE_ID_RIGHT_END_STOP              10// min.: 0, max.: 1, default: 0
#define TYPE_ID_LEFT_END_STOP               11// min.: 0, max.: 1, default: 0
#define TYPE_ID_AUTOMATIC_RIGHT_STOP        12// min.: 0, max.: 1, default: 0
#define TYPE_ID_AUTOMATIC_LEFT_STOP         13// min.: 0, max.: 1, default: 0
#define TYPE_ID_SW_MODE_REGISTER            14// min.: 0, max.: 1, default: 0

#define TYPE_ID_ACCLERATION_A1              15// min.: 0, max.: 1, default: 0
#define TYPE_ID_VELOCITY_V1                 16// min.: 0, max.: 1, default: 0
#define TYPE_ID_MAXIMUN_DECELERATION        17// min.: 0, max.: 1, default: 0
#define TYPE_ID_DECELERATION_D1             18// min.: 0, max.: 1, default: 0
#define TYPE_ID_VELOCITY_VSTART             19// min.: 0, max.: 1, default: 0
#define TYPE_ID_VELOCITY_VSTOP              20// min.: 0, max.: 1, default: 0

#define TYPE_ID_WAITING_TIME_AFTER_RAMP_DOWN            21// min.: 0, max.: 1, default: 0
#define TYPE_ID_SPEED_THRESHOD_FOR_HIGH_SPEED_MODE      23// min.: 0, max.: 1, default: 0
#define TYPE_ID_MINIMUN_SPEED_FOR_SWITCHING_TO_DCSTEP   24// min.: 0, max.: 1, default: 0
#define TYPE_ID_HIGH_SPEED_CHOPPER_MODE                 27// min.: 0, max.: 1, default: 0
#define TYPE_ID_HIGH_SPEED_FULLSTEP_MODE                28// min.: 0, max.: 1, default: 0
#define TYPE_ID_MEASURED_SPEED                          29// min.: 0, max.: 1, default: 0

#define TYPE_ID_ANALOG_I_SCALE                          33// min.: 0, max.: 1, default: 0
#define TYPE_ID_INTERNAL_R_SENSE                        34// min.: 0, max.: 1, default: 0

#define TYPE_ID_MICROSTEP_RESOLUTION                                140// min.: 0, max.: 1, default: 0
#define TYPE_ID_CHOPPER_BLANK_TIME                                  162// min.: 0, max.: 1, default: 0
#define TYPE_ID_CONSTANT_TOFF_MODE                                  163// min.: 0, max.: 1, default: 0
#define TYPE_ID_DISABLE_FAST_DECAY_COMPARATOR                       164// min.: 0, max.: 1, default: 0
#define TYPE_ID_CHOPPER_HESTERESIS_END_FAST_DECAY_TIME              165// min.: 0, max.: 1, default: 0
#define TYPE_ID_CHOPPER_HESTERESIS_START_SINE_WAVE_OFFSET           166// min.: 0, max.: 1, default: 0
#define TYPE_ID_CHOPPER_OFFTIME                                     167// min.: 0, max.: 1, default: 0
#define TYPE_ID_SMART_ENEGRY_CURRENT_MINIMUM                        168// min.: 0, max.: 1, default: 0
#define TYPE_ID_SMART_ENEGRY_CURRENT_DOWM_STEP                      169// min.: 0, max.: 1, default: 0
#define TYPE_ID_SMART_ENEGRY_HESTERESIS                             170// min.: 0, max.: 1, default: 0
#define TYPE_ID_SMART_ENEGRY_CURRENT_UP_STEP                        171// min.: 0, max.: 1, default: 0
#define TYPE_ID_SMART_ENEGRY_HESTERESIS_START                       172// min.: 0, max.: 1, default: 0

#define TYPE_ID_STALL_GUARD2_FILTER_ENBALE                          173// min.: 0, max.: 1, default: 0
#define TYPE_ID_STALL_GUARD2_THRESHOLD                              174// min.: 0, max.: 1, default: 0

#define TYPE_ID_SMART_ENEGRY_ACTUAL_CURRENT                         180// min.: 0, max.: 1, default: 0
#define TYPE_ID_SMART_ENEGRY_STALL_VELOCITY                         181// min.: 0, max.: 1, default: 0

#define TYPE_ID_SMART_ENEGRY_THRESHOLD_SPEED                        182// min.: 0, max.: 1, default: 0

#define TYPE_ID_RANDOM_TOFF_MODE           184// min.: 0, max.: 1, default: 0
#define TYPE_ID_CHOPPER_SYNCHRONIZATION    185// min.: 0, max.: 1, default: 0
#define TYPE_ID_PWM_THRESHOLD_SPEED        186// min.: 0, max.: 1, default: 0
#define TYPE_ID_PWM_GRADIENT               187// min.: 0, max.: 1, default: 0
#define TYPE_ID_PWM_AMPLITUDE              188// min.: 0, max.: 1, default: 0
#define TYPE_ID_PWM_FREQURENCY             191// min.: 0, max.: 1, default: 0
#define TYPE_ID_PWM_AUTO_SCALE             204// min.: 0, max.: 1, default: 0
#define TYPE_ID_LOAD_VALUE                 206// min.: 0, max.: 1, default: 0
#define TYPE_ID_ENCODE_POSITON             209// min.: 0, max.: 1, default: 0
#define TYPE_ID_ENCODE_RESOLUTION          210// min.: 0, max.: 1, default: 0

//function declaration

uint32_t TMC5160_init(uint8_t motor);

void SPIReadWrite(uint8_t chl,uint8_t *send_data,uint8_t *recv_data);
uint32_t right(uint8_t motor, int32_t velocity);
uint32_t left(uint8_t motor, int32_t velocity);
uint32_t rotate(uint8_t motor, int32_t velocity);
uint32_t start(uint8_t motor,TMC_START_PARA start_para);
uint32_t stop(uint8_t motor);
uint32_t moveTo(uint8_t motor, int32_t position);
uint32_t moveBy(uint8_t motor, int32_t *ticks);
uint32_t GAP(uint8_t type, uint8_t motor, int32_t *value);
uint32_t SAP(uint8_t type, uint8_t motor, int32_t value);
void readRegister(uint8_t motor, uint8_t address, int32_t *value);
void writeRegister(uint8_t motor, uint8_t address, int32_t value);
uint32_t getMeasuredSpeed(uint8_t motor, int32_t *value);

void periodicJob(uint32_t tick);
void checkErrors(uint32_t tick);
void deInit(void);
uint32_t userFunction(uint8_t type, uint8_t motor, int32_t *value);

uint8_t reset();
void enableDriver(DriverState state);

void tmc5160_writeDatagram(uint8_t motor, uint8_t address, uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4);
void tmc5160_writeInt(uint8_t motor, uint8_t address, int value);
int tmc5160_readInt(uint8_t motor, uint8_t address);


//extern function declaration
//extern int (*SPIReadWrite)(int chl,int data, int lastchar);


#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_TMC_TMC5160_H_ */
