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
** File Name:  picam_app.c
**
** Title:  Function Definitions for PICAM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-15
**
** Purpose:  This source file contains all necessary function definitions to run PICAM
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
**   2018-03-15 | Daniel McGann | Build #: Code Started
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

#include "picam_platform_cfg.h"
#include "picam_mission_cfg.h"
#include "picam_app.h"
#include "pisatz_hwlib.h"

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
PICAM_AppData_t  g_PICAM_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/
    
/*=====================================================================================
** Name: PICAM_InitEvent
**
** Purpose: To initialize and register event table for PICAM application
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
**    PICAM_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_PICAM_AppData.EventTbl
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 PICAM_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_PICAM_AppData.EventTbl, 0x00, sizeof(g_PICAM_AppData.EventTbl));

    g_PICAM_AppData.EventTbl[0].EventID = PICAM_RESERVED_EID;
    g_PICAM_AppData.EventTbl[1].EventID = PICAM_INF_EID;
    g_PICAM_AppData.EventTbl[2].EventID = PICAM_INIT_INF_EID;
    g_PICAM_AppData.EventTbl[3].EventID = PICAM_ILOAD_INF_EID;
    g_PICAM_AppData.EventTbl[4].EventID = PICAM_CDS_INF_EID;
    g_PICAM_AppData.EventTbl[5].EventID = PICAM_CMD_INF_EID;

    g_PICAM_AppData.EventTbl[ 6].EventID = PICAM_ERR_EID;
    g_PICAM_AppData.EventTbl[ 7].EventID = PICAM_INIT_ERR_EID;
    g_PICAM_AppData.EventTbl[ 8].EventID = PICAM_ILOAD_ERR_EID;
    g_PICAM_AppData.EventTbl[ 9].EventID = PICAM_CDS_ERR_EID;
    g_PICAM_AppData.EventTbl[10].EventID = PICAM_CMD_ERR_EID;
    g_PICAM_AppData.EventTbl[11].EventID = PICAM_PIPE_ERR_EID;
    g_PICAM_AppData.EventTbl[12].EventID = PICAM_MSGID_ERR_EID;
    g_PICAM_AppData.EventTbl[13].EventID = PICAM_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_PICAM_AppData.EventTbl,
                               PICAM_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("PICAM - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: PICAM_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for PICAM application
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
**    PICAM_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_PICAM_AppData.usSchPipeDepth
**    g_PICAM_AppData.cSchPipeName
**    g_PICAM_AppData.SchPipeId
**    g_PICAM_AppData.usCmdPipeDepth
**    g_PICAM_AppData.cCmdPipeName
**    g_PICAM_AppData.CmdPipeId
**    g_PICAM_AppData.usTlmPipeDepth
**    g_PICAM_AppData.cTlmPipeName
**    g_PICAM_AppData.TlmPipeId
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 PICAM_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_PICAM_AppData.usSchPipeDepth = PICAM_SCH_PIPE_DEPTH;
    memset((void*)g_PICAM_AppData.cSchPipeName, '\0', sizeof(g_PICAM_AppData.cSchPipeName));
    strncpy(g_PICAM_AppData.cSchPipeName, "PICAM_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_PICAM_AppData.SchPipeId,
                                 g_PICAM_AppData.usSchPipeDepth,
                                 g_PICAM_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PICAM_WAKEUP_MID, g_PICAM_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("PICAM - Sch Pipe failed to subscribe to PICAM_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto PICAM_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("PICAM - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto PICAM_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_PICAM_AppData.usCmdPipeDepth = PICAM_CMD_PIPE_DEPTH ;
    memset((void*)g_PICAM_AppData.cCmdPipeName, '\0', sizeof(g_PICAM_AppData.cCmdPipeName));
    strncpy(g_PICAM_AppData.cCmdPipeName, "PICAM_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_PICAM_AppData.CmdPipeId,
                                 g_PICAM_AppData.usCmdPipeDepth,
                                 g_PICAM_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(PICAM_CMD_MID, g_PICAM_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("PICAM - CMD Pipe failed to subscribe to PICAM_CMD_MID. (0x%08X)\n", iStatus);
            goto PICAM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(PICAM_SEND_HK_MID, g_PICAM_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("PICAM - CMD Pipe failed to subscribe to PICAM_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto PICAM_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("PICAM - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto PICAM_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_PICAM_AppData.usTlmPipeDepth = PICAM_TLM_PIPE_DEPTH;
    memset((void*)g_PICAM_AppData.cTlmPipeName, '\0', sizeof(g_PICAM_AppData.cTlmPipeName));
    strncpy(g_PICAM_AppData.cTlmPipeName, "PICAM_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_PICAM_AppData.TlmPipeId,
                                 g_PICAM_AppData.usTlmPipeDepth,
                                 g_PICAM_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_PICAM_AppData.TlmPipeId);
        */
    }
    else
    {
        CFE_ES_WriteToSysLog("PICAM - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto PICAM_InitPipe_Exit_Tag;
    }

PICAM_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: PICAM_InitData
**
** Purpose: To initialize global variables used by PICAM application
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
**    PICAM_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_PICAM_AppData.InData
**    g_PICAM_AppData.OutData
**    g_PICAM_AppData.HkTlm
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 PICAM_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_PICAM_AppData.InData, 0x00, sizeof(g_PICAM_AppData.InData));

    /* Init output data */
    memset((void*)&g_PICAM_AppData.OutData, 0x00, sizeof(g_PICAM_AppData.OutData));
    CFE_SB_InitMsg(&g_PICAM_AppData.OutData,
                   PICAM_OUT_DATA_MID, sizeof(g_PICAM_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_PICAM_AppData.HkTlm, 0x00, sizeof(g_PICAM_AppData.HkTlm));
    CFE_SB_InitMsg(&g_PICAM_AppData.HkTlm,
                   PICAM_HK_TLM_MID, sizeof(g_PICAM_AppData.HkTlm), TRUE);

    g_PICAM_AppData.CaptureCount = 0;    

    return (iStatus);
}
    
/*=====================================================================================
** Name: PICAM_InitApp
**
** Purpose: To initialize all data local to and used by PICAM application
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
**    PICAM_InitEvent
**    PICAM_InitPipe
**    PICAM_InitData
**
** Called By:
**    PICAM_AppMain
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 PICAM_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_PICAM_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("PICAM - Failed to register the app (0x%08X)\n", iStatus);
        goto PICAM_InitApp_Exit_Tag;
    }

    if ((PICAM_InitEvent() != CFE_SUCCESS) || 
        (PICAM_InitPipe() != CFE_SUCCESS) || 
        (PICAM_InitData() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto PICAM_InitApp_Exit_Tag;
    }

    //Create directory to store payload data.
    //If folder already exists this function call will fail, however that is ok
    OS_mkdir(PICAM_DATA_DIR, 0);

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&PICAM_CleanupCallback);

PICAM_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(PICAM_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "PICAM - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("PICAM - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: PICAM_CleanupCallback
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
}
    
/*=====================================================================================
** Name: PICAM_RcvMsg
**
** Purpose: To receive and process messages for PICAM application
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
**    PICAM_ProcessNewCmds
**    PICAM_ProcessNewData
**    PICAM_SendOutData
**
** Called By:
**    PICAM_Main
**
** Global Inputs/Reads:
**    g_PICAM_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_PICAM_AppData.uiRunStatus
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 PICAM_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(PICAM_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_PICAM_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(PICAM_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case PICAM_WAKEUP_MID:
                PICAM_ProcessNewCmds();
                PICAM_ProcessNewData();
                PICAM_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(PICAM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "PICAM - Recvd invalid SCH msgId (0x%08X)", MsgId);
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
        CFE_EVS_SendEvent(PICAM_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "PICAM: SB pipe read error (0x%08X), app will exit", iStatus);
        g_PICAM_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: PICAM_ProcessNewData
**
** Purpose: To process incoming data subscribed by PICAM application
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
**    PICAM_RcvMsg
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_PICAM_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                default:
                    CFE_EVS_SendEvent(PICAM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "PICAM - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(PICAM_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "PICAM: CMD pipe read error (0x%08X)", iStatus);
            g_PICAM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: PICAM_ProcessNewCmds
**
** Purpose: To process incoming command messages for PICAM application
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
**    PICAM_ProcessNewAppCmds
**    PICAM_ReportHousekeeping
**
** Called By:
**    PICAM_RcvMsg
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_PICAM_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case PICAM_CMD_MID:
                    PICAM_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case PICAM_SEND_HK_MID:
                    PICAM_ReportHousekeeping();
                    break;

                default:
                    CFE_EVS_SendEvent(PICAM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "PICAM - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(PICAM_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "PICAM: CMD pipe read error (0x%08X)", iStatus);
            g_PICAM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: PICAM_ProcessNewAppCmds
**
** Purpose: To process command messages targeting PICAM application
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
**    PICAM_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_PICAM_AppData.HkTlm.usCmdCnt
**    g_PICAM_AppData.HkTlm.usCmdErrCnt
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case PICAM_NOOP_CC:
                g_PICAM_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(PICAM_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "PICAM - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case PICAM_RESET_CC:
                g_PICAM_AppData.HkTlm.usCmdCnt = 0;
                g_PICAM_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(PICAM_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "PICAM - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            case PICAM_CAPTURE_CC:
                g_PICAM_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(PICAM_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "PICAM - Recvd CAMPTURE cmd (%d)", uiCmdCode);
                PICAM_CapturePhoto(640, 480);
                break;
            default:
                g_PICAM_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(PICAM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "PICAM - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: PICAM_CapturePhoto
**
** Purpose: Captures a picam photo of the specified size and saves it to a timestamped 
**  file
**
** Arguments:
**    int width - the width of the picture in pixels
**    int height - the height of the picture in pixels
**
**
**
** Called By:
**    PICAM_ProcessNewCmds
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_CapturePhoto(int width, int height) {
    //Update the capture counter
    g_PICAM_AppData.CaptureCount++;    

    //Concat a Unique Filename with file type
    char file[OS_MAX_FILE_NAME *4];
    char name[8];
    sprintf(name, "%d", g_PICAM_AppData.CaptureCount);

    //add the global file path to the file path
    strcpy(file, "/home/pi/cFE/build/cpu1/exe");
    //add the filename to the result
    strcat(file, PICAM_DATA_DIR);
    //add the unique name to the result
    strcat(file, name);
    //add the filetype to the result
    strcat(file, PICAM_FILETYPE);

    //Capture the Image and save the data to the file note: 50 is image "quality"
    PICAM_LIB_Capture(width, height, 50, file);
}

/*=====================================================================================
** Name: PICAM_ReportHousekeeping
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
**    PICAM_ProcessNewCmds
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */    
    g_PICAM_AppData.HkTlm.CaptureCount = g_PICAM_AppData.CaptureCount;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_PICAM_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_PICAM_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: PICAM_SendOutData
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
**    PICAM_RcvMsg
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_PICAM_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_PICAM_AppData.OutData);
}
    
/*=====================================================================================
** Name: PICAM_VerifyCmdLength
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
**    PICAM_ProcessNewCmds
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean PICAM_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            CFE_EVS_SendEvent(PICAM_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "PICAM - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_PICAM_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: PICAM_AppMain
**
** Purpose: To define PICAM application's entry point and main process loop
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
**    PICAM_InitApp
**    PICAM_RcvMsg
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
** History:  Date Written  2018-03-15
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void PICAM_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(PICAM_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (PICAM_InitApp() != CFE_SUCCESS)
    {
        g_PICAM_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(PICAM_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(PICAM_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(PICAM_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_PICAM_AppData.uiRunStatus) == TRUE)
    {
        PICAM_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(PICAM_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_PICAM_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file picam_app.c
**=====================================================================================*/
    
