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
**   $Id: pisatz_hwlib.c $
**
** Purpose: 
**   PiSat Zero Hardware library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "pisatz_hwlib.h"
#include "pisatz_hwlib_version.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/


/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 PISATZ_LibInit(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PISATZ_LibInit(void)
{
    int32 status;
    OS_printf ("PISATZ HWLIB Initialized.  Version %d.%d.%d.%d \n",
                PISATZ_HWLIB_MAJOR_VERSION,
                PISATZ_HWLIB_MINOR_VERSION,
                PISATZ_HWLIB_REVISION,
                PISATZ_HWLIB_MISSION_REV);
     
    //Initialize all libraries
    
    //LED Lib
    status = LEDS_LibInit();
    if(status == CFE_SUCCESS) {   
        OS_printf("LEDS HW Lib Initialized. \n");
    }
    else {
        OS_printf("LEDS HW Lib Init ERROR. \n");
    }
     
    //LSM303D Lib
    status = LSM303D_LibInit();
    if(status == CFE_SUCCESS) {   
        OS_printf("LSM303D HW Lib Initialized. \n");
    }
    else {
        OS_printf("LSM303D HW Lib Init ERROR. \n");
    }
    
    //bmp280 Lib
    status = BMP280_LibInit();
    if(status == CFE_SUCCESS) {   
        OS_printf("BMP280 HW Lib Initialized. \n");
    }
    else {
        OS_printf("BMP280 HW Lib Init ERROR. \n");
    }

    //TCS3472 Lib
    status = TCS3472_LibInit();
    if(status == CFE_SUCCESS) {   
        OS_printf("TCS3472 HW Lib Initialized. \n");
    }
    else {
        OS_printf("TCS3472 HW Lib Init ERROR. \n");
    }
    
    return CFE_SUCCESS;
   
}/* End PISATZ_HWLIB_LibInit */


/************************/
/*  End of File Comment */
/************************/
