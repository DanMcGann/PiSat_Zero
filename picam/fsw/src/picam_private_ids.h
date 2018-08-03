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
** File Name:  picam_private_ids.h
**
** Title:  ID Header File for PICAM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-15
**
** Purpose:  This header file contains declarations and definitions of PICAM's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-15 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PICAM_PRIVATE_IDS_H_
#define _PICAM_PRIVATE_IDS_H_

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
#define PICAM_RESERVED_EID  0

#define PICAM_INF_EID        1
#define PICAM_INIT_INF_EID   2
#define PICAM_ILOAD_INF_EID  3
#define PICAM_CDS_INF_EID    4
#define PICAM_CMD_INF_EID    5

#define PICAM_ERR_EID         51
#define PICAM_INIT_ERR_EID    52
#define PICAM_ILOAD_ERR_EID   53
#define PICAM_CDS_ERR_EID     54
#define PICAM_CMD_ERR_EID     55
#define PICAM_PIPE_ERR_EID    56
#define PICAM_MSGID_ERR_EID   57
#define PICAM_MSGLEN_ERR_EID  58

#define PICAM_EVT_CNT  14

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

#endif /* _PICAM_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file picam_private_ids.h
**=====================================================================================*/
    