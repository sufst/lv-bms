

#ifndef BQ796XX_FAULT_TYPES_H_
#define BQ796XX_FAULT_TYPES_H_

// fault summary
typedef union {
    uint8_t all_flags;
    struct {
        bool fault_pwr      : 1; // Bit 0
        bool fault_sys      : 1; // Bit 1
        bool fault_ovuv     : 1; // Bit 2
        bool fault_otut     : 1; // Bit 3
        bool fault_comm     : 1; // Bit 4
        bool fault_otp      : 1; // Bit 5
        bool fault_comp_adc : 1; // Bit 6
        bool fault_prot     : 1; // Bit 7
    } bits;
} fault_summary_t;

// power faults
typedef union {
    struct {
        uint8_t fault_pwr1;
        uint8_t fault_pwr2;
        uint8_t fault_pwr3;
    } registers;
    struct {
        bool avdd_ov             : 1; // Bit 0
        bool avdd_osc            : 1; // Bit 1
        bool dvdd_ov             : 1; // Bit 2
        bool cvdd_ov             : 1; // Bit 3
        bool cvdd_uv             : 1; // Bit 4
        bool refhm_open          : 1; // Bit 5
        bool dvss_open           : 1; // Bit 6
        bool cvss_open           : 1; // Bit 7
                   
        bool tsref_ov            : 1; // Bit 0
        bool tsref_uv            : 1; // Bit 1
        bool tsref_osc           : 1; // Bit 2
        bool neg5v_uv            : 1; // Bit 3
        bool refh_osc            : 1; // Bit 4
        bool reserved_21         : 1; // Bit 5
        bool power_bist_fail     : 1; // Bit 6
        bool reserved_22         : 1; // Bit 7
                   
        bool avdduv_drst         : 1; // Bit 0
        uint8_t reserved_3       : 7; // Bit 1-7
    } bits;
} pwr_faults_t;

// sys faults
typedef union {
    uint8_t all_flags;
    struct {
        bool twarn         : 1; // Bit 0
        bool tshut         : 1; // Bit 1
        bool cts           : 1; // Bit 2
        bool ctl           : 1; // Bit 3
        bool drst          : 1; // Bit 4
        bool gpio          : 1; // Bit 5
        bool reserved_1    : 1; // Bit 6
        bool lfo           : 1; // Bit 7
    } bits;
} sys_faults_t;

// voltage faults
typedef union {
    struct {
        uint8_t fault_ov1;
        uint8_t fault_ov2;
        uint8_t fault_uv1;
        uint8_t fault_uv2;     
    } registers;
    struct {
        // FAULT_OV1 (0x53C)
        bool ov9_det  : 1; // bit 0
        bool ov10_det : 1; // bit 1
        bool ov11_det : 1; // bit 2
        bool ov12_det : 1; // bit 3
        bool ov13_det : 1; // bit 4
        bool ov14_det : 1; // bit 5
        bool ov15_det : 1; // bit 6
        bool ov16_det : 1; // bit 7

        // FAULT_OV2 (0x53D)
        bool ov1_det  : 1; // bit 0 
        bool ov2_det  : 1; // bit 1
        bool ov3_det  : 1; // bit 2
        bool ov4_det  : 1; // bit 3
        bool ov5_det  : 1; // bit 4
        bool ov6_det  : 1; // bit 5
        bool ov7_det  : 1; // bit 6
        bool ov8_det  : 1; // bit 7

        // FAULT_UV1 (0x53E)
        bool uv9_det  : 1; // bit 0
        bool uv10_det : 1; // bit 1
        bool uv11_det : 1; // bit 2
        bool uv12_det : 1; // bit 3
        bool uv13_det : 1; // bit 4
        bool uv14_det : 1; // bit 5
        bool uv15_det : 1; // bit 6
        bool uv16_det : 1; // bit 7

        // FAULT_UV2 (0x53F)
        bool uv1_det  : 1; // bit 0
        bool uv2_det  : 1; // bit 1
        bool uv3_det  : 1; // bit 2
        bool uv4_det  : 1; // bit 3
        bool uv5_det  : 1; // bit 4
        bool uv6_det  : 1; // bit 5
        bool uv7_det  : 1; // bit 6
        bool uv8_det  : 1; // bit 7
    } bits;
} ovuv_faults_t;

// temperature faults
typedef union {
    struct {
        uint8_t fault_ot;
        uint8_t fault_ut;
    } registers;
    struct {
        bool ot1_det  : 1; // bit 0 
        bool ot2_det  : 1; // bit 1
        bool ot3_det  : 1; // bit 2
        bool ot4_det  : 1; // bit 3
        bool ot5_det  : 1; // bit 4
        bool ot6_det  : 1; // bit 5
        bool ot7_det  : 1; // bit 6
        bool ot8_det  : 1; // bit 7
        
        bool ut1_det  : 1; // bit 0
        bool ut2_det  : 1; // bit 1
        bool ut3_det  : 1; // bit 2
        bool ut4_det  : 1; // bit 3
        bool ut5_det  : 1; // bit 4
        bool ut6_det  : 1; // bit 5
        bool ut7_det  : 1; // bit 6
        bool ut8_det  : 1; // bit 7
    } bits;
} otut_faults_t;

// communication faults
typedef union {
    struct {
        uint8_t fault_comm1;
        uint8_t fault_comm2;
        uint8_t fault_comm3;
    } registers;
    struct {
        bool stop_det                  : 1; // Bit 0
        bool comm_clr_det              : 1; // Bit 1
        bool uart_rc                   : 1; // Bit 2
        bool uart_rr                   : 1; // Bit 3
        bool uart_tr                   : 1; // Bit 4
        uint8_t reserved_1             : 3; // Bit 5 - 7
        
        bool comh_bit                  : 1; // Bit 0
        bool comh_rc                   : 1; // Bit 1
        bool comh_rr                   : 1; // Bit 2
        bool comh_tr                   : 1; // Bit 3
        bool coml_bit                  : 1; // Bit 4
        bool coml_rc                   : 1; // Bit 5
        bool coml_rr                   : 1; // Bit 6
        bool coml_tr                   : 1; // Bit 7
        
        bool hb_fast                   : 1; // Bit 0
        bool hb_fail                   : 1; // Bit 1
        bool ftone_det                 : 1; // Bit 2
        bool fcomm_det                 : 1; // Bit 3
        uint8_t reserved_3             : 4; // Bit 4 - 7
    } bits;
} comm_faults_t;

// otp faults
typedef union {
    uint8_t all_flags;
    struct {
        bool gblov_err                 : 1; // Bit 0
        bool factld_err                : 1; // Bit 1
        bool custld_err                : 1; // Bit 2
        bool fact_crc                  : 1; // Bit 3
        bool cust_crc                  : 1; // Bit 4
        bool sec_det                   : 1; // Bit 5
        bool ded_det                   : 1; // Bit 6
        bool reserved                  : 1; // Bit 7
    };
} otp_faults_t;

// adc comparision faults
//      accessed as [feature].registers.[reg] or [feature].bits.[bit_id]
typedef struct {
    // fault_comp_gpio
    union {
        struct {
            uint8_t r1;
        } registers;
        struct {
            bool gpio1_fail            : 1; // Bit 0
            bool gpio2_fail            : 1; // Bit 1
            bool gpio3_fail            : 1; // Bit 2
            bool gpio4_fail            : 1; // Bit 3
            bool gpio5_fail            : 1; // Bit 4
            bool gpio6_fail            : 1; // Bit 5
            bool gpio7_fail            : 1; // Bit 6
            bool gpio8_fail            : 1; // Bit 7
        } bits;
    }fault_comp_gpio;
    
    // fault_comp_vccb
    union {
        struct {
            uint8_t high;
            uint8_t low;
        } registers;
        struct {
            bool cell_9   : 1; // bit 0 
            bool cell_10  : 1; // bit 1
            bool cell_11  : 1; // bit 2
            bool cell_12  : 1; // bit 3
            bool cell_13  : 1; // bit 4
            bool cell_14  : 1; // bit 5
            bool cell_15  : 1; // bit 6
            bool cell_16  : 1; // bit 7

            bool cell_1  : 1; // bit 0
            bool cell_2  : 1; // bit 1
            bool cell_3  : 1; // bit 2
            bool cell_4  : 1; // bit 3
            bool cell_5  : 1; // bit 4
            bool cell_6  : 1; // bit 5
            bool cell_7  : 1; // bit 6
            bool cell_8  : 1; // bit 7
        } bits;
    }fault_comp_vccb;
    
    // fault_comp_vcow
    union {
        struct {
            uint8_t high;
            uint8_t low;
        } registers;
        struct {
            bool cell_9   : 1; // bit 0 
            bool cell_10  : 1; // bit 1
            bool cell_11  : 1; // bit 2
            bool cell_12  : 1; // bit 3
            bool cell_13  : 1; // bit 4
            bool cell_14  : 1; // bit 5
            bool cell_15  : 1; // bit 6
            bool cell_16  : 1; // bit 7

            bool cell_1  : 1; // bit 0
            bool cell_2  : 1; // bit 1
            bool cell_3  : 1; // bit 2
            bool cell_4  : 1; // bit 3
            bool cell_5  : 1; // bit 4
            bool cell_6  : 1; // bit 5
            bool cell_7  : 1; // bit 6
            bool cell_8  : 1; // bit 7
        } bits;
    }fault_comp_vcow;
    
    // fault_comp_cbow
    union {
        struct {
            uint8_t high;
            uint8_t low;
        } registers;
        struct {
            bool cell_9   : 1; // bit 0 
            bool cell_10  : 1; // bit 1
            bool cell_11  : 1; // bit 2
            bool cell_12  : 1; // bit 3
            bool cell_13  : 1; // bit 4
            bool cell_14  : 1; // bit 5
            bool cell_15  : 1; // bit 6
            bool cell_16  : 1; // bit 7

            bool cell_1  : 1; // bit 0
            bool cell_2  : 1; // bit 1
            bool cell_3  : 1; // bit 2
            bool cell_4  : 1; // bit 3
            bool cell_5  : 1; // bit 4
            bool cell_6  : 1; // bit 5
            bool cell_7  : 1; // bit 6
            bool cell_8  : 1; // bit 7
        } bits;
    }fault_comp_cbow;
    
    // fault_comp_cbfet
    union {
        struct {
            uint8_t high;
            uint8_t low;
        } registers;
        struct {
            bool cell_9   : 1; // bit 0 
            bool cell_10  : 1; // bit 1
            bool cell_11  : 1; // bit 2
            bool cell_12  : 1; // bit 3
            bool cell_13  : 1; // bit 4
            bool cell_14  : 1; // bit 5
            bool cell_15  : 1; // bit 6
            bool cell_16  : 1; // bit 7

            bool cell_1  : 1; // bit 0
            bool cell_2  : 1; // bit 1
            bool cell_3  : 1; // bit 2
            bool cell_4  : 1; // bit 3
            bool cell_5  : 1; // bit 4
            bool cell_6  : 1; // bit 5
            bool cell_7  : 1; // bit 6
            bool cell_8  : 1; // bit 7
        } bits;
    }fault_comp_cbfet;
    
    // fault_comp_misc
    union {
        struct {
            uint8_t r1;
        } registers;
        struct {
            bool lpf_fail              : 1; // Bit 0
            bool comp_adc_abort        : 1; // Bit 1
            uint8_t reserved           : 6; // Bit 2 - 7
        } bits;
    }fault_comp_misc;
} comp_adc_faults_t;
    
    

// ovuv otut protector faults
typedef union {
    struct {
        uint8_t fault_prot1;
        uint8_t fault_prot2;
    } registers;
    struct {
        bool vparity_fail   : 1;
        bool tparity_fail   : 1;
        uint8_t reserved_1  : 6;
        
        bool uvcomp_fail       : 1; // bit 0
        bool ovcomp_fail       : 1; // bit 1
        bool otcomp_fail       : 1; // bit 2
        bool utcomp_fail       : 1; // bit 3
        bool vpath_fail        : 1; // bit 4
        bool tpath_fail        : 1; // bit 5
        bool bist_abort        : 1; // bit 6
        bool reserved_2        : 1; // bit 7
    } bits;
} prot_fault_t;

#endif
