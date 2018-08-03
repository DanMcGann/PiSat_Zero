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
** File Name:  picam_mission_cfg.h
**
** Title:  Mission Configuration Header File for PICAM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-15
**
** Purpose:  This header file contains declartions and definitions of all PICAM's 
**           mission-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-15 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PICAM_MISSION_CFG_H_
#define _PICAM_MISSION_CFG_H_

/*
** Include Files
*/
#include "cfe.h"



/*
** PICAM Mission Configuration Parameter Definitions
*/

/*  PICAM_DATA_DIR
*   The data location that the picam app will save its data
*/
#define PICAM_DATA_DIR "/cf/picam_data/"

/*  PICA_FILETYPE
*   The data format the picam should save its data as
*/
#define PICAM_FILETYPE ".jpg"

#endif /* _PICAM_MISSION_CFG_H_ */

/*=======================================================================================
** End of file picam_mission_cfg.h
**=====================================================================================*/
    
