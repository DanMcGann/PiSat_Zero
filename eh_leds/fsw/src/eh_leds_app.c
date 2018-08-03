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
** File Name:  eh_leds_app.c
**
** Title:  Function Definitions for EH_LEDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-01-31
**
** Purpose:  This source file contains all necessary function definitions to run EH_LEDS
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-01-31 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>

#include "cfe.h"

#include "eh_leds_platform_cfg.h"
#include "eh_leds_mission_cfg.h"
#include "eh_leds_app.h"

#include "leds_lib.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
EH_LEDS_AppData_t  g_EH_LEDS_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/
    
/*=====================================================================================
** Name: EH_LEDS_InitEvent
**
** Purpose: To initialize and register event table for EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    EH_LEDS_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_EH_LEDS_AppData.EventTbl
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 EH_LEDS_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_EH_LEDS_AppData.EventTbl, 0x00, sizeof(g_EH_LEDS_AppData.EventTbl));

    g_EH_LEDS_AppData.EventTbl[0].EventID = EH_LEDS_RESERVED_EID;
    g_EH_LEDS_AppData.EventTbl[1].EventID = EH_LEDS_INF_EID;
    g_EH_LEDS_AppData.EventTbl[2].EventID = EH_LEDS_INIT_INF_EID;
    g_EH_LEDS_AppData.EventTbl[3].EventID = EH_LEDS_ILOAD_INF_EID;
    g_EH_LEDS_AppData.EventTbl[4].EventID = EH_LEDS_CDS_INF_EID;
    g_EH_LEDS_AppData.EventTbl[5].EventID = EH_LEDS_CMD_INF_EID;

    g_EH_LEDS_AppData.EventTbl[ 6].EventID = EH_LEDS_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[ 7].EventID = EH_LEDS_INIT_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[ 8].EventID = EH_LEDS_ILOAD_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[ 9].EventID = EH_LEDS_CDS_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[10].EventID = EH_LEDS_CMD_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[11].EventID = EH_LEDS_PIPE_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[12].EventID = EH_LEDS_MSGID_ERR_EID;
    g_EH_LEDS_AppData.EventTbl[13].EventID = EH_LEDS_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_EH_LEDS_AppData.EventTbl,
                               EH_LEDS_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("EH_LEDS - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: EH_LEDS_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    EH_LEDS_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_EH_LEDS_AppData.usSchPipeDepth
**    g_EH_LEDS_AppData.cSchPipeName
**    g_EH_LEDS_AppData.SchPipeId
**    g_EH_LEDS_AppData.usCmdPipeDepth
**    g_EH_LEDS_AppData.cCmdPipeName
**    g_EH_LEDS_AppData.CmdPipeId
**    g_EH_LEDS_AppData.usTlmPipeDepth
**    g_EH_LEDS_AppData.cTlmPipeName
**    g_EH_LEDS_AppData.TlmPipeId
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 EH_LEDS_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_EH_LEDS_AppData.usSchPipeDepth = EH_LEDS_SCH_PIPE_DEPTH;
    memset((void*)g_EH_LEDS_AppData.cSchPipeName, '\0', sizeof(g_EH_LEDS_AppData.cSchPipeName));
    strncpy(g_EH_LEDS_AppData.cSchPipeName, "EH_LEDS_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_EH_LEDS_AppData.SchPipeId,
                                 g_EH_LEDS_AppData.usSchPipeDepth,
                                 g_EH_LEDS_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(EH_LEDS_WAKEUP_MID, g_EH_LEDS_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("EH_LEDS - Sch Pipe failed to subscribe to EH_LEDS_WAKEUP_MID. (0x%08lX)\n", iStatus);
            goto EH_LEDS_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("EH_LEDS - Failed to create SCH pipe (0x%08lX)\n", iStatus);
        goto EH_LEDS_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_EH_LEDS_AppData.usCmdPipeDepth = EH_LEDS_CMD_PIPE_DEPTH ;
    memset((void*)g_EH_LEDS_AppData.cCmdPipeName, '\0', sizeof(g_EH_LEDS_AppData.cCmdPipeName));
    strncpy(g_EH_LEDS_AppData.cCmdPipeName, "EH_LEDS_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_EH_LEDS_AppData.CmdPipeId,
                                 g_EH_LEDS_AppData.usCmdPipeDepth,
                                 g_EH_LEDS_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(EH_LEDS_CMD_MID, g_EH_LEDS_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("EH_LEDS - CMD Pipe failed to subscribe to EH_LEDS_CMD_MID. (0x%08lX)\n", iStatus);
            goto EH_LEDS_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(EH_LEDS_SEND_HK_MID, g_EH_LEDS_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("EH_LEDS - CMD Pipe failed to subscribe to EH_LEDS_SEND_HK_MID. (0x%08lX)\n", iStatus);
            goto EH_LEDS_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("EH_LEDS - Failed to create CMD pipe (0x%08lX)\n", iStatus);
        goto EH_LEDS_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_EH_LEDS_AppData.usTlmPipeDepth = EH_LEDS_TLM_PIPE_DEPTH;
    memset((void*)g_EH_LEDS_AppData.cTlmPipeName, '\0', sizeof(g_EH_LEDS_AppData.cTlmPipeName));
    strncpy(g_EH_LEDS_AppData.cTlmPipeName, "EH_LEDS_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_EH_LEDS_AppData.TlmPipeId,
                                 g_EH_LEDS_AppData.usTlmPipeDepth,
                                 g_EH_LEDS_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_EH_LEDS_AppData.TlmPipeId);
        */
    }
    else
    {
        CFE_ES_WriteToSysLog("EH_LEDS - Failed to create TLM pipe (0x%08lX)\n", iStatus);
        goto EH_LEDS_InitPipe_Exit_Tag;
    }

EH_LEDS_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: EH_LEDS_InitData
**
** Purpose: To initialize global variables used by EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    EH_LEDS_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_EH_LEDS_AppData.InData
**    g_EH_LEDS_AppData.OutData
**    g_EH_LEDS_AppData.HkTlm
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 EH_LEDS_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_EH_LEDS_AppData.InData, 0x00, sizeof(g_EH_LEDS_AppData.InData));

    /* Init output data */
    memset((void*)&g_EH_LEDS_AppData.OutData, 0x00, sizeof(g_EH_LEDS_AppData.OutData));
    CFE_SB_InitMsg(&g_EH_LEDS_AppData.OutData,
                   EH_LEDS_OUT_DATA_MID, sizeof(g_EH_LEDS_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_EH_LEDS_AppData.HkTlm, 0x00, sizeof(g_EH_LEDS_AppData.HkTlm));
    CFE_SB_InitMsg(&g_EH_LEDS_AppData.HkTlm,
                   EH_LEDS_HK_TLM_MID, sizeof(g_EH_LEDS_AppData.HkTlm), TRUE);

    return (iStatus);
}
    
/*=====================================================================================
** Name: EH_LEDS_InitApp
**
** Purpose: To initialize all data local to and used by EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    EH_LEDS_InitEvent
**    EH_LEDS_InitPipe
**    EH_LEDS_InitData
**
** Called By:
**    EH_LEDS_AppMain
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 EH_LEDS_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_EH_LEDS_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("EH_LEDS - Failed to register the app (0x%08lX)\n", iStatus);
        goto EH_LEDS_InitApp_Exit_Tag;
    }

    if ((EH_LEDS_InitEvent() != CFE_SUCCESS) || 
        (EH_LEDS_InitPipe() != CFE_SUCCESS) || 
        (EH_LEDS_InitData() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto EH_LEDS_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&EH_LEDS_CleanupCallback);

    LEDS_HwInit();
    
EH_LEDS_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(EH_LEDS_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "EH_LEDS - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("EH_LEDS - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: EH_LEDS_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
}
    
/*=====================================================================================
** Name: EH_LEDS_RcvMsg
**
** Purpose: To receive and process messages for EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization 
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    EH_LEDS_ProcessNewCmds
**    EH_LEDS_ProcessNewData
**    EH_LEDS_SendOutData
**
** Called By:
**    EH_LEDS_Main
**
** Global Inputs/Reads:
**    g_EH_LEDS_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_EH_LEDS_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 EH_LEDS_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(EH_LEDS_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_EH_LEDS_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(EH_LEDS_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case EH_LEDS_WAKEUP_MID:
                EH_LEDS_ProcessNewCmds();
                EH_LEDS_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                   automatically publish new output. */
                EH_LEDS_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(EH_LEDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "EH_LEDS - Recvd invalid SCH msgId (0x%08X)", (unsigned int) MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or nothing */
    }
    else
    {
        /* This is an example of exiting on an error.
        ** Note that a SB read error is not always going to result in an app quitting.
        */
        CFE_EVS_SendEvent(EH_LEDS_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "EH_LEDS: SB pipe read error (0x%08lX), app will exit", iStatus);
        g_EH_LEDS_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: EH_LEDS_ProcessNewData
**
** Purpose: To process incoming data subscribed by EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**
** Called By:
**    EH_LEDS_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_EH_LEDS_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                /* TODO:  Add code to process all subscribed data here 
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         EH_LEDS_ProcessNavData(TlmMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(EH_LEDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "EH_LEDS - Recvd invalid TLM msgId (0x%08X)", (unsigned int) TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(EH_LEDS_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "EH_LEDS: CMD pipe read error (0x%08X)", (unsigned int) iStatus);
            g_EH_LEDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: EH_LEDS_ProcessNewCmds
**
** Purpose: To process incoming command messages for EH_LEDS application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    EH_LEDS_ProcessNewAppCmds
**    EH_LEDS_ReportHousekeeping
**
** Called By:
**    EH_LEDS_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_EH_LEDS_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case EH_LEDS_CMD_MID:
                    EH_LEDS_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case EH_LEDS_SEND_HK_MID:
                    EH_LEDS_ReportHousekeeping();
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         EH_LEDS_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(EH_LEDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "EH_LEDS - Recvd invalid CMD msgId (0x%08X)", (unsigned int) CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(EH_LEDS_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "EH_LEDS: CMD pipe read error (0x%08X)", (unsigned int) iStatus);
            g_EH_LEDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: EH_LEDS_ProcessNewAppCmds
**
** Purpose: To process command messages targeting EH_LEDS application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    EH_LEDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_EH_LEDS_AppData.HkTlm.usCmdCnt
**    g_EH_LEDS_AppData.HkTlm.usCmdErrCnt
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case EH_LEDS_NOOP_CC:
                g_EH_LEDS_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(EH_LEDS_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "EH_LEDS - Recvd NOOP cmd (%d)", (unsigned int) uiCmdCode);
                break;

            case EH_LEDS_RESET_CC:
                g_EH_LEDS_AppData.HkTlm.usCmdCnt = 0;
                g_EH_LEDS_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(EH_LEDS_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "EH_LEDS - Recvd RESET cmd (%d)", (unsigned int) uiCmdCode);
                break;
            case EH_LEDS_ON_CC:
                g_EH_LEDS_AppData.HkTlm.usCmdCnt++;
                g_EH_LEDS_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(EH_LEDS_CMD_INF_EID, CFE_EVS_INFORMATION,
                        "EH_LEDS - Recvd ON cmd (%d)", (unsigned int) uiCmdCode); 
                LEDS_On();
                
                break;
            case EH_LEDS_OFF_CC:
                g_EH_LEDS_AppData.HkTlm.usCmdCnt++;
                g_EH_LEDS_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(EH_LEDS_CMD_INF_EID, CFE_EVS_INFORMATION,
                        "EH_LEDS - Recvd OFF cmd (%d)", (unsigned int) uiCmdCode);
                LEDS_Off();
                
                break;
            default:
                g_EH_LEDS_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(EH_LEDS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "EH_LEDS - Recvd invalid cmdId (%d)", (unsigned int) uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: EH_LEDS_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    EH_LEDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, Daniel McGann
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_EH_LEDS_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_EH_LEDS_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: EH_LEDS_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    EH_LEDS_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_EH_LEDS_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_EH_LEDS_AppData.OutData);
}
    
/*=====================================================================================
** Name: EH_LEDS_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    EH_LEDS_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean EH_LEDS_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(EH_LEDS_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "EH_LEDS - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              (unsigned int) MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_EH_LEDS_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: EH_LEDS_AppMain
**
** Purpose: To define EH_LEDS application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    EH_LEDS_InitApp
**    EH_LEDS_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Daniel McGann 
**
** History:  Date Written  2018-01-31
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void EH_LEDS_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(EH_LEDS_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (EH_LEDS_InitApp() != CFE_SUCCESS)
    {
        g_EH_LEDS_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(EH_LEDS_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(EH_LEDS_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(EH_LEDS_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_EH_LEDS_AppData.uiRunStatus) == TRUE)
    {
        EH_LEDS_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(EH_LEDS_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_EH_LEDS_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file eh_leds_app.c
**=====================================================================================*/
    
