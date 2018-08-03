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
** File Name:  picam_app.h
**
** Title:  Header File for PICAM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-15
**
** Purpose:  To define PICAM's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-15 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PICAM_APP_H_
#define _PICAM_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "picam_platform_cfg.h"
#include "picam_mission_cfg.h"
#include "picam_private_ids.h"
#include "picam_private_types.h"
#include "picam_perfids.h"
#include "picam_msgids.h"
#include "picam_msg.h"



/*
** Local Defines
*/
#define PICAM_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[PICAM_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;
    
    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in picam/fsw/src/picam_private_types.h */
    PICAM_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in picam/fsw/src/picam_private_types.h */
    PICAM_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in picam/fsw/src/picam_msg.h */
    PICAM_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */
    int32           CaptureCount;

} PICAM_AppData_t;

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
int32  PICAM_InitApp(void);
int32  PICAM_InitEvent(void);
int32  PICAM_InitData(void);
int32  PICAM_InitPipe(void);

void  PICAM_AppMain(void);

void  PICAM_CleanupCallback(void);

int32  PICAM_RcvMsg(int32 iBlocking);

void  PICAM_ProcessNewData(void);
void  PICAM_ProcessNewCmds(void);
void  PICAM_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  PICAM_ReportHousekeeping(void);
void  PICAM_SendOutData(void);

boolean  PICAM_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

void PICAM_CapturePhoto(int width, int height);

#endif /* _PICAM_APP_H_ */

/*=======================================================================================
** End of file picam_app.h
**=====================================================================================*/
    
