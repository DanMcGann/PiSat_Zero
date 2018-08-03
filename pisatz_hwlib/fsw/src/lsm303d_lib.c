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
**   $Id: lsm303d_lib.c $
**
** Purpose: 
**   lsm303d Magnometer and Accelerometer library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/

#include "lsm303d_lib.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include <math.h>
#include <time.h>
/*************************************************************************
** Macro Definitions
*************************************************************************/

// LSM303D Address //
#define ADDR    0x1D

// LSM303D Registers //
#define TEMP_OUT_L      0x05
#define TEMP_OUT_H      0x06 
#define STATUS_REG_M    0X07
#define OUT_X_L_M       0X08
#define OUT_X_H_M       0X09
#define OUT_Y_L_M       0X0A
#define OUT_Y_H_M       0x0B
#define OUT_Z_L_M       0x0C
#define OUT_Z_H_M       0x0D
#define WHO_AM_I        0x0F
#define INT_CTRL_M      0x12
#define INT_SRC_M       0x13
#define INT_THS_L_M     0x14
#define INT_THS_H_M     0x15
#define OFFSET_X_L_M    0X16
#define OFFSET_X_H_M    0X17
#define OFFSET_Y_L_M    0X18
#define OFFSET_Y_H_M    0x19
#define OFFSET_Z_L_M    0x1A
#define OFFSET_Z_H_M    0x1B
#define REFRENCE_X      0x1C
#define REFRENCE_Y      0x1D
#define REFRENCE_Z      0x1E
#define CTRL_REG0       0x1F
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG3       0x22
#define CTRL_REG4       0x23
#define CTRL_REG5       0x24
#define CTRL_REG6       0x25
#define CTRL_REG7       0x26
#define STATUS_REG_A    0x27
#define OUT_X_L_A       0X28
#define OUT_X_H_A       0X29
#define OUT_Y_L_A       0X2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D
#define FIFO_CTRL       0x2E
#define FIFO_SRC        0x2F
#define IG_CFG1         0x30
#define IG_SRC1         0x31
#define IG_THS1         0x32
#define IG_DUR1         0x33
#define IG_CFG2         0x34
#define IG_SRC2         0x35
#define IG_THS2         0x36
#define IG_DUR2         0x37
#define CLICK_CFG       0x38
#define CLICK_SRC       0x39
#define CLICK_THS       0x3A
#define TIME_LIMIT      0x3B
#define TIME_LATENCY    0x3C
#define TIME_WINDOW     0x3D
#define ACT_THS         0x3E
#define ACT_DUR         0x3F

// Magnometer Scale //
#define MAG_SCALE_2     0x00 // +/- 2 gauss
#define MAG_SCALE_4     0x20 // +/- 4 gauss
#define MAG_SCALE_8     0x40 // +/- 8 gauss
#define MAG_SCALE_12    0x60

// Accelerometer Scale //
#define ACCEL_SCALE     2 // +/- 2 g


/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 LSM303D_LibInit(void);
int32 LSM_303D_HwInit(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LSM303D_LibInit(void) {
    
    if (LSM303D_HwInit() != CFE_SUCCESS) {
        OS_printf("LSM303D Hardware Init Failed. \n");
    }
    
    return CFE_SUCCESS;

} /* End LSM303D_LibInit */

int32 LSM303D_HwInit(void) {
    int fd =  wiringPiI2CSetup(ADDR);
    
    if (fd < 0) { return -1; }

    int32 whoami = wiringPiI2CReadReg8(fd, WHO_AM_I);

    if (whoami != 0x49) { return -1; };

    wiringPiI2CWriteReg8(fd, CTRL_REG1, 0x57); // ACCEL ODR = 50 hz
    wiringPiI2CWriteReg8(fd, CTRL_REG2, (3<<6)|(0<<3)); // set full scale +/- 2 g
    wiringPiI2CWriteReg8(fd, CTRL_REG3, 0x00); // no interupt
    wiringPiI2CWriteReg8(fd, CTRL_REG4, 0x00); // no interupt
    wiringPiI2CWriteReg8(fd, CTRL_REG5, (4<<2)); // MAG ODR = 50 hz
    wiringPiI2CWriteReg8(fd, CTRL_REG6, MAG_SCALE_2); // Mag scale +/- 2 g
    wiringPiI2CWriteReg8(fd, CTRL_REG7, 0x00);

    close(fd);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LSM303D Lib functions                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//returns magnometer vector
LSM303D_Vector_t LSM303D_GetMagData(void) {

    LSM303D_Vector_t mdata;
    
    int fd =  wiringPiI2CSetup(ADDR);
    int16 mx = (uint16)(wiringPiI2CReadReg8(fd, OUT_X_L_M) |
        wiringPiI2CReadReg8(fd, OUT_X_H_M) << 8);
    int16 my = (uint16)(wiringPiI2CReadReg8(fd, OUT_Y_L_M) |
        wiringPiI2CReadReg8(fd, OUT_Y_H_M) << 8);
    int16 mz = (uint16)(wiringPiI2CReadReg8(fd, OUT_Z_L_M) |
        wiringPiI2CReadReg8(fd, OUT_Z_H_M) << 8);

    //scale raw data to gauss
    mdata.x = mx * 0.00008F;
    mdata.y = my * 0.00008F;
    mdata.z = mz * 0.00008F;
    
    close(fd);

    return mdata;
} /* End LSM303D_GetMagData */

//returns accelerometer data
LSM303D_Vector_t LSM303D_GetAccelData(void) {

    LSM303D_Vector_t adata;

    int fd =  wiringPiI2CSetup(ADDR);
    int16 ax = (uint16)(wiringPiI2CReadReg8(fd, OUT_X_H_A) << 8 |
        wiringPiI2CReadReg8(fd, OUT_X_L_A));
    int16 ay = (uint16)(wiringPiI2CReadReg8(fd, OUT_Y_H_A) << 8 |
        wiringPiI2CReadReg8(fd, OUT_Y_L_A));
    int16 az = (uint16)(wiringPiI2CReadReg8(fd, OUT_Z_H_A) << 8 |
         wiringPiI2CReadReg8(fd, OUT_Z_L_A));

    //scale raw data to Gs
    adata.x = ax * 0.000061F;
    adata.y = ay * 0.000061F;
    adata.z = az * 0.000061F;
    close(fd);
    return adata;
} /* End LSM303D_GetAccelData */

/************************/
/*  End of File Comment */
/************************/
