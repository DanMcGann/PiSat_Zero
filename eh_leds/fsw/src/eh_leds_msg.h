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
/*=======================================================================================
** File Name:  eh_leds_msg.h
**
** Title:  Message Definition Header File for EH_LEDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-01-31
**
** Purpose:  To define EH_LEDS's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-01-31 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _EH_LEDS_MSG_H_
#define _EH_LEDS_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/



/*
** Local Defines
*/

/*
** EH_LEDS command codes
*/
#define EH_LEDS_NOOP_CC                 0
#define EH_LEDS_RESET_CC                1

#define EH_LEDS_ON_CC                   2
#define EH_LEDS_OFF_CC                  3

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* declarations for Custom housekeeping data here */
    uint8              ledStatus;

} EH_LEDS_HkTlm_t;


#endif /* _EH_LEDS_MSG_H_ */

/*=======================================================================================
** End of file eh_leds_msg.h
**=====================================================================================*/
    
