/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
 
/****************************************************************************/
/**
 *  \file   global_errors.h
 *  \brief  This file contains the global error codes.
 *
 */

#ifndef GLOBALERRORS_H
#define GLOBALERRORS_H

//
// Literals & definitions
//
//****************************************************************************
// type of the error/result codes.
typedef long   LIB_RESULT; ///< The type used by interface routines as return value

/// LIB_RESULT values
/// a list of values that may be returned, depending on the application being used
static const LIB_RESULT LIB_SUCCESS =  0;                      ///< No error was detected.
static const LIB_RESULT LIB_FAILURE = -1;                      ///< Unspecified error has occurred.
static const LIB_RESULT LIB_ERROR_UNSUPPORTED_FUNCTION   = -2; ///< The function called is not supported in this revision of the API.
static const LIB_RESULT LIB_FAILURE_UNSUPPORTED_FUNCTION = -2; ///< The function called is not supported in this revision of the API.

//****************************************************************************
// Errors from the drivers.
static const LIB_RESULT DRIVERS_BASE               = 0x00060000;
static const LIB_RESULT DRIVERS_SENSOR_BASE        = 0x00061000; ///< Sensor
static const LIB_RESULT DRIVER_SENSOR_OV7846_BASE  = 0x00061100;
static const LIB_RESULT DRIVER_SENSOR_OV9640_BASE  = 0x00061200;
static const LIB_RESULT DRIVERSENSOR_LZ0P3731_BASE = 0x00061300;
static const LIB_RESULT DRIVERS_GDI_BASE           = 0x00061400;
static const LIB_RESULT DRIVERS_AC_BASE            = 0x00062000; ///< Array processor
static const LIB_RESULT DRIVERS_MMC_BASE           = 0x00063000; ///< Memory card
static const LIB_RESULT DRIVERS_SC_BASE            = 0x00064000; ///< Serial Control (I2C)
static const LIB_RESULT DRIVERS_DMA_BASE           = 0x00065000; ///< DMA control
static const LIB_RESULT DRIVERS_FSM_BASE           = 0x00066000; ///< File system
static const LIB_RESULT DRIVERS_CH_BASE            = 0x00067000; ///< Channel driver
static const LIB_RESULT DRIVERS_HPI_BASE           = 0x00069000; ///< HPI
static const LIB_RESULT DRIVERS_SPI_BASE           = 0x0006A000; ///< SPI
static const LIB_RESULT DRIVERS_UART_BASE          = 0x0006B000; ///< UART
static const LIB_RESULT DRIVERS_BITBLIT_BASE       = 0x0006C000; ///< BITBLIT
static const LIB_RESULT DRIVERS_BITBLITMINI_BASE   = 0x0006D000; ///< BITBLITMINI
static const LIB_RESULT DRIVERS_DIP_BASE           = 0x0006E000; ///< DIP
static const LIB_RESULT DRIVERS_ROTATOR_BASE       = 0x0006F000; ///< ROTATOR
static const LIB_RESULT DRIVERS_DISPLAY_BASE       = 0x00070000; ///< DISPLAY
static const LIB_RESULT DRIVERS_SIF_BASE           = 0x00071000; ///< SIF
static const LIB_RESULT DRIVERS_STM_BASE           = 0x00072000; ///< System Timing Manager
static const LIB_RESULT DLL_BASE                   = 0x00073000; ///< Dll sub-system

//****************************************************************************

static const LIB_RESULT OAL_BASE                   = 0x00090000; ///< Errors from the OAL library bease.
static const LIB_RESULT GDI_ERROR_BASE             = 0x000B1000; ///< Errors for "NEW" GDI base.
static const LIB_RESULT SDI_ERROR_BASE             = 0x000B2000; ///< Errors for SDI base.
static const LIB_RESULT STIMG_ERROR_BASE           = 0x000B3000; ///< Errors for Still Image base.
static const LIB_RESULT MESSAGING_ERROR_BASE       = 0x000B4000; ///< Errors for MsgMgr and EventMgr.
static const LIB_RESULT RESOURCE_MANAGER_ERROR_BASE= 0x000B5000; /*/< Errors for ResMgr. */
static const LIB_RESULT APP_ERROR_BASE             = 0x000B6000; /*/< Errors for Applications. */


// Errors from the system and utilities.
static const LIB_RESULT UTIL_QUEUE_ERR_INIT_FAILED        = 0x1101;
static const LIB_RESULT UTIL_QUEUE_ERR_NOT_INITIALIZED    = 0x1102;
static const LIB_RESULT UTIL_QUEUE_ERR_EMPTY              = 0x1103;
static const LIB_RESULT UTIL_QUEUE_ERR_FULL               = 0x1104;
static const LIB_RESULT UTIL_QUEUE_ERR_BUFFER_TOO_LARGE   = 0x1105;
static const LIB_RESULT UTIL_QUEUE_ERR_BUFFER_TOO_SMALL   = 0x1106;

// Errors from the J22xx drivers.
//Sensor
static const LIB_RESULT DRIVERS_SENSOR_ERR_NONE_VALID_DETECTED          = 0x61001;
static const LIB_RESULT DRIVERS_SENSOR_ERR_MODECHANGE_NOT_SUPPORTED     = 0x61002;
static const LIB_RESULT DRIVERS_SENSOR_ERR_RATECHANGE_NOT_SUPPORTED     = 0x61003;
static const LIB_RESULT DRIVERS_SENSOR_ERR_MODE_NOT_SUPPORTED           = 0x61004;
static const LIB_RESULT DRIVERS_SENSOR_ERR_RATE_NOT_SUPPORTED           = 0x61005;
static const LIB_RESULT DRIVERS_SENSOR_ERR_ORIENT_NOT_SUPPORTED         = 0x61006;
static const LIB_RESULT DRIVERS_SENSOR_ERR_RESOLUTION_NOT_SUPPORTED     = 0x61007;
static const LIB_RESULT DRIVERS_SENSOR_ERR_FUNCTION_NOT_SUPPORTED       = 0x61008;

// AC
static const LIB_RESULT DRIVERS_AC_ERR_COMMAND_ID_NOT_VALID             = 0x62001;
static const LIB_RESULT DRIVERS_AC_ERR_NOT_ENOUGH_SPACE_IN_Q            = 0x62002;
static const LIB_RESULT DRIVERS_AC_ERR_NOT_ENOUGH_SPACE_FOR_MICROCODE   = 0x62003;
static const LIB_RESULT DRIVERS_AC_ERR_NOT_ENOUGH_SPACE_FOR_CODE        = 0x62004;
static const LIB_RESULT DRIVERS_AC_ERR_LOAD_NOT_VALID                   = 0x62005;
static const LIB_RESULT DRIVERS_AC_ERR_NOT_ENOUGH_SPACE_FOR_CONSTANTS   = 0x62006;
static const LIB_RESULT DRIVERS_AC_ERR_NOT_ENOUGH_SPACE_FOR_LOOPS       = 0x62007;
static const LIB_RESULT DRIVERS_AC_ERR_QUEUE_WRITE                      = 0x62008;

// BITBLIT
static const LIB_RESULT DRIVERS_BITBLIT_SCALING_ERROR                   = 0x6C001;
static const LIB_RESULT DRIVERS_BITBLIT_INPUTFORMAT_ERROR               = 0x6C002;
static const LIB_RESULT DRIVERS_BITBLIT_ROTATION_NOTSUPPORTED           = 0x6C003;
static const LIB_RESULT DRIVERS_BITBLIT_OUTPUT_FORMAT_ERROR             = 0x6C004;
static const LIB_RESULT DRIVERS_BITBLIT_FIFO_TIMEOUT_ERROR              = 0x6C005;

// BITBLITMINI
static const LIB_RESULT DRIVERS_BITBLITMINI_SCALING_ERROR               = 0x6D001;
static const LIB_RESULT DRIVERS_BITBLITMINI_INPUTFORMAT_ERROR           = 0x6D002;

// ROTATOR
static const LIB_RESULT DRIVERS_ROTATOR_INVALID_DIMENSIONS              = 0x6F001;
static const LIB_RESULT DRIVERS_ROTATOR_INVALID_COLOR_FORMAT            = 0x6F002;
static const LIB_RESULT DRIVERS_ROTATOR_INVALID_OPERATION               = 0x6F003;

// MMC
static const LIB_RESULT DRIVERS_MMC_ERR_NOT_INITIALIZED                 = 0x63001;
static const LIB_RESULT DRIVERS_MMC_ERR_ALREADY_INITIALIZED             = 0x63002;
static const LIB_RESULT DRIVERS_MMC_ERR_INITIALIZATION_FAILED           = 0x63003;
static const LIB_RESULT DRIVERS_MMC_ERR_READ_ERROR                      = 0x63004;
static const LIB_RESULT DRIVERS_MMC_ERR_WRITE_ERROR                     = 0x63005;
static const LIB_RESULT DRIVERS_MMC_ERR_CRC_ERROR                       = 0x63006;
static const LIB_RESULT DRIVERS_MMC_ERR_DMATIMEOUT_ERROR                = 0x63007;


// STM
static const LIB_RESULT DRIVERS_STM_ERR_MAX_REGISTER_FUNC_RITCHED       = 0x72001;
static const LIB_RESULT DRIVERS_STM_ERR_SYSTEM_CLOCK_NOT_INIT           = 0x72002;
static const LIB_RESULT DRIVERS_STM_WARNING_AP_MAXED_MIPS               = 0x72003;
static const LIB_RESULT DRIVERS_STM_WARNING_ARM_MAXED_MIPS              = 0x72004;
static const LIB_RESULT DRIVERS_STM_WARNING_SYSTEM_MAXED_MIPS           = 0x72005;
static const LIB_RESULT DRIVERS_STM_WARNING_SOME_MAXED_MIPS             = 0x72006;
static const LIB_RESULT DRIVERS_STM_ERR_AP_CHANGING                     = 0x72007;
static const LIB_RESULT DRIVERS_STM_ERR_ARM_CHANGING                    = 0x72008;
static const LIB_RESULT DRIVERS_STM_ERR_SYSTEM_CHANGING                 = 0x72009;

static const LIB_RESULT DRIVERS_STM_ERR_PLL_WRONG_FREQ                  = 0x7200A;
static const LIB_RESULT DRIVERS_STM_ERR_PLL_WRONG_DIV_ASSOCIATION       = 0x7200B;
static const LIB_RESULT DRIVERS_STM_ERR_PLL_NOT_INIT                    = 0x7200C;
static const LIB_RESULT DRIVERS_STM_WARNING_SECOND_ARM_FREQ_NOT_EXACT   = 0x7200D;

static const LIB_RESULT DRIVERS_STM_ERROR_POWERDOWN_CORE                = 0x7200E;
// FS
static const LIB_RESULT DRIVERS_FSM_ERR_NOT_INITIALIZED                 = 0x66001;
static const LIB_RESULT DRIVERS_FSM_ERR_ALREADY_INITIALIZED             = 0x66002;
static const LIB_RESULT DRIVERS_FSM_ERR_INITIALIZATION_FAILED           = 0x66003;
static const LIB_RESULT DRIVERS_FSM_ERR_UNKNOWN_DEVICE                  = 0x66004;
static const LIB_RESULT DRIVERS_FSM_ERR_UNSUPPORTED_DEVICE              = 0x66005;

// ???
static const LIB_RESULT DRIVERS_SC_ERR_ADDRESS_NOT_CONFIGURED           = 0x64001;
static const LIB_RESULT DRIVERS_SC_ERR_WAIT_TIMEOUT                     = 0x64002;
static const LIB_RESULT DRIVERS_SC_ERR_PIN_WAIT_TIMEOUT                 = 0x64003;
static const LIB_RESULT DRIVERS_SC_ERR_STOP_WAIT_TIMEOUT                = 0x64004;
static const LIB_RESULT DRIVERS_SC_ERR_INVALID_PARAMETERS               = 0x64005;
static const LIB_RESULT DRIVERS_SC_ERR_DRIVER_NOT_INITIALIZED           = 0x64006;
static const LIB_RESULT DRIVERS_SC_ERR_DRIVER_NOT_LOCKED                = 0x64007;

// DMA
static const LIB_RESULT DRIVERS_DMA_ERR_DRIVER_NOT_INITIALIZED          = 0x65001;

// CHANNEL
static const LIB_RESULT DRIVERS_CH_ERR_NOT_INITIALIZED                  = 0x67001; ///< Not initialized
static const LIB_RESULT DRIVERS_CH_ERR_EMPTY                            = 0x67002; ///< Chennel empty
static const LIB_RESULT DRIVERS_CH_ERR_INVALID_CHANNEL                  = 0x67003; ///< Invalid channel id
static const LIB_RESULT DRIVERS_CH_ERR_CHANNEL_OPEN                     = 0x67004; ///< Channel already open
static const LIB_RESULT DRIVERS_CH_ERR_CHANNEL_NOT_OPEN                 = 0x67005; ///< Channel not open
static const LIB_RESULT DRIVERS_CH_ERR_INVALID_DESTINATION              = 0x67006; ///< Bad destination specifier 
static const LIB_RESULT DRIVERS_CH_ERR_DESTINATION_NOT_READY            = 0x67007; ///< Destination not ready
static const LIB_RESULT DRIVERS_CH_ERR_BUFFER_TOO_LARGE                 = 0x67008; ///< Buffer too large
static const LIB_RESULT DRIVERS_CH_ERR_OUT_OF_CHANNELS                  = 0x67009; ///< Ran out of channels
static const LIB_RESULT DRIVERS_CH_ERR_CHANNEL_FULL                     = 0x6700A; ///< Channel full
static const LIB_RESULT DRIVERS_CH_ERR_CHANNEL_SUSPENDED                = 0x6700B; ///< Channel operation is suspended

// GDI
static const LIB_RESULT DRIVERS_GDI_ERR_OVERLAY_DOESNOTEXIST            = 0x61401;
static const LIB_RESULT DRIVERS_GDI_ERR_MAX_NUMBER_OVERLAY_EXCEDED      = 0x61402;
static const LIB_RESULT DRIVERS_GDI_ERR_NUMBER_ACTIVE_OVERLAY_EXCEDED   = 0x61403;


// Errors from the AAI lib.
static const LIB_RESULT AAI_ERR_INVALID_HANDLE                          = 0x80001; ///< The handle provided is not valid.
static const LIB_RESULT AAI_ERR_INVALID_POINTER                         = 0x80002;
static const LIB_RESULT AAI_ERR_NOT_READY                               = 0x80003; ///< Application not ready to run/pause.
static const LIB_RESULT AAI_ERR_COMMAND_INVALID                         = 0x80004; ///< The AAI command sent to the application was not valid. Check the (application state, command sent) pair and see if it makes sense.
static const LIB_RESULT AAI_ERR_APP_EXISTS                              = 0x80005; ///< Trying to create an application that currently exists
static const LIB_RESULT AAI_ERR_APP_MEMALLOC                            = 0x80006; ///< Application cannot allocate required memory.

static const LIB_RESULT AAI_ERR_AENC_MEMALLOC                           = 0x80201; ///< Error allocating memory for the audio encoder handle
static const LIB_RESULT AAI_ERR_AENC_MSI_ADD                            = 0x80202; ///< Error adding audio encoder to the task list
static const LIB_RESULT AAI_ERR_AENC_MSI_REMOVE                         = 0x80203; ///< Error removing audio encoder from the task list
static const LIB_RESULT AAI_ERR_AENC_NOT_STOPPED                        = 0x80204; ///< Trying to kill the audio encoder without stopping it first
static const LIB_RESULT AAI_ERR_AENC_STOPPED                            = 0x80205; ///< Trying to stop an already stopped audio encoder
static const LIB_RESULT AAI_ERR_AENC_RUNNING                            = 0x80206; ///< Trying to start an already started audio encoder
static const LIB_RESULT AAI_ERR_AENC_CREATED                            = 0x80207; ///< Trying to create an audio encoder that is already created
static const LIB_RESULT AAI_ERR_AENC_NOT_SUPPORTED                      = 0x80208; ///< Trying to create an audio encoder that is not supported

static const LIB_RESULT AAI_EER_PLAYER_NOT_PAUSED                       = 0x80701; ///< Error reconfiguring a player without pausing first
static const LIB_RESULT AAI_ERR_VDEC_MEMALLOC                           = 0x80C01; ///< Error allocating memory for the video decoder handle
static const LIB_RESULT AAI_ERR_VDEC_MSI_ADD                            = 0x80C02; ///< Error adding video decoder to the task list
static const LIB_RESULT AAI_ERR_VDEC_MSI_REMOVE                         = 0x80C03; ///< Error removing video decoder from the task list
static const LIB_RESULT AAI_ERR_VDEC_NOT_STOPPED                        = 0x80C04; ///< Trying to kill the video decoder without stopping it first
static const LIB_RESULT AAI_ERR_VDEC_STOPPED                            = 0x80C05; ///< Trying to stop an already stopped video decoder
static const LIB_RESULT AAI_ERR_VDEC_RUNNING                            = 0x80C06; ///< Trying to start an already started video decoder
static const LIB_RESULT AAI_ERR_VDEC_INVALID_TRACK                      = 0x80C07; ///< Trying to perform an operation on a non-existant track
static const LIB_RESULT AAI_EER_VDEC_IMAGE_BUFFER_TOO_SMALL             = 0x80C08; ///< Capture image buffer provided too small


static const LIB_RESULT AAI_ERR_ADEC_MEMALLOC                           = 0x80301; ///< Error allocating memory for the audio decoder handle
static const LIB_RESULT AAI_ERR_ADEC_MSI_ADD                            = 0x80302; ///< Error adding audio decoder to the task list
static const LIB_RESULT AAI_ERR_ADEC_MSI_REMOVE                         = 0x80303; ///< Error removing audio decoder from the task list
static const LIB_RESULT AAI_ERR_ADEC_NOT_STOPPED                        = 0x80304; ///< Trying to kill the audio decoder without stopping it first
static const LIB_RESULT AAI_ERR_ADEC_STOPPED                            = 0x80305; ///< Trying to stop an already stopped audio decoder
static const LIB_RESULT AAI_ERR_ADEC_RUNNING                            = 0x80306; ///< Trying to start an already started audio decoder
static const LIB_RESULT AAI_ERR_ADEC_CREATED                            = 0x80307; ///< Trying to create an audio decoder that is already created
static const LIB_RESULT AAI_ERR_ADEC_NOT_SUPPORTED                      = 0x80308; ///< Trying to create an audio decoder that is not supported

static const LIB_RESULT AAI_ERR_MUX_MSI_REMOVE                          = 0x80601; ///< Error removing multiplexer from the task list
static const LIB_RESULT AAI_ERR_MUX_INVALID_STREAM                      = 0x80602; ///< Error caused my trying to mux an unsupported stream

static const LIB_RESULT AAI_ERR_STATS_NOT_UPDATED                       = 0x80801; ///< New values same as old ones
static const LIB_RESULT AAI_ERR_STATS_NOT_AVAILABLE                     = 0x80802; ///< No new values available, contents unchanged
static const LIB_RESULT AAI_ERR_STATS_MAX_OUT_OF_RANGE                  = 0x80804; ///< The max value is not 0-255.
static const LIB_RESULT AAI_ERR_STATS_MIN_OUT_OF_RANGE                  = 0x80805; ///< The min value is not 0-255
static const LIB_RESULT AAI_ERR_STATS_MAX_LE_MIN                        = 0x80806; ///< The max is not greater than the min.

static const LIB_RESULT AAI_ERR_MP3_CREATED                             = 0x80501; ///< Trying to create an mp3 decoder that is already created
static const LIB_RESULT AAI_ERR_MIDI_CREATED                            = 0x80502; ///< Trying to create a MIDI decoder that is already created

/* Flute library errors */
static const LIB_RESULT AAI_FLUTE_PENDING                               = 0x80D00;
static const LIB_RESULT AAI_FLUTE_FILE_RECEIVED                         = 0x80D01;
static const LIB_RESULT AAI_FLUTE_ALL_FILES_RECEIVED                    = 0x80D02;
static const LIB_RESULT AAI_FLUTE_CLOSE_SESSION                         = 0x80D03;
static const LIB_RESULT AAI_FLUTE_NO_UPDATE                             = 0x80D04;
static const LIB_RESULT AAI_FLUTE_ERROR_INVALID_XML                     = 0x80D05;
static const LIB_RESULT AAI_FLUTE_ERROR_FEC_BUILDING_BLOCK              = 0x80D06;
static const LIB_RESULT AAI_FLUTE_ERROR_FDT_EXPIRED                     = 0x80D07;
static const LIB_RESULT AAI_FLUTE_ERROR_INVALID_FDT                     = 0x80D08;
static const LIB_RESULT AAI_FLUTE_ERROR_FDT_PARSING                     = 0x80D09;
static const LIB_RESULT AAI_FLUTE_ERROR_INVALID_URI                     = 0x80D0A;
static const LIB_RESULT AAI_FLUTE_ERROR_NOT_SUPPORT_FEC_BUILDING_BLOCK  = 0x80D0B;
static const LIB_RESULT AAI_FLUTE_ERROR_UNKNOWN                         = 0x80D0C;
static const LIB_RESULT AAI_FLUTE_ERROR_OAL_BUFFER                      = 0x80D0D;
static const LIB_RESULT AAI_FLUTE_ERROR_OAL_FILE                        = 0x80D0E;
static const LIB_RESULT AAI_FLUTE_ERROR_OAL_ADD_FTDO                    = 0x80D0F;


   // Errors from the OAL lib.
static const LIB_RESULT OAL_ERR_LOG_EINVALIDPARAM                       = 0x90001; ///< Invalid parameter

static const LIB_RESULT OAL_ERR_SEM_ESEMLOCKED                          = 0x90002; ///< Semaphore is locked
static const LIB_RESULT OAL_ERR_SEM_ETIMEOUT                            = 0x90003; ///< Timeout waiting on Semaphore
static const LIB_RESULT OAL_ERR_SEM_EBADSEM                             = 0x90004; ///< Semaphore is invalid
static const LIB_RESULT OAL_ERR_SEM_EBADTYPE                            = 0x90005; ///< Semaphore type is invalid 
static const LIB_RESULT OAL_ERR_SEM_ENOMEM                              = 0x90006; ///< Failed to allocate memory for semaphore
static const LIB_RESULT OAL_ERR_TASKMGR_ENOMEM                          = 0x90007; ///< Failed to allocate memory for Task Manager
static const LIB_RESULT OAL_ERR_BUF_ENOMEM                              = 0x90008; ///< Failed to allocate OAL buffer memory
static const LIB_RESULT OAL_ERR_SISR_ENOMEM                             = 0x90009; ///< Failed to allocate SISR memory
static const LIB_RESULT OAL_ERR_SISR_EINITFAIL                          = 0x90010; ///< Failed to initialize SISR
static const LIB_RESULT OAL_ERR_PIPE_RESET                              = 0x90011;
static const LIB_RESULT OAL_ERR_PIPE_TIMEOUT                            = 0x90012;
static const LIB_RESULT OAL_ERR_INVALID_FILE_DESCRIPTOR                 = 0x90013; ///< An invalid file descriptor was specified.
static const LIB_RESULT OAL_ERR_INVALID_FILE_PATH                       = 0x90014; ///< An invalid file path was specified. 
static const LIB_RESULT OAL_ERR_INVALID_DRIVE_LETTER                    = 0x90015; ///< An invalid driver letter was specified. 
static const LIB_RESULT OAL_ERR_NO_FILE                                 = 0x90016; ///< No directory entry found to match pattern
static const LIB_RESULT OAL_ERR_FILE_SYSTEM                             = 0x90017; ///< An unspecified file system error occurred.
static const LIB_RESULT OAL_ERR_FILE_IO                                 = 0x90018; ///< A file I/O error occurred. 
static const LIB_RESULT OAL_ERR_FILE_OUT_OF_SPACE                       = 0x90019; ///< The file device is full. 
static const LIB_RESULT OAL_ERR_FILE_EXISTS                             = 0x90020; ///< The file or directory specified already exists. 
static const LIB_RESULT OAL_ERR_FILE_ACCESS                             = 0x90021; ///< Directory entry attributes prohibit operation. 
static const LIB_RESULT OAL_ERR_FILE_DISK_UNFORMAT                      = 0x90022; ///< Disk is not formatted. 
static const LIB_RESULT OAL_ERR_MEMORY_OUT_OF_MEMORY                    = 0x90023; ///< System has not enough memory for request. 
static const LIB_RESULT OAL_ERR_SEM_RESET                               = 0x90024; ///< The Semaphore is in Reset state. 
static const LIB_RESULT OAL_ERR_INVALID_TIMER_ID                        = 0x90025; ///< Invalid timer ID 
static const LIB_RESULT OAL_ERR_DISK_NOT_READY                          = 0x90026; ///< Disk is not ready to use - wait. 
static const LIB_RESULT OAL_ERR_DISK_NOT_PRESENT                        = 0x90027; ///< Disk is not present. 
static const LIB_RESULT OAL_ERR_DISK_NEED_PHYSICAL_FORMAT               = 0x90028; ///< Disk needs physical format. 
static const LIB_RESULT OAL_ERR_INVALID_DIR_HANDLE                      = 0x90029; ///< Invalid directory handle 
static const LIB_RESULT OAL_ERR_FILE_SHARED_VIOLATION                   = 0x9002A; ///< Multi-tasks access violation 

static const LIB_RESULT OAL_ERR_INVALID_DRM_FORMAT                      = 0x90030; ///< INvalid DRM format
static const LIB_RESULT OAL_ERR_INVALID_CREDENTIAL                      = 0x90031; ///< Invalid credential for DRM
static const LIB_RESULT OAL_ERR_INVALID_LICENSE                         = 0x90032; ///< Invalid license for DRM
static const LIB_RESULT OAL_ERR_EOF                                     = 0x90033; ///< End Of File 
static const LIB_RESULT OAL_ERR_INVALID_PARAMS                          = 0x90034; ///< Invalid parameters 

/** Define service completion status constants.  */
/** Nucleus FILE status value */
static const LIB_RESULT OAL_NUF_BAD_USER                                = 0x90BB8; ///< Not a file user
static const LIB_RESULT OAL_NUF_BADDRIVE                                = 0x90BB9; ///< Bad drive number
static const LIB_RESULT OAL_NUF_BADPARM                                 = 0x90BBA; ///< Invalid parametor given
    /* Disk */
static const LIB_RESULT OAL_NUF_NOT_OPENED                              = 0x90BBB; ///< The disk is not opened yet. 
static const LIB_RESULT OAL_NUF_NO_DISK                                 = 0x90BBC; ///< Disk is removed. 
static const LIB_RESULT OAL_NUF_DISK_CHANGED                            = 0x90BBD; ///< Disk is changed. 
static const LIB_RESULT OAL_NUF_INVALID_CSIZE                           = 0x90BBE; ///< The Disk has invalid clluster size. 
static const LIB_RESULT OAL_NUF_FATCORE                                 = 0x90BBF; ///< Fat cache table too small
static const LIB_RESULT OAL_NUF_DEFECTIVEC                              = 0x90BC0; ///< Defective cluster detected
static const LIB_RESULT OAL_NUF_BADDISK                                 = 0x90BC1; ///< Bad Disk 
static const LIB_RESULT OAL_NUF_NO_PARTITION                            = 0x90BC2; ///< No partition in disk 
static const LIB_RESULT OAL_NUF_ROOT_FULL                               = 0x90BC3; ///< Root directry full 
    /* Format */
static const LIB_RESULT OAL_NUF_NOFAT                                   = 0x90BC4; ///< No FAT type in this partition. Can't Format. 
static const LIB_RESULT OAL_NUF_FMTCSIZE                                = 0x90BC5; ///< Too many clusters for this partition. Can't Format.
static const LIB_RESULT OAL_NUF_FMTFSIZE                                = 0x90BC6; ///< File allocation table too small. Can't Format. 
static const LIB_RESULT OAL_NUF_FMTRSIZE                                = 0x90BC7; ///< Numroot must be an even multiple of 16.  
static const LIB_RESULT OAL_NUF_FORMAT                                  = 0x90BC8; ///< Not formatted this disk 
    /* Path */
static const LIB_RESULT OAL_NUF_LONGPATH                                = 0x90BC9; ///< Path or filename is too long.
static const LIB_RESULT OAL_NUF_INVNAME                                 = 0x90BCA; ///< Path or filename is invalid. 
    /* File */
static const LIB_RESULT OAL_NUF_PEMFILE                                 = 0x90BCB; ///< No file descriptors available (too many files open) 
static const LIB_RESULT OAL_NUF_BADFILE                                 = 0x90BCC; ///< Invalid file descriptor 
static const LIB_RESULT OAL_NUF_ACCES                                   = 0x90BCD; ///< Attempt to open a read only file or a special (directory) 
static const LIB_RESULT OAL_NUF_NOSPC                                   = 0x90BCE; ///< Write failed, presumably because of no space. 
static const LIB_RESULT OAL_NUF_SHARE                                   = 0x90BCF; ///< The access conflict from multiple task to a specific file 
static const LIB_RESULT OAL_NUF_NOFILE                                  = 0x90BD0; ///< File not found or path to file not found 
static const LIB_RESULT OAL_NUF_EXIST                                   = 0x90BD1; ///< Exclusive access is requested but file already exists. 
static const LIB_RESULT OAL_NUF_NVALFP                                  = 0x90BD2; ///< Seek to negative file pointer attempted
static const LIB_RESULT OAL_NUF_MAXFILE_SIZE                            = 0x90BD3; ///< Over the maximum file size 
static const LIB_RESULT OAL_NUF_NOEMPTY                                 = 0x90BD4; ///< Directory is not empty.     
static const LIB_RESULT OAL_NUF_INVPARM                                 = 0x90BD5; ///< Invalid parameter is specified.  
static const LIB_RESULT OAL_NUF_INVPARCMB                               = 0x90BD6; ///< Invalid parameter combination is specified.  
    /* Memory  */
static const LIB_RESULT OAL_NUF_NO_MEMORY                               = 0x90BD7; ///< Can't allocate internal buffer. 
static const LIB_RESULT OAL_NUF_NO_BLOCK                                = 0x90BD8; ///< No block buffer is available. 
static const LIB_RESULT OAL_NUF_NO_FINODE                               = 0x90BD9; ///< No FINODE buffer is available. 
static const LIB_RESULT OAL_NUF_NO_DROBJ                                = 0x90BDA; ///< No DROBJ buffer is available. 
static const LIB_RESULT OAL_NUF_IO_ERROR                                = 0x90BDB; ///< Driver IO function routine returned error. 

static const LIB_RESULT OAL_NUF_INTERNAL                                = 0x90BDC; ///< Nucleus FILE internal error 

/* New return values for VFS API */
static const LIB_RESULT OAL_NUF_IN_USE                                  = 0x90BDD; ///< Resource is in use, prevents requested operation. 

/* Device name errors */
static const LIB_RESULT OAL_NUF_INVALID_DEVNAME                         = 0x90BDE;
static const LIB_RESULT OAL_NUF_DEVICE_TABLE_FULL                       = 0x90BDF;
static const LIB_RESULT OAL_NUF_DUPLICATE_DEVNAME                       = 0x90BE0;

/* Disk handle errors */
static const LIB_RESULT OAL_NUF_DISK_TABLE_FULL                         = 0x90BE1;

/* Mount table errors */
static const LIB_RESULT OAL_NUF_MOUNT_TABLE_FULL                        = 0x90BE2;
static const LIB_RESULT OAL_NUF_MOUNT_NOT_AVAILABLE                     = 0x90BE3; ///< Mount is busy or not known to system. 

/* File system table errors */
static const LIB_RESULT OAL_NUF_FS_NOT_FOUND                            = 0x90BE4; ///< File system is not known to system. 
static const LIB_RESULT OAL_NUF_DUPLICATE_FSNAME                        = 0x90BE5;
static const LIB_RESULT OAL_NUF_FS_TABLE_FULL                           = 0x90BE6;

/* Partition services errors */
static const LIB_RESULT OAL_NUF_PART_TABLE_FULL                         = 0x90BE7; ///< Only four primaries, or one extended and three primaries can exist. 
static const LIB_RESULT OAL_NUF_PART_EXT_EXISTS                         = 0x90BE8; ///< Only one extended partition can exist. 
static const LIB_RESULT OAL_NUF_PART_NO_EXT                             = 0x90BE9; ///< Logical partitions can only be created within an extended partition. 
static const LIB_RESULT OAL_NUF_PART_LOG_EXISTS                         = 0x90BEA; ///< Extended partitions cannot be removed if logical partitions exist. 

/* Device driver errors */
static const LIB_RESULT OAL_NUF_INVALID_DEV_IOCTL                       = 0x90BEB; ///< The requested IOCTL command is undefined by the driver. 

static const LIB_RESULT OAL_FILE_DESCRIPTOR_TABLE_FULL                  = 0x90FA0; ///< Nucleus FILE internal error

//DRM errors
static const LIB_RESULT DRM_ERR_FILE_OPEN                               = 0xB8001;
static const LIB_RESULT DRM_ERR_FILE_CLOSE                              = 0xB8002;
static const LIB_RESULT DRM_ERR_FILE_SEEK                               = 0xB8003;
static const LIB_RESULT DRM_ERR_FILE_READ                               = 0xB8004;
static const LIB_RESULT DRM_ERR_NO_DRM_FILE                             = 0xB8005;

   // Errors from the AAC lib.
static const LIB_RESULT AAC_ERR_INVALID_HANDLE                          = 0x21201;
static const LIB_RESULT AAC_ERR_ADTS_SYNC_NOT_FOUND                     = 0x21202;
static const LIB_RESULT AAC_ERR_NOT_ACC_LC                              = 0x21203;
static const LIB_RESULT AAC_ERR_MORE_THAN_TWO_CH                        = 0x21204;
static const LIB_RESULT AAC_ERR_CRC_FAIL                                = 0x21205;
static const LIB_RESULT AAC_ERR_UNKNOWN_ELEMENT                         = 0x21206;

   // Errors from the AMR lib.
static const LIB_RESULT AMR_ERR_INVALID_ENC_HANDLE                      = 0x20301;
static const LIB_RESULT AMR_ERR_INVALID_DEC_HANDLE                      = 0x21301;

   //Errors from parsr lib.
static const LIB_RESULT PARSER_EOS                                      = 0x45001; ///< End of stream reached
static const LIB_RESULT PARSER_ERROR                                    = 0x45002; ///< Parsing error
static const LIB_RESULT PARSER_LARGE_AU                                 = 0x45003; ///< Frame size larger than available buffer
static const LIB_RESULT PARSER_NO_VIDEO_TRACK                           = 0x45004; ///< There is no video track.
static const LIB_RESULT PARSER_BOS                                      = 0x45005; ///< Begin of stream reached.
static const LIB_RESULT PARSER_AU_NOT_AVAILABLE                         = 0x45006;
static const LIB_RESULT PARSER_STSS_NOT_PRESENT                         = 0x45007;
static const LIB_RESULT PARSER_UNSUPPORTED_CONTENT                      = 0x45008;
static const LIB_RESULT PARSER_SEEK_TABLE_NOT_AVAILABLE                 = 0x45009;

//errors from mpe4 decoder lib
static const LIB_RESULT MPEG4_DEC_UNSUPPORTED_CONTENT                   = 0x41001;
static const LIB_RESULT MPEG4_DEC_MEMORY_ALLOCATION_ERROR               = 0x41002;
static const LIB_RESULT MPEG4_DEC_WAITING_FOR_I_FRAME                   = 0x41003;

//errors from h264 decoder lib
static const LIB_RESULT H264_ERR_INVALID_BITSTREAM                      = 0x46001;
static const LIB_RESULT H264_ERR_TEXTURE_DECODING                       = 0x46002;
static const LIB_RESULT H264_ERR_DMA                                    = 0x46003;
static const LIB_RESULT H264_ERR_OUT_OF_MEMORY                          = 0x46004;
static const LIB_RESULT H264_ERR_VLD_MISSING_NALS                       = 0x46005;
static const LIB_RESULT H264_ERR_INVALID_PPS_SPS                        = 0x46006;
static const LIB_RESULT H264_ERR_INVALID_PROFILE                        = 0x46007;
static const LIB_RESULT H264_ERR_FMO_NOT_SUPPORTED                      = 0x46008;
static const LIB_RESULT H264_ERR_RESOURCE_TIMEOUT                       = 0x46009;
static const LIB_RESULT H264_ERR_AU_EXCEED_SIZE_OF_AUBUFFER             = 0x4600A;
static const LIB_RESULT H264_ERR_WAITING_FOR_I_FRAME                    = 0x4600B;
static const LIB_RESULT H264_ERR_MISSING_REFERENCE                      = 0x4600C;
static const LIB_RESULT H264_ERR_UNSUPPORTED                            = 0x4600D;
static const LIB_RESULT H264_ERR_NO_HW_RESOURCE                         = 0x46010;

//errors from MPEG2 decoder lib
static const LIB_RESULT MPEG2_ERR_INVALID_BITSTREAM                     = 0x47001;
static const LIB_RESULT MPEG2_ERR_UNSUPPORTED                           = 0x47002;
static const LIB_RESULT MPEG2_ERR_OUT_OF_MEMORY                         = 0x47003;
static const LIB_RESULT MPEG2_ERR_VLD_ERROR                             = 0x47004;
static const LIB_RESULT MPEG2_ERR_INVALID_TILESIZE                      = 0x47005;
static const LIB_RESULT MPEG2_REF_ALLOCATION_FAILURE                    = 0x47006;
static const LIB_RESULT MPEG2_ERR_INVALID_SEQ_HEADER                    = 0x47007;
static const LIB_RESULT MPEG2_ERR_WAITING_FOR_I_FRAME                   = 0x47008;
static const LIB_RESULT MPEG2_ERR_DISCARD_B_FRAME                       = 0x47008;

//errors from Sorenson decoder lib
static const LIB_RESULT SRN_ERR_INVALID_BITSTREAM                       = 0x48001;
static const LIB_RESULT SRN_ERR_UNSUPPORTED                             = 0x48002;
static const LIB_RESULT SRN_ERR_OUT_OF_MEMORY                           = 0x48003;
static const LIB_RESULT SRN_REF_ALLOCATION_FAILURE                      = 0x48004;
static const LIB_RESULT SRN_ERR_EOS                                     = 0x48005; ///<End Of Stream
static const LIB_RESULT SRN_ERR_ALLOC_FAILED                            = 0x48006;
static const LIB_RESULT SRN_ERR_INVALID_BLOCK_MODE                      = 0x48007;

   // Media Source error
static const LIB_RESULT AAI_MEDIA_OPERATION_NOTCOMPLETED                = 0xA0000;
static const LIB_RESULT AAI_MEDIA_SOURCE_UNSUPPORTED                    = 0xA0001;
static const LIB_RESULT AAI_MEDIA_SOURCE_INVALID_TRACK_ID               = 0xA0002;

   // Media Decoder error
static const LIB_RESULT AAI_MEDIA_DECODER_UNSUPPORTED                   = 0xA1000;

   // Former AMPI Error
static const LIB_RESULT LIB_ERROR_OUT_OF_MEMORY                         = 0xB0008; ///< Out of memory.
static const LIB_RESULT LIB_ERROR_INVALID_SIZE                          = 0xB0015; ///< The specified size is invalid.
static const LIB_RESULT LIB_ERROR_NOT_ENCODING                          = 0xB0019; ///< Not encoding - corruption during encoding
static const LIB_RESULT LIB_ERROR_NOT_DECODING                          = 0xB001A; ///< Not decoding - corruption during decoding
static const LIB_RESULT LIB_ERROR_INVALID_IMAGE_HANDLE                  = 0xB001B; ///< An invalid image handle was specified.
static const LIB_RESULT LIB_ERROR_UNSUPPORTED_EFFECT                    = 0xB001E; ///< An unsupported effect was specified.
static const LIB_RESULT LIB_ERROR_BUFFER_POOL_INIT_FAILED               = 0xB0032; ///< Buffer descriptor pool initialization failed.
static const LIB_RESULT LIB_ERROR_AUDIO_INIT                            = 0xB0041; ///< The audio player failed to initialize.
static const LIB_RESULT LIB_ERROR_AUDIO_CONFIG                          = 0xB0042; ///< The audio player failed to configure.
static const LIB_RESULT LIB_ERROR_AUDIO_START                           = 0xB0043; ///< The audio player failed to start.
static const LIB_RESULT LIB_ERROR_AUDIO_STOP                            = 0xB0044; ///< The audio player failed to stop.
static const LIB_RESULT LIB_ERROR_AUDIO_KILL                            = 0xB0045; ///< The audio player process failed to be terminated.
static const LIB_RESULT LIB_ERROR_AUDIO_FINISHED                        = 0xB0046; ///< The audio player process failed due to end of file.
static const LIB_RESULT LIB_ERROR_UNSUPPORTED_AUDIO_CODEC               = 0xB0047; ///< The audio player failed due to unsupported audio codec.
static const LIB_RESULT LIB_ERROR_AUDIO_FADE_OUT_DONE                   = 0xB0048; ///< The audio encoder finished fade out before pausing.

/* OAL_QUEUE errors*/
static const LIB_RESULT LIB_INVALID_QUEUE                               = 0xB0071;
static const LIB_RESULT LIB_INVALID_MEMORY                              = 0xB0072;
static const LIB_RESULT LIB_INVALID_MESSAGE                             = 0xB0073;
static const LIB_RESULT LIB_INVALID_POINTER                             = 0xB0074; ///< Provided pointer was invalid
static const LIB_RESULT LIB_INVALID_SIZE                                = 0xB0075; ///< Provided size was invalid
static const LIB_RESULT LIB_INVALID_SUSPEND                             = 0xB0076;
static const LIB_RESULT LIB_QUEUE_EMPTY                                 = 0xB0077;
static const LIB_RESULT LIB_QUEUE_FULL                                  = 0xB0078;
static const LIB_RESULT LIB_QUEUE_TIMEOUT                               = 0xB0079;
static const LIB_RESULT LIB_QUEUE_DELETED                               = 0xB007A;
static const LIB_RESULT LIB_QUEUE_RESET                                 = 0xB007B;
static const LIB_RESULT LIB_SENDING_NOT_ALLOWED                         = 0xB007C;
static const LIB_RESULT LIB_RECEIVING_NOT_ALLOWED                       = 0xB007D;

/* SDI */
static const LIB_RESULT LIB_ERROR_UNSUPPORTED_RESOLUTION                = 0xB2001; ///< Resolution is unsupported.
static const LIB_RESULT LIB_ERROR_UNSUPPORTED_FRAMERATE                 = 0xB2002; ///< Frame Rate is unsupported.
static const LIB_RESULT LIB_WARNING_UNSUPPORTED_FRAMERATE_FOR_RESOLUTION = 0xB2003; ///< Frame Rate for this resolution is unsupported.
static const LIB_RESULT LIB_ERROR_INVALID_ZOOM_LEVEL                    = 0xB2004; ///< According to current configuration zoom level is invalid.
static const LIB_RESULT LIB_ERROR_SENSOR_CONFIGURATION_FAILURE          = 0xB2005; ///< Failed to configure the sensor
static const LIB_RESULT LIB_ERROR_SENSOR_MALFUNCTION                    = 0xB2006; ///< Sensor is malfunctionned.
static const LIB_RESULT LIB_ERROR_UNSUPPORTED_FILTER                    = 0xB2007; ///< Filter is unsupported.
static const LIB_RESULT LIB_ERROR_SDI_HW_ALLOCATION_FAILURE             = 0xB2008; ///< HW resource allocation is failed in SDI.
static const LIB_RESULT LIB_ERROR_UNRECOVERED_BUFFER                    = 0xB2009; ///< User failed to recover all of his buffer; implies memory leak


/* StillImage */
static const LIB_RESULT LIB_StillImage_WARNING_FILESIZETOOLARGE         = 0xB3001;
static const LIB_RESULT LIB_STILLIMAGE_OUT_OF_MEMORY_DAT                = 0xB3002;
static const LIB_RESULT LIB_STILLIMAGE_OUT_OF_MEMORY_CMD                = 0xB3003;
static const LIB_RESULT LIB_STILLIMAGE_CAPTURE_ERROR_SDI                = 0xB3004;
static const LIB_RESULT LIB_STILLIMAGE_ERR_HW_RES_MANAGR                = 0xB3005;
static const LIB_RESULT LIB_STILLIMAGE_ERR_SENSOR_TIMOUT                = 0xB3006;
static const LIB_RESULT LIB_STILLIMAGE_ERR_GDI_CONVERSION               = 0xB3007;
static const LIB_RESULT LIB_STILLIMAGE_ERR_EXIF_HEADER                  = 0xB3008;
static const LIB_RESULT LIB_STILLIMAGE_ERR_FILE_SYSTEM                  = 0xB3009;
static const LIB_RESULT LIB_STILLIMAGE_ERR_ENCODER                      = 0xB300A;
static const LIB_RESULT LIB_STILLIMAGE_ERR_JPG_OTF                      = 0xB300B;
static const LIB_RESULT LIB_STILLIMAGE_ERR_DECODER                      = 0xB300C; ///< Decoding error
static const LIB_RESULT LIB_STILLIMAGE_ERR_UNSUPPORTED_CONFIGURATION    = 0xB300D;

// GDI

static const LIB_RESULT GDI_ERROR_NOT_INITIALIZED                       = 0xB1001; ///< GDI has not been initialized.
static const LIB_RESULT GDI_ERROR_INTERNAL                              = 0xB1002; ///< An internal error has occured.
static const LIB_RESULT GDI_ERROR_INVALID_WINDOW_ID                     = 0xB1003; ///< Invalid window id
static const LIB_RESULT GDI_ERROR_INVALID_DISPLAY_ID                    = 0xB1004; ///< Invalid display id
static const LIB_RESULT GDI_ERROR_WINDOW_INACTIVE                       = 0xB1005; ///< Inactive window specified
static const LIB_RESULT GDI_ERROR_DISPLAY_UNAVAILABLE                   = 0xB1006; ///< Unavailable display specified
static const LIB_RESULT GDI_ERROR_UNSUPPORTED_COLOR_FORMAT              = 0xB1007; ///< Unsupported color format specified
static const LIB_RESULT GDI_ERROR_OBJECT_CREATION_FAILED                = 0xB1008; ///< Object creation failure
static const LIB_RESULT GDI_ERROR_TOO_MANY_WINDOWS                      = 0xB1009; ///< Maximum number of windows has been exceeded.
static const LIB_RESULT GDI_ERROR_INVALID_WINDOW_HANDLE                 = 0xB100A; ///< Invalid window handle specified.
static const LIB_RESULT GDI_ERROR_DMA_ERROR                             = 0xB100B; ///< Some DMA error occured.
static const LIB_RESULT GDI_ERROR_DISPLAY_INACTIVE                      = 0xB100C; ///< Inactive display specified.
static const LIB_RESULT GDI_ERROR_WINDOW_FORMAT_UNDEFINED               = 0xB100D; ///< The window color format is undefined.
static const LIB_RESULT GDI_ERROR_WINDOW_SIZE_UNDEFINED                 = 0xB100E; ///< The window size is undefined.
static const LIB_RESULT GDI_ERROR_INVALID_WINDOW_REGION                 = 0xB100F; ///< The window region specified is invalid.
static const LIB_RESULT GDI_ERROR_INVALID_DISPLAY_REGION                = 0xB1010; ///< The display region specified is invalid.
static const LIB_RESULT GDI_ERROR_UNSUPPORTED_DISPLAY                   = 0xB1011; ///< The display is not supported.
static const LIB_RESULT GDI_ERROR_WINDOW_QUEUE_EMPTY                    = 0xB1012; ///< The window queue is empty (used internally by GDI).
static const LIB_RESULT GDI_ERROR_INVALID_LAYER_ID                      = 0xB1013; ///< Invalid layer id
static const LIB_RESULT GDI_ERROR_INVALID_DISPLAY_COMBINATION           = 0xB1014; ///< The specified display cannot be enabled with the currently enabled displays.
static const LIB_RESULT GDI_ERROR_NO_WINDOW_BUFFER                      = 0xB1015; ///< The specified window has not been updated or does not have a placeholder buffer.
static const LIB_RESULT GDI_ERROR_INVALID_WINDOW_SIZE                   = 0xB1016; ///< The specified window dimensions are invalid.
static const LIB_RESULT GDI_ERROR_NULL_POINTER                          = 0xB1017; ///< A null pointer was passed for a required parameter.
static const LIB_RESULT GDI_ERROR_LAYER_OCCUPIED                        = 0xB1018; ///< The specified layer is already occupied by a window.
static const LIB_RESULT GDI_ERROR_2XPLUS_SCALING                        = 0xB1019; ///< The specified horizontal scaling exceeds limit of 2X.
static const LIB_RESULT GDI_ERROR_WINDOW_QUEUE_FULL                     = 0xB101A; ///< The window queue is full (it has reached configured limit).
static const LIB_RESULT GDI_ERROR_INVALID_DMA_PRIORITY                  = 0xB101B; ///< The dma priority is invalid.
static const LIB_RESULT GDI_ERROR_WINDOW_NOT_ATTACHED_TO_DISPLAY        = 0xB101C; ///< The specified window is not attached to the display.
static const LIB_RESULT GDI_INCOMPLETE_CONFIGURATION                    = 0xB101D; ///< The configuration is incomplete.
static const LIB_RESULT GDI_ERROR_WINDOW_ALREADY_ATTACHED_TO_DISPLAY    = 0xB101E; ///< The window is already attached to the display.
static const LIB_RESULT GDI_ERROR_INVALID_COLOR_FORMAT_WIDTH            = 0xB101F; ///< The width parameter specified is not valid for the specified image format.
static const LIB_RESULT GDI_ERROR_LOCK_ALREADY_RELEASED                 = 0xB1020; ///< A task attempted to release the GDI Lock when it did not own it.
static const LIB_RESULT GDI_ERROR_INVALID_LCD_INSTRUCTION               = 0xB1021; ///< An invalid LCD instruction type was specified.
static const LIB_RESULT GDI_ERROR_MAXIMUM_LCD_INSTRUCTION_DELAY_EXCEEDED = 0xB1022; ///< The maximum total time delay for LCD instruction sequence has been exceeded.
static const LIB_RESULT GDI_ERROR_INVALID_COLOR_FORMAT                  = 0xB1023; ///< The specified image format is not valid.
static const LIB_RESULT GDI_ERROR_UNSUPPORTED_ROTATION                  = 0xB1024; ///< The value of the angle is not supported for this type of rotation
static const LIB_RESULT GDI_ERROR_REGION_IS_INVALID                     = 0xB1025; ///< The region is invalid.
static const LIB_RESULT GDI_SUCCESS_REGION_IS_VALID                     = 0xB1026; ///< The region is invalid.
static const LIB_RESULT GDI_WARNING_REGION_TOO_LARGE                    = 0xB1027; ///< The region is valid but too big, requires clipping.
static const LIB_RESULT GDI_WARNING_WINDOW_REGION_WAS_CLIPPED           = 0xB1028; ///< The requested window region was clipped to make it fit in the frame.
static const LIB_RESULT GDI_WARNING_DISPLAY_REGION_WAS_CLIPPED          = 0xB1029; ///< The requested display region was clipped to make it fit in the frame.
static const LIB_RESULT GDI_ERROR_EXCEEDED_UPSAMPLING_LIMIT             = 0xB102A; ///< The amount of upsampling exceeded the established limits (CV220x - 64x).
static const LIB_RESULT GDI_ERROR_ALPHABLEND_DIFFERENTWINDOWSIZE        = 0xB102B;
static const LIB_RESULT GDI_ERROR_INVALID_ALPHA                         = 0xB102C; ///< The alpha value is outside the legal range.
static const LIB_RESULT GDI_ERROR_BUFFER_TOO_SMALL                      = 0xB102D; ///< Buffer given to GDI was to small to complete the operation.
static const LIB_RESULT GDI_NO_BUFFER_AVAILABLE                         = 0xB102E; ///< A buffer was requested but none were available.
static const LIB_RESULT GDI_ERROR_OBJECT_NOT_ENABLED                    = 0xB102F; ///< A object could not satisfy a request because it(object) was not in the enabled state.


// MsgMgr and EventMgr

static const LIB_RESULT LIB_ERROR_NAME_ALREADY_REGISTERED               = 0xB4000; ///< The cstring name is already registered.
static const LIB_RESULT LIB_ERROR_OBJECT_NOT_FOUND_IN_CONTAINER         = 0xB4001; ///< Object was not in the container.
static const LIB_RESULT LIB_ERROR_INVALID_ARGUMENT                      = 0xB4002; ///< One or more of the arguments was invalid.

/* ResMgr */
static const LIB_RESULT LIB_RESOURCES_NOT_CURRENTLY_AVAILABLE           = 0xB5000; ///< The allocation request is valid but the not all resources were available presently
static const LIB_RESULT LIB_ERROR_ATTEMPT_DEALLOC_UNOWNED_RESOURCE      = 0xB5001; ///< User attempted to deallocate a resource which did no belong to him/her
static const LIB_RESULT LIB_ERROR_USER_STILL_OWNS_RESOURCES             = 0xB5002; ///< User attempted to deallocate a user who still has resources that need to be deallocated
static const LIB_RESULT LIB_ERROR_USER_WAITING_ON_RESOURCES             = 0xB5003; ///< User attempted to deallocate a user who is still waiting on resources to be allocated
static const LIB_RESULT LIB_ERROR_INITIALIZATION_VALUE_TOO_SMALL        = 0xB5004; ///< User attempted passed a value larger than what is supported by the values passed to ResMgr_Initialize
static const LIB_RESULT LIB_ERROR_INVALID_USER_ID                       = 0xB5005; ///< User attempted to reuse a user id which is no longer valid or provide a wrong value
static const LIB_RESULT LIB_ERROR_OUTPUT_ARRAY_SIZE_TOO_LARGE           = 0xB5006; ///< User passed a value larger than what is supported by the values passed to ResMgr_Initialize
static const LIB_RESULT LIB_ERROR_USER_INDEX_TOO_LARGE                  = 0xB5007; ///< User passed a value larger than what is supported by the values passed to ResMgr_Initialize
static const LIB_RESULT LIB_ERROR_SESSION_ID_MISMATCH                   = 0xB5008; ///< User session id does not match with current session for that user index
static const LIB_RESULT LIB_ERROR_USER_STILL_HAS_RESOURCES              = 0xB5009; ///< User attempted to allocate resources before freeing those he currently owns
static const LIB_RESULT LIB_NO_USER_WAITING_FOR_RESOURCES               = 0xB500A; ///< There are no users waiting for resources, meaning the queue is empty
static const LIB_RESULT LIB_USER_OBTAINED_RESOURCES                     = 0xB500B; ///< A user was allocated resources in the returning function, if he was blocked he should be unblocked
static const LIB_RESULT LIB_USER_WAITING_FOR_RESOURCES                  = 0xB500C; ///< There is a user waiting for resources, other allocation requests should be queued

/* Apps */
static const LIB_RESULT LIB_ERROR_OUTPUT_DOES_NOT_MATCH_REFERENCE       = 0xB6000; ///< The applications' output does not match the reference output.
static const LIB_RESULT LIB_APP_IS_CLOSING                              = 0xB6001; ///< The application has finished and is closing.

//****************************************************************************
#endif /* GLOBALERRORS_H */

