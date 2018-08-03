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
** File Name:  picam_private_types.h
**
** Title:  Type Header File for PICAM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-15
**
** Purpose:  This header file contains declarations and definitions of all PICAM's private
**           data structures and data types.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-15 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PICAM_PRIVATE_TYPES_H_
#define _PICAM_PRIVATE_TYPES_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} PICAM_NoArgCmd_t;


typedef struct
{
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices or data subscribed from other apps' output data.
    */

} PICAM_InData_t;

typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} PICAM_OutData_t;

/* TODO:  Add more private structure definitions here, if necessary. */

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#endif /* _PICAM_PRIVATE_TYPES_H_ */

/*=======================================================================================
** End of file picam_private_types.h
**=====================================================================================*/
    