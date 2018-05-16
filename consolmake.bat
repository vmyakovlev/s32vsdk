@echo off
SET S32V234_SDK_ROOT=%1
IF [%S32V234_SDK_ROOT%]==[] SET S32V234_SDK_ROOT=%~dp0
IF EXIST %S32V234_SDK_ROOT%\build\nbuild GOTO SDK_FOUND
ECHO Error: Either this batch should be in {Layout}\S32DS\s32v234_sdk or specify S32V234_SDK_ROOT location in the parameter
EXIT

:SDK_FOUND
IF EXIST %CD%\makefile GOTO START
ECHO Error: Folder %CD% does not contain MakeFile
EXIT

:START
ECHO ---- %~n0: location %CD% ----
SET SDK_ROOT=%S32V234_SDK_ROOT:\=/%
SET LINUX_S32V234_DIR=%SDK_ROOT%/kernels
SET APU_TOOLS=%SDK_ROOT%/../APUC
PUSHD %S32V234_SDK_ROOT%\..\..\
PATH=%CD%\Cross_Tools\gcc-aarch64-linux-gnu-4_9\bin;%CD%\Cross_Tools\gcc-aarch64-elf-4_9\bin;%CD%\Cross_Tools\gcc-arm-none-eabi-4_9\bin;%CD%\utils\msys32\usr\bin;%CD%\utils\msys32\mingw32\bin
POPD

make APU_COMP=nxp allsub 1> %TEMP%\make.log
ECHO ---- %~n0: end. ----
