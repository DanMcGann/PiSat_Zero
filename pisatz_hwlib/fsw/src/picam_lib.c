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
**   $Id: picam_lib.c $
**
** Purpose: 
**   Leds CFS library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "picam_lib.h"

#include "stdio.h"
#include "wiringPi.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/

/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 PICAM_LibInit(void);

int32 PICAM_HwInit(void);
void PICAM_LIB_Capture(int width, int height, int quality, char* file);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PICAM_LibInit(void)
{
    return CFE_SUCCESS;
 
}/* End PICAM_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Leds Lib functions                                             */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//PICAM hardware initilization
int32 PICAM_HwInit(void) {
    return CFE_SUCCESS;
}   /* End PICAM_HwInit */


void PICAM_LIB_Capture(int width, int height, int quality, char* filename) {
    char cmd_str[OS_MAX_FILE_NAME*5];
    strcpy(cmd_str, "raspistill -n -o ");
    strcat(cmd_str, filename);


    //make width and height into string
    char w[8];
    sprintf(w, "%d", width);
    
    strcat(cmd_str, " -w ");
    strcat(cmd_str, w);

    char h[8];
    sprintf(h, "%d", height);
    
    strcat(cmd_str, " -h ");
    strcat(cmd_str, h);

    char q[8];
    sprintf(q, "%d", quality);

    strcat(cmd_str, " -q ");
    strcat(cmd_str, q); 

    //Command option to fork process and not block program
    strcat(cmd_str, " &");

    system(cmd_str);
}


/************************/
/*  End of File Comment */
/************************/
