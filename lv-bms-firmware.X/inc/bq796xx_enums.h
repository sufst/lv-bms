


#ifndef BQ796XX_ENUMS_H_
#define BQ796XX_ENUMS_H_

//****************************************************
//  ENUMs for register access types
//****************************************************

typedef enum {
    FRMWRT_SGL_R     = 0x00,   //single device READ
    FRMWRT_SGL_W	 = 0x10,   //single device WRITE
    FRMWRT_STK_R	 = 0x20,   //stack READ
    FRMWRT_STK_W	 = 0x30,   //stack WRITE
    FRMWRT_ALL_R	 = 0x40,   //broadcast READ
    FRMWRT_ALL_W	 = 0x50,   //broadcast WRITE
    FRMWRT_REV_ALL_W = 0x60,   //broadcast WRITE reverse direction
} FRMWRT_RW_TYPE_t;

//****************************************************
//  ENUMs for register fields
//****************************************************

// low pass filter for voltage monitoring
typedef enum {
    LPF_6_5Hz  = 0x0,  // 154 ms average
    LPF_13Hz   = 0x1,  // 77 ms average
    LPF_26Hz   = 0x2,  // 38 ms average
    LPF_53Hz   = 0x3,  // 19 ms average
    LPF_111Hz  = 0x4,  // 9 ms average
    LPF_240Hz  = 0x5,  // 4 ms average
    LPF_600Hz  = 0x6,  // 1.6 ms average
    LPF_240Hz_2 = 0x7  // Same as 0x5, possibly reserved or alias
} LPF_CUTOFF_t;

// duty cycle for auto cell balancing
typedef enum {
    BAL_DUTY_5S     = 0x0,  // 5 seconds
    BAL_DUTY_10S    = 0x1,  // 10 seconds
    BAL_DUTY_30S    = 0x2,  // 30 seconds
    BAL_DUTY_60S    = 0x3,  // 60 seconds
    BAL_DUTY_5MIN   = 0x4,  // 5 minutes
    BAL_DUTY_10MIN  = 0x5,  // 10 minutes
    BAL_DUTY_20MIN  = 0x6,  // 20 minutes
    BAL_DUTY_30MIN  = 0x7   // 30 minutes
} BAL_DUTY_t;

// over voltage detector thresholds
typedef enum {
    OV_THRESH_2700mV = 0x02,
    OV_THRESH_2725mV = 0x03,
    OV_THRESH_2750mV = 0x04,
    OV_THRESH_2775mV = 0x05,
    OV_THRESH_2800mV = 0x06,
    OV_THRESH_2825mV = 0x07,
    OV_THRESH_2850mV = 0x08,
    OV_THRESH_2875mV = 0x09,
    OV_THRESH_2900mV = 0x0A,
    OV_THRESH_2925mV = 0x0B,
    OV_THRESH_2950mV = 0x0C,
    OV_THRESH_2975mV = 0x0D,
    OV_THRESH_3000mV = 0x0E,

    OV_THRESH_3500mV = 0x12,
    OV_THRESH_3525mV = 0x13,
    OV_THRESH_3550mV = 0x14,
    OV_THRESH_3575mV = 0x15,
    OV_THRESH_3600mV = 0x16,
    OV_THRESH_3625mV = 0x17,
    OV_THRESH_3650mV = 0x18,
    OV_THRESH_3675mV = 0x19,
    OV_THRESH_3700mV = 0x1A,
    OV_THRESH_3725mV = 0x1B,
    OV_THRESH_3750mV = 0x1C,
    OV_THRESH_3775mV = 0x1D,
    OV_THRESH_3800mV = 0x1E,

    OV_THRESH_4175mV = 0x22,
    OV_THRESH_4200mV = 0x23,
    OV_THRESH_4225mV = 0x24,
    OV_THRESH_4250mV = 0x25,
    OV_THRESH_4275mV = 0x26,
    OV_THRESH_4300mV = 0x27,
    OV_THRESH_4325mV = 0x28,
    OV_THRESH_4350mV = 0x29,
    OV_THRESH_4375mV = 0x2A,
    OV_THRESH_4400mV = 0x2B,
    OV_THRESH_4425mV = 0x2C,
    OV_THRESH_4450mV = 0x2D,
    OV_THRESH_4475mV = 0x2E
} OV_THRESH_t;

// under voltage detection thresholds
typedef enum {
    UV_THRESH_1200mV = 0x00,
    UV_THRESH_1250mV = 0x01,
    UV_THRESH_1300mV = 0x02,
    UV_THRESH_1350mV = 0x03,
    UV_THRESH_1400mV = 0x04,
    UV_THRESH_1450mV = 0x05,
    UV_THRESH_1500mV = 0x06,
    UV_THRESH_1550mV = 0x07,
    UV_THRESH_1600mV = 0x08,
    UV_THRESH_1650mV = 0x09,
    UV_THRESH_1700mV = 0x0A,
    UV_THRESH_1750mV = 0x0B,
    UV_THRESH_1800mV = 0x0C,
    UV_THRESH_1850mV = 0x0D,
    UV_THRESH_1900mV = 0x0E,
    UV_THRESH_1950mV = 0x0F,
    UV_THRESH_2000mV = 0x10,
    UV_THRESH_2050mV = 0x11,
    UV_THRESH_2100mV = 0x12,
    UV_THRESH_2150mV = 0x13,
    UV_THRESH_2200mV = 0x14,
    UV_THRESH_2250mV = 0x15,
    UV_THRESH_2300mV = 0x16,
    UV_THRESH_2350mV = 0x17,
    UV_THRESH_2400mV = 0x18,
    UV_THRESH_2450mV = 0x19,
    UV_THRESH_2500mV = 0x1A,
    UV_THRESH_2550mV = 0x1B,
    UV_THRESH_2600mV = 0x1C,
    UV_THRESH_2650mV = 0x1D,
    UV_THRESH_2700mV = 0x1E,
    UV_THRESH_2750mV = 0x1F,
    UV_THRESH_2800mV = 0x20,
    UV_THRESH_2850mV = 0x21,
    UV_THRESH_2900mV = 0x22,
    UV_THRESH_2950mV = 0x23,
    UV_THRESH_3000mV = 0x24,
    UV_THRESH_3050mV = 0x25,
    UV_THRESH_3100mV = 0x26
} UV_THRESH_t;

// cell balancing completion threshold
//  balancing can be stopped automatically on a voltage threshold
//  Requires OVUV is running
//  Range from 2.45-V to 4-V with 25-mV steps, where
//  0x00 = Disables voltage based on CB_DONE comparison
//  0x01 = threshold of 2.45-V
//  0x3F = threshold of 4-V
// datasheet (p157- VCB_DONE_THRESH)
typedef enum {
    CB_DONE_DISABLED  = 0x00,
    CB_DONE_2450mV    = 0x01,
    CB_DONE_2475mV    = 0x02,
    CB_DONE_2500mV    = 0x03,
    CB_DONE_2525mV    = 0x04,
    CB_DONE_2550mV    = 0x05,
    CB_DONE_2575mV    = 0x06,
    CB_DONE_2600mV    = 0x07,
    CB_DONE_2625mV    = 0x08,
    CB_DONE_2650mV    = 0x09,
    CB_DONE_2675mV    = 0x0A,
    CB_DONE_2700mV    = 0x0B,
    CB_DONE_2725mV    = 0x0C,
    CB_DONE_2750mV    = 0x0D,
    CB_DONE_2775mV    = 0x0E,
    CB_DONE_2800mV    = 0x0F,
    CB_DONE_2825mV    = 0x10,
    CB_DONE_2850mV    = 0x11,
    CB_DONE_2875mV    = 0x12,
    CB_DONE_2900mV    = 0x13,
    CB_DONE_2925mV    = 0x14,
    CB_DONE_2950mV    = 0x15,
    CB_DONE_2975mV    = 0x16,
    CB_DONE_3000mV    = 0x17,
    CB_DONE_3025mV    = 0x18,
    CB_DONE_3050mV    = 0x19,
    CB_DONE_3075mV    = 0x1A,
    CB_DONE_3100mV    = 0x1B,
    CB_DONE_3125mV    = 0x1C,
    CB_DONE_3150mV    = 0x1D,
    CB_DONE_3175mV    = 0x1E,
    CB_DONE_3200mV    = 0x1F,
    CB_DONE_3225mV    = 0x20,
    CB_DONE_3250mV    = 0x21,
    CB_DONE_3275mV    = 0x22,
    CB_DONE_3300mV    = 0x23,
    CB_DONE_3325mV    = 0x24,
    CB_DONE_3350mV    = 0x25,
    CB_DONE_3375mV    = 0x26,
    CB_DONE_3400mV    = 0x27,
    CB_DONE_3425mV    = 0x28,
    CB_DONE_3450mV    = 0x29,
    CB_DONE_3475mV    = 0x2A,
    CB_DONE_3500mV    = 0x2B,
    CB_DONE_3525mV    = 0x2C,
    CB_DONE_3550mV    = 0x2D,
    CB_DONE_3575mV    = 0x2E,
    CB_DONE_3600mV    = 0x2F,
    CB_DONE_3625mV    = 0x30,
    CB_DONE_3650mV    = 0x31,
    CB_DONE_3675mV    = 0x32,
    CB_DONE_3700mV    = 0x33,
    CB_DONE_3725mV    = 0x34,
    CB_DONE_3750mV    = 0x35,
    CB_DONE_3775mV    = 0x36,
    CB_DONE_3800mV    = 0x37,
    CB_DONE_3825mV    = 0x38,
    CB_DONE_3850mV    = 0x39,
    CB_DONE_3875mV    = 0x3A,
    CB_DONE_3900mV    = 0x3B,
    CB_DONE_3925mV    = 0x3C,
    CB_DONE_3950mV    = 0x3D,
    CB_DONE_3975mV    = 0x3E,
    CB_DONE_4000mV    = 0x3F
} CB_DONE_THRESH_t;

// balancing time (datasheet p156 - CB_CELL16_CTRL through CB_CELL1_CTRL)
typedef enum {
    BAL_TIME_STOP       = 0x00, // 0 s (stop balancing)
    BAL_TIME_10S        = 0x01, // 10 s
    BAL_TIME_30S        = 0x02, // 30 s
    BAL_TIME_60S        = 0x03, // 60 s
    BAL_TIME_300S       = 0x04, // 5 min (300 s)

    BAL_TIME_10MIN      = 0x05, // 10 min
    BAL_TIME_20MIN      = 0x06, // 20 min
    BAL_TIME_30MIN      = 0x07, // 30 min
    BAL_TIME_40MIN      = 0x08, // 40 min
    BAL_TIME_50MIN      = 0x09, // 50 min
    BAL_TIME_60MIN      = 0x0A, // 60 min
    BAL_TIME_70MIN      = 0x0B, // 70 min
    BAL_TIME_80MIN      = 0x0C, // 80 min
    BAL_TIME_90MIN      = 0x0D, // 90 min
    BAL_TIME_100MIN     = 0x0E, // 100 min
    BAL_TIME_110MIN     = 0x0F, // 110 min
    BAL_TIME_120MIN     = 0x10, // 120 min

    BAL_TIME_150MIN     = 0x11, // 150 min
    BAL_TIME_180MIN     = 0x12, // 180 min
    BAL_TIME_210MIN     = 0x13, // 210 min
    BAL_TIME_240MIN     = 0x14, // 240 min
    BAL_TIME_270MIN     = 0x15, // 270 min
    BAL_TIME_300MIN     = 0x16, // 300 min
    BAL_TIME_330MIN     = 0x17, // 330 min
    BAL_TIME_360MIN     = 0x18, // 360 min
    BAL_TIME_390MIN     = 0x19, // 390 min
    BAL_TIME_420MIN     = 0x1A, // 420 min
    BAL_TIME_450MIN     = 0x1B, // 450 min
    BAL_TIME_480MIN     = 0x1C, // 480 min
    BAL_TIME_510MIN     = 0x1D, // 510 min
    BAL_TIME_540MIN     = 0x1E, // 540 min
    BAL_TIME_600MIN     = 0x1F  // 600 min
} BAL_TIME_t;

typedef enum {
    TIMEOUT_DISABLED = 0b000,   // Disables short communication timeout
    TIMEOUT_100MS    = 0b001,   // 100 ms
    TIMEOUT_2S       = 0b010,   // 2 seconds
    TIMEOUT_10S      = 0b011,   // 10 seconds
    TIMEOUT_1MIN     = 0b100,   // 1 minute
    TIMEOUT_10MIN    = 0b101,   // 10 minutes
    TIMEOUT_30MIN    = 0b110,   // 30 minutes
    TIMEOUT_1HR      = 0b111    // 1 hour
} TIMEOUT_t;

typedef enum {
    LONG_T_O_SLEEP = 0,
    LONG_T_O_SHUTDOWN = 1,
} LONG_TIMEOUT_ACTION_t;

typedef enum {
    com_dir_upward = 0,
    com_dir_downward = 1,        
}COMM_DIR_t ;

typedef enum {
    MSK_PWR             = 1 << 0,
    MSK_SYS             = 1 << 1,
    MSK_COMP            = 1 << 2,
    MSK_OV              = 1 << 3,
    MSK_UV              = 1 << 4,
    MSK_OT              = 1 << 5,
    MSK_UT              = 1 << 6,
    MSK_PROT            = 1 << 7,
    MSK_COMM1           = 1 << 8,
    MSK_COMM2           = 1 << 9,
    MSK_COMM3_HB        = 1 << 10,
    MSK_COMM3_FTONE     = 1 << 11,
    MSK_COMM3_FCOMM     = 1 << 12,
    MSK_OTP_DATA        = 1 << 13,
    MSK_OTP_CRC         = 1 << 14,
    MSK_ALL =   MSK_PWR         | MSK_SYS       | MSK_COMP      | MSK_OV | 
                MSK_UV          | MSK_OT        | MSK_UT        | MSK_PROT |
                MSK_COMM1       | MSK_COMM2     | MSK_COMM3_HB  | MSK_COMM3_HB |
                MSK_COMM3_FTONE |MSK_COMM3_FCOMM| MSK_OTP_DATA  | MSK_OTP_CRC,  

}FAULT_MASK_t;

#endif
