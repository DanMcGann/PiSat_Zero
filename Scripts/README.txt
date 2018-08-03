PiSatZero - Scripts

Purpose: This Folder contains useful scripts for building cFS on a raspberry pi using cFS Classic build system.

Note1: These scripts should only be run on the Raspberry Pi after cFS basic installation has been preformed. 

Note2: These scripts assume that the cFS classic build system (using make files) is being used to build the software. The PiSat Hardware application contained in this repository as of (5/2/2018) are configured to use this build system.


Scripts:

m32-flag-rm.sh - Removes unneeded build flags from cFS.

    Description: On most modern x64 platforms gcc requires that the -m32 flag be when building software that needs to be compatable with 32bit architecture. However, because of the 32 bit architecture of the raspberry pi, everything compiled on it is based on that architecutre. Therefore the Rasbian gcc toolchain does not recoginize this flag, and its inclusion invokes an error. This script when run removes all instances of this flag from cFS so that it can be usccessfully built on a raspberry pi.

    Assumptions - This script assumes that the cFS project is located in a directory ~/cFE/


pi_make_cfs.sh - Configures and builds cFS on a Rasberry Pi.

    Description: This script both configures and builds a cFS Project on a Raspberry Pi. It does so by removing all instances of -m32 flags (see above), configuring XL-Lab parameters, and then invoking the build sequence for cFS.

    Assumptions: 
        1. The cFS project is located at ~/cFE/
        2. The project make file located at ~/cFE/build/cpu1/ has been set up properly for all desired applications.
        
    Notes: If the build is using the Cross link Lab applications XLO and XLI. The commented lines in this script can be uncommented, and will prompt the developer for information to input satellite specific link configuration parameters. If the build is not using these applications this can be ignored.
