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
** File: bmp280_lib.h
**
** Purpose: 
**   Specification for the bmp280 library functions.
**
*************************************************************************/
#ifndef _bmp280_lib_h_
#define _bmp280_lib_h_

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "wiringPi.h"


/************************************************************************
** Type Definitions
*************************************************************************/

/*************************************************************************
** Exported Functions
*************************************************************************/
extern int32 BMP280_LibInit(void);
extern int32 BMP280_HwInit(void);

extern int32_t BMP280_GetPressRaw(void);
extern int32_t BMP280_GetTempRaw(void);
extern float BMP280_GetPress(void);
extern float BMP280_GetTemp(void);

#endif /* _bmp280_lib_h_ */

/************************/
/*  End of File Comment */
/************************/
