/*
 *  @file bq796xx.c
 *
 *  @author Vince Toledo - Texas Instruments Inc.
 *  @date February 2020
 *  @version 1.2
 *  @note Built with CCS for Hercules Version: 8.1.0.00011
 *  @note Built for TMS570LS1224 (LAUNCH XL2)
 */

/* 
* Copyright (C) 2011-2022 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


//#include "bq796xx_spiMaster.h"
#include <xc.h>
#include "bq796xx.h"
#include "millis.h"
#include "uart4.h"
#include "mcc.h"
#include "BQ796XXA0_reg.h"

#define TRUE 1
#define FALSE 0
#define delayms(x) delay(x)
#define delayus(x) delay(x/1000)
#define BIT(x) (1 << x)
#define RESPONSE_DATA_START_INDEX    4U
#define ACCUMULATION_DATA_SIZE       4U
#define SW_OUTPUT_OFF                0U
#define SW_OUTPUT_LOW                1U
#define SW_OUTPUT_HIGH               2U
#define VLSB_CS                      0.1
#define TCS_REFRESH                  64U


//GLOBAL VARIABLES (use these to avoid stack overflows by creating too many function variables)
//avoid creating variables/arrays in functions, or you will run out of stack space quickly
#if UART_COMM == TRUE
uint8_t response_frame2[(MAXBYTES+6)*TOTALBOARDS]; //response frame to be used by every read
uint8_t fault_frame[39*TOTALBOARDS]; //hold fault frame if faults are printed
#elif SPI_COMM == TRUE
uint16_t response_frame2[(MAXBYTES+6)*TOTALBOARDS]; //response frame to be used by every read
uint16_t fault_frame[39*TOTALBOARDS]; //hold fault frame if faults are printed
#endif

char tmp_string[780];
uint8_t currentCell = 0;
uint8_t currentBoard = 0;
uint8_t bReturn = 0;
int bRes = 0;
uint32_t count = 10000;
uint8_t bBuf[8];
uint8_t pFrame[64];
uint16_t wCRC = 0;
uint16_t wCRC16 = 0;
int crc_i = 0;
static volatile unsigned int delayval = 0; //for delayms and delayus functions
char resultNames[24][10] =
{
 "VF2",
 "VF1",
 "CP",
 "RSVD",
 "CURRENT1",
 "CURRENT2",
 "OC1",
 "OC2",
 "RSVD",
 "GPIO1",
 "GPIO2",
 "GPIO3",
 "GPIO4",
 "GPIO5",
 "GPIO6",
 "GPIO7",
 "GPIO8",
 "GPIO9",
 "GPIO10",
 "GPIO11",
 "GPIO12",
 "GPIO13",
 "GPIO14",
 "GPIO15"
};

FRMWRT_RW_TYPE_t WriteType = FRMWRT_SGL_W; 
FRMWRT_RW_TYPE_t ReadType = FRMWRT_SGL_R;

// internal register access function declarations
int WriteReg(uint8_t bID, uint16_t wAddr, uint64_t dwData, uint8_t bLen, FRMWRT_RW_TYPE_t bWriteType);
int ReadReg(uint8_t bID, uint16_t wAddr, uint8_t * pData, uint8_t bLen, uint32_t dwTimeOut, FRMWRT_RW_TYPE_t bWriteType);
int WriteFrame(uint8_t bID, uint16_t wAddr, uint8_t * pData, uint8_t bLen, FRMWRT_RW_TYPE_t bWriteType);
int ReadFrameReq(uint8_t bID, uint16_t wAddr, uint8_t bByteToReturn, FRMWRT_RW_TYPE_t bWriteType);
uint16_t dma1_read_from_uart(uint8_t* rx_data_buff, uint16_t rx_len, uint32_t timeout);

// debug logging
void log_reg( const char* format, ... ) {
    if (bq796xx_log_level >= BQ_LOG_REG) {
        va_list args;
        va_start( args, format );
        printf("REG : ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_dbg( const char* format, ... ) {
    if (bq796xx_log_level >= BQ_LOG_DBG) {
        va_list args;
        va_start( args, format );
        printf("DBG : ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_info( const char* format, ... ) {
    if (bq796xx_log_level >= BQ_LOG_INFO) {
        va_list args;
        va_start( args, format );
        printf("INFO: ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_warn( const char* format, ... ) {
    if (bq796xx_log_level >= BQ_LOG_WARN) {
        va_list args;
        va_start( args, format );
        printf("WARN: ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_err( const char* format, ... ) {
    if (bq796xx_log_level >= BQ_LOG_ERR) {
        va_list args;
        va_start( args, format );
        printf("ERR : ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}


// *****************************************************************************
//  power state management
// *****************************************************************************
void Wake796XX(void) {
    log_info("wake");
//    sciREG->GCR1 &= ~(1U << 7U); // put SCI into reset
//    sciREG->PIO0 &= ~(1U << 2U); // disable transmit function - now a GPIO
//    sciREG->PIO3 &= ~(1U << 2U); // set output to low
    INTERRUPT_GlobalInterruptDisable();
    // toggle uart TX low for 2.5ms
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    delay(2); // WAKE ping = 2ms to 2.5ms
    for(int _ = 0; _ <100; _++ ); // little more delay
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    
    INTERRUPT_GlobalInterruptEnable();
    
    // wait 12 ms for the device to power up
    delay(12);
    
//    sciInit();
//    sciSetBaudrate(sciREG, BAUDRATE);
    log_dbg("wake done");
}

void SD796XX(void) {
    log_info("shutdown");
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    delay(7); // SD ping = 7ms to 10ms
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    log_dbg("shutdown done");
}

void StA796XX(void) {
    log_info("sleep to active");
    INTERRUPT_GlobalInterruptDisable();
    
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL    
    for(int _ = 0; _ <90; _++ );  // StA ping = 250us to 300us
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL

    INTERRUPT_GlobalInterruptEnable();
    log_dbg("sleep to active done");
}

void HWRST796XX(void) {
    log_info("HW reset");
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    delay(36); // reset ping = 36ms
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    
    delay(75); // wait for reset
    log_dbg("HW reset done");
}

// *****************************************************************************
//  generic config 
// *****************************************************************************
void set_config(uint8_t bID, dev_conf_t conf) {
    log_info("set config 0x%02x", conf);
    set_reg_value(bID, DEV_CONF, (uint8_t)conf);
    log_dbg("set config done");
}

void set_active_cells(uint8_t bID, uint8_t cell_count) {
    log_info("set active cells %d", cell_count);
    
    if(cell_count > 16) {
        log_err("invalid cell count: %d", cell_count);
        return;
    }
    
    if(cell_count < 6) {
        log_warn("cell count of %d requested, capping to min of 6", cell_count);
        cell_count = 6;
    }
    
    set_reg_value(bID, ACTIVE_CELL, cell_count-6);
    
    log_dbg("set active cells done");
}

void set_bb_loc(uint8_t bID, uint8_t loc) { // BBP_LOC register - datasheet P130
    if (loc > 0x10) {
        log_err("invalid BBP_LOC value requested: 0x%02x", loc);
        return;
    }
    
    log_info("set BBP_loc 0x%02x", loc);
    set_reg_value(bID, BBP_LOC, loc);
}

// *****************************************************************************
//  communications
// *****************************************************************************

// reset uart engine on the bq79616
void COM_CLR_796XX(void) {
    log_info("com clear");
    INTERRUPT_GlobalInterruptDisable();
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    // 15-20 bit periods @ 1Mb/s = 15-20 us 
    for(int _ = 0; _ <6; _++ );  
    U4CON2 = U4CON2 ^ (1 << 2); // invert TXPOL
    INTERRUPT_GlobalInterruptEnable();
    log_dbg("com clear done");
}

int AutoAddress(COMM_DIR_t dir)
{
    log_info("auto addressing");
    uint8_t autoaddr_response_frame[64+5];
    //DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL (IF A DEVICE RESET OCCURED PRIOR TO THIS)
        WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
        WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);

        //ENABLE AUTO ADDRESSING MODE
        WriteReg(0, CONTROL1, 0X01, 1, FRMWRT_ALL_W);

        //SET ADDRESSES FOR EVERY BOARD
        for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
        {
            WriteReg(0, DIR0_ADDR, currentBoard, 1, FRMWRT_ALL_W);
        }

        //BROADCAST WRITE TO SET ALL DEVICES AS STACK DEVICE
        WriteReg(0, COMM_CTRL, 0x02, 1, FRMWRT_ALL_W);

        //SET THE HIGHEST DEVICE IN THE STACK AS BOTH STACK AND TOP OF STACK
        WriteReg(TOTALBOARDS-1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W);

        //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
        ReadReg(0, OTP_ECC_DATAIN1, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN2, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN3, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN4, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN5, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN6, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN7, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);
        ReadReg(0, OTP_ECC_DATAIN8, autoaddr_response_frame, 1, 0, FRMWRT_STK_R);

        //OPTIONAL: read back all device addresses
        for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
        {
            ReadReg(currentBoard, DIR0_ADDR, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);
        }

        //OPTIONAL: read register address 0x2001 and verify that the value is 0x14
        ReadReg(0, 0x2001, autoaddr_response_frame, 1, 0, FRMWRT_SGL_R);

        log_dbg("auto addressing done");
        return -1;
}

//RUN BASIC REVERSE ADDRESSING SEQUENCE
void ReverseAddressing()
{
    log_info("reverse addressing");
    //CHANGE BASE DEVICE DIRECTION
    WriteReg(0, CONTROL1, 0x80, 1, FRMWRT_SGL_W);

    //CHANGE REST OF STACK DIRECTION
    WriteReg(0, CONTROL1, 0x80, 1, FRMWRT_REV_ALL_W);

    delayus(50);

    WriteReg(0, COMM_CTRL, 0x00, 1, FRMWRT_ALL_W);

    //DO NORMAL AUTO ADDRESS SEQUENCE, BUT FOR DIR1_ADDR
    //DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL (IF A DEVICE RESET OCCURED PRIOR TO THIS)
    WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);

    //ENABLE AUTO ADDRESSING MODE, WHILE KEEPING REVERSE DIRECTION
    WriteReg(0, CONTROL1, 0X81, 1, FRMWRT_ALL_W);

    //SET ADDRESSES FOR EVERY BOARD (REVERSE DIRECTION)
    for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
    {
        WriteReg(0, DIR1_ADDR, currentBoard, 1, FRMWRT_ALL_W);
        delayus(50);
        //ReadReg(currentBoard, DIR1_ADDR, response_frame2, 1, 0, FRMWRT_SGL_R);
        delayus(50);
    }

    WriteReg(0, COMM_CTRL, 0x02, 1, FRMWRT_ALL_W); //set everything as a stack device first

    if(TOTALBOARDS==1) //if there's only 1 board, it's the base AND top of stack, so change it to those
    {
        WriteReg(0, COMM_CTRL, 0x01, 1, WriteType);
    }
    else //otherwise set the base and top of stack individually
    {

        WriteReg(TOTALBOARDS-1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W);
    }

    //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
    ReadReg(0, OTP_ECC_DATAIN1, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN2, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN3, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN4, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN5, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN6, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN7, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN8, response_frame2, 1, 0, FRMWRT_STK_R);

    ////OPTIONAL: read back all device addresses
    //for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
    //{
    //    ReadReg(currentBoard, DIR0_ADDR, response_frame2, 1, 0, ReadType);
    //    printf("board %d\n",response_frame2[4]);
    //}

    //RESET ANY COMM FAULT CONDITIONS FROM STARTUP
    WriteReg(0, FAULT_RST2, 0x03, 1, WriteType);
    log_dbg("reverse addressing done");
}

void enable_daisy_chain(bool en) {
    log_info("enable daisy chain: %d", en);
    Wake796XX();
    dev_conf_t conf = get_reg_value(0, DEV_CONF);
    if(en) {
        conf &= !DEV_CONF_MULTIDROP_EN;
        conf |= DEV_CONF_FCOMM_EN;
    } else {
        conf |= !DEV_CONF_MULTIDROP_EN;
        conf &= !DEV_CONF_FCOMM_EN;
    }
    WriteReg(0, DEV_CONF, conf, 1, FRMWRT_ALL_W);
    
    log_info("enable daisy chain done");
}

void daisy_chain_dll_sync() {
    log_info("daisy chain dll sync");
    
    //DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL (IF A DEVICE RESET OCCURED PRIOR TO THIS)
    log_dbg("syncing write (up stack)");
    WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);
    
    log_dbg("syncing read (down stack)");
    ReadReg(0, OTP_ECC_DATAIN1, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN2, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN3, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN4, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN5, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN6, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN7, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN8, response_frame2, 1, 0, FRMWRT_STK_R);
    
    log_dbg("daisy chain dll sync done");
}

// *****************************************************************************
//  Register write and read functions
// *****************************************************************************

uint8_t get_reg_value(uint8_t bID, uint16_t addr) {
    uint8_t rx_buff[7] = {0};
    ReadReg(bID, addr, rx_buff, 1, 100, FRMWRT_SGL_R);
    log_reg("reading dev: %d, reg 0x%02x = 0x%02x", bID, addr, rx_buff[4]);
    return rx_buff[4];
}

void set_reg_value(uint8_t bID, uint16_t addr, uint8_t data) {
    log_reg("writting dev: %d, reg 0x%02x = 0x%02x", bID, addr, data);
    WriteReg(bID, addr, data, 1, FRMWRT_SGL_W);
}

//FORMAT WRITE DATA, SEND TO
//BE COMBINED WITH REST OF FRAME
int WriteReg(uint8_t bID, uint16_t wAddr, uint64_t dwData, uint8_t bLen, FRMWRT_RW_TYPE_t bWriteType) {
    // device address, register start address, data bytes, data length, write type (single, broadcast, stack)
    bRes = 0;
    memset(bBuf,0,sizeof(bBuf));
    switch (bLen) {
    case 1:
        bBuf[0] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 1, bWriteType);
        break;
    case 2:
        bBuf[0] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[1] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 2, bWriteType);
        break;
    case 3:
        bBuf[0] = (dwData & 0x0000000000FF0000) >> 16;
        bBuf[1] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[2] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 3, bWriteType);
        break;
    case 4:
        bBuf[0] = (dwData & 0x00000000FF000000) >> 24;
        bBuf[1] = (dwData & 0x0000000000FF0000) >> 16;
        bBuf[2] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[3] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 4, bWriteType);
        break;
    case 5:
        bBuf[0] = (dwData & 0x000000FF00000000) >> 32;
        bBuf[1] = (dwData & 0x00000000FF000000) >> 24;
        bBuf[2] = (dwData & 0x0000000000FF0000) >> 16;
        bBuf[3] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[4] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 5, bWriteType);
        break;
    case 6:
        bBuf[0] = (dwData & 0x0000FF0000000000) >> 40;
        bBuf[1] = (dwData & 0x000000FF00000000) >> 32;
        bBuf[2] = (dwData & 0x00000000FF000000) >> 24;
        bBuf[3] = (dwData & 0x0000000000FF0000) >> 16;
        bBuf[4] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[5] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 6, bWriteType);
        break;
    case 7:
        bBuf[0] = (dwData & 0x00FF000000000000) >> 48;
        bBuf[1] = (dwData & 0x0000FF0000000000) >> 40;
        bBuf[2] = (dwData & 0x000000FF00000000) >> 32;
        bBuf[3] = (dwData & 0x00000000FF000000) >> 24;
        bBuf[4] = (dwData & 0x0000000000FF0000) >> 16;
        bBuf[5] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[6] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 7, bWriteType);
        break;
    case 8:
        bBuf[0] = (dwData & 0xFF00000000000000) >> 56;
        bBuf[1] = (dwData & 0x00FF000000000000) >> 48;
        bBuf[2] = (dwData & 0x0000FF0000000000) >> 40;
        bBuf[3] = (dwData & 0x000000FF00000000) >> 32;
        bBuf[4] = (dwData & 0x00000000FF000000) >> 24;
        bBuf[5] = (dwData & 0x0000000000FF0000) >> 16;
        bBuf[6] = (dwData & 0x000000000000FF00) >> 8;
        bBuf[7] = dwData & 0x00000000000000FF;
        bRes = WriteFrame(bID, wAddr, bBuf, 8, bWriteType);
        break;
    default:
        break;
    }
    return bRes;
}

//GENERATE READ COMMAND FRAME AND THEN WAIT FOR RESPONSE DATA (INTERRUPT MODE FOR SCIRX)
int ReadReg(uint8_t bID,                   // device id
                uint16_t wAddr,               // register address
                uint8_t * pData,               // rx data buffer
                uint8_t bLen,                  // rx data buffer len
                uint32_t dwTimeOut,           // timeout for the rx operation (cpu cycles)
                FRMWRT_RW_TYPE_t bWriteType // what type of read is this
                ) {
    bRes = 0;
    count = dwTimeOut; //timeout after this many attempts
    
    switch (bWriteType){
        case FRMWRT_SGL_R:
//            memset(pData, 0, sizeof(pData)); // unsure what this does - it seems to just zero the first entry of the rx data buffer
            ReadFrameReq(bID, wAddr, bLen, bWriteType);
            bRes = (int)dma1_read_from_uart(pData, bLen + 6, dwTimeOut);
            break;
            
        case FRMWRT_STK_R:
            bRes = ReadFrameReq(bID, wAddr, bLen, bWriteType);
//            memset(pData, 0, sizeof(pData)); // unsure what this does - it seems to just zero the first entry of the rx data buffer
            for(bRes = 0; bRes < (bLen + 6) * (TOTALBOARDS - 1); bRes++) {
                while(!UART4_is_rx_ready() && count>0) count--; /* Wait for data*/
                if(count == 0) return bRes;
                pData[bRes] = UART4_Read();
            }
            break;
            
        default:
            bRes = 0;
            break;
    }

    //CHECK IF CRC IS CORRECT
    for(crc_i=0; crc_i<bRes; crc_i+=(bLen+6))
    {
        if(CRC16(&pData[crc_i], bLen+6)!=0)
        {
            log_warn("BAD CRC reading reg: 0x%02x", wAddr);
        }
    }
    
    return bRes;
}

//GENERATE COMMAND FRAME
int WriteFrame(uint8_t bID, uint16_t wAddr, uint8_t * pData, uint8_t bLen, FRMWRT_RW_TYPE_t bWriteType) {
    int bPktLen = 0;
    uint8_t * pBuf = pFrame;
    memset(pFrame, 0x7F, sizeof(pFrame));
    *pBuf++ = 0x80 | ((uint8_t)bWriteType) | (((uint8_t)bWriteType & 0x10) ? bLen - 0x01 : 0x00); //Only include blen if it is a write; Writes are 0x90, 0xB0, 0xD0
    if (bWriteType == FRMWRT_SGL_R || bWriteType == FRMWRT_SGL_W)
    {
        *pBuf++ = (bID & 0x00FF);
    }
    *pBuf++ = (wAddr & 0xFF00) >> 8;
    *pBuf++ = wAddr & 0x00FF;

    while (bLen--)
        *pBuf++ = *pData++;

    bPktLen = pBuf - pFrame;

    wCRC = CRC16(pFrame, bPktLen);
    *pBuf++ = wCRC & 0x00FF;
    *pBuf++ = (wCRC & 0xFF00) >> 8;
    bPktLen += 2;
    //THIS SEEMS to occasionally drop bytes from the frame. Sometimes is not sending the last frame of the CRC.
    //(Seems to be caused by stack overflow, so take precautions to reduce stack usage in function calls)
//    sciSend(sciREG, bPktLen, pFrame);
    // send bytes
    
    for(int i = 0; i < bPktLen; i++) {
        while(!UART4_is_tx_ready()); 
        UART4_Write(pFrame[i]);
    }

    return bPktLen;
}

int ReadFrameReq(uint8_t bID, uint16_t wAddr, uint8_t bByteToReturn, FRMWRT_RW_TYPE_t bWriteType) {
    bReturn = bByteToReturn - 1;

    if (bReturn > 127)
        return 0;

    return WriteFrame(bID, wAddr, &bReturn, 1, bWriteType);
}

// read from uart into buffer using DMA. blocks until timeout or rx_len bytes have been read
// returns number of bytes read
uint16_t dma1_read_from_uart(uint8_t* rx_data_buff, uint16_t rx_len, uint32_t timeout) {
    // naive implementaton - too slow so we have to use DMA
//    for(int bRes = 0; bRes < rx_len; bRes++) {
//        while(!UART4_is_rx_ready() && count>0) count--; /* Wait for data*/
//        if(count == 0) break;
//        rx_data_buff[bRes] = UART4_Read();
//    }
//    return bRes;
    
    // ensure we're accessing the right DMA channel
    DMASELECT = 0;
    // set destination       
    DMA1_SetDestinationAddress((uint16_t)rx_data_buff);
    DMA1_SetDestinationSize(rx_len);
    
    // load dest and dest len into the DMA's internal registers by turning it off and on again
    DMAnCON0bits.EN=0; // turn it off
    DMAnCON0bits.EN=1; // turn it on
    
    // enable triggering from the uart interrupt
    DMA1_StartTransferWithTrigger();
    // wait for the length of message to be received
    uint32_t count = timeout;
    while(DMAnCON0bits.SIRQEN && count > 0) count--;
    DMA1_StopTransfer();
    
    // if the correct number of bytes are RX'ed, then the DMAnDCNT resets back 
    // to rx_len, so we can't always use it to tell how many were RX'ed
    if (count > 0) {
        return rx_len; // everything expected received
    } else {
        return rx_len - DMAnDCNT; // timed out
    }
}

// *****************************************************************************
// utilities
// *****************************************************************************

// CRC16 TABLE
// ITU_T polynomial: x^16 + x^15 + x^2 + 1
const uint16_t crc16_table[256] = { 0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301,
        0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1,
        0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81,
        0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00,
        0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
        0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380,
        0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141,
        0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501,
        0x35C0, 0x3480, 0xF441, 0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0,
        0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881,
        0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401,
        0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1,
        0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0, 0x6180,
        0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
        0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01,
        0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1,
        0xA881, 0x6840, 0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80,
        0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200,
        0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1,
        0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780,
        0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41,
        0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901,
        0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
        0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80,
        0x8C41, 0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };

uint16_t CRC16(uint8_t *pBuf, int nLen) {
    uint16_t wCRC = 0xFFFF;
    int i;

    for (i = 0; i < nLen; i++) {
        wCRC ^= (*pBuf++) & 0x00FF;
        wCRC = crc16_table[wCRC & 0x00FF] ^ (wCRC >> 8);
    }

    return wCRC;
}
//****************************
//END WRITE AND READ FUNCTIONS
//****************************

;
// *****************************************************************************
//  fault handling
// *****************************************************************************

bool GetFaultStat() {

/*    if (!gioGetBit(gioPORTA, 0))
        return 0;
    return 1;*/
    
    return nFAULT_GetValue();
}

void PrintAllFaults(uint8_t bID, FRMWRT_RW_TYPE_t bWriteType)
{
    //PRINT 39 REGISTERS STARTING FROM FAULT_SUMMARY (INCLUDES RESERVED REGISTERS)
    printf("\n");
    currentBoard = 0;
    currentCell = 0;
    memset(fault_frame,0,sizeof(fault_frame));
    if(bWriteType==ReadType)
    {
        ReadReg(0, FAULT_SUMMARY, fault_frame, 39, 0, ReadType);
        for(currentBoard = 0; currentBoard<TOTALBOARDS; currentBoard++)
        {
            printf("BOARD %d FAULTS:\t",TOTALBOARDS-currentBoard);
            for(currentCell = 0; currentCell<39; currentCell++)
            {
                printf("%02x ",fault_frame[(currentBoard*(39+6))+4+currentCell]);
            }
            printf("\n");
        }
    }
    else if(bWriteType==ReadType)
    {
        ReadReg(bID, FAULT_SUMMARY, fault_frame, 39, 0, ReadType);
        printf("BOARD %d FAULTS:\t",bID);
        for(currentCell = 0; currentCell<39; currentCell++)
        {
            printf("%02x ",fault_frame[4+currentCell]);
        }
        printf("\n");
    }
    else if(bWriteType==FRMWRT_STK_R)
    {
        ReadReg(0, FAULT_SUMMARY, fault_frame, 39, 0, FRMWRT_STK_R);
        for(currentBoard = 0; currentBoard<(TOTALBOARDS-1); currentBoard++)
        {
            printf("BOARD %d FAULTS:\t",TOTALBOARDS-currentBoard);
            for(currentCell = 0; currentCell<39; currentCell++)
            {
                printf("%02x ",fault_frame[(currentBoard*(39+6))+4+currentCell]);
            }
            printf("\n");
        }
    }
    else
    {
        printf("ERROR: PrintAllFaults bWriteType incorrect\n");
    }
    printf("\n");
}

void set_fault_msk(uint8_t bID, FAULT_MASK_t mask) {
    set_reg_value(bID, FAULT_MSK1, mask & (0x00ff));
    set_reg_value(bID, FAULT_MSK2, mask >> 8);
    log_info("set fault mask: 0x%04x", mask);
}

void reset_faults(uint8_t bID, FAULT_MASK_t mask) {
    set_reg_value(bID, FAULT_RST1, mask & (0x00ff));
    set_reg_value(bID, FAULT_RST2, mask >> 8);
    log_info("reset faults: 0x%04x", mask);
}

fault_summary_t get_fault_summary(uint8_t bID) {
    fault_summary_t fs;
    fs.all_flags = get_reg_value(bID, FAULT_SUMMARY);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_fault_summary(tmp_string, sizeof tmp_string, fs);
        log_info(tmp_string);
    }
    
    return fs;
}

pwr_faults_t get_pwr_faults(uint8_t bID) {
    pwr_faults_t pf;
    pf.registers.fault_pwr1 = get_reg_value(bID, FAULT_PWR1);
    pf.registers.fault_pwr2 = get_reg_value(bID, FAULT_PWR2);
    pf.registers.fault_pwr3 = get_reg_value(bID, FAULT_PWR3);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_pwr_faults(tmp_string, sizeof tmp_string, pf);
        log_info(tmp_string);
    }
    
    return pf;
}

sys_faults_t get_sys_faults(uint8_t bID) {
    sys_faults_t sf;
    sf.all_flags = get_reg_value(bID, FAULT_SYS);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_sys_faults(tmp_string, sizeof tmp_string, sf);
        log_info(tmp_string);
    }
    
    return sf;
}

ovuv_faults_t get_ovuv_faults(uint8_t bID) {
    ovuv_faults_t vf;
    vf.registers.fault_ov1 = get_reg_value(bID, FAULT_OV1);
    vf.registers.fault_ov2 = get_reg_value(bID, FAULT_OV2);
    vf.registers.fault_uv1 = get_reg_value(bID, FAULT_UV1);
    vf.registers.fault_uv2 = get_reg_value(bID, FAULT_UV2);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_ovuv_faults(tmp_string, sizeof tmp_string, vf);
        log_info(tmp_string);
    }
    
    return vf;
}

otut_faults_t get_otut_faults(uint8_t bID) {
    otut_faults_t tf;
    tf.registers.fault_ot = get_reg_value(bID, FAULT_OT);
    tf.registers.fault_ut = get_reg_value(bID, FAULT_UT);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_otut_faults(tmp_string, sizeof tmp_string, tf);
        log_info(tmp_string);
    }
    
    return tf;
}

comm_faults_t get_comm_faults(uint8_t bID) {
    comm_faults_t cf;
    cf.registers.fault_comm1 = get_reg_value(bID, FAULT_COMM1);
    cf.registers.fault_comm2 = get_reg_value(bID, FAULT_COMM2);
    cf.registers.fault_comm3 = get_reg_value(bID, FAULT_COMM3);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_comm_faults(tmp_string, sizeof tmp_string, cf);
        log_info(tmp_string);
    }
    
    return cf;
}

otp_faults_t get_otp_faults(uint8_t bID) {
    otp_faults_t of;
    of.all_flags = get_reg_value(bID, FAULT_OTP);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_otp_faults(tmp_string, sizeof tmp_string, of);
        log_info(tmp_string);
    }
    
    return of;
}

comp_adc_faults_t get_comp_adc_faults(uint8_t bID) {
    comp_adc_faults_t af;
    af.fault_comp_cbfet.registers.high = get_reg_value(bID, FAULT_COMP_CBFET1);
    af.fault_comp_cbfet.registers.low  = get_reg_value(bID, FAULT_COMP_CBFET2);
    af.fault_comp_cbow.registers.high  = get_reg_value(bID, FAULT_COMP_CBOW1);
    af.fault_comp_cbow.registers.low   = get_reg_value(bID, FAULT_COMP_CBOW2);
    af.fault_comp_gpio.registers.r1    = get_reg_value(bID, FAULT_COMP_GPIO);
    af.fault_comp_misc.registers.r1    = get_reg_value(bID, FAULT_COMP_MISC);
    af.fault_comp_vccb.registers.high  = get_reg_value(bID, FAULT_COMP_VCCB1);
    af.fault_comp_vccb.registers.low   = get_reg_value(bID, FAULT_COMP_VCCB2);
    af.fault_comp_vcow.registers.high  = get_reg_value(bID, FAULT_COMP_VCOW1);
    af.fault_comp_vcow.registers.low   = get_reg_value(bID, FAULT_COMP_VCOW2);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_adc_comp_faults(tmp_string, sizeof tmp_string, af);
        log_info(tmp_string);
    }
    
    return af;
}

prot_fault_t get_prot_faults(uint8_t bID) {
    prot_fault_t pf;
    pf.registers.fault_prot1 = get_reg_value(bID, FAULT_PROT1);
    pf.registers.fault_prot2 = get_reg_value(bID, FAULT_PROT2);
    
    if(bq796xx_log_level >= BQ_LOG_INFO) {
        snprint_prot_faults(tmp_string, sizeof tmp_string, pf);
        log_info(tmp_string);
    }
    
    return pf;
}

// these are quite big so they're doing to be toggle-able
#ifdef BQ796XX_FAULT_PRINTS
int snprint_fault_summary(char * s, size_t n, fault_summary_t fs) {
    return snprintf(s, n, "fault summary: PROT:%d, COMP_ADC:%d, OTP:%d, COMM:%d, OTUT:%d, OVUV:%d, SYS:%d, PWR:%d ",
            fs.bits.fault_prot,
            fs.bits.fault_comp_adc,
            fs.bits.fault_otp,
            fs.bits.fault_comm,
            fs.bits.fault_otut,
            fs.bits.fault_ovuv,
            fs.bits.fault_sys,
            fs.bits.fault_pwr);
}

int snprint_pwr_faults(char * s, size_t n, pwr_faults_t pf) {
    return snprintf(s, n,
        "pwr faults: "
        "AVDDUV_DRST:%d, "
        "POWER_BIST_FAIL:%d, REFH_OSC:%d, NEG5V_UV:%d, TSREF_OSC:%d, TSREF_UV:%d, TSREF_OV:%d, "
        "CVSS_OPEN:%d, DVSS_OPEN:%d, REFHM_OPEN:%d, CVDD_UV:%d, CVDD_OV:%d, DVDD_OV:%d, AVDD_OSC:%d, AVDD_OV:%d",
        pf.bits.avdduv_drst,
        pf.bits.power_bist_fail, pf.bits.refh_osc, pf.bits.neg5v_uv, pf.bits.tsref_osc, pf.bits.tsref_uv, pf.bits.tsref_ov,
        pf.bits.cvss_open, pf.bits.dvss_open, pf.bits.refhm_open, pf.bits.cvdd_uv, pf.bits.cvdd_ov, pf.bits.dvdd_ov, pf.bits.avdd_osc, pf.bits.avdd_ov
    );
}

int snprint_sys_faults(char * s, size_t n, sys_faults_t sf) {
    return snprintf(s, n,
        "sys faults: LFO:%d, GPIO:%d, DRST:%d, CTL:%d, CTS:%d, TSHUT:%d, TWARN:%d",
        sf.bits.lfo,
        sf.bits.gpio,
        sf.bits.drst,
        sf.bits.ctl,
        sf.bits.cts,
        sf.bits.tshut,
        sf.bits.twarn
    );
}

int snprint_ovuv_faults(char * s, size_t n, ovuv_faults_t vf) {
    return snprintf(s, n,
        "ovuv faults:\n"
        "UV16:%d, UV15:%d, UV14:%d, UV13:%d, UV12:%d, UV11:%d, UV10:%d, UV9:%d, "
        "UV8:%d, UV7:%d, UV6:%d, UV5:%d, UV4:%d, UV3:%d, UV2:%d, UV1:%d\n"
        
        "OV16:%d, OV15:%d, OV14:%d, OV13:%d, OV12:%d, OV11:%d, OV10:%d, OV9:%d, "
        "OV8:%d, OV7:%d, OV6:%d, OV5:%d, OV4:%d, OV3:%d, OV2:%d, OV1:%d",
            
        vf.bits.uv16_det, vf.bits.uv15_det, vf.bits.uv14_det, vf.bits.uv13_det,
        vf.bits.uv12_det, vf.bits.uv11_det, vf.bits.uv10_det, vf.bits.uv9_det,
            
        vf.bits.uv8_det, vf.bits.uv7_det, vf.bits.uv6_det, vf.bits.uv5_det,
        vf.bits.uv4_det, vf.bits.uv3_det, vf.bits.uv2_det, vf.bits.uv1_det,

        vf.bits.ov16_det, vf.bits.ov15_det, vf.bits.ov14_det, vf.bits.ov13_det,
        vf.bits.ov12_det, vf.bits.ov11_det, vf.bits.ov10_det, vf.bits.ov9_det,
            
        vf.bits.ov8_det, vf.bits.ov7_det, vf.bits.ov6_det, vf.bits.ov5_det,
        vf.bits.ov4_det, vf.bits.ov3_det, vf.bits.ov2_det, vf.bits.ov1_det
    );
}

int snprint_otut_faults(char * s, size_t n, otut_faults_t tf) {
    return snprintf(s, n,
        "otut faults:\n"
        "UT8:%d, UT7:%d, UT6:%d, UT5:%d, UT4:%d, UT3:%d, UT2:%d, UT1:%d\n"
        "OT8:%d, OT7:%d, OT6:%d, OT5:%d, OT4:%d, OT3:%d, OT2:%d, OT1:%d",
        tf.bits.ut8_det, tf.bits.ut7_det, tf.bits.ut6_det, tf.bits.ut5_det,
        tf.bits.ut4_det, tf.bits.ut3_det, tf.bits.ut2_det, tf.bits.ut1_det,

        tf.bits.ot8_det, tf.bits.ot7_det, tf.bits.ot6_det, tf.bits.ot5_det,
        tf.bits.ot4_det, tf.bits.ot3_det, tf.bits.ot2_det, tf.bits.ot1_det
    );
}

int snprint_comm_faults(char * s, size_t n, comm_faults_t cf) {
    return snprintf(s, n,
        "comm faults: "
        "FCOMM_DET:%d, FTONE_DET:%d, HB_FAIL:%d, HB_FAST:%d, "
        "COML_TR:%d, COML_RR:%d, COML_RC:%d, COML_BIT:%d, COMH_TR:%d, COMH_RR:%d, COMH_RC:%d, COMH_BIT:%d, "
        "UART_TR:%d, UART_RR:%d, UART_RC:%d, COMM_CLR_DET:%d, STOP_DET:%d",
        cf.bits.fcomm_det, cf.bits.ftone_det, cf.bits.hb_fail, cf.bits.hb_fast,
        cf.bits.coml_tr, cf.bits.coml_rr, cf.bits.coml_rc, cf.bits.coml_bit,
        cf.bits.comh_tr, cf.bits.comh_rr, cf.bits.comh_rc, cf.bits.comh_bit,
        cf.bits.uart_tr, cf.bits.uart_rr, cf.bits.uart_rc, cf.bits.comm_clr_det, cf.bits.stop_det
    );
}

int snprint_otp_faults(char * s, size_t n, otp_faults_t of) {
    return snprintf(s, n,
        "otp faults: DED_DET:%d, SEC_DET:%d, CUST_CRC:%d, FACT_CRC:%d, CUSTLD_ERR:%d, FACTLD_ERR:%d, GBLOV_ERR:%d",
        of.ded_det,
        of.sec_det,
        of.cust_crc,
        of.fact_crc,
        of.custld_err,
        of.factld_err,
        of.gblov_err
    );
}

int snprint_adc_comp_faults(char * s, size_t n, comp_adc_faults_t cf) {
    return snprintf(s, n,
        "adc comp faults: \n"
        "GPIO8:%d, GPIO7:%d, GPIO6:%d, GPIO5:%d, GPIO4:%d, GPIO3:%d, GPIO2:%d, GPIO1:%d, \n"
            
        "VCCB16:%d, VCCB15:%d, VCCB14:%d, VCCB13:%d, VCCB12:%d, VCCB11:%d, VCCB10:%d, VCCB9:%d, \n"
        "VCCB8 :%d, VCCB7 :%d, VCCB6 :%d, VCCB5 :%d, VCCB4 :%d, VCCB3 :%d, VCCB2 :%d, VCCB1:%d, \n"
            
        "VCOW16:%d, VCOW15:%d, VCOW14:%d, VCOW13:%d, VCOW12:%d, VCOW11:%d, VCOW10:%d, VCOW9:%d, \n"
        "VCOW8 :%d, VCOW7 :%d, VCOW6 :%d, VCOW5 :%d, VCOW4 :%d, VCOW3 :%d, VCOW2 :%d, VCOW1:%d, \n"
            
        "CBOW16:%d, CBOW15:%d, CBOW14:%d, CBOW13:%d, CBOW12:%d, CBOW11:%d, CBOW10:%d, CBOW9:%d, \n"
        "CBOW8 :%d, CBOW7 :%d, CBOW6 :%d, CBOW5 :%d, CBOW4 :%d, CBOW3 :%d, CBOW2 :%d, CBOW1:%d, \n"
            
        "CBFET16:%d, CBFET15:%d, CBFET14:%d, CBFET13:%d, CBFET12:%d, CBFET11:%d, CBFET10:%d, CBFET9:%d, \n"
        "CBFET8 :%d, CBFET7 :%d, CBFET6 :%d, CBFET5 :%d, CBFET4 :%d, CBFET3 :%d, CBFET2 :%d, CBFET1:%d, \n"
            
        "COMP_ADC_ABORT:%d, LPF_FAIL:%d",
            
        cf.fault_comp_gpio.bits.gpio8_fail, cf.fault_comp_gpio.bits.gpio7_fail,
        cf.fault_comp_gpio.bits.gpio6_fail, cf.fault_comp_gpio.bits.gpio5_fail,
        cf.fault_comp_gpio.bits.gpio4_fail, cf.fault_comp_gpio.bits.gpio3_fail,
        cf.fault_comp_gpio.bits.gpio2_fail, cf.fault_comp_gpio.bits.gpio1_fail,

        cf.fault_comp_vccb.bits.cell_16, cf.fault_comp_vccb.bits.cell_15,
        cf.fault_comp_vccb.bits.cell_14, cf.fault_comp_vccb.bits.cell_13,
        cf.fault_comp_vccb.bits.cell_12, cf.fault_comp_vccb.bits.cell_11,
        cf.fault_comp_vccb.bits.cell_10, cf.fault_comp_vccb.bits.cell_9,
        cf.fault_comp_vccb.bits.cell_8, cf.fault_comp_vccb.bits.cell_7,
        cf.fault_comp_vccb.bits.cell_6, cf.fault_comp_vccb.bits.cell_5,
        cf.fault_comp_vccb.bits.cell_4, cf.fault_comp_vccb.bits.cell_3,
        cf.fault_comp_vccb.bits.cell_2, cf.fault_comp_vccb.bits.cell_1,

        cf.fault_comp_vcow.bits.cell_16, cf.fault_comp_vcow.bits.cell_15,
        cf.fault_comp_vcow.bits.cell_14, cf.fault_comp_vcow.bits.cell_13,
        cf.fault_comp_vcow.bits.cell_12, cf.fault_comp_vcow.bits.cell_11,
        cf.fault_comp_vcow.bits.cell_10, cf.fault_comp_vcow.bits.cell_9,
        cf.fault_comp_vcow.bits.cell_8, cf.fault_comp_vcow.bits.cell_7,
        cf.fault_comp_vcow.bits.cell_6, cf.fault_comp_vcow.bits.cell_5,
        cf.fault_comp_vcow.bits.cell_4, cf.fault_comp_vcow.bits.cell_3,
        cf.fault_comp_vcow.bits.cell_2, cf.fault_comp_vcow.bits.cell_1,

        cf.fault_comp_cbow.bits.cell_16, cf.fault_comp_cbow.bits.cell_15,
        cf.fault_comp_cbow.bits.cell_14, cf.fault_comp_cbow.bits.cell_13,
        cf.fault_comp_cbow.bits.cell_12, cf.fault_comp_cbow.bits.cell_11,
        cf.fault_comp_cbow.bits.cell_10, cf.fault_comp_cbow.bits.cell_9,
        cf.fault_comp_cbow.bits.cell_8, cf.fault_comp_cbow.bits.cell_7,
        cf.fault_comp_cbow.bits.cell_6, cf.fault_comp_cbow.bits.cell_5,
        cf.fault_comp_cbow.bits.cell_4, cf.fault_comp_cbow.bits.cell_3,
        cf.fault_comp_cbow.bits.cell_2, cf.fault_comp_cbow.bits.cell_1,

        cf.fault_comp_cbfet.bits.cell_16, cf.fault_comp_cbfet.bits.cell_15,
        cf.fault_comp_cbfet.bits.cell_14, cf.fault_comp_cbfet.bits.cell_13,
        cf.fault_comp_cbfet.bits.cell_12, cf.fault_comp_cbfet.bits.cell_11,
        cf.fault_comp_cbfet.bits.cell_10, cf.fault_comp_cbfet.bits.cell_9,
        cf.fault_comp_cbfet.bits.cell_8, cf.fault_comp_cbfet.bits.cell_7,
        cf.fault_comp_cbfet.bits.cell_6, cf.fault_comp_cbfet.bits.cell_5,
        cf.fault_comp_cbfet.bits.cell_4, cf.fault_comp_cbfet.bits.cell_3,
        cf.fault_comp_cbfet.bits.cell_2, cf.fault_comp_cbfet.bits.cell_1,

        cf.fault_comp_misc.bits.comp_adc_abort,
        cf.fault_comp_misc.bits.lpf_fail
    );
}


int snprint_prot_faults(char * s, size_t n, prot_fault_t pf) {
    return snprintf(s, n,
        "prot faults: BIST_ABORT:%d, TPATH_FAIL:%d, VPATH_FAIL:%d, UTCOMP_FAIL:%d, OTCOMP_FAIL:%d, OVCOMP_FAIL:%d, UVCOMP_FAIL:%d, TPARITY_FAIL:%d, VPARITY_FAIL:%d",
        pf.bits.bist_abort,
        pf.bits.tpath_fail,
        pf.bits.vpath_fail,
        pf.bits.utcomp_fail,
        pf.bits.otcomp_fail,
        pf.bits.ovcomp_fail,
        pf.bits.uvcomp_fail,
        pf.bits.tparity_fail,
        pf.bits.vparity_fail
    );
}
#endif //BQ796XX_FAULT_PRINTS
// empty functions for if the prints are disabled
#ifndef BQ796XX_FAULT_PRINTS
int snprint_fault_summary(char * s, size_t n, fault_summary_t fs)       { return 0;}
int snprint_pwr_faults(char * s, size_t n, pwr_faults_t pf)             { return 0;}
int snprint_sys_faults(char * s, size_t n, sys_faults_t sf)             { return 0;}
int snprint_ovuv_faults(char * s, size_t n, ovuv_faults_t vf)           { return 0;}
int snprint_otut_faults(char * s, size_t n, otut_faults_t tf)           { return 0;}
int snprint_comm_faults(char * s, size_t n, comm_faults_t cf)           { return 0;}
int snprint_otp_faults(char * s, size_t n, otp_faults_t of)             { return 0;}
int snprint_adc_comp_faults(char * s, size_t n, comp_adc_faults_t cf)   { return 0;}
int snprint_prot_faults(char * s, size_t n, prot_fault_t pf)            { return 0;}
#endif //BQ796XX_FAULT_PRINTS

// *****************************************************************************
//  GPIO
// *****************************************************************************
void set_gpio_conf(uint8_t bID, uint8_t gpioNum, gpio_conf_t conf) {
    if(gpioNum == 0 || gpioNum > 8) {
        log_err("tried to set config of invalid gpio: %d", gpioNum);
        return;
    }
    
    uint16_t reg_addr = GPIO_CONF1 + (gpioNum-1)/2;
    uint8_t reg_val = get_reg_value(bID, reg_addr);
    if(gpioNum % 2) {
        reg_val &= ~(BIT(2) | BIT(1) | BIT(0));
        reg_val |= conf;
    } else {
        reg_val &= ~(BIT(5) | BIT(4) | BIT(3));
        reg_val |= conf << 3;
    }
    set_reg_value(bID, reg_addr, reg_val);
    log_info("set gpio%d to conf 0x%02x", gpioNum, conf);
}

gpio_conf_t get_gpio_conf(uint8_t bID, uint8_t gpioNum) {
    if(gpioNum == 0 || gpioNum > 8) {
        log_err("tried to get config of invalid gpio: %d", gpioNum);
        return GPIO_CONF_DISABLED;
    }
    
    // finding the gpio conf is a bit messy, look at datasheet p162 - 9.5.4.9.1 GPIO_CONF1
    // have to find the right register, then shift it if it is even
    gpio_conf_t gpio_conf = get_reg_value(bID, GPIO_CONF1 + (gpioNum-1)/2) >> ((gpioNum % 2)? 0 : 3);
    log_info("gpio%d conf: 0x%02x", gpioNum, (uint8_t)gpio_conf);
    return gpio_conf;
}


// *****************************************************************************
//  thermistor config
// *****************************************************************************
void enable_tsref(uint8_t bID) {
    set_reg_value(bID, CONTROL2, get_reg_value(bID, CONTROL2) | BIT(0));
    log_info("enabled TSREF");
}

void disable_tsref(uint8_t bID) {
    set_reg_value(bID, CONTROL2, get_reg_value(bID, CONTROL2) & ~BIT(0));
    log_info("disabled TSREF");
}

// *****************************************************************************
//  ADC control
// *****************************************************************************
void main_ADC_start(uint8_t bID) {
    uint8_t d =  get_reg_value(bID, ADC_CTRL1);
    
    d &= ~(BIT(0) | BIT(1));
    d |= BIT(2) | BIT(1);
    
    set_reg_value(bID, ADC_CTRL1, d);
    log_info("starting main ADC");
}

void main_ADC_run_once(uint8_t bID) {
    uint8_t d =  get_reg_value(bID, ADC_CTRL1);
    
    d &= ~(BIT(0) | BIT(1));
    d |= BIT(2) | BIT(0);
    
    set_reg_value(bID, ADC_CTRL1, d);
    log_info("running main ADC once");
}

void main_ADC_stop(uint8_t bID){
    uint8_t d =  get_reg_value(bID, ADC_CTRL1);
    
    d &= ~(BIT(0) | BIT(1));
    d |= BIT(2);
    
    set_reg_value(bID, ADC_CTRL1, d);
    log_info("stopping main ADC");
}

void aux_ADC_start(uint8_t bID) {
    uint8_t d =  get_reg_value(bID, ADC_CTRL3);
    
    d &= ~(BIT(0) | BIT(1));
    d |= BIT(2) | BIT(1);
    
    set_reg_value(bID, ADC_CTRL3, d);
    log_info("starting aux ADC");    
}

void aux_ADC_run_once(uint8_t bID) {
    uint8_t d =  get_reg_value(bID, ADC_CTRL3);
    
    d &= ~(BIT(0) | BIT(1));
    d |= BIT(2) | BIT(0);
    
    set_reg_value(bID, ADC_CTRL3, d);
    log_info("running aux ADC once");
}

void aux_ADC_stop(uint8_t bID) {
    uint8_t d =  get_reg_value(bID, ADC_CTRL3);
    
    d &= ~(BIT(0) | BIT(1));
    d |= BIT(2);
    
    set_reg_value(bID, ADC_CTRL3, d);
    log_info("stopping aux ADC");
}

bool get_main_ADC_running(uint8_t bID) {
    bool running = get_reg_value(bID, DEV_STAT) & 0x01;
    log_info("main ADC running: %d", running);
    return running;
}

 // has the main ADC read each channel at least once
bool get_main_ADC_RR_complete(uint8_t bID) {
    bool run = get_reg_value(bID, ADC_STAT1) & 0x01;
    log_info("main ADC one RR complete: %d", run);
    return run;
}

bool get_aux_ADC_running(uint8_t bID) {
    bool running = get_reg_value(bID, DEV_STAT) & 0x02;
    log_info("aux ADC running: %d", running);
    return running;
}

// has the aux ADC read each channel at least once
bool get_aux_ADC_RR_complete(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, ADC_STAT1);
    bool run = (reg & BIT(1)) && (reg & BIT(2)) && (reg & BIT(3));
    log_dbg("aux ADC - DRDY_AUX_GPIO:%d,  DRDY_AUX_CELL:%d,  DRDY_AUX_MISC:%d",  (bool)(reg & BIT(3)), (bool)(reg & BIT(2)), (bool)(reg & BIT(1)));
    log_info("aux ADC one RR complete: %d", run);
    return run;
}

void enable_LPF_cells(uint8_t bID, LPF_CUTOFF_t freq) {
    // config
    uint8_t conf = get_reg_value(bID, ADC_CONF1);
    conf &= ~(BIT(2) | BIT(1) | BIT(0));
    conf |= freq;
    set_reg_value(bID, ADC_CONF1, conf);
    // enable
    uint8_t ctrl = get_reg_value(bID, ADC_CTRL1);
    ctrl |= BIT(3);
    set_reg_value(bID, ADC_CTRL1, ctrl);
    
    log_info("enabling cells LPF - cutoff: 0x%02x", freq);
}

void enable_LPF_BB(uint8_t bID, LPF_CUTOFF_t freq) {
    // config
    uint8_t conf = get_reg_value(bID, ADC_CONF1);
    conf &= ~(BIT(5) | BIT(4) | BIT(3));
    conf |= freq << 3;
    set_reg_value(bID, ADC_CONF1, conf);
    // enable
    uint8_t ctrl = get_reg_value(bID, ADC_CTRL1);
    ctrl |= BIT(4);
    set_reg_value(bID, ADC_CTRL1, ctrl);
    log_info("enabling BB LPF: - cutoff: 0x%02x", freq);
}

void disable_LPF_cells(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, ADC_CTRL1);
    reg &= ~BIT(3);
    set_reg_value(bID, ADC_CTRL1, reg);
    log_info("disabling cells LPF");
}

void disable_LPF_BB(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, ADC_CTRL1);
    reg &= ~BIT(4);
    set_reg_value(bID, ADC_CTRL1, reg);
    log_info("disabling BB LPF");
}



// *****************************************************************************
//  voltage comparators
// *****************************************************************************
void OVUV_config(uint8_t bID, OV_THRESH_t OV_thresh, UV_THRESH_t UV_thresh, uint8_t cell_count) {
    if (OV_thresh < OV_THRESH_2700mV || OV_thresh > OV_THRESH_4475mV) {
        log_err("tried to start OVUV with invalid OV thresh: 0x%02x", OV_thresh);
        return;
    }
    if (UV_thresh > UV_THRESH_3100mV) {
        log_err("tried to start OVUV with invalid UV thresh: 0x%02x", UV_thresh);
        return;
    }
    if (cell_count > 16) {
        log_err("tried to start OVUV with invalid number of cells: %d", cell_count);
        return;
    }
    
    set_reg_value(bID, OV_THRESH, (uint8_t)OV_thresh);
    set_reg_value(bID, UV_THRESH, (uint8_t)UV_thresh);
    
    // disable UV for the cells that aren't enabled
    uint16_t UV_disable_mask = ~(0xffff >> (16-cell_count)); // sets all, then clears the lowest cell_count bits
    set_reg_value(bID, UV_DISABLE1, (uint8_t)(UV_disable_mask >> 8));
    set_reg_value(bID, UV_DISABLE2, (uint8_t)(UV_disable_mask & 0xff));
    
    log_info("config'd OVUV: UV_THRESH:0x%02x, UV_THRESH:0x%02x, cell count:%d", OV_thresh, UV_thresh, cell_count);
}

void OVUV_start(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, OVUV_CTRL);
    reg &= ~(BIT(0) | BIT(1));
    reg |= BIT(2) | BIT(0);
    set_reg_value(bID, OVUV_CTRL, reg);
    log_info("started OVUV");
}

void OVUV_stop(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, OVUV_CTRL);
    reg &= ~(BIT(0) | BIT(1));
    reg |= BIT(2);
    set_reg_value(bID, OVUV_CTRL, reg);
    log_info("stopped OVUV");
}

bool get_OVUV_running(uint8_t bID) {
    bool running = get_reg_value(bID, DEV_STAT) & BIT(3);
    log_info("OVUV running:%d", running);
    return running;
}


// *****************************************************************************
//  Temperature comparators
// *****************************************************************************
void OTUT_config(uint8_t bID, uint8_t OT_thr_percent, uint8_t UT_thr_percent) {
    if (OT_thr_percent < 10  || OT_thr_percent > 39 ) {
        log_err("tried setting OTUT with invalid OT_thr_percent:%d%%", OT_thr_percent);
        return;
    }
    if (UT_thr_percent < 66 || UT_thr_percent > 80) {
        log_err("tried setting OTUT with invalid UT_thr_percent:%d%%", UT_thr_percent);
        return;
    }
    // workout what to set the OTUT_THRESH reg to
    uint8_t UT_THR = (UT_thr_percent - 66) * 2;
    uint8_t OT_THR = (OT_thr_percent) - 10; // the datasheet is a bit misleading with this one 
    uint8_t reg = (uint8_t)(UT_THR << 5) | OT_THR;
    
    set_reg_value(bID, OTUT_THRESH, reg);
    log_info("config'd OTUT to UT:%d%%, OT:%d%%", UT_thr_percent, OT_thr_percent);
}

void OTUT_start(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, OTUT_CTRL);
    reg &= ~(BIT(0) | BIT(1));
    reg |= BIT(2) | BIT(0);
    set_reg_value(bID, OTUT_CTRL, reg);
    log_info("started OTUT");
}

void OTUT_stop(uint8_t bID) {
    uint8_t reg = get_reg_value(bID, OTUT_CTRL);
    reg &= ~(BIT(0) | BIT(1));
    reg |= BIT(2);
    set_reg_value(bID, OTUT_CTRL, reg);
    log_info("stopped OTUT");
}

bool get_OTUT_running(uint8_t bID) {
    bool running = get_reg_value(bID, DEV_STAT) & BIT(4);
    log_info("OTUT running:%d", running);
    return running;
}


// *****************************************************************************
//  balancing
// *****************************************************************************
void balancing_start(uint8_t bID); // configure before starting!!
void balancing_stop(uint8_t bID);
void balancing_pause(uint8_t bID, bool paused);
bool get_bal_paused(uint8_t bID);

void enable_auto_balancing(uint8_t bID, BAL_DUTY_t duty_cycle); 
void disable_auto_balancing(uint8_t bID);
bool get_balancing_running(uint8_t bID);

void set_balancing_timer(uint8_t bID, uint8_t cell_number, BAL_TIME_t time); 
BAL_TIME_t get_balancing_timer(uint8_t bID, uint8_t cell_number); 
bool get_balancing_done(uint8_t bID, uint8_t cell_number);

void set_module_balancing_timer(uint8_t bID, BAL_TIME_t time);
BAL_TIME_t get_module_balancing_timer(uint8_t bID);
bool get_module_balancing_done(uint8_t bID);

void enable_VCB_stop_thresh(uint8_t bID, CB_DONE_THRESH_t vcb_thr);
void enable_VMB_stop_thres(uint8_t bID, uint8_t vmb_thr);

void enable_OTCB(uint8_t bID, uint8_t OT_thr_percent, uint8_t cooloff_thr_percent);
bool get_OTCB_running(uint8_t bID);


// *****************************************************************************
//  Reading voltages and temperatures
// *****************************************************************************
int16_t get_cell_voltage(uint8_t bID, uint8_t cell_number) {
    if(cell_number == 0 || cell_number > 16) {
        log_err("tried to read aux voltage from non existent cell : %d", cell_number);
        return INT16_MIN;
    }
    
    log_dbg("reading cell voltage: %d", cell_number);
    
    int16_t hi = get_reg_value(bID, VCELL1_HI - 2 * (cell_number - 1));
    int16_t lo = get_reg_value(bID, VCELL1_LO - 2 * (cell_number - 1));
    int16_t v = lo + (hi << 8);
    
    // 0x8000 is the default value, so the ADC may not have run yet - log that if so
    if(v == 0x8000){
        if(!get_main_ADC_RR_complete(bID)){
            log_err("tried to read voltage %d before main ADC has run - data is invalid", cell_number);   
        }
    }
    
    log_info("main adc v%d: %d = %fV", cell_number, v, v * V_LSB_ADC);
    return v;
}

int16_t get_cell_voltage_aux(uint8_t bID, uint8_t cell_number) {
    int16_t ret_val;
    
    if(cell_number == 0 || cell_number > 16) {
        log_err("tried to read aux voltage from non existent cell : %d", cell_number);
        return INT16_MIN;
    }
    
    log_dbg("reading aux cell voltage: %d", cell_number);
    
    // lock CB_MUX to a single cell
    uint8_t ctrl2 = get_reg_value(bID, ADC_CTRL2);
    ctrl2 &= ~(BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0));
    ctrl2 |= cell_number + 1;
    set_reg_value(bID, ADC_CTRL2, ctrl2);
    set_reg_value(bID, ADC_CTRL3, get_reg_value(bID, ADC_CTRL3) | BIT(2)); // aux ADC GO
    log_dbg("CB_MUX locked - ctrl2: 0x%02x", ctrl2);
    
    // wait for value to update
    int retrys = 10;
    while(!get_aux_ADC_RR_complete(bID) && --retrys); // waiting for one rr to complete
    log_dbg("wait for aux RR to happen once done");
    
    if(retrys == 0) {
        log_dbg("wait timed out");
        if(!get_aux_ADC_running(bID)) {
            log_err("cannot read aux voltage when aux adc is stopped - data is invalid");
        }
        ret_val = INT16_MIN;
    } else {
        // read value
        ret_val = ((int16_t)get_reg_value(bID, AUX_CELL_HI) << 8) + (int16_t)get_reg_value(bID, AUX_CELL_LO);
        log_dbg("aux hi/lo regs read");
    }
    
    // unlock CB_MUX
    ctrl2 &= ~(BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0));
    set_reg_value(bID, ADC_CTRL2, ctrl2);
    set_reg_value(bID, ADC_CTRL3, get_reg_value(bID, ADC_CTRL3) | BIT(2)); // aux ADC GO
    log_dbg("CB_MUX released");
    
    log_info("aux adc v%d : %d = %fV", cell_number, ret_val, ret_val * V_LSB_ADC);
    return ret_val;
}

int16_t get_BB_voltage(uint8_t bID); 
int16_t get_BB_voltage_aux(uint8_t bID); 

int16_t get_gpio_voltage(uint8_t bID, uint8_t gpio_number) {
    if(gpio_number == 0 || gpio_number > 8) {
        log_err("tried to read gpio voltage from non existent gpio : %d", gpio_number);
        return INT16_MIN;
    }
    
    log_dbg("reading gpio voltage: %d", gpio_number);
    
    int16_t hi = get_reg_value(bID, GPIO1_HI + 2 * (gpio_number - 1));
    int16_t lo = get_reg_value(bID, GPIO1_LO + 2 * (gpio_number - 1));
    int16_t v = lo + (hi << 8);
    
    // 0x8000 is the default value, so the ADC may not have run yet - log that if so
    if(v == 0x8000){
        if(!get_main_ADC_RR_complete(bID)){
            log_err("tried to read gpio%d voltage before main ADC has run - data is invalid", gpio_number);   
        }
        gpio_conf_t conf = get_gpio_conf(bID, gpio_number);
        if( conf == GPIO_CONF_DISABLED | 
            conf == GPIO_CONF_DIGITAL_INPUT |
            conf == GPIO_CONF_OUTPUT_HIGH |
            conf == GPIO_CONF_OUTPUT_LOW) {
            log_err("tried to read gpio%d voltage when it isn't configured to ADC: conf=0x%02x", gpio_number, conf);
        }
    }
    
    log_info("main adc gpio%d: %d = %fV", gpio_number, v, v * V_LSB_ADC);
    return v;
    
}

int16_t get_tsref_voltage(uint8_t bID) {
    int16_t tsref_v  = ((int16_t)get_reg_value(bID, TSREF_HI) << 8) + (int16_t)get_reg_value(bID, TSREF_LO);
    if (tsref_v == INT16_MIN) {
        if(!get_main_ADC_running(bID)) {
            log_err("tried to tsref voltage with main ADC not running - data invalid");
        }
    }
    log_info("tsref: %d = %fV", tsref_v, tsref_v * V_LSB_ADC);
    return tsref_v;
}


int16_t get_die_temp_1(uint8_t bID) {
    int16_t temp  = ((int16_t)get_reg_value(bID, DIETEMP1_HI) << 8) + (int16_t)get_reg_value(bID, DIETEMP1_LO);
    if (temp == INT16_MIN) {
        if(!get_main_ADC_running(bID)) {
            log_err("tried to read die temp 1 with main ADC not running - data invalid");
        }
    }
    log_info("Die temp 1: %d = %f'C", temp, temp * V_LSB_DIETEMP);
    return temp;
}


int16_t get_die_temp_2(uint8_t bID) {
    int16_t temp  = ((int16_t)get_reg_value(bID, DIETEMP2_HI) << 8) + (int16_t)get_reg_value(bID, DIETEMP2_LO);
    if (temp == INT16_MIN) {
        if(!get_aux_ADC_running(bID)) {
            log_err("tried to read die temp 2 with aux ADC not running - data invalid");
        }
    }
    log_info("Die temp 2: %d = %f'C", temp, temp * V_LSB_DIETEMP);
    return temp;
}



//RUN BASIC CELL BALANCING FOR ALL DEVICES
/*
void RunCB()
{
    //SET BALANCING TIMERS TO 30 s
    WriteReg(0, CB_CELL16_CTRL, 0x0202020202020202, 8, WriteType);   //cell 16-9 (8 byte max write)
    WriteReg(0, CB_CELL8_CTRL, 0x0202020202020202, 8, WriteType);    //cell 8-1

    //SET DUTY CYCLE TO 10 s (default)
    WriteReg(0, BAL_CTRL1, 0x01, 1, WriteType);   //10s duty cycle

    //OPTIONAL: SET VCBDONE THRESH TO 3V, AND OVUV_GO
    WriteReg(0, VCB_DONE_THRESH, 0x08, 1, WriteType);    //3V threshold (8*25mV + 2.8V)
    WriteReg(0, OVUV_CTRL, 0x05, 1, WriteType);          //round-robin and OVUV_GO

    //START BALANCING
    WriteReg(0, BAL_CTRL2, 0x03, 1, WriteType); //auto balance and BAL_GO
}
*/

#if 0
/** @fn int ReadDeviceStat2(uint8_t *)
*   @brief Read the device stat register 
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The function reads the Dev_stat2 register and returns if the Coulomb count
*   feature is enabled.
*/
#if UART_COMM == TRUE
int ReadDeviceStat2(uint8_t *response_frame)
#else
int ReadDeviceStat2(uint8_t *response_frame)
#endif
{
   int retval = 0U;

   ReadReg(0, DEV_STAT2, response_frame, 1, 0, ReadType);
   for(currentBoard=0; currentBoard<TOTALBOARDS; currentBoard++)
   {
      printf("Dev_Stat2 val for stack dev id %d is 0x%x\n", currentBoard, response_frame[RESPONSE_DATA_START_INDEX + currentBoard]);
      /* Check if the Coulomb count function is running */
      if ( 0x1U != ((response_frame[RESPONSE_DATA_START_INDEX + currentBoard] >> 0x5U) &  0x1U ))
      {
         retval = -1;
         break; 
      }
   }

   return retval;
}

/** @fn void ReadCoulumbCount(uint8_t *)
*   @brief Read the coulomb count accumulation data value 
*   @note This function is empty by default.
*
*   This function is called periodically.
*   The function reads the accumulation data register and prints its value 
*/

#if UART_COMM == TRUE
void ReadCoulumbCount(uint8_t *response_frame, uint8_t rshunt_u8)
#else
void ReadCoulumbCount(uint8_t *response_frame, uint8_t rshunt_u8)
#endif
{
    int i = 0U;
    uint32 cc_acc_val_u32;
    uint8_t cc_cnt_u8;

    /* Do CC_CLR */
    WriteReg(0, CC_CTRL, 0x07, 1, WriteType);

    
    /* Read the CC Accumulation data ( 4 bytes for each stack device ) */
    ReadReg(0U, CC_ACC_HI, response_frame, ACCUMULATION_DATA_SIZE, 0U, ReadType);

    for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + (TOTALBOARDS * ACCUMULATION_DATA_SIZE)); 
		    i += ACCUMULATION_DATA_SIZE )
    {
       cc_acc_val_u32 = response_frame[i] << 24;
       cc_acc_val_u32 = response_frame[i + 1] << 16;
       cc_acc_val_u32 = response_frame[i + 2] << 8;
       cc_acc_val_u32 = response_frame[i + 3];
    }

    ReadReg(0U, CC_CNT, response_frame, 1U, 0U, ReadType);
    cc_cnt_u8 = response_frame[RESPONSE_DATA_START_INDEX];

    if ( cc_cnt_u8 < 0xFFU )
    {
       printf("value of Iaverage %f\n", (cc_acc_val_u32 * VLSB_CS ) / ( rshunt_u8 * cc_cnt_u8));
       printf("value of coulomb count %f\n", (cc_acc_val_u32 * VLSB_CS * TCS_REFRESH * cc_cnt_u8) 
		                                 / (rshunt_u8) );
    }
}

/** @fn void ConfigureOverCurrent(void)
*   @brief configures over current register on BQ79631 stack device 
*   @note This function is empty by default.
*
*   This function is called at startup.
*   The function write 'DIAG_OC_CTRL1' and 'DIAG_OC_CTRL2' registers 
*/
void ConfigureOverCurrent()
{
    /* Set the over current diagnostic injection level */
    WriteReg(0, DIAG_OC_CTRL1, 0xFF, 1, WriteType);
    delayms(10);
    /* Enable Over current diagnostics by setting 'DIAG_OC_MODE' and 'DIAG_OC_GO' */
    WriteReg(0, DIAG_OC_CTRL2, 0x03, 1, WriteType);
}

/** @fn void ReadDebugRegisters(uint8_t *)
*   @brief Read the debug register values and prints its value
*   @note This function is empty by default.
*
*   This function is called periodically.
*   The function reads the debug register and prints its value 
*/

#if UART_COMM == TRUE
void ReadDebugRegisters(uint8_t *response_frame)
#elif SPI_COMM == TRUE
void ReadDebugRegisters(uint8_t *response_frame)
#endif
{
   fault_summary_t flt_summary;
   fault_adc_misc_t flt_adc_misc;
   fault_pwr1_t fault_pwr1;
   fault_pwr2_t fault_pwr2;
   fault_comm1_t flt_comm1;
   fault_comm2_t flt_comm2;
   fault_otp_t flt_otp;
   fault_sys1_t flt_sys1;
   fault_sys2_t flt_sys2;
   fault_adc_gpio1_t flt_adc_gpio1;
   fault_adc_gpio2_t flt_adc_gpio2;
   fault_adc_vf_t flt_adc_vf;
   fault_adc_dig1_t flt_adc_dig1;
   fault_adc_dig2_t flt_adc_dig2;
   fault_oc_t flt_oc;
   uint8_t i = 0;


   ReadReg(0, FAULT_SUMMARY, response_frame, 1, 0, ReadType);

   for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
   {
      flt_summary.fault_summary = response_frame[i];
   
      printf("Fault summary val is 0x%x and 0x%x\n", flt_summary.fault_summary, response_frame[i]);
   
      printf(" FAULT SUMMARY: Over current fault: %d   ADC / CC fault : %d system fault   : %d \n", 
                   flt_summary.fs.fault_oc_b1, flt_summary.fs.fault_adc_cc_b1, flt_summary.fs.fault_sys_b1);
      printf(" FAULT SUMMARY: otp fault         : %d   Comm fault     : %d power  fault   : %d \n", 
                   flt_summary.fs.fault_otp_b1, flt_summary.fs.fault_comm_b1, flt_summary.fs.fault_pwr_b1);
   }

   if ( TRUE == flt_summary.fs.fault_pwr_b1 )
   {
      ReadReg(0, FAULT_PWR1, response_frame, 1, 0, ReadType);

      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         fault_pwr1.fault_pwr1 = response_frame[i];
         printf("Fault power1 val is 0x%x\n", fault_pwr1.fault_pwr1);
      
         printf(" FAULT POWER1 : PWRBIST FAIL  : %d   VSS_OPEN       : %d TSREF_OSC      : %d \n", 
                   fault_pwr1.fs.pwrbist_fail_b1, fault_pwr1.fs.vss_open_b1, fault_pwr1.fs.tsref_osc_b1);
         printf(" FAULT POWER1 : TSREF_UV      : %d   TSREF_OV       : %d DVDD_OV        : %d \n", 
                   fault_pwr1.fs.tsref_uv_b1, fault_pwr1.fs.tsref_ov_b1, fault_pwr1.fs.dvdd_ov_b1);
         printf(" FAULT POWER1 : AVDD_OSC      : %d   AVDD_OV        : %d \n", 
                   fault_pwr1.fs.avdd_osc_b1, fault_pwr1.fs.avdd_ov_b1);
      }
      
      ReadReg(0, FAULT_PWR2, response_frame, 1, 0, ReadType);

      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         fault_pwr2.fault_pwr2 = response_frame[i];
         printf("Fault power1 val is 0x%x\n", fault_pwr2.fault_pwr2);
      
         printf(" FAULT POWER2 : CP OV         : %d   CP UV          : %d \n", 
                   fault_pwr2.fs.cp_ov_b1, fault_pwr2.fs.cp_uv_b1);
      }
   }
   else
   {
      /* Do nothing */
   }

   if ( TRUE == flt_summary.fs.fault_comm_b1 )
   {
      ReadReg(0, FAULT_COMM1, response_frame, 1, 0, ReadType);
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_comm1.fault_comm1 = response_frame[i];
         printf("Fault comm1 val is 0x%x\n", flt_comm1.fault_comm1);
      
         printf(" FAULT COMM1 : FCOMM_DET     : %d   FTONE_DET       : %d HB_FAIL        : %d \n", 
                   flt_comm1.fs.fcomm_det_b1, flt_comm1.fs.ftone_det_b1, flt_comm1.fs.hb_fail_b1);
         printf(" FAULT COMM1 : COML          : %d   COMH            : %d uart_frame     : %d \n", 
                   flt_comm1.fs.coml_b1, flt_comm1.fs.comh_b1, flt_comm1.fs.uart_frame_b1);
         printf(" FAULT COMM1 : COMM_CLR_DET  : %d   STOP_DET        : %d \n", 
                   flt_comm1.fs.comm_clr_det_b1, flt_comm1.fs.stop_det_b1);
      }
   
      ReadReg(0, FAULT_COMM2, response_frame, 1, 0, ReadType);
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_comm2.fault_comm2 = response_frame[i];
         printf("Fault comm2 val is 0x%x\n", flt_comm2.fault_comm2);
      
         printf(" FAULT COMM2 :  VIF_DIS      : %d   SPI             : %d \n", 
                   flt_comm2.fs.vif_dis_b1, flt_comm2.fs.spi_b1);
      }
   }
   else
   {
      /* Do nothing */
   }
  
   if ( TRUE == flt_summary.fs.fault_otp_b1 )
   {
      ReadReg(0, FAULT_OTP, response_frame, 1, 0, ReadType);
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_otp.fault_otp = response_frame[i];
         printf("Fault otp val is 0x%x\n", flt_otp.fault_otp);
      
         printf(" FAULT OTP : DED_DET         : %d   SEC_DET         : %d CUST_CRC       : %d \n", 
                   flt_otp.fs.ded_det_b1, flt_otp.fs.sec_det_b1, flt_otp.fs.cust_crc_b1);
         printf(" FAULT OTP : FACT CRC        : %d   LOAD ERR        : %d GBLOV ERR      : %d \n", 
                   flt_otp.fs.fact_crc_b1, flt_otp.fs.load_err_b1, flt_otp.fs.gblov_err_b1);
      }
   }
   else
   {
      /* Do nothing */
   }

   if ( TRUE == flt_summary.fs.fault_sys_b1 )
   {
      ReadReg(0, FAULT_SYS1, response_frame, 1, 0, ReadType);

      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_sys1.fault_sys1 = response_frame[i];
         printf("Fault sys1 val is 0x%x\n", flt_sys1.fault_sys1);
      
         printf(" FAULT SYS1 : GPIO           : %d   I2C low         : %d I2C NACK       : %d \n", 
                   flt_sys1.fs.gpio_b1, flt_sys1.fs.i2c_low_b1, flt_sys1.fs.i2c_nack_b1);
         printf(" FAULT SYS1 : LFO            : %d   DRST            : %d TSHUT          : %d \n", 
                   flt_sys1.fs.lfo_b1, flt_sys1.fs.drst_b1, flt_sys1.fs.tshut_b1);
      }

      ReadReg(0, FAULT_SYS2, response_frame, 1, 0, ReadType);

      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_sys2.fault_sys2 = response_frame[i];
         printf("Fault sys2 val is 0x%x\n", flt_sys2.fault_sys2);
      
         printf(" FAULT SYS2 :  I2C BUSY      : %d   MSPI_BUSY       : %d MSPI_SS        : %d\n", 
                   flt_sys2.fs.i2c_busy_b1, flt_sys2.fs.mspi_busy_b1, flt_sys2.fs.mspi_ss_b1);
      }
   }
   else
   {
      /* Do nothing */
   }
   
   if ( TRUE == flt_summary.fs.fault_adc_cc_b1 )
   {
      ReadReg(0, FAULT_ADC_GPIO1, response_frame, 1, 0, ReadType);
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_adc_gpio1.fault_adc_gpio1 = response_frame[i];
      
         printf(" FAULT GPIO1 : GPIO15 AFAIL  : %d   GPIO14 AFAIL    : %d GPIO13 AFAIL   : %d \n", 
                      flt_adc_gpio1.fs.gpio15_afail_b1, flt_adc_gpio1.fs.gpio14_afail_b1, flt_adc_gpio1.fs.gpio13_afail_b1);
         printf(" FAULT GPIO1 : GPIO12 AFAIL  : %d   GPIO11 AFAIL    : %d GPIO10 AFAIL   : %d \n", 
                      flt_adc_gpio1.fs.gpio12_afail_b1, flt_adc_gpio1.fs.gpio11_afail_b1, flt_adc_gpio1.fs.gpio10_afail_b1);
         printf(" FAULT GPIO1 : GPIO9_AFAIL   : %d \n", 
                      flt_adc_gpio1.fs.gpio9_afail_b1);
      }
   
      ReadReg(0, FAULT_ADC_GPIO2, response_frame, 1, 0, ReadType);
      
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_adc_gpio2.fault_adc_gpio2 = response_frame[i];
      
         printf(" FAULT GPIO2 : GPIO8 AFAIL   : %d   GPIO7 AFAIL    : %d GPIO6 AFAIL   : %d \n", 
                      flt_adc_gpio2.fs.gpio8_afail_b1, flt_adc_gpio2.fs.gpio7_afail_b1, flt_adc_gpio2.fs.gpio6_afail_b1);
         printf(" FAULT GPIO2 : GPIO5 AFAIL   : %d   GPIO4 AFAIL    : %d GPIO3 AFAIL   : %d \n", 
                      flt_adc_gpio2.fs.gpio5_afail_b1, flt_adc_gpio2.fs.gpio4_afail_b1, flt_adc_gpio2.fs.gpio3_afail_b1);
         printf(" FAULT GPIO2 : GPIO2_AFAIL   : %d  GPIO1_AFAIL     : %d \n", 
                      flt_adc_gpio2.fs.gpio2_afail_b1, flt_adc_gpio2.fs.gpio1_afail_b1);
      }
   
      ReadReg(0, FAULT_ADC_VF, response_frame, 1, 0, ReadType);
      
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_adc_vf.fault_adc_vf = response_frame[i];
      
         printf(" FAULT ADC VF : VF2_AFAIL    : %d  VF1_AFAIL       : %d \n", 
                      flt_adc_vf.fs.vf2_afail_b1, flt_adc_vf.fs.vf1_afail_b1);
      }
   
      ReadReg(0, FAULT_ADC_DIG1, response_frame, 1, 0, ReadType);
      
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_adc_dig1.fault_adc_dig1 = response_frame[i];
        
         printf(" FAULT ADC DIG1 : GP3_DFAIL  : %d  GP1_DFAIL       : %d \n", 
                      flt_adc_dig1.fs.gp3_dfail_b1, flt_adc_dig1.fs.gp1_dfail_b1);
      }
   
      ReadReg(0, FAULT_ADC_DIG2, response_frame, 1, 0, ReadType);
   
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         flt_adc_dig2.fault_adc_dig2 = response_frame[i];
      
         printf(" FAULT ADC VF : VF1_DFAIL    : %d  VF2_DFAIL       : %d \n", 
                      flt_adc_dig2.fs.vf1_dfail_b1, flt_adc_dig2.fs.vf2_dfail_b1);
      }
   
      ReadReg(0, FAULT_ADC_MISC, response_frame, 1, 0, ReadType);
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         printf("Fault ADC MISC val is 0x%x\n", response_frame[i]);
         flt_adc_misc.fault_adc_misc = response_frame[4];
      
         printf(" FAULT ADC MISC: CC overflow : %d   adc pfail      : %d diag meas pfail: %d \n", 
                     flt_adc_misc.fs.cc_ovf_b1, flt_adc_misc.fs.adc_pfail_b1, flt_adc_misc.fs.diag_meas_pfail_b1);
         printf(" FAULT ADC MISC: diag ana pfail: %d   diag ana abort : %d \n", 
                     flt_adc_misc.fs.diag_ana_pfail_b1, flt_adc_misc.fs.diag_ana_abort_b1);
      }
   
      ReadReg(0, FAULT_OC, response_frame, 1, 0, ReadType);
   
      for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + TOTALBOARDS); i++ )
      {
         printf("Fault over current val is 0x%x\n", response_frame[4]);
      
         flt_oc.fault_oc = response_frame[i];
      
         printf(" FAULT OC: DIAG_OC_ABORT     : %d   OCC2           : %d OCD2           : %d \n", 
                     flt_oc.fs.diag_oc_abort_b1, flt_oc.fs.occ2_b1, flt_oc.fs.ocd2_b1);
      
         printf(" FAULT OC: OC PFAIL          : %d   OCC1           : %d OCD1           : %d \n", 
                     flt_oc.fs.oc_pfail_b1, flt_oc.fs.occ1_b1, flt_oc.fs.ocd1_b1);
      }
   }
   else
   {
      /* Do nothing */
   }
}

/** @fn void set_SW_CTRL(uint8_t pinNum, uint8_t value)
*   @brief set the value of SW_CTRL register 
*   takes the SW pin number and the value to set the pin high / low
*/

void set_SW_CTRL(uint8_t pinNum, uint8_t value )
{
    printf("Writing SW_CTRL with value %x\n", (value << ((pinNum - 1U) * 2)));

    if ((value <= SW_OUTPUT_HIGH) || 
	    (pinNum <= 4))
    { 
       WriteReg(0, SW_CTRL, (value << ((pinNum - 1U) * 2)), 1U, WriteType);
    }
    else
    {
       /* Do nothing */
       printf("SW_CTRL arguments out of range\n");
    }
}

/** @fn void set_GPIO_Out_Val(uint8_t pinNum, boolean value)
*   @brief set the value of GPIO[1 - 15] output High / Low 
*   takes the gpio number and the value its output to high / low
*/

void set_GPIO_Out_Val(uint8_t gpioNum, uint8_t val )
{
   uint8_t value = 0;
   if ( gpioNum > 8U )
   {
       /*  decrement gpioNum with 9 to index bit 0 to bit 6 to represent 
	*  gpio num 9 to 15 */
       value |= (val << (gpioNum - 9U));
       WriteReg(0, GPIO_CTRL1, value, 1U, WriteType);
       printf("Set GPIO_CTRL1 val %x\n", value);
   }
   else
   {
       /* bit index starts from 0 so decrement gpioNum by 1 */
       value |= (val << (gpioNum - 1U));
       WriteReg(0, GPIO_CTRL2, value, 1U, WriteType);
       printf("Set GPIO_CTRL2 val %x\n", value);
   }

}

/** @fn void set_GPIO_As_PWM(uint8_t gpioNum, boolean val)
*   @brief set GPIO[12 - 15] as PWM or GPIO 
*   takes the gpio number and the value representing PWM / GPIO 
*/

void set_GPIO_As_PWM(uint8_t gpioNum, uint8_t val )
{
   uint8_t value = 0;
   if ( (gpioNum >= 12U) && (gpioNum < 16U)  )
   {
       /*  decrement gpioNum with 9 to index bit 0 to bit 6 to represent 
	*  gpio num 9 to 15 */
       value |= (val << (gpioNum - 9U));
       WriteReg(0, GPIO_PWM_CTRL, value, 1U, WriteType);
       printf("Set GPIO_PWM_CTRL val %x\n", value);
   }
   else
   {
       /* Do nothing */       
       printf("GPIO number argument is incorrect\n");
   }

}
#endif
//***************************
//END MISCELLANEOUS FUNCTIONS
//***************************

//EOF

