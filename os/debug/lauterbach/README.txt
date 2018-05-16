PREREQUISITES FOR S32V234 DEBUG
===============================

1. Environment variable S32V234_SDK_ROOT pointing to the s32v234_sdk directory (should be set by the installer)
2. Lauterbach T32 V2.3.3 (tested on S.2015.03.000061311)

DEBUG OF S32V234 EVB (standalone)
=================================

1. Open the S32V234.ts2 configuration file
	a. (optional) Or open the T32Start, right click to the blank area, choose "File->Load from file and add" and select this file
	
2. On opened T32Start window, select S32V234 EVB setting and press "Start"
3. Two T32 instances will open, from now, only PowerView for CA53 will be used (all scripts are ran from here)
4. Reset the board (first press SW3 and then SW4 - each for 1 second).
5. Run the script in s32v234/demos/*/build-v234ce-gnu-sa-d/*.cmm to run the specific demo


DEBUGGING T32 ISSUES
====================

- before each application load, the board reset must be issued (both reset buttons, top and bottom pressed in correct order)
- if any warning comes during the script load ("Debugger acts like slave...."), please reset the board and restart the T32 (from the point 2.)