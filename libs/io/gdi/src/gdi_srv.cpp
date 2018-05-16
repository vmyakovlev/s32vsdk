/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file gdi_srv.cpp
* \brief GDI server process functionality implementation
* \author Tomas Babinec
* \version 0.1
* \date 05-September-2013
****************************************************************************/
#include <stdint.h>
#include <string.h>

// oal functionality
#include "oal.h"
#include "oal_extras.h"
#include "oal_process.h"

//#define VDB_LOG_NOTE_OFF

// gdi functionality
#include "gdi_private.hpp"
#include "gdi_com.hpp"

#include "vdb_log.h"

#include "gdi_srv.hpp"

/*****************************************************************************
 * global data
 *****************************************************************************/
//const int __ShmClientMaxMaps = 2500;	///< max number of memory pages allowed to be allocated by shm client as

/*****************************************************************************
 * local (static) functions prototypes
 *****************************************************************************/

/*****************************************************************************/
/** Initializes server in case the process was created by client process.
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
static LIB_RESULT SrvExecutedByClient();

/*****************************************************************************/
/** Called in case server process was not created by client.
 *
 * Not supported server start up.
 * Leads to error logging followed by termination of the process.
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
static LIB_RESULT SrvExecutedByUser();

/****************************************************************************/
/** Initializes server side of GDI API.
 *
 * Creates and opens shared memory and required
 * semaphores. Starts message loop.
 *
 * \return LIB_SUCCESS if all ok. LIB_FAILURE returned if error is encountered
 * during init.
 *****************************************************************************/
static LIB_RESULT SrvInit();

/****************************************************************************/
/** Initializes server side process as client.
 *
 * Used only for future ServerCloseForceCall() action.
 *
 * \return LIB_SUCCESS if all ok. LIB_FAILURE returned if error is encountered
 * during init.
 *****************************************************************************/
static LIB_RESULT InitAsClient();

/****************************************************************************/
/** Initializes GDI environment as client from server side.
 *
 * \note Intended to be used with INTEGRITY and no RFS.
 *
 * \return LIB_SUCCESS if all ok. LIB_FAILURE returned if error is encountered
 * during init.
 *****************************************************************************/
static LIB_RESULT InitGdiEnvAsClient();

/****************************************************************************/
/** Server loop for clients' message processing.
*
* Waits on server semaphore for clients' messages, and calls message processing
* function. Runs until last client deinitilizes.
*
* \return LIB_SUCCESS when last client deinit message processed. LIB_FAILURE
* if some error encountered.
*****************************************************************************/
static LIB_RESULT SrvLoop();

/****************************************************************************/
/** Clients' messages processing.
*
* Calls message handler appropriate to specified message type.
*
* \param aCliCnt reference to an output variable where current clients' count
* will be set if GDI_MSG_CLIENT_DEINIT is called.
*
* \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
*****************************************************************************/
static LIB_RESULT SrvMsgProc(uint32_t &aCliCnt);

/****************************************************************************/
/** Test message handler.
*
* For GDI communication testing purposes only. Might be removed in the future.
*
* \return LIB_SUCCESS always.
*****************************************************************************/
static LIB_RESULT SrvMsgProcTest();

/****************************************************************************/
/** Close & unlink all opened server and shared resources.
*
* Called after last client deinit message processed. Server shutdown follows.
*
* \return LIB_SUCCESS of all ok, otherwise LIB_FAILURE.
*****************************************************************************/

static LIB_RESULT SrvClose(void);
/****************************************************************************/
/** Client deinit message handler.
*
* Finds out current number of clients. If
*
* \param aClientCnt currant count of clients.
*
* \return LIB_SUCCESS of all ok, otherwise LIB_FAILURE.
*****************************************************************************/
static LIB_RESULT ClientDeInit(uint32_t &aClientCnt);

/****************************************************************************/
/** Instructs server to shutdown its execution.
 *
 * Causes server to shutdown even if clients are still out there.
 *
 * \return LIB_SUCCESS if all ok, else LIB_FAILURE.
 ****************************************************************************/
static LIB_RESULT ServerCloseForceCall();

/****************************************************************************/
/** Forces server to shutdown.
 *
 * Doesn't care if there are any clients connected or not.
 *
 * \return LIB_SUCCESS if all ok, LIB_FAILURE if init semaphore obtain fails
 ****************************************************************************/
static LIB_RESULT ServerCloseForce();

/*****************************************************************************
 * local (static) functions prototypes
 *****************************************************************************/

int32_t main(int32_t argc, char* argv[])
{

  //
  // GDI server part
  //

  // prepare logging
  FILE *log_stream;
  FILE *log_file = NULL;
  // set logging output stream to stdout
  printf("****** ///// HELLO, GDI SERVER HERE ///////////// **************");
  log_stream = stdout;
  vdb_LogInitStr(log_stream);


  VDB_LOG_NOTE("GDI server process has been executed.\n");
  printf("****** ///// HELLO, GDI SERVER HERE ///////////// **************");

  // vars for returned values handling
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;

	argc = 2;
  // check cmd line args
  if(argc > 1)
  {
    // check if executed by user or client process
    if(strcmp(argv[1],"clipr") == 0)
    {
      VDB_LOG_NOTE("Server executed by client process.\n");
      tmpLres = SrvExecutedByClient();
      if (tmpLres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Server: \"execution by client\" failed.\n");
        lres = LIB_FAILURE;
      } // if SrvExecutedByClient() failed
    }else
    {
      // executed otherwise (from shell probably)
      // check if GDI server stop is requested
      if(strcmp(argv[1],"stop") != 0)
      {
	// check if GDI server init is requested
	if(strcmp(argv[1],"init") == 0)
	{
	  tmpLres = InitGdiEnvAsClient();
	  if (tmpLres != LIB_SUCCESS)
	  {
	    VDB_LOG_ERROR("Server: GDI environment initialization failed.\n");
	    lres = LIB_FAILURE;
	  } // if SrvGdiEnvAsClient() failed
	  else
	  {
	    tmpLres = SrvExecutedByClient();
	    if (tmpLres != LIB_SUCCESS)
	    {
	      OAL_SemaphoreDelete(sOalSemInit);
	      VDB_LOG_ERROR("Server: Execution as client failed.\n");
	      lres = LIB_FAILURE;
	    } // if SrvExecutedByClient() failed
	  } // else from if SrvGdiEnvAsClient() failed
	} // if GDI env init requested
	else
	{
	  VDB_LOG_WARNING("Unknown first command line parameter. Terminating execution.\n");
	  lres = LIB_FAILURE;
	} // else form if GDI env init requested
      }else
      {
        //
        // real server should run already
        // and will be forced to shutdown
        //

        VDB_LOG_NOTE("stop parameter given\n");

        // first try to connect to server as normal client
        // do not try to create server process if none detected
        tmpLres = InitAsClient();
        if( tmpLres != LIB_SUCCESS )
        {
          VDB_LOG_ERROR("Server as client: Attempt to connect this process to real server failed.\n");
          lres = LIB_FAILURE;
        }else
        {
          // force real server to close
          tmpLres = ServerCloseForceCall();
          if (tmpLres != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Server as client: Forcing real server to close did not succeed.\n");
            lres = LIB_FAILURE;
          } // if SeverCloseForceCall() failed

          // close this sides shared resources
          tmpLres = SrvClose();
          if (tmpLres != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Server as client: Closing shared resources failed.\n");
            lres = LIB_FAILURE;
          } // if SrvExecutedByCmdln() failed
        } //else from if GDI_Init != LIB_SUCCESS
      } // else from if != "stop"
    }// else from if executed by client
  }else
  {
    // executed from command line with no params
    tmpLres = SrvExecutedByUser();
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: \"execution from command line\" failed.\n");
      lres = LIB_FAILURE;
    } // if SrvExecutedByCmdln() failed
  } // else from if (argc >1)

  if( log_file )
  {
    // close log file if any
    fclose(log_file);
  }
  // stop logging
  vdb_LogStop();
  return lres;
}

static LIB_RESULT SrvExecutedByClient()
{
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;
  // executed by client process > only open init semaphore
  // which has been already obtained by client process
  tmpLres = OAL_SemaphoreGetByName(&sOalSemInit,spSemInitName);
  if(0)//tmpLres != LIB_SUCCESS)
  {
    // log error
    VDB_LOG_ERROR("Server: SemaphoreGetByName on Init failed.\n");
    lres = LIB_FAILURE;
  }else
  {
    // Init semaphore opened => continue server initialization
    tmpLres = SrvInit();
    if( tmpLres != LIB_SUCCESS )
    {
      VDB_LOG_ERROR("Server: Initialization failed. Terminating.\n");
      lres = LIB_FAILURE;
    }else
    {
      // run server semaphore sync loop
      VDB_LOG_NOTE("Server: SrvInit finished successfully > executing server loop.\n");

      tmpLres = SrvLoop();
      if( tmpLres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Server: SrvLoop failed.\n");
        lres = LIB_FAILURE;
      }

      VDB_LOG_NOTE("Server: server loop ended.\n");

      // deInit server stuff
      tmpLres = SrvClose();
      if( tmpLres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Server: SrvClose failed.\n");
        lres = LIB_FAILURE;
      } // if SrvClose failed
    } // else from if SrvInit failed
  } // else from if (tmpLres != LIB_SUCCESS)
  return lres;
} // SrvExecutedByClient

static LIB_RESULT SrvExecutedByUser()
{
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;
  // executed from command line > not supported right now
  VDB_LOG_NOTE("Server: executed from command line.\n");
  VDB_LOG_ERROR("Server: execution from command line not supported.\n");
  tmpLres = LIB_FAILURE;
  lres = tmpLres;
  return lres;
} // SrvExecutedFromCmdln()

static LIB_RESULT InitGdiEnvAsClient()
{
  LIB_RESULT lres = LIB_SUCCESS, tmpLres = LIB_FAILURE;

  //bool firstProcess = false;

  //tmpLres = GDI_InitSemSynchronize(&initSem, &firstProcess);
  tmpLres = OAL_SemaphoreCreate(&sOalSemInit,
                               spSemInitName,
                               OAL_SEMAPHORE_COUNTING,
                               0);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Init semaphore cration failed.\n");
    lres = LIB_FAILURE;
  } // if init sem cration failed
  
  return lres;
} // InitGdiEnvAsClient()

static LIB_RESULT SrvLoop()
{
  LIB_RESULT lres = 0;
  uint32_t msg_counter = 0;
  uint32_t cliCnt = 1;
  while (cliCnt)
  {
    lres = OAL_SemaphoreObtain(sOalSemSrv, 0xFFFFFFFF);
    // check wait result
    if (lres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("server: server semaphore wait failed.\n");
      return LIB_FAILURE;
    }
    msg_counter++;
    VDB_LOG_NOTE("server: processing %u. message.\n", msg_counter);

    if(SrvMsgProc(cliCnt) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("server: message procedure failed.\n");
    } // if

    // post client semaphore to allow other messages to be sent
    lres = OAL_SemaphoreRelease(sOalSemCli);
    if (lres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("server: client semaphore post failed.\n");
      return LIB_FAILURE;
    } // if
  } // while
  return lres;
} // SrvLoop()

static LIB_RESULT SrvMsgProc(uint32_t &aCliCnt)
{
  LIB_RESULT ret = LIB_SUCCESS, lres = LIB_FAILURE; // variable for GDI result checking

  // get message type from shared memory message
  GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
  // set pointer to message data
  void *msg_data = (void*) (((char*) spSrvCliMsgBuf) + sizeof(GDI_MSG_TYPE));

  // call correct handler
  switch (*msg_type)
  {
    case GDI_MSG_DISPLAY_CONTROL:
    {
      VDB_LOG_NOTE("GDI_MSG_DISPLAY_CONTROL\n");
      // convert message type
      Gdi_Msg_DC *msgDC = (Gdi_Msg_DC*) msg_data;
      // call manager handler

      lres = GDI_Manager::DisplayControl(msgDC);
      // set response
      msgDC->mResult = lres;
    }
      break;

    case GDI_MSG_DISPLAY_CONTROL_UPDATE:
    {
      VDB_LOG_NOTE("GDI_MSG_DISPLAY_CONTROL_UPDATE\n");
      // convert message type
      Gdi_Msg_DC *msgDC = (Gdi_Msg_DC*) msg_data;
      // call manager handler

      lres = GDI_Manager::DisplayControlUpdate(msgDC);
      // set response
      msgDC->mResult = lres;

    }
      break;
    case GDI_MSG_DISPLAY_INFO_GET:
    {
      VDB_LOG_NOTE("GDI_MSG_DISPLAY_INFO_GET\n");
      // convert message type
      Gdi_Msg_DIG *msgDIG = (Gdi_Msg_DIG*) msg_data;

      // call manager handler
      lres = GDI_Manager::DisplayInfoGet(msgDIG);
      // set response
      msgDIG->mResult = lres;

    }
      break;
    case GDI_MSG_DISPLAY_COLOR_FILL:
    {
      VDB_LOG_NOTE("GDI_MSG_DISPLAY_COLOR_FILL\n");
      // convert message type
      Gdi_Msg_DCF *msgDCF = (Gdi_Msg_DCF*) msg_data;

      // call manager handler
      lres = GDI_Manager::DisplayColorFill(msgDCF);
      // set response
      msgDCF->mResult = lres;

    }
      break;
    case GDI_MSG_DISPLAY_BUFFER_COPY:
    {
      VDB_LOG_NOTE("GDI_MSG_DISPLAY_BUFFER_COPY\n");
      // convert message type
      Gdi_Msg_DBC *msgDBC = (Gdi_Msg_DBC*) msg_data;
      // call manager handler
      lres = GDI_Manager::DisplayBufferCopy(msgDBC, (char*) spShmIdx);
      // set response
      msgDBC->mResult = lres;
    }
      break;

    case GDI_MSG_WINDOW_CREATE:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_CREATE\n");
      // convert message type
      Gdi_Msg_WC *msgWC = (Gdi_Msg_WC*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowCreate(msgWC);
      // set response
      msgWC->mResult = lres;
    }
      break;
    case GDI_MSG_WINDOW_DESTROY:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_DESTROY\n");
      // convert message type
      Gdi_Msg_WD *msgWD = (Gdi_Msg_WD*) msg_data;

      // call manager handler
      lres = GDI_Manager::WindowDestroy(msgWD);

      // set response
      msgWD->mResult = lres;

    }
      break;
    case GDI_MSG_MAPPING_CREATE:
    {
      VDB_LOG_NOTE("GDI_MSG_MAPPING_CREATE\n");
      // convert message type
      Gdi_Msg_MC *msgMC = (Gdi_Msg_MC*) msg_data;
      // call manager handler
      lres = GDI_Manager::MappingCreate(msgMC);
      // set response
      msgMC->mResult = lres;

    }
      break;
    case GDI_MSG_MAPPING_DESTROY:
    {
      VDB_LOG_NOTE("GDI_MSG_MAPPING_DESTROY\n");
      // convert message type
      Gdi_Msg_MD *msgMD = (Gdi_Msg_MD*) msg_data;
      // call manager handler
      lres = GDI_Manager::MappingDestroy(msgMD);
      // set response
      msgMD->mResult = lres;

    }
      break;

    case GDI_MSG_MAPPING_MODIFY:
    {
      VDB_LOG_NOTE("GDI_MSG_MAPPING_MODIFY\n");
      // convert message type
      // mapping create structure is used here because
      // the data are the same for mapping modify
      Gdi_Msg_MC *msgMC = (Gdi_Msg_MC*) msg_data;
      // call manager handler
      lres = GDI_Manager::MappingModify(msgMC);
      // set response
      msgMC->mResult = lres;

    }
      break;
    case GDI_MSG_CLIENT_DEINIT:
    {
      VDB_LOG_NOTE("GDI_MSG_CLIENT_DEINIT\n");
      // call handler
      lres = ClientDeInit(aCliCnt);
      VDB_LOG_NOTE("GDI_MSG_CLIENT_DEINIT returned.\n");
      // set response
      *((LIB_RESULT*) msg_data) = lres;
      ret = lres;
      if( ret != LIB_SUCCESS )
      {
        VDB_LOG_ERROR("Server: client deinit failed. Server will be terminated.\n");
      }

    }
      break;
    case GDI_MSG_SERVER_CLOSE_FORCE:
    {
      // call handler
      VDB_LOG_NOTE("GDI_MSG_SERVER_CLOSE_FORCE\n");
      lres = ServerCloseForce();
      aCliCnt = 0;

      // set response
      *((LIB_RESULT*) msg_data) = lres;
      ret = lres;
      if( ret != LIB_SUCCESS )
      {
        VDB_LOG_ERROR("Server: Server forced close failed. Server will be terminated anyway.\n");
      }
    }
    break;
    case GDI_MSG_WINDOW_COLOR_FILL:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_COLOR_FILL\n");
      // convert message type
      Gdi_Msg_WCF *msgWCF = (Gdi_Msg_WCF*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowColorFill(msgWCF);
      // set response
      msgWCF->mResult = lres;

    }
      break;

    case GDI_MSG_WINDOW_UPDATE:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_UPDATE\n");
      // convert message type
      Gdi_Msg_WU *msgWU = (Gdi_Msg_WU*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowUpdate(msgWU, (uint8_t*) spShmIdx);
      // set response
      msgWU->mResult = lres;
    }
      break;
    case GDI_MSG_WINDOW_UPDATE_REGION:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_UPDATE_REGION\n");
      // convert message type
      Gdi_Msg_WUR *msgWUR = (Gdi_Msg_WUR*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowUpdateRegion(msgWUR, (uint8_t*) spShmIdx);
      // set response
      msgWUR->mResult = lres;
    }
      break;

			
#ifdef GDI_SWB
    case GDI_MSG_WINDOW_BUFFER_GET:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_BUFFER_GET\n");
      // convert message type
      Gdi_Msg_WBG *msgWBG = (Gdi_Msg_WBG*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowBufferGet(msgWBG);
      // set response
      msgWBG->mResult = lres;
    }
      break;
    case GDI_MSG_WINDOW_BUFFER_SET:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_BUFFER_SET\n");
      // convert message type
      Gdi_Msg_WBS *msgWBS = (Gdi_Msg_WBS*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowBufferSet(msgWBS);
      // set response
      msgWBS->mResult = lres;
    }
      break;
    case GDI_MSG_WINDOW_BUFFER_DROP:
    {
      VDB_LOG_NOTE("GDI_MSG_WINDOW_BUFFER_DROP\n");
      // convert message type
      Gdi_Msg_WBD *msgWBD = (Gdi_Msg_WBD*) msg_data;
      // call manager handler
      lres = GDI_Manager::WindowBufferDrop(msgWBD);
      // set response
      msgWBD->mResult = lres;
    }
      break;
#endif // ifdef GDI_SWB
			
    case GDI_MSG_TEST:
    {
      VDB_LOG_NOTE("GDI_MSG_TEST\n");
      // call handler
      lres = SrvMsgProcTest();
      // set response, rewrites handler settings
      *((LIB_RESULT*) spSrvCliMsgBuf) = lres;

    }
      break;
    default:
    {
      // unknown message type > default processing
      VDB_LOG_ERROR("server: uknown message > default processing.\n");
      // set message type to GDI_MSG_UNKNOWN
      *msg_type = GDI_MSG_UNKNOWN;
      // put LIB_FAILURE behind message type
      *((LIB_RESULT*) msg_data) = LIB_FAILURE;
    }
      break;
  } //  switch

  // post response semaphore
  lres = OAL_SemaphoreRelease(sOalSemResp);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("server: response semaphore post failed.\n");
    return LIB_FAILURE; // some failure = NOK
  }

  VDB_LOG_NOTE("Server: response sem released ok.\n");

  // server semaphore wait
  lres = OAL_SemaphoreObtain(sOalSemSrv, 0xFFFFFFFF);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("server: server semaphore wait failed.\n");
    return LIB_FAILURE; // some failure = NOK
  }
  return ret;
} //  SrvMsgProc()

//
//  clients' communication messages' handlers
//

static LIB_RESULT SrvMsgProcTest()
{
  // read shared memory buffer
  char str_buf[GDI_SHM_GEN_SIZE];
  (void)strcpy(str_buf, ((char*) spSrvCliMsgBuf) + sizeof(GDI_MSG_TYPE));

  // execute handler
  VDB_LOG_NOTE("server: Hello! This is server. Reading from shared memory: %s\n",
         str_buf);

  // write result
  *((int32_t*) spSrvCliMsgBuf) = 11;

  return LIB_SUCCESS;
} //  SrvMsgProcTest()

static LIB_RESULT SrvInit()
{
  VDB_LOG_NOTE("GDI server process initialization in progress.\n");
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;

  // take ownership of init semaphore
  // server is responsible for unlinking semaphore
  tmpLres = OAL_SemaphoreOwnershipSet(sOalSemInit, true);
  if (tmpLres != LIB_SUCCESS)
  {
    printf("ERROR, semaphore ownership get failed.\n");
    VDB_LOG_ERROR("Server: getting init semaphore ownership failed.\n");
    lres = LIB_FAILURE;
  }else
  {
    // create general shared memory area
    tmpLres = OAL_SharedMemoryCreate(&sOalShm, spOalShmName, GDI_SHM_GEN_SIZE);
    if (tmpLres != LIB_SUCCESS)
    {
      // no check anymore
      (void)OAL_SemaphoreDelete(sOalSemInit);
      VDB_LOG_ERROR("Server: general shared memory creation failed\n");
      return LIB_FAILURE; // some failure = NOK
    } else
    {
      VDB_LOG_NOTE("Server: shared memory object created and opened \n");
    } // if

    // get pointer to general shared memory
    tmpLres = OAL_SharedMemoryGetPointer(&spShm, sOalShm);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: getting general shared memory pointer failed.\n");
      return LIB_FAILURE;
    }

    // set server/client stats pointer
    spSrvCliStats = (GDI_SrvCliStats*) spShm;
    // init stats values
    spSrvCliStats->mClientCnt = 0;
    spSrvCliStats->mServerAlive = GDI_SERVER_ON;

    // set server/client message buffer pointer - behind SrvCliStats
    spSrvCliMsgBuf = (void*) ((char*) spShm + sizeof(GDI_SrvCliStats));

    // create image data exchange shared memory area
    tmpLres = OAL_SharedMemoryCreate(&sOalShmIdx, spOalShmIdxName,
    GDI_SHM_IMG_DATA_SIZE);
    if (tmpLres != LIB_SUCCESS)
    {
      OAL_SharedMemoryDestroy(sOalShm);
      VDB_LOG_ERROR("Server: image data exchange shm memory creation failed\n");
      return LIB_FAILURE; // some failure = NOK
    } else
    {
      VDB_LOG_NOTE("Server: image data exchange shared memory object created and opened \n");
    } // if

    // get pointer to image data exchange shared memory area
    tmpLres = OAL_SharedMemoryGetPointer(&spShmIdx, sOalShmIdx);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: getting image data exchange shared memory pointer failed.\n");
      return LIB_FAILURE;
    }

    // create open server semaphore
    //pgSemSrv = sem_open(pgSemSrvName, O_RDWR | O_CREAT | O_EXCL, 0666, 0);
    tmpLres = OAL_SemaphoreCreate(&sOalSemSrv,
                               spSemSrvName,
                               OAL_SEMAPHORE_COUNTING,
                               0);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: server semaphore creation failed\n");
      return LIB_FAILURE; // some failure = NOK
    }
    VDB_LOG_NOTE("Server: server semaphore created as locked and opened\n");

    // create open client semaphore with one post
    tmpLres = OAL_SemaphoreCreate(&sOalSemCli,
                               spSemCliName,
                               OAL_SEMAPHORE_COUNTING,
                               1);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: client semaphore creation failed\n");
      return LIB_FAILURE; // some failure = NOK
    }
    VDB_LOG_NOTE("Server: client semaphore created with 1 post and opened\n");

    // create open response semaphore
    tmpLres = OAL_SemaphoreCreate(&sOalSemResp,
                               spSemRespName,
                               OAL_SEMAPHORE_COUNTING,
                               0);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: response semaphore creation failed\n");
      return LIB_FAILURE; // some failure = NOK
    }
    VDB_LOG_NOTE("Server: response semaphore created locked and opened\n");

		// initialize GDI_Manager
		GDI_Manager::Init();

#ifdef GDI_SWB
#ifndef GDI_CONTIG
		// initialize shared window buffer server
		GDI_OSWBServer::Init();
#endif // GDI_CONTIG
#endif // GDI_SWB
		// TODO: do further server init stuff if something comes up
		
    // post init semaphor  -  everything prepared for clients
    tmpLres = OAL_SemaphoreRelease(sOalSemInit);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Server: init semaphore post failed\n");
      return LIB_FAILURE; // some failure = NOK
    }
  }
  return lres;
} // SrvInit()


static LIB_RESULT InitAsClient()
{
  LIB_RESULT lres = LIB_SUCCESS;

  // try to open init semaphore
  lres = OAL_SemaphoreGetByName(&sOalSemInit, spSemInitName);
  if (lres != LIB_SUCCESS)
  {
    if (lres == OAL_ERR_SEM_EBADSEM)
    {
      //  server probably ended in between
      VDB_LOG_WARNING("Client: no server detected. Returning.\n");
    } else //if  BADSEM
    {
      VDB_LOG_ERROR("Client: init semaphore get by name failed. Returning.\n");
    }
    return LIB_FAILURE;
  } // if != LIB_SUCCESS
  VDB_LOG_NOTE("Client: init semaphore opened.\n");

  //
  // this is the client
  //

  // wait for init semaphor posted again - in case server is initializing
  // then initialize this client
  // todo: set time out for wait, server might have been forked to but not executed
  lres = OAL_SemaphoreObtain(sOalSemInit, 0xFFFFFFFF);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: init semaphore obtain failed. Returning.\n");
    // try to release resources - no checking anymore
    (void)OAL_SemaphoreDelete(sOalSemInit);
    return LIB_FAILURE; // some failure = NOK
  } // if

  // open general shared memory area - should be prepared already
  lres = OAL_SharedMemoryGetByName(&sOalShm, spOalShmName);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_NOTE("Client: general shared memory open failed. Server probably "\
        "ended during client init procedure. Trying to repeat init process.\n");
    // server probably ended in between

    // post init semaphore, in case the servere wont manage to unlik it before
    // this client tries to reopen it
    lres = OAL_SemaphoreRelease(sOalSemInit);
    if (lres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Client: init semaphore release failed. Returning.\n");
      // try to release resources - no checking anymore
      (void)OAL_SemaphoreDelete(sOalSemInit);
      return LIB_FAILURE; // some failure = NOK
    } // if

    // close init semaphore
    lres = OAL_SemaphoreDelete(sOalSemInit);
    if (lres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Client: init semaphore close failed. Returning.\n");
      return LIB_FAILURE;
    } // if

    return LIB_FAILURE;
  }

  VDB_LOG_NOTE("Client: general shared memory object opened.\n");

  lres = OAL_SharedMemoryGetPointer(&spShm, sOalShm);
  if (lres != LIB_SUCCESS)
  {
    // try to release resources - no checking anymore
    VDB_LOG_ERROR("Client: getting general shared memory pointer failed. Returning\n");
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    return LIB_FAILURE;
  }

  // set server/client stats pointer
  spSrvCliStats = (GDI_SrvCliStats*) spShm;
  // set server/client message buffer pointer - behind SrvCliStats
  spSrvCliMsgBuf = (void*) ((char*) spShm + sizeof(GDI_SrvCliStats));

  // check if server is still alive, otherwise goal has been reached (server stopped)
  if (spSrvCliStats->mServerAlive == GDI_SERVER_OFF)
  {
    VDB_LOG_WARNING("Client: server not alive anymore. Goal reached.\n");
    // try to release resources - no checking anymore
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    return LIB_SUCCESS;
  }    // if GDI_SERVER_OFF

  // update clients count
  if (spSrvCliStats->mClientCnt < GDI_MAX_CLIENT_CNT)
  {
    spSrvCliStats->mClientCnt++;
  } else
  {
    VDB_LOG_ERROR("Client: max number of GDI clients has been reached already. Returning LIB_FAILURE.\n");
    // try to release resources - no checking anymore
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    (void)OAL_SharedMemoryDestroy(sOalShmIdx);
    return LIB_FAILURE;
  } // else <GDI_MAX_CLIENT_CNT

  // open server semaphore
  lres = OAL_SemaphoreGetByName(&sOalSemSrv, spSemSrvName);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: server semaphore get by name failed. Returning.\n");
    // try to release resources - no checking anymore
    spSrvCliStats->mClientCnt--;
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    (void)OAL_SharedMemoryDestroy(sOalShmIdx);
    return LIB_FAILURE; // some failure = NOK
  } // if SEM_FAILED
  VDB_LOG_NOTE("Client: server semaphore opened. Returning.\n");

  // open client semaphore
  lres = OAL_SemaphoreGetByName(&sOalSemCli, spSemCliName);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: client semaphore open failed. Returning.\n");
    // try to release resources - no checking anymore
    spSrvCliStats->mClientCnt--;
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemSrv);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    (void)OAL_SharedMemoryDestroy(sOalShmIdx);
    return LIB_FAILURE;
  } // if SEM_FAILED
  VDB_LOG_NOTE("Client: client semaphore opened.\n");

  // open response semaphore
  lres = OAL_SemaphoreGetByName(&sOalSemResp, spSemRespName);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: response semaphore open failed.\n");
    // try to release resources - no checking anymore
    spSrvCliStats->mClientCnt--;
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemSrv);
    (void)OAL_SemaphoreDelete(sOalSemCli);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    (void)OAL_SharedMemoryDestroy(sOalShmIdx);
    return LIB_FAILURE;
  } // if SEM_FAILED
  VDB_LOG_NOTE("Client: response semaphore opened.\n");

  // post init semaphore for other possibly starting/deinitialising processes
  lres = OAL_SemaphoreRelease(sOalSemInit);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: init semaphore post failed.\n");
    // try to release resources - no checking anymore
    spSrvCliStats->mClientCnt--;
    (void)OAL_SemaphoreRelease(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemInit);
    (void)OAL_SemaphoreDelete(sOalSemSrv);
    (void)OAL_SemaphoreDelete(sOalSemCli);
    (void)OAL_SemaphoreDelete(sOalSemResp);
    (void)OAL_SharedMemoryDestroy(sOalShm);
    (void)OAL_SharedMemoryDestroy(sOalShmIdx);
    return LIB_FAILURE;
  } // if sem_post failed

  VDB_LOG_NOTE("Client: init finished successfully > returning.\n");
  return LIB_SUCCESS;
} // InitAsClient()


static LIB_RESULT SrvClose()
{
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;

  // release all forgotten resources
  tmpLres = GDI_Manager::Clean();
  if( tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: Cleaning windows manager resources failed.\n");
    lres = LIB_FAILURE;
  } // GDI_Manager::Clean() failed

  // destroy general shared memory
  tmpLres = OAL_SharedMemoryDestroy(sOalShm);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: destroying general shared memory failed.\n");
    lres = LIB_FAILURE;
  }
  // reset general shared memory pointers
  spShm = NULL;
  spSrvCliStats = NULL;
  spSrvCliMsgBuf = NULL;

  // destroy image data exchange shared memory
  tmpLres = OAL_SharedMemoryDestroy(sOalShmIdx);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: destroying image data exchange shared memory failed.\n");
    lres = LIB_FAILURE;
  }

  // reset image data exchange memory pointer
  spShmIdx = NULL;

  // post init semaphore for other possibly starting processes.
  // Has to happen after shm close and unlink - this way clients
  // waiting for init will know, the server is OFF.
  tmpLres = OAL_SemaphoreRelease(sOalSemInit);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: init semaphore post failed.\n");
    lres = LIB_FAILURE; // some failure = NOK
  }

  // close init semaphore
  tmpLres = OAL_SemaphoreDelete(sOalSemInit);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: init semaphore close failed.\n");
    lres = LIB_FAILURE;
  }

  // close server semaphore
  tmpLres = OAL_SemaphoreDelete(sOalSemSrv);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: server semaphore close failed.\n");
    lres = LIB_FAILURE;
  }

  // close client semaphore
  tmpLres = OAL_SemaphoreDelete(sOalSemCli);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: client semaphore close failed.\n");
    lres = LIB_FAILURE;
  }

  // close response semaphore
  tmpLres = OAL_SemaphoreDelete(sOalSemResp);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: response semaphore close failed.\n");
    lres = LIB_FAILURE;
  }

#ifdef GDI_SWB
#ifndef GDI_CONTIG
		// close shared window buffer server
		GDI_OSWBServer::Close();
#endif // GDI_CONTIG
#endif // GDI_SWB

  VDB_LOG_NOTE("Server: terminating.\n");
  // server ready to end
  return lres;
}

static LIB_RESULT ClientDeInit(uint32_t &aClientCnt)
{
  LIB_RESULT lres = LIB_SUCCESS;
  // wait for Init semaphore - need to access pgSrvCliStat
  lres = OAL_SemaphoreObtain(sOalSemInit, GDI_SEM_WAIT_TIME);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: init semaphore wait failed.\n");
    lres = LIB_FAILURE; // some failure = NOK
  }

  if(spSrvCliStats->mClientCnt > 0)
  {
    VDB_LOG_NOTE("Server: client count = %u.\n", spSrvCliStats->mClientCnt);
    --spSrvCliStats->mClientCnt;
    // got init semaphore -> any clients left?
    aClientCnt = spSrvCliStats->mClientCnt;
    if (aClientCnt > 0)
    {
      // still clients out there
      // leave init section > continue normal execution
      // post init semaphore
      // res = sem_post(pgSemInit);
      lres = OAL_SemaphoreRelease(sOalSemInit);
      if (lres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Server: init semaphore post failed.\n");
        return LIB_FAILURE; // some failure = NOK
      }else
      {
        VDB_LOG_NOTE("Server: init released ok.\n");
      }
    }else
    {
      VDB_LOG_NOTE("Server: Last client closed.\n");
      // set server status to OFF
      spSrvCliStats->mServerAlive = GDI_SERVER_OFF;
			// release Init semaphore
			lres = OAL_SemaphoreRelease(sOalSemInit);
      if (lres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Server: init semaphore post failed.\n");
        return LIB_FAILURE; // some failure = NOK
      }else
      {
        VDB_LOG_NOTE("Server: init released ok.\n");
      }
    } // else from if clients left
    // continue normal execution
    return lres;
  }else
  {
    VDB_LOG_ERROR("Server: client count already 0.\n");
    return LIB_FAILURE;
  }

  // no clients left
  // stay in init section and begin server closing

  return lres;
}

static LIB_RESULT ServerCloseForceCall()
{
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;

  // wait for Client semaphore post
  tmpLres = OAL_SemaphoreObtain(sOalSemCli, 0xFFFFFFFF);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: client semaphore wait failed.\n");
    lres = LIB_FAILURE;
  }else
  {
    // use shared memory as message buffer
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;

    // write message type to shared memory
    *msg_type = GDI_MSG_SERVER_CLOSE_FORCE;

    // server semaphore post
    tmpLres = OAL_SemaphoreRelease(sOalSemSrv);
    if (tmpLres != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Client: server semaphore post failed.\n");
      // todo: should probably kill server
      // try to release client semaphore for other possibly waiting processes
      if(OAL_SemaphoreRelease(sOalSemCli))
      {
       VDB_LOG_ERROR("Client: client semaphore post failed.\n");
      }
      lres = LIB_FAILURE;
    }else
    {
      // server response semaphore wait
      tmpLres = OAL_SemaphoreObtain(sOalSemResp, 0xFFFFFFFF);
      if (tmpLres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("response semaphore wait failed\n");
        // todo: should probably kill server
        // try to release client semaphore for other possibly waiting processes
        if(OAL_SemaphoreRelease(sOalSemCli))
        {
         VDB_LOG_ERROR("Client: client semaphore post failed.\n");
        }
        lres = LIB_FAILURE;
      }else
      {
        // read response to local client variable
        tmpLres = *((LIB_RESULT*) ((int8_t*) spSrvCliMsgBuf
            + sizeof(GDI_MSG_TYPE)));
        if(tmpLres != LIB_SUCCESS)
        {
          lres = LIB_FAILURE;
        } // if

        tmpLres = OAL_SemaphoreRelease(sOalSemSrv);
        if (tmpLres != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Client: server semaphore post failed\n");
          lres = LIB_FAILURE; // some failure = NOK
        } // if
      }
    }
  }
  return lres;
} // ServerCloseForceCall()

static LIB_RESULT ServerCloseForce()
{
  LIB_RESULT lres;
  // wait for Init semaphore - need to access pgSrvCliStat
  lres = OAL_SemaphoreObtain(sOalSemInit, 0xFFFFFFFF);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: init semaphore wait failed.\n");
    lres = LIB_FAILURE; // some failure = NOK
  }

  if (spSrvCliStats->mClientCnt > 0)
  {
    // still clients out there
    // doesn't matter
    // only log warning
    VDB_LOG_WARNING("Server: Clients still out there but Srv shutdown forced.\n");
  } // if mClientCnt > 0

  // set server status to OFF
  spSrvCliStats->mServerAlive = GDI_SERVER_OFF;

  return lres;
} // SrvInit()
