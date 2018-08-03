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
**   $Id: tcs3472_hwlib.c $
**
** Purpose: 
**   tcs3472 Light and Color Sensor Hardware library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "tcs3472_lib.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "cfe.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
//Device Address
#define ADDR 0x29

//Device Registers
#define REG_CMD             0b10000000
#define REG_CMD_AUTO_INC    0b00100000

#define REG_CLEAR_L         REG_CMD | REG_CMD_AUTO_INC | 0x14
#define REG_RED_L           REG_CMD | REG_CMD_AUTO_INC | 0x16
#define REG_GREEN_L         REG_CMD | REG_CMD_AUTO_INC | 0x18
#define REG_BLUE_L          REG_CMD | REG_CMD_AUTO_INC | 0x1A

#define REG_ENABLE          REG_CMD | 0
#define REG_ATIME           REG_CMD | 1
#define REG_CONTROL         REG_CMD | 0x0F
#define REG_STATUS          REG_CMD | 0x13


#define REG_CONTROL_GAIN_1X     0b00000000
#define REG_CONTROL_GAIN_4X     0b00000001
#define REG_CONTROL_GAIN_16X    0b00000010
#define REG_CONTROL_GAIN_60X    0b00000011

#define REG_ENABLE_INTERUPT     1 << 4
#define REG_ENABLE_WAIT         1 << 3
#define REG_ENABLE_RGBC         1 << 1
#define REG_ENABLE_POWER        1


//Color Constants
#define RED_CH      0
#define GREEN_CH    1
#define BLUE_CH     2
#define CLEAR_CH    3


/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 tcs3472_LibInit(void);
int32 tcs3472_HwInit(void);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TCS3472_LibInit(void)
{
    int32 status = TCS3472_HwInit();
    if (status != CFE_SUCCESS) {
        return 0;
    }
    else {    
        return CFE_SUCCESS;
    }
}/* End tcs3472_LibInit */


//Initialize tsc3472 hardware 
int32 TCS3472_HwInit(void) {
    int fd = wiringPiI2CSetup(ADDR);

    if (fd < 0) { return -1; }

    wiringPiI2CWriteReg8(fd, REG_ENABLE, REG_ENABLE_RGBC | REG_ENABLE_POWER);

    //check Status of device if none found break
    if ((wiringPiI2CReadReg8(fd, REG_STATUS) & 1) < 0) { return -1; }

    //set integration time
    wiringPiI2CWriteReg8(fd, REG_ATIME, 43);

    close(fd);
    return CFE_SUCCESS;

} /* END HwInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* TCS3472 Lib functions                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//returns raw color channels
void TCS3472_GetRawData(int data[]) {
    int fd = wiringPiI2CSetup(ADDR);

    data[RED_CH] = wiringPiI2CReadReg16(fd, REG_RED_L);
    data[GREEN_CH] = wiringPiI2CReadReg16(fd, REG_GREEN_L);
    data[BLUE_CH] = wiringPiI2CReadReg16(fd, REG_BLUE_L);
    data[CLEAR_CH] = wiringPiI2CReadReg16(fd, REG_CLEAR_L);
    
    close(fd);
} /* END GetRawData */


//returns scaled color channels 0 1.0 based on clear
void TCS3472_GetScaledData(float data[]) {
    int raw[4];
    TCS3472_GetRawData(raw);

    data[RED_CH] = (float) *(raw + RED_CH) / (float) *(raw + CLEAR_CH);
    data[GREEN_CH] = (float) *(raw + GREEN_CH) / (float) *(raw + CLEAR_CH);
    data[BLUE_CH] = (float) *(raw + BLUE_CH) / (float) *(raw + CLEAR_CH);    
} /* END GetScaledData */


//returns the RGB Values
void TCS3472_GetRGB(float data[]) {
    float scaled[3];
    TCS3472_GetScaledData(scaled);    

    data[RED_CH] = (int32) ( *(scaled + RED_CH) * 255);
    data[GREEN_CH] = (int32) ( *(scaled + GREEN_CH) * 255);
    data[BLUE_CH] = (int32) ( *(scaled + BLUE_CH) * 255);
    
} /* END GetRGB */


//returns the raw light level
int32 TCS3472_GetLightLvl(void) {
    int data [4]; 
    TCS3472_GetRawData(data);

    return (int32) *(data + CLEAR_CH);
} /* end GetLightLvl */


/************************/
/*  End of File Comment */
/************************/
