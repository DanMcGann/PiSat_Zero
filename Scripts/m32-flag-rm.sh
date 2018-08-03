#*********************************************************************************
#DISTRIBUTION STATEMENT A. Approved for public release. Distribution is unlimited.
#
#This material is based upon work supported under Air Force Contract No. FA8721-05-C-0002
#and/or FA8702-15-D-0001. Any opinions, findings, conclusions or recommendations
#expressed in this material are those of the author(s) and do not necessarily reflect the
#views of the U.S. Air Force.
#
#© 2018 Massachusetts Institute of Technology.
#
#The software/firmware is provided to you on an As-Is basis
#
#Delivered to the U.S. Government with Unlimited Rights, as defined in DFARS Part
#252.227-7013 or 7014 (Feb 2014). Notwithstanding any copyright notice, U.S. Government
#rights in this work are defined by DFARS 252.227-7013 or DFARS 252.227-7014 as detailed
#above. Use of this work other than as specifically authorized by the U.S. Government may
#violate any copyrights that exist in this work.
#********************************************************************************/ 
echo Removing all -m32 Flags from cFE Build Commands


echo - editing psp compiler-options.mak
sed -i 's/-m32//g' /home/pi/cFE/psp/fsw/pc-linux/make/compiler-opts.mak
sed -i 's/-melf_i386//g' /home/pi/cFE/psp/fsw/pc-linux/make/compiler-opts.mak

echo - editing psp link-rules.mak
sed -i 's/-m32//g' /home/pi/cFE/psp/fsw/pc-linux/make/link-rules.mak

echo - editing tools elf2cfetbl CMakeList.txt
sed -i 's/-m32//g' /home/pi/cFE/tools/elf2cfetbl/CMakeLists.txt

echo - editing tools elf2cfetbl/for_build Makefile
sed -i 's/-m32//g' /home/pi/cFE/tools/elf2cfetbl/for_build/Makefile

echo !! DONE !!
