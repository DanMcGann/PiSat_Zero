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
** File Name:  eh_tp_app.h
**
** Title:  Header File for EH_TP Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-02-02
**
** Purpose:  To define EH_TP's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-02-02 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _EH_TP_APP_H_
#define _EH_TP_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "eh_tp_platform_cfg.h"
#include "eh_tp_mission_cfg.h"
#include "eh_tp_private_ids.h"
#include "eh_tp_private_types.h"
#include "eh_tp_perfids.h"
#include "eh_tp_msgids.h"
#include "eh_tp_msg.h"

#include "bmp280_lib.h"

/*
** Local Defines
*/
#define EH_TP_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[EH_TP_EVT_CNT];

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
       Data structure should be defined in eh_tp/fsw/src/eh_tp_private_types.h */
    EH_TP_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in eh_tp/fsw/src/eh_tp_private_types.h */
    EH_TP_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in eh_tp/fsw/src/eh_tp_msg.h */
    EH_TP_HkTlm_t  HkTlm;

    float iTemp;
    float iPress;
} EH_TP_AppData_t;

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
int32  EH_TP_InitApp(void);
int32  EH_TP_InitEvent(void);
int32  EH_TP_InitData(void);
int32  EH_TP_InitPipe(void);

void  EH_TP_AppMain(void);

void  EH_TP_CleanupCallback(void);

void EH_TP_UpdateData(void);

int32  EH_TP_RcvMsg(int32 iBlocking);

void  EH_TP_ProcessNewData(void);
void  EH_TP_ProcessNewCmds(void);
void  EH_TP_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  EH_TP_ReportHousekeeping(void);
void  EH_TP_SendOutData(void);

boolean  EH_TP_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _EH_TP_APP_H_ */

/*=======================================================================================
** End of file eh_tp_app.h
**=====================================================================================*/
    
