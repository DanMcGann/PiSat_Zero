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
** File Name:  eh_cls_private_ids.h
**
** Title:  ID Header File for EH_CLS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-02-02
**
** Purpose:  This header file contains declarations and definitions of EH_CLS's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-02-02 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _EH_CLS_PRIVATE_IDS_H_
#define _EH_CLS_PRIVATE_IDS_H_

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
*/

/* Event IDs */
#define EH_CLS_RESERVED_EID  0

#define EH_CLS_INF_EID        1
#define EH_CLS_INIT_INF_EID   2
#define EH_CLS_ILOAD_INF_EID  3
#define EH_CLS_CDS_INF_EID    4
#define EH_CLS_CMD_INF_EID    5

#define EH_CLS_ERR_EID         51
#define EH_CLS_INIT_ERR_EID    52
#define EH_CLS_ILOAD_ERR_EID   53
#define EH_CLS_CDS_ERR_EID     54
#define EH_CLS_CMD_ERR_EID     55
#define EH_CLS_PIPE_ERR_EID    56
#define EH_CLS_MSGID_ERR_EID   57
#define EH_CLS_MSGLEN_ERR_EID  58

#define EH_CLS_EVT_CNT  14

/*
** Local Structure Declarations
*/

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

#endif /* _EH_CLS_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file eh_cls_private_ids.h
**=====================================================================================*/
    