/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-27     Fpi_lc       the first version
 */
#ifndef _TMC5160_TMC5160_API_HEADER_H_
#define _TMC5160_TMC5160_API_HEADER_H_


/*   include files  */
#include <stdlib.h>
//#include "stdint.h"
#include "tmc5160_type.h"
#include "tmc5160_constants.h"


// TODO: Restructure these.
/*
 * Goal: Just give these values here as status back to the IDE when used with EvalSystem.
 * Currently, this is obtained by just leaving out implementation specific error bits here.
 */
typedef enum {
    TMC_ERROR_NONE      = 0x00,
    TMC_ERROR_GENERIC   = 0x01,
    TMC_ERROR_FUNCTION  = 0x02,
    TMC_ERROR_MOTOR     = 0x08,
    TMC_ERROR_VALUE     = 0x10,
    TMC_ERROR_CHIP      = 0x40
} TMCError;



/*
 * Config.h
 *
 *  Created on: 13.06.2018
 *      Author: LK
 */


// Callback functions have IC-dependent parameters
// To store the function pointers we use this dummy type, which is never
// called without casting it to the IC-specific type first.
// (Casting between function pointers is allowed by the C standard)
typedef void (*tmc_callback_config)(void);

// States of a configuration
typedef enum {
    CONFIG_READY,
    CONFIG_RESET,
    CONFIG_RESTORE
} ConfigState;

// structure for configuration mechanism
typedef struct
{
    ConfigState          state;
    uint8_t                configIndex;
    int32_t                shadowRegister[TMC_REGISTER_COUNT];
    uint8_t (*reset)       (void);
    uint8_t (*restore)     (void);
    tmc_callback_config  callback;
    uint8_t                   channel;
} ConfigurationTypeDef;




/*
 * Macros.h
 *
 *  Created on: 04.01.2018
 *      Author: LH
 */


/* Cast a n bit signed int to a 32 bit signed int
 * This is done by checking the MSB of the signed int (Bit n).
 * If it is 1, the value is negative and the Bits 32 to n+1 are set to 1
 * If it is 0, the value remains unchanged
 */
#define CAST_Sn_TO_S32(value, n) ((value) | (((value) & (1<<((n)-1)))? ~((0x1<<(n))-1) : 0 ))

// Min/Max macros
#ifndef MIN
    #define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef MAX
    #define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif

// Static Array length
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

// Generic mask/shift macros
#define FIELD_GET(data, mask, shift) \
    (((data) & (mask)) >> (shift))
#define FIELD_SET(data, mask, shift, value) \
    (((data) & (~(mask))) | (((value) << (shift)) & (mask)))

// Register read/write/update macros using Mask/Shift:
#define FIELD_READ(read, motor, address, mask, shift) \
    FIELD_GET(read(motor, address), mask, shift)
#define FIELD_WRITE(write, motor, address, mask, shift, value) \
    (write(motor, address, ((value)<<(shift)) & (mask)))
#define FIELD_UPDATE(read, write, motor, address, mask, shift, value) \
    (write(motor, address, FIELD_SET(read(motor, address), mask, shift, value)))

// Macro to surpress unused parameter warnings
#ifndef UNUSED
    #define UNUSED(x) (void)(x)
#endif

// Memory access helpers
// Force the compiler to access a location exactly once
#define ACCESS_ONCE(x) *((volatile typeof(x) *) (&x))

// Macro to remove write bit for shadow register array access
#define TMC_ADDRESS(x) ((x) & (TMC_ADDRESS_MASK))


/*
 * bit_define.h
 *
 *  Created on: 04.01.2018
 *      Author: LH
 */

    #define  BIT0   0x00000001
    #define  BIT1   0x00000002
    #define  BIT2   0x00000004
    #define  BIT3   0x00000008
    #define  BIT4   0x00000010
    #define  BIT5   0x00000020
    #define  BIT6   0x00000040
    #define  BIT7   0x00000080
    #define  BIT8   0x00000100
    #define  BIT9   0x00000200
    #define  BIT10  0x00000400
    #define  BIT11  0x00000800
    #define  BIT12  0x00001000
    #define  BIT13  0x00002000
    #define  BIT14  0x00004000
    #define  BIT15  0x00008000
    #define  BIT16  0x00010000
    #define  BIT17  0x00020000
    #define  BIT18  0x00040000
    #define  BIT19  0x00080000
    #define  BIT20  0x00100000
    #define  BIT21  0x00200000
    #define  BIT22  0x00400000
    #define  BIT23  0x00800000
    #define  BIT24  0x01000000
    #define  BIT25  0x02000000
    #define  BIT26  0x04000000
    #define  BIT27  0x08000000
    #define  BIT28  0x10000000
    #define  BIT29  0x20000000
    #define  BIT30  0x40000000
    #define  BIT31  0x80000000

    #define BYTE0_MASK   0x00000000000000FF
    #define BYTE0_SHIFT  0
    #define BYTE1_MASK   0x000000000000FF00
    #define BYTE1_SHIFT  8
    #define BYTE2_MASK   0x0000000000FF0000
    #define BYTE2_SHIFT  16
    #define BYTE3_MASK   0x00000000FF000000
    #define BYTE3_SHIFT  24
    #define BYTE4_MASK   0x000000FF00000000
    #define BYTE4_SHIFT  32
    #define BYTE5_MASK   0x0000FF0000000000
    #define BYTE5_SHIFT  40
    #define BYTE6_MASK   0x00FF000000000000
    #define BYTE6_SHIFT  48
    #define BYTE7_MASK   0xFF00000000000000
    #define BYTE7_SHIFT  56

    #define SHORT0_MASK   (BYTE0_MASK|BYTE1_MASK)
    #define SHORT0_SHIFT  BYTE0_SHIFT
    #define SHORT1_MASK   (BYTE2_MASK|BYTE3_MASK)
    #define SHORT1_SHIFT  BYTE2_SHIFT
    #define SHORT2_MASK   (BYTE4_MASK|BYTE5_MASK)
    #define SHORT2_SHIFT  BYTE4_SHIFT
    #define SHORT3_MASK   (BYTE6_MASK|BYTE7_MASK)
    #define SHORT3_SHIFT  BYTE6_SHIFT

    #define WORD0_MASK    (SHORT0_MASK|SHORT1_MASK)
    #define WORD0_SHIFT   SHORT0_SHIFT
    #define WORD1_MASK    (SHORT2_MASK|SHORT3_MASK)
    #define WORD1_SHIFT   SHORT2_SHIFT

    #define NIBBLE(value, n)  (((value) >> ((n) << 2)) & 0x0F)
    #define BYTE(value, n)    (((value) >> ((n) << 3)) & 0xFF)
    #define SHORT(value, n)   (((value) >> ((n) << 4)) & 0xFFFF)
    #define WORD(value, n)    (((value) >> ((n) << 5)) & 0xFFFFFFFF)

    #define _8_16(__1, __0)                                (((__1) << BYTE1_SHIFT) | ((__0) << BYTE0_SHIFT))

    #define _8_32(__3, __2, __1, __0)                      (((__3) << BYTE3_SHIFT) | ((__2) << BYTE2_SHIFT) | ((__1) << BYTE1_SHIFT) | ((__0) << BYTE0_SHIFT))
    #define _16_32(__1, __0)                               (((__1) << SHORT1_SHIFT) | ((__0) << SHORT0_SHIFT))

    #define _8_64(__7, __6, __5, __4, __3, __2, __1, __0)  (((__7) << BYTE7_SHIFT) | ((__6) << BYTE6_SHIFT) | ((__5) << BYTE5_SHIFT) | ((__4) << BYTE4_SHIFT) | ((__3) << BYTE3_SHIFT) | ((__2) << BYTE2_SHIFT) | ((__1) << BYTE1_SHIFT) | ((__0) << BYTE0_SHIFT))
    #define _16_64(__3, __2, __1, __0)                     (((__3) << SHORT3_SHIFT) | ((__2) << SHORT2_SHIFT) | ((__1) << SHORT1_SHIFT) | ((__0) << SHORT0_SHIFT))
    #define _32_64(__1, __0)                               (((__1) << WORD1_SHIFT) | ((__0) << WORD0_SHIFT))



/*
 * CRC.h
 *
 *  Created on: 04.12.2017
 *      Author: LH
 */


// Amount of CRC tables available
// Each table takes ~260 bytes (257 bytes, one bool and structure padding)
#define CRC_TABLE_COUNT 2

uint8_t tmc_fillCRC8Table(uint8_t polynomial, bool isReflected, uint8_t index);
uint8_t tmc_CRC8(uint8_t *data, uint32_t bytes, uint8_t index);

uint8_t tmc_tableGetPolynomial(uint8_t index);
bool  tmc_tableIsReflected(uint8_t index);



/*
 * RegisterAccess.h
 *
 *  Created on: 12.07.2017
 *      Author: LK
 *
 *  The permission system aims to allow a general-purpose implementation for
 *  all common hardware register usages. This includes:
 *  - Trivial Cases: Read, Write, Read & Write
 *
 *  - Read & Write accesses that route to different values/functions of a chip.
 *    (e.g. serial communication, where read/write corresponds to RX/TX)
 *  - Read to clear, write to clear. This does not directly affect the access,
 *    but can be used to implement a software shadow register for flags
 *    (ORing the read value into a shadow register instead of overwriting).
 *  - Registers with default values that are not known (e.g. Factory configuration
 *    values that should not be overwritten by default).
 */


// Register access bits
/* Lower nibble is used for read/write, higher nibble is used for
 * special case registers. This makes it easy to identify the read/write
 * part of the permissions in a hexadecimal permission number.
 * The dirty bit will only ever be set at runtime, so we keep the easily
 * readable lower nibble.
 */
#define TMC_ACCESS_NONE        0x00

#define TMC_ACCESS_READ        0x01
#define TMC_ACCESS_WRITE       0x02
                            // 0x04 is currently unused
#define TMC_ACCESS_DIRTY       0x08  // Register has been written since reset -> shadow register is valid for restore

// Special Register bits
#define TMC_ACCESS_RW_SPECIAL  0x10  // Read and write are independent - different values and/or different functions
#define TMC_ACCESS_FLAGS       0x20  // Register has read or write to clear flags.
#define TMC_ACCESS_HW_PRESET   0x40  // Register has hardware presets (e.g. Factory calibrations) - do not write a default value
                            // 0x80 is currently unused

// Permission combinations
#define TMC_ACCESS_RW              (TMC_ACCESS_READ  | TMC_ACCESS_WRITE)        // 0x03 - Read and write
#define TMC_ACCESS_RW_SEPARATE     (TMC_ACCESS_RW    | TMC_ACCESS_RW_SPECIAL)   // 0x13 - Read and write, with separate values/functions
#define TMC_ACCESS_R_FLAGS         (TMC_ACCESS_READ  | TMC_ACCESS_FLAGS)        // 0x21 - Read, has flags (read to clear)
#define TMC_ACCESS_RW_FLAGS        (TMC_ACCESS_RW    | TMC_ACCESS_FLAGS)        // 0x23 - Read and write, has flags (read or write to clear)
#define TMC_ACCESS_W_PRESET        (TMC_ACCESS_WRITE | TMC_ACCESS_HW_PRESET)    // 0x42 - Write, has hardware preset - skipped in reset routine
#define TMC_ACCESS_RW_PRESET       (TMC_ACCESS_RW    | TMC_ACCESS_HW_PRESET)    // 0x43 - Read and write, has hardware presets - skipped in reset routine

// Helper macros
#define TMC_IS_READABLE(x)    ((x) & TMC_ACCESS_READ)
#define TMC_IS_WRITABLE(x)    ((x) & TMC_ACCESS_WRITE)
#define TMC_IS_DIRTY(x)       ((x) & TMC_ACCESS_DIRTY)
#define TMC_IS_RESETTABLE(x)  (((x) & (TMC_ACCESS_W_PRESET)) == TMC_ACCESS_WRITE) // Write bit set, Hardware preset bit not set
#define TMC_IS_RESTORABLE(x)  (((x) & TMC_ACCESS_WRITE) && (!(x & TMC_ACCESS_HW_PRESET) || (x & TMC_ACCESS_DIRTY))) // Write bit set, if it's a hardware preset register, it needs to be dirty

// Struct for listing registers that have constant contents which we cannot
// obtain by reading them due to the register not being read-back.
typedef struct
{
    uint8_t address;
    int32_t value;
} TMCRegisterConstant;

// Helper define:
// Most register permission arrays are initialized with 128 values.
// In those fields its quite hard to have an easy overview of available
// registers. For that, ____ is defined to 0, since 4 underscores are
// very easy to distinguish from the 2-digit hexadecimal values.
// This way, the used registers (permission != ACCESS_NONE) are easily spotted
// amongst unused (permission == ACCESS_NONE) registers.
#define ____ 0x00

// Helper define:
// Default reset values are not used if the corresponding register has a
// hardware preset. Since this is not directly visible in the default
// register reset values array, N_A is used as an indicator for a preset
// value, where any value will be ignored anyways (N_A: not available).
#define N_A 0









#endif /* APPLICATIONS_TMC5160_TMC5160_API_HEADER_H_ */
