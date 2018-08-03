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
**   $Id: leds_lib.c $
**
** Purpose: 
**   Leds CFS library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "leds_lib.h"

#include "stdio.h"
#include "wiringPi.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
#define POUT 7


#define HIGH 1
#define LOW  0

/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 LEDS_LibInit(void);

int32 LEDS_HwInit(void);
void LEDS_On(void);
void LEDS_Off(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LEDS_LibInit(void)
{
    return CFE_SUCCESS;
 
}/* End LEDS_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Leds Lib functions                                             */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//LEDS hardware initilization
int32 LEDS_HwInit(void) {
    if (wiringPiSetup() == -1) {
        OS_printf("LEDS_LIB: gpio not enabled - wiringPiSetup failed");
        return 0;
    }

    pinMode(POUT, OUTPUT);

    return CFE_SUCCESS;
}   /* End LEDS_HwInit */


//LEDS on function
void LEDS_On( void ) {
    digitalWrite(POUT, HIGH);
}   /* End LEDS_On */


//LEDS off function
void LEDS_Off( void ) {
    digitalWrite(POUT, LOW);
}   /* End LEDS_Off */


/************************/
/*  End of File Comment */
/************************/
