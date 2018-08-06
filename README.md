# PiSat_Zero
Core Flight System (cFS) Apps for PiSat Zero Hardware

  These are a collection of apps and hardware libraries used to interface with the envirophat sensor used ont he PiSat Zero. More specifically these apps provide functionality for the follwoing sensors.
  
  #### Color and Light Sensor: tcs3472 
  The tcs3472 sensor reads light level and color information. This is interfaced with by the tcs3472_lib located as part of the pisat_hwlib cfe library. This is controlled by the eh_cls cfe app.
  #### Temperature and Pressure: bmp280
  The bmp280 sensor provides temperature and pressure information. This is interfaces with by the bmp280_lib located as part of the pisat_hwlib cfe library. This is controlled by the eh_tp cfe app.
  #### Magnetometer and Accelerometer: lsm303d
  The lsm303d sensor provides temperature and pressure information. This is interfaces with by the lsm303_lib located as part of the pisat_hwlib cfe library. This is controlled by the eh_imu cfe app.
    
## Installation and Config

  ### Requirements
   - cFS: NASA Goddard's Core Flight System
      - A stock version of cFS installed and configured on your PiSat Zero.
      - Instructions and source code can be found here https://github.com/nasa/cFE
      - Additional Instructions
        - cFS may not build imediatly on the raspberry pi. This is caused by the raspberry pi gcc toolchain rejecting the inclusion of any -m32 make flags (On other systems these specify that the project is to be built for 32 bit architectures, however on the pi there is no other option and these flags are not accepted).
        - To avoid this run the script m32-flag-rm.sh located in the Scripts folder.
        - Alternatily build cfs using the pi_make_cfs.sh script which does this automatically.
      
   - WiringPi: Raspberry Pi gpio interaction suite
      - Raspbian Stretch may come with this preinstalled ( UNCONFIRMED ).
      - Installation Instructions can be found here http://wiringpi.com/download-and-install/
    
   - COSMOS
      - Open Souce Ground Station Avaliable from Ball Aero Space 
      - Download and Install Information can be found here http://cosmosrb.com/

  ### Installation
  Note this installation and build process assumes that the make build system is being used. Seting up building of the code using the CMake build system is different, a refrence for how to use that system can be found in /cfe/cmake/README.md from the cFS distribution.
   1. Clone or download this repo onto your PiSat.
   2. Copy the folders eh_imu, eh_cls, eh_leds, and pisat_hwlib into your cFS apps folder.
        Usually found 
                      
                       home -
                          cFE - 
                              -apps <------here
                              -build
                              -cfe
                              -docs
                              -osal
                              -psp
                              -tools
    
    
    
   3. Add the apps to be made by adding the following link to /cFE/build/cpu1/Makefile
          
          THE_APPS+= pisatz_hwlib eh_imu eh_tp eh_cls eh_leds
          
      The makefile now should have the following directly before **export THE_APPS**
          
          THE_APPS= sample_app sample_lib
          THE_APPS+= ci_lab to_lab
          THE_APPS+= pisatz_hwlib eh_imu eh_tp eh_cls eh_leds
     
     
     
   4. Add the apps to /cFE/build/cpu1/exe/cfe_startup_script
      
      This is done by adding the following lines
      
          CFE_LIB, /cf/apps/pisatz_hwlib.so,  PISATZ_LibInit,         PISATZ_HWLIB,   0,     0, 0x0, 0;
          CFE_APP, /cf/apps/eh_leds.so,       EH_LEDS_AppMain,        EH_LEDS_APP,   55,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_imu.so,        EH_IMU_AppMain,         EH_IMU_APP,    55,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_cls.so,        EH_CLS_AppMain,         EH_LCS_APP,    55,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_tp.so,         EH_TP_AppMain,          EH_TP_APP,     55,  8192, 0x0, 0;

          
      The cfe_startup_script should now look like this...
           
          CFE_LIB, /cf/apps/pisatz_hwlib.so,  PISATZ_LibInit,         PISATZ_HWLIB,   0,     0, 0x0, 0;
          CFE_LIB, /cf/apps/sample_lib.so,    SAMPLE_LibInit,         SAMPLE_LIB,     0,     0, 0x0, 0;
          CFE_APP, /cf/apps/sample_app.so,    SAMPLE_AppMain,         SAMPLE_APP,    50,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_leds.so,       EH_LEDS_AppMain,        EH_LEDS_APP,   55,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_imu.so,        EH_IMU_AppMain,         EH_IMU_APP,    55,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_cls.so,        EH_CLS_AppMain,         EH_LCS_APP,    55,  8192, 0x0, 0;
          CFE_APP, /cf/apps/eh_tp.so,         EH_TP_AppMain,          EH_TP_APP,     55,  8192, 0x0, 0;

          CFE_APP, /cf/apps/to_lab.so,      TO_Lab_AppMain,  TO_LAB_APP,   70,   8192, 0x0, 0;
          CFE_APP, /cf/apps/sch_lab.so,     SCH_Lab_AppMain, SCH_LAB_APP,  80,   8192, 0x0, 0;
      
      
      
   5. Add the apps to the scheduling lab table in ~/cFE/apps/sch_lab/fsw/platform_inc/sch_lab_sched_tab.h
      
      - At the top of the file add the following app msgids headers to the includes section
            
            #include "eh_leds_msgids.h"
            #include "eh_imu_msgids.h"
            #include "eh_cls_msgids.h"
            #include "eh_tp_msgids.h"
            
      - Add the app wakeup, and hk request msgs to the table.  The table should now look like this...
      
            
            SCH_LAB_ScheduleTable_t SCH_LAB_ScheduleTable[SCH_LAB_MAX_SCHEDULE_ENTRIES] =
            {
                 { CFE_ES_SEND_HK_MID,   4, 0 },  /* Housekeeping requests go out every 4 seconds */
                 { CFE_EVS_SEND_HK_MID,  4, 0 },
                 { CFE_TIME_SEND_HK_MID, 4, 0 },
                 { CFE_SB_SEND_HK_MID,   4, 0 },
                 { CFE_TBL_SEND_HK_MID,  4, 0 },

                 { CI_LAB_SEND_HK_MID,   4, 0 },
                 { TO_LAB_SEND_HK_MID,   4, 0 },

                 /* PiSatZ Apps Wake Up */  <--------------------- Start of new code
                 { EH_LEDS_WAKEUP_MID,   1, 0 },
                 { EH_IMU_WAKEUP_MID,    1, 0 },
                 { EH_CLS_WAKEUP_MID,    1, 0 },
                 { EH_TP_WAKEUP_MID,     1, 0 },

            #if 0
                 { SC_SEND_HK_MID,       4, 0 },
                 { SC_1HZ_WAKEUP_MID,    1, 0 },  /* Example of a 1hz packet */
                 { HS_SEND_HK_MID,       4, 0 },
                 { FM_SEND_HK_MID,       4, 0 },
                 { DS_SEND_HK_MID,       4, 0 },
                 { LC_SEND_HK_MID,       4, 0 },

            #endif
                 /* PiSatz hk schedualing */
                 { EH_LEDS_SEND_HK_MID,  4, 0 },
                 { EH_IMU_SEND_HK_MID,   4, 0 },
                 { EH_CLS_SEND_HK_MID,   4, 0 },
                 { EH_TP_SEND_HK_MID,    4, 0 },

                 { SCH_LAB_END_OF_TABLE, 0, 0 }
            };
            
      This sets the apps to wake up at at a rate of 1 Hz and send out hk data every 4 seconds.


  6. Add the apps to the to_lab sub table at ~/cFE/apps/to_lab/fsw/platform_inc/to_lab_sub_table.h
  
   - Agian first all the app msgids header files to the includes section
         
          #include "eh_leds_msgids.h"
          #include "eh_imu_msgids.h"
          #include "eh_cls_msgids.h"
          #include "eh_tp_msgids.h"
          
   - Add the app msgs to the table. The table should not look like this 
        
          
          static TO_subsciption_t  TO_SubTable[] =
          {
                      /* CFS App Subscriptions */
                      {TO_LAB_HK_TLM_MID,     {0,0},  4},
                      {TO_LAB_DATA_TYPES_MID, {0,0},  4},
                      {CI_LAB_HK_TLM_MID,     {0,0},  4},

          #if 0
                      /* Add these if needed */
                      {HS_HK_TLM_MID,         {0,0},  4},
                      {FM_HK_TLM_MID,         {0,0},  4},
                      {SC_HK_TLM_MID,         {0,0},  4},
                      {DS_HK_TLM_MID,         {0,0},  4},
                      {LC_HK_TLM_MID,         {0,0},  4},
          #endif
                      /* PiSatz Apps */ <---------------------------Start of new code
                      {EH_LEDS_HK_TLM_MID,    {0,0},  4},
                      {EH_IMU_HK_TLM_MID,     {0,0},  4},
                      {EH_CLS_HK_TLM_MID,     {0,0},  4},
                      {EH_TP_HK_TLM_MID,      {0,0},  4},

                      /* cFE Core subsciptions */
                      {CFE_ES_HK_TLM_MID,     {0,0},  4},
                      {CFE_EVS_HK_TLM_MID,    {0,0},  4},
                      {CFE_SB_HK_TLM_MID,     {0,0},  4},
                      {CFE_TBL_HK_TLM_MID,    {0,0},  4},
                      {CFE_TIME_HK_TLM_MID,   {0,0},  4},
                      {CFE_TIME_DIAG_TLM_MID, {0,0},  4},
                      {CFE_SB_STATS_TLM_MID,  {0,0},  4},
                      {CFE_TBL_REG_TLM_MID,   {0,0},  4},
                      {CFE_EVS_EVENT_MSG_MID, {0,0},  32},
                      {CFE_ES_SHELL_TLM_MID,  {0,0},  32},
                      {CFE_ES_APP_TLM_MID,    {0,0},   4},
                      {CFE_ES_MEMSTATS_TLM_MID,{0,0},  4},

                      {TO_UNUSED,              {0,0},  0},
                      {TO_UNUSED,              {0,0},  0},
                      {TO_UNUSED,              {0,0},  0}
          };
  
   Now your PiSat Zero software should be ready to be build and run. this can be done by executing the pi_make_cfs.sh script located in the Scripts folder.
    
  ### Configuring Cosmos
   To configure cosmos foruse with a PiSat on your ground station computer...
	1. Copy PISAT0001 folder from COSMOS_cfg to your /your/path/to/cosmos/config/targets
	2. Copy the cmd_tlm_server.txt from COSMOS_cfg to your/path/to/cosmosconfig/tools/cmd_tlm_server replacing the existing file of the same name if it exists.
	3. Edit the cmd_tlm_server.txt file (now located in /cosmos/config/tools/cmd_tlm_server/) replacing...
		- 10.0.0.30 with the IP address of your PiSat
		- 1235 With the TO_lab Port of your PiSat (1235 is default and the configuration is located in /cFE/apps/to_lab/src/fsw/to_lab_app.h)
  
  
## Non cFS Software
  
 ### OLED Display 
  The OLED folder contains non cFS Python Scripts for the control of the OLED Display on the PiSat Zero. (Currently Only one). The folder should not be placed in the cFE directory on your PiSat and instead it is recommended to be placed in the home directory.
  
  To use this script first install the necissary components to the Raspberry Pi with the following commands.
         
    sudo apt-get install git
    git clone https://github.com/adafruit/Adafruit_Python_SSD1306.git
    cd Adafruit_Python_SSD1306
    sudo python setup.py install
  
  Additionally the following dependencies should be preinstalled on Rasbian but if not will need to be installed using sudo apt-get install <package-name>
  
    build-essential 
    python-dev 
    python-pip
    python-imaging 
    python-smbus

  And then using pip install RPi.GIPO using the following command

    sudo pip install RPi.GPIO 

  Finally to use any text editor to modify the script where marked with satellite specific information. Note: GPIO Pin definitions may need to be changed if PiSat Zero was wired different.
  
  ALSO for ease of use, this script can also be automatically run on PiSat boot. This can be done by adding a line to rc.local located at /etc/rc.local above the line that says exit 0.
    
    python ~/path/to/folder/OLED/PiSatZ_disp.py
    
  ### COSMOS Configurations
  To use cosmos to communicate with the PiSat the existing configuration files found under the COSMOS_cfg folder. For a guide on how to incoorporate these configuration files into COSMOS use the following guides provided by the COSMOS documentation. 
  - http://cosmosrb.com/docs/system/
  - http://cosmosrb.com/docs/command/
  - http://cosmosrb.com/docs/telemetry/
  - http://cosmosrb.com/docs/interfaces/
