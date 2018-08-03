/*********************************************************************************
*DISTRIBUTION STATEMENT A. Approved for public release. Distribution is unlimited.
*
*This material is based upon work supported under Air Force Contract No. FA8721-05-C-0002
*and/or FA8702-15-D-0001. Any opinions, findings, conclusions or recommendations
*expressed in this material are those of the author(s) and do not necessarily reflect the
*views of the U.S. Air Force.
*
*© 2018 Massachusetts Institute of Technology.
*
*The software/firmware is provided to you on an As-Is basis
*
*Delivered to the U.S. Government with Unlimited Rights, as defined in DFARS Part
*252.227-7013 or 7014 (Feb 2014). Notwithstanding any copyright notice, U.S. Government
*rights in this work are defined by DFARS 252.227-7013 or DFARS 252.227-7014 as detailed
*above. Use of this work other than as specifically authorized by the U.S. Government may
*violate any copyrights that exist in this work.
*********************************************************************************/ 
/*************************************************************************
** File:
**   $Id: bmp280_lib.c $
**
** Purpose: 
**   bmp280 CFS library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "bmp280_lib.h"

#include "stdio.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "time.h"


/*************************************************************************
** Macro Definitions
*************************************************************************/
//device Address
#define ADDR 0x77

//device settings

#define POWER_MODE      3 // Power mode
                          // 0 = sleep 
                          // 1 = forced 
                          // 2 = forced
                          // 3 = normal

#define OSRS_T          5 //Temperature resolution
                          // 0 = skipped
                          // 1 = 16 bit
                          // 2 = 17 bit
                          // 3 = 18 bit
                          // 4 = 19 bit
                          // 5 = 20 bit

#define OSRS_P          5 //Pressure resolution
                          // 0 = skipped
                          // 1 = 16 bit
                          // 2 = 17 bit
                          // 3 = 18 bit
                          // 4 = 19 bit
                          // 5 = 20 bit

#define FILTER          4 //

#define T_SB            4 //500ms standby

// Device Registers
#define REG_DIG_T1      0x88
#define REG_DIG_T2      0x8A
#define REG_DIG_T3      0x8C
#define REG_DIG_P1      0x8E
#define REG_DIG_P2      0x90
#define REG_DIG_P3      0x92
#define REG_DIG_P4      0x94
#define REG_DIG_P5      0x96
#define REG_DIG_P6      0x98
#define REG_DIG_P7      0x9A
#define REG_DIG_P8      0x9C
#define REG_DIG_P9      0x9E
#define REG_CHIPID      0xD0
#define REG_VERSION     0xD1
#define REG_SOFTRESET   0xE0
#define REG_CONTROL     0xF4
#define REG_CONFIG      0xF5
#define REG_STATUS      0xF3

#define REG_TEMP_MSB    0xFA
#define REG_TEMP_LSB    0xFB
#define REG_TEMP_XLSB   0xFC

#define REG_PRESS_MSB   0xF7
#define REG_PRESS_LSB   0xF8
#define REG_PRESS_XLSB  0xF9

/*************************************************************************
** Private Data
*************************************************************************/

//Dig values needed in temperature calcualtion
uint16 dt1;
int16_t dt2;
int16_t dt3;

uint16 dp1;
int16_t dp2;
int16_t dp3;
int16_t dp4;
int16_t dp5;
int16_t dp6;
int16_t dp7;
int16_t dp8;
int16_t dp9;

int32_t t_fine;


/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 BMP280_LibInit(void);
int32 BMP280_HwInit(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BMP280_LibInit(void)
{
    int32 status = BMP280_HwInit();
    if (status != CFE_SUCCESS) {
        return 0;
    }
    else {
        return CFE_SUCCESS;
    }

}/* End BMP280_LibInit */


int32 BMP280_HwInit(void) {
    int fd =  wiringPiI2CSetup(ADDR);
    
    // successful connection?
    if (fd < 0) { return 0; }
    if (wiringPiI2CReadReg8(fd, REG_CHIPID) != 0x58) { return -1; }

    uint8 config = (T_SB << 5) + (FILTER << 2);
    uint8 ctrl_meas = (OSRS_T << 5) + (OSRS_P << 2) + POWER_MODE;
    
    //applying device settings
    wiringPiI2CWriteReg8(fd, REG_SOFTRESET, 0xB6); //reset sensor
    sleep(0.2);
    wiringPiI2CWriteReg8(fd, REG_CONTROL, ctrl_meas);
    sleep(0.2);
    wiringPiI2CWriteReg8(fd, REG_CONFIG, config);
    sleep(0.2);
    
    //read dig values

    dt1 = wiringPiI2CReadReg16(fd, REG_DIG_T1);
    dt2 = wiringPiI2CReadReg16(fd, REG_DIG_T2);
    dt3 = wiringPiI2CReadReg16(fd, REG_DIG_T3);

    dp1 = wiringPiI2CReadReg16(fd, REG_DIG_P1);
    dp2 = wiringPiI2CReadReg16(fd, REG_DIG_P2);
    dp3 = wiringPiI2CReadReg16(fd, REG_DIG_P3);
    dp4 = wiringPiI2CReadReg16(fd, REG_DIG_P4);
    dp5 = wiringPiI2CReadReg16(fd, REG_DIG_P5);
    dp6 = wiringPiI2CReadReg16(fd, REG_DIG_P6);
    dp7 = wiringPiI2CReadReg16(fd, REG_DIG_P7);
    dp8 = wiringPiI2CReadReg16(fd, REG_DIG_P8);
    dp9 = wiringPiI2CReadReg16(fd, REG_DIG_P9);

    close(fd);
    return CFE_SUCCESS;
} /* END BMP280_HwInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* BMP280 Lib functions                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//returns current temperature raw data
int32_t BMP280_GetTempRaw(void) {
    int fd = wiringPiI2CSetup(ADDR);
    uint8 t_msb = wiringPiI2CReadReg8(fd, REG_TEMP_MSB);
    uint8 t_lsb = wiringPiI2CReadReg8(fd, REG_TEMP_LSB);
    uint8 t_xlsb = wiringPiI2CReadReg8(fd, REG_TEMP_XLSB);

    int32_t rt = ((t_msb << 16) + (t_lsb << 8) + t_xlsb) >> 4; //combine the 3 bytes of data

    close(fd);
    return rt;
}   /* END BMP280_GetTempRaw */

//returns current pressure raw data
int32_t BMP280_GetPressRaw(void) {
    int fd = wiringPiI2CSetup(ADDR);
    uint8 p_msb = wiringPiI2CReadReg8(fd, REG_PRESS_MSB);
    uint8 p_lsb = wiringPiI2CReadReg8(fd, REG_PRESS_LSB);
    uint8 p_xlsb = wiringPiI2CReadReg8(fd, REG_PRESS_XLSB);

    int32_t rp = ((p_msb << 16) + (p_lsb << 8) + p_xlsb) >> 4; //combine the 3 bytes of data

    close(fd);    

    return rp;
}   /* END BMP_GetPressRaw */

//returns current temp
float BMP280_GetTemp(void) {
    int32_t raw_t = BMP280_GetTempRaw();

    //calculations from BMP data sheet
    int32_t tv1, tv2;
    tv1 = ((((raw_t >> 3) - ((int32_t) dt1 <<1))) * ((int32_t)dt2)) >> 11;
    tv2 = (((((raw_t >> 4) - ((int32_t)dt1)) * ((raw_t >> 4) - ((int32_t)dt1))) >> 12) * ((int32_t)dt3)) >> 14;
    t_fine = tv1 + tv2;

    float T = (t_fine * 5 + 128) >> 8;
    return T/100;
} /* END BMP_GetTemp*/ 

//returns current pressure
float BMP280_GetPress(void) {
    //Make sure t_fine is calculated
    BMP280_GetTemp();

    //get the raw pressure reading
    int32_t raw_p = BMP280_GetPressRaw();
    
    int64_t pv1, pv2, p;

    pv1 = ((int64_t) t_fine) - 128000; 
    pv2 = pv1 * pv1 * (int64_t)dp6;
    pv2 = pv2 + ((pv1*(int64_t)dp5)<<17);
    pv2 = pv2 + (((int64_t)dp4) << 35);
    pv1 = ((pv1 * pv1 * (int64_t)dp3) >> 8) + ((pv1 * (int64_t)dp2) << 12);
    pv1 = ((((int64_t)1)<<47)+pv1)*((int64_t)dp1) >> 33;
    
    if (pv1 == 0) {
        OS_printf("BMP280 ERROR: divide by 0 error (pressure var1) \n");
        return 0;
    }
    
    p = 1048576 - raw_p;
    p = (((p<<31) - pv2)*3125) / pv1;
    pv1 = (((int64_t)dp9) * (p >> 13) * (p >> 13)) >> 25;
    pv2 = (((int64_t)dp8) * p) >> 19;

    p = ((p + pv1 + pv2) >> 8) + (((int64_t)dp7) << 4);
    
    return (float)p/256;
    
} /* END BMP_GetPress */


/************************/
/*  End of File Comment */
/************************/
