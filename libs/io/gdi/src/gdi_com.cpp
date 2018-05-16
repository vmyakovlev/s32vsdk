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
 * \file     gdi_com.cpp
 * \brief    gdi api functionality implementation
 * \author   Tomas Babinec
 * \version  0.1
 * \date     30.7.2013
 * \note
 ****************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "oal.h"
#include "oal_extras.h"
#include "oal_process.h"
#include "oal_cont_alloc.h"

// gdi functionality
#include "gdi_types_public.hpp"
#include "gdi_private.hpp"
#include "gdi_com.hpp"

// log library
#include "vdb_log.h"

#ifdef ARM
#define GDI_SRV_BIN_PATH "./gdi_srv.elf"
#else
//#define GDI_SRV_BIN_PATH "/home/tomas/code/vision/s32v234_sdk/libs/gdi/build-deskx11-d"
//#define GDI_SRV_BIN_PATH "../../../libs/gdi/build-deskx11-d/gdi_srv"
#define GDI_SRV_BIN_PATH "gdi_srv"
#endif

// maximum count for GDI init repetition
#define MAX_REINIT 10u

/*****************************************************************************
 * local variables
 *****************************************************************************/

// shared memory
/// general shared memory name
static const char *spOalShmName = "gdiMgen";
/// general shared memory representation
static OAL_SHARED_MEM sOalShm;

///  pointer to beginning of whole general shared memory mapping address
static void* spShm = NULL;
///  pointer to ServerClient stats at the beginning of general shared memory
static GDI_SrvCliStats* spSrvCliStats;
///  pointer to ServerClient message buffer in general shared memory
///  located directly after stats structure
static void* spSrvCliMsgBuf = NULL;

/// image data exchange shared memory name
static const char *spOalShmIdxName = "gdiMidx";
/// image data exchange shared memory representation
static OAL_SHARED_MEM sOalShmIdx;

/// pointer to image data exchange shared memory mapping address
static void* spShmIdx = NULL;

// semaphores
/// init semaphore name
static const char *spSemInitName = "gdiSini";
/// init semaphore representation
static OAL_SEMAPHORE sOalSemInit;

/// server semaphore name
static const char *spSemSrvName = "gdiSsrv";
/// server semaphore representation
static OAL_SEMAPHORE sOalSemSrv;

/// client semaphore name
static const char *spSemCliName = "gdiScli";
/// client semaphore representation
static OAL_SEMAPHORE sOalSemCli;

/// response semaphore name
static const char *spSemRespName = "gdiSrsp";
/// response semaphore representation
static OAL_SEMAPHORE sOalSemResp;

/// thread counter
static uint32_t sgThreadCnt = 0;

/*****************************************************************************
 * local (static) functions prototypes
 *****************************************************************************/

/*****************************************************************************/
/** Function called in case the server process spawn fails.
 *
 * \param apParam not used now
 * \return always -1
 *****************************************************************************/
static int SrvInitFail(void *apParam);

/*****************************************************************************/
/** Creates server process.
 *
 * Calls OAL_ProcessSpawn() function.
 *
 * \return LIB_SUCCESS if all ok, else LIB_FAILURE
 *****************************************************************************/
static LIB_RESULT SrvCreate();

/***************************************************************************/
/** GDI init specific to first process.
 *
 * \param apInitSem init semaphore
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
 ***************************************************************************/
static LIB_RESULT GDI_InitFirst(OAL_SEMAPHORE apInitSem);

/***************************************************************************/
/** GDI init every first thread of each process has to go through.
 *
 * \param arRepeat if set by the function init procedure should be repeated
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
 ***************************************************************************/
static LIB_RESULT GDI_InitSubseq(bool &arRepeat);

/*****************************************************************************
 * local (static) functions
 *****************************************************************************/

//
static int SrvInitFail(void */*apParam*/)
{
  // log the error
  VDB_LOG_ERROR("Client: server process spawn failed.\n");
  return -1;
} // SrvInitFail()

//
static LIB_RESULT SrvCreate()
{
  LIB_RESULT lres = LIB_SUCCESS, tmpLres;

  // try out OAL process spawn
  char *arg = (char *)"clipr";
  tmpLres = OAL_ProcessSpawn(GDI_SRV_BIN_PATH,
                             1,
                             &arg,
                             SrvInitFail,
                             NULL);
  if (tmpLres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: server ProcessSpawn failed.\n");
    lres = LIB_FAILURE;
  } else
  {
    VDB_LOG_NOTE("Client: server process spawn succeeded. Further steps will determine if server is really running.\n");
  }

  return lres;
} // SrvCreate()

/*****************************************************************************
 * global (export) functions
 *****************************************************************************/

LIB_RESULT GDI_Init(uint32_t aThreadId)
{
  LIB_RESULT lres;
  OAL_SEMAPHORE initSem = NULL;
  uint32_t repeat_cnt = 0;

  bool firstProcess;
  bool repeat = false;

  do
  {
    ++repeat_cnt;
    lres = LIB_SUCCESS;
    firstProcess = false;
    repeat = false;

    // synchronize with init semaphore
    lres = GDI_InitSemSynchronize(&initSem,
                                  &firstProcess);

    // the firstProcess value not needed otherwise
    if (firstProcess)
    {
      VDB_LOG_NOTE("First process %u\n",
                   aThreadId);
    }
    // continue only if all ok so far
    if (lres == LIB_SUCCESS)
    {
      // Init semaphore is already obtained at this point
      // check if GDI initialized for this process already
      if (sOalSemInit == NULL)
      {
        VDB_LOG_NOTE("First thread %u\n",
                     aThreadId);
        // gdi has not been initialized for this process yet
        if (GDI_InitSubseq(repeat) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Subsequent call to GDI initialization failed. %s\n",
                        repeat?"repeat":"return");
          lres = LIB_FAILURE;
        } // if InitSubseq() failed
        else
        {
          // gdi is now initialised for this process
          sOalSemInit = initSem;
        } // else form if gdi client process init failed

#ifdef GDI_SWB
#ifndef GDI_CONTIG
		// initialize shared window buffer client
		GDI_OSWBClient::Init();
#endif // GDI_CONTIG
#endif // GDI_SWB

        if (OAL_SemaphoreRelease(sOalSemInit) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Init semaphore release failed.\n");
          lres = LIB_FAILURE;
          if (GDI_Close(0) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Client: Closing GDI failed.\n");
            lres = LIB_FAILURE;
          } // if GDI_Close() failed
        } // if init semaphore release failed

      } else
      {
        if (spSrvCliStats->mServerAlive)
        {
          if (spSrvCliStats->mClientCnt >= GDI_MAX_CLIENT_CNT)
          {
            VDB_LOG_ERROR("Maximum number of GDI clients reached already. Nothing done.\n");
            lres = LIB_FAILURE;
          } else
          {
            // increase counters
            ++(spSrvCliStats->mClientCnt);
            ++sgThreadCnt;
          } // else from if SDI_MAX_CLIENT_NUM reached
          // release SDI init semaphore
          if (OAL_SemaphoreRelease(sOalSemInit) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Failed to release init semaphore.\n");
            lres = LIB_FAILURE;
            // decrease counters
            --(spSrvCliStats->mClientCnt);
            --sgThreadCnt;
          } // if SemaphoreRelease(init) failed

					// Integrity doesnt allow to close same semaphore object more than once when opened multiple times
          /*if (OAL_SemaphoreDelete(initSem) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Failed to delete local init semaphore.\n");
            lres = LIB_FAILURE;
            // nothing more to be done here
          } // if  SemaphoreDelete() failed*/
					initSem = 0;
        } else
        {
          VDB_LOG_ERROR("Server vanished unexpectedly.\n");
          lres = LIB_FAILURE;
        } // else from if server alive check
      } // else from first thread in this process
      VDB_LOG_NOTE("Thread %u, thread cnt: %u, client cnt: %u\n",
                   aThreadId, sgThreadCnt, spSrvCliStats->mClientCnt);
    } // if all ok so far
  } while (repeat && (repeat_cnt <= MAX_REINIT));

  if( repeat )
  {
    VDB_LOG_ERROR("GDI initialization unsuccessful. Attempted %u times.\n", repeat_cnt - 1);
    lres = LIB_FAILURE;
  } // if repeat still true here == MAX_REINIT cnt reached

  //VDB_LOG_NOTE("%u Returning from INIT: overall cnt %u thread cnt %u\n", aTaskNum, *mpInitCounter, mThreadCounter);
  return lres;
} // GDI_Init(aSrvCreate)

LIB_RESULT GDI_InitSemSynchronize(OAL_SEMAPHORE *apInitSem,
                                  bool *apFirstProcess)
{
  LIB_RESULT lres = LIB_SUCCESS;
  *apFirstProcess = false;
  uint32_t repeat_cnt = 0;

  while (1)
  {
    repeat_cnt++;
    if(repeat_cnt > MAX_REINIT)
    {
      VDB_LOG_ERROR("INIT semaphore synchronisation failed. Attempted %u times.\n", repeat_cnt - 1);
      lres = LIB_FAILURE;
      break;
    }
    lres = OAL_SemaphoreCreate(apInitSem,
                               spSemInitName,
                               OAL_SEMAPHORE_COUNTING,
                               0);
    if (lres != LIB_SUCCESS)
    {
      if (lres == LIB_FAILURE)
      {
        // semaphore probably exists - server is running already 
        // open existing server semaphore
        lres = OAL_SemaphoreGetByName(apInitSem,
                                      spSemInitName);
        // check again
        if (lres != LIB_SUCCESS)
        {
          if (lres == OAL_ERR_SEM_EBADSEM)
          {
            //  server probably ended in between try repeat sem_open O_CREAT
            VDB_LOG_WARNING("GDI probably closed during client init. Repeating init procedure.");
            continue;
          }
          VDB_LOG_ERROR("Init semaphore get by name failed. Returning.\n");
          lres = LIB_FAILURE;
          *apInitSem = NULL;
          break;  // leave the loop
        } else
        {
          LIB_RESULT tmpLres;
          // try to obtain init semaphore
          tmpLres = OAL_SemaphoreObtain(*apInitSem,
                                           GDI_SEM_WAIT_TIME);
          if (tmpLres != LIB_SUCCESS)
          {
            if (tmpLres == OAL_ERR_SEM_EBADSEM)
            {
              //  server probably ended in between try repeat sem_open O_CREAT
              VDB_LOG_WARNING("GDI probably closed during client init. Repeating init procedure.");
              continue;
            } else if (tmpLres == OAL_ERR_SEM_ETIMEOUT)
            {
              VDB_LOG_ERROR("Init semaphore obtain timedout. Returning.\n");
              lres = LIB_FAILURE;
            } else
            {
              VDB_LOG_ERROR("Init semaphore obtain failed. Returning.\n");
              lres = LIB_FAILURE;
            }  // else from if sdi closed in between
            if (OAL_SemaphoreDelete(*apInitSem) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Init semaphore delete failed. Returning.\n");
            }
            *apInitSem = NULL;
            // break loop to return with previously set LIB_FAILURE
            break;
          } else
          {
            VDB_LOG_NOTE("Init semaphore opened and locked.\n");
            break;
          } // if SemaphoreTimedWait failed
        } // if != LIB_SUCCESS
      } // if semaphore exists already
      else
      {
        VDB_LOG_ERROR("Init semaphore open failed. Returning.\n");
        lres = LIB_FAILURE; // some failure = NOK
        break; // leave the loop
      } // else creating INIT semaphore failed
    } else
    {
      // this is first call to library - setup shared memmory
      *apFirstProcess = true;
      if (GDI_InitFirst(*apInitSem) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("First process calling GDI initialization failed.\n");
        if( OAL_SemaphoreDelete(*apInitSem) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Client: deleting init semaphore failed. Returning.\n");
        } // if INIT delete failed
        lres = LIB_FAILURE;
      } // if Shared memory ownership set failed
      break;
    } // else SEM_FAILED
  } // while(1)
  return lres;
} // GDI_InitSemSynchronize()

LIB_RESULT GDI_InitFirst(OAL_SEMAPHORE apInitSem)
{
  LIB_RESULT lres = LIB_SUCCESS;

  VDB_LOG_NOTE("Client: init semaphore created locked.\n");
  // client side not responsible for unlinking semaphore
  if (OAL_SemaphoreOwnershipSet(apInitSem,
                                false) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: releasing init semaphore ownership failed. Returning.\n");
    lres = LIB_FAILURE;
  } else
  {
    if (SrvCreate() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Client: server creation failed. Returning.\n");
      lres = LIB_FAILURE;
    } else
    {
      if (OAL_SemaphoreObtain(apInitSem,
                                 GDI_SEM_WAIT_TIME) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: init semaphore obtain %s. Returning.\n",
                      lres == OAL_ERR_SEM_ETIMEOUT?"timedout":"failed");
        lres = LIB_FAILURE; // some failure = NOK
      } // if INIT wait failed
    } // else from if SrvCreate() failed
    if (lres == LIB_FAILURE)
    {
      // try to reset init semaphore ownership
      if (OAL_SemaphoreOwnershipSet(apInitSem,
                                    true) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: releasing init semaphore ownership failed. Returning.\n");
      } // if OwnershipSet() failed
    } // if LIB_FAILURE
  }  // else if OwnershipSet() failed
  return lres;
} // GDI_InitFirst()

LIB_RESULT GDI_InitSubseq(bool &arRepeat)
{
  LIB_RESULT lres = LIB_SUCCESS;

  // open general shared memory area - should be prepared already
  if (OAL_SharedMemoryGetByName(&sOalShm,
                                spOalShmName, GDI_SHM_GEN_SIZE) != LIB_SUCCESS)
  {
    VDB_LOG_NOTE("Client: general shared memory open failed. Server probably ended in between.\n");
    arRepeat = true;
    lres = LIB_FAILURE;
  } else
  {
    VDB_LOG_NOTE("Client: general shared memory object opened.\n");

    if (OAL_SharedMemoryGetPointer(&spShm,
                                   sOalShm) != LIB_SUCCESS)
    {
      // try to release resources - no checking anymore
      VDB_LOG_ERROR("Client: getting general shared memory pointer failed. Returning\n");
      lres = LIB_FAILURE;
    } else
    {
      // set server/client stats pointer
      spSrvCliStats = (GDI_SrvCliStats*) spShm;
      // set server/client message buffer pointer - behind SrvCliStats
      spSrvCliMsgBuf = (void*) ((char*) spShm + sizeof(GDI_SrvCliStats));

      // check if server is still alive, otherwise must repeat the init procedure
      // and possibly restart the server - probably not needed now after shm_open
      if (spSrvCliStats->mServerAlive == GDI_SERVER_OFF)
      {
        VDB_LOG_WARNING("Client: Server not alive anymore. Retry init procedure\n");
        lres = LIB_FAILURE;
        arRepeat = true;
      } else
      {
        // update clients count
        if (spSrvCliStats->mClientCnt >= GDI_MAX_CLIENT_CNT)
        {
          VDB_LOG_ERROR("Client: Maximum number of clients reached already.\n");
          lres = LIB_FAILURE;
        } else
        {
          // increase clients' count
          ++spSrvCliStats->mClientCnt;
          ++sgThreadCnt;
          // open image data exchange shared memory area
          if (OAL_SharedMemoryGetByName(&sOalShmIdx,
                                        spOalShmIdxName, GDI_SHM_IMG_DATA_SIZE) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Client: image data exchange shared memory get by name failed.\n");
            lres = LIB_FAILURE;
          } else
          {
            lres = OAL_SharedMemoryGetPointer(&spShmIdx,
                                              sOalShmIdx);
            if (OAL_SharedMemoryGetPointer(&spShmIdx,
                                           sOalShmIdx) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Client: getting pointer to image data exchange shared memory failed.\n");
              lres = LIB_FAILURE;
            } else
            {
              // open server semaphore
              if (OAL_SemaphoreGetByName(&sOalSemSrv,
                                         spSemSrvName) != LIB_SUCCESS)
              {
                VDB_LOG_ERROR("Client: server semaphore get by name failed. Returning.\n");
                lres = LIB_FAILURE;
              } else
              {
                VDB_LOG_NOTE("Client: server semaphore opened.\n");
                // open client semaphore
                if (OAL_SemaphoreGetByName(&sOalSemCli,
                                           spSemCliName) != LIB_SUCCESS)
                {
                  VDB_LOG_ERROR("Client: client semaphore open failed. Returning.\n");
                  lres = LIB_FAILURE;
                } else
                {
                  VDB_LOG_NOTE("Client: client semaphore opened.\n");
                  // open response semaphore
                  if (OAL_SemaphoreGetByName(&sOalSemResp,
                                             spSemRespName) != LIB_SUCCESS)
                  {
                    VDB_LOG_ERROR("Client: response semaphore open failed.\n");
                    lres = LIB_FAILURE;
                  } else
                  {
                    VDB_LOG_NOTE("Client: init finished successfully > returning.\n");
                  } //else from SemaphoreGetByName(response) failed
                  if (lres == LIB_FAILURE)
                  {
                    if (OAL_SemaphoreDelete(sOalSemCli) != LIB_SUCCESS)
                    {
                      VDB_LOG_ERROR("Client: Deleting client semaphore failed.\n");
                    } // if SemaphoreDelete(client) failed
                  } // if LIB_FAILURE
                } // else from SemaphoreGetByName(client) failed
                if (lres == LIB_FAILURE)
                {
                  if (OAL_SemaphoreDelete(sOalSemSrv) != LIB_SUCCESS)
                  {
                    VDB_LOG_ERROR("Client: Deleting server semaphore failed.\n");
                  } // if SemaphoreDelete(server) failed
                } // if LIB_FAILURE
              } // else from SemaphoreGetByName(server) failed
            } // else from SharedMemoryGetPtr(ShmIdx) failed
            if (lres == LIB_FAILURE)
            {
              if (OAL_SharedMemoryDestroy(sOalShmIdx) != LIB_SUCCESS)
              {
                VDB_LOG_ERROR("Client: image data exchange shared memory destroy failed.\n");
              } // if SharedMemoryDestroy() failed
            } // if LIB_FAILURE
          } // else from SharedMemoryGetByName(ShmIdx) failed
          if (lres == LIB_FAILURE)
          {
            --spSrvCliStats->mClientCnt;
            --sgThreadCnt;
          } // if LIB_FAILURE
        } // else from if maximum number of clients reached
      } // else from if GDI_SERVER_OFF
      if (lres == LIB_FAILURE)
      {
        spSrvCliStats = NULL;
        spSrvCliMsgBuf = NULL;
      } // if LIB_FAILURE
    } // else from SharedMemoryGetPointer() failed
    if (lres == LIB_FAILURE)
    {
      if (OAL_SharedMemoryDestroy(sOalShm) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Destroing shared memory failed.\n");
      } // if SharedMemoryDestroy(Shm) failed
    } // if LIB_FAILURE
  } // else from if SharedMemoryGetByName() failed
  return lres;
} // GDI_InitSubseq()

//
// deinitialize GDI for this client
//

LIB_RESULT GDI_Close(uint32_t aThreadId)
{
  LIB_RESULT lres = LIB_SUCCESS;

  // try to let server know, one client deinitialising
  if (GDI_ClientDeInitCall() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: client deinit message call failed %u.\n",
                  aThreadId);
    lres = LIB_FAILURE;
  } // if DeInitCall()

  // wait for Init semaphore - need to access sgThreadCnt
  if (OAL_SemaphoreObtain(sOalSemInit,
                             GDI_SEM_WAIT_TIME) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: Init semaphore timed wait failed %u.\n",
                  aThreadId);
    lres = LIB_FAILURE;
  } else
  {
    VDB_LOG_NOTE("Client: deinit %u. Threads: %u, Clients: %u\n",
                 aThreadId, sgThreadCnt, spSrvCliStats->mClientCnt);
    --sgThreadCnt;
    if (sgThreadCnt == 0)
    {
      // no new thread appeared in between
      // try to release/close resources

#ifdef GDI_SWB
#ifndef GDI_CONTIG
		// initialize shared window buffer server
		GDI_OSWBClient::Close();
#endif // GDI_CONTIG
#endif // GDI_SWB
			
      // unmap general shared memory
      if (OAL_SharedMemoryDestroy(sOalShm) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: general shared memory destroy failed.\n");
        lres = LIB_FAILURE;
      }      // if SharedMemoryDestroy(Shm)

      // reset general shared memory pointers
      spShm = NULL;
      spSrvCliMsgBuf = NULL;
      spSrvCliStats = NULL;

      // destroy image data exchange shared memory
      if (OAL_SharedMemoryDestroy(sOalShmIdx) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: image data exchange shared memory destroy failed.\n");
        lres = LIB_FAILURE;
      } // if SharedMemoryDestrou(ShmIdx)

      // reset image data exchange memory pointer
      spShmIdx = NULL;

      // close client semaphore
      if (OAL_SemaphoreDelete(sOalSemCli) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: client semaphore delete failed.\n");
        lres = LIB_FAILURE;
      } // if SemaphoreDelete(Cli)
      // unlink will be done by server

      // close response semaphore
      if (OAL_SemaphoreDelete(sOalSemResp) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: response semaphore delete failed.\n");
        lres = LIB_FAILURE;
      } // if SemaphoreDelete(Resp)

      // close server semaphore
      if (OAL_SemaphoreDelete(sOalSemSrv) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: server semaphore delete failed.\n");
        lres = LIB_FAILURE;
      } // if SemaphoreDelete(Srv)

      OAL_SEMAPHORE initSem = sOalSemInit;
      sOalSemInit = NULL;

      if (OAL_SemaphoreRelease(initSem) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: failed to release init semaphore.\n");
        lres = LIB_FAILURE;
      } // if SemaphoreRelease(init) failed

      // close init semaphore
      if (OAL_SemaphoreDelete(initSem) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: init semaphore delete failed.\n");
        lres = LIB_FAILURE;
      } // if SemaphoreDelete() failed
    } else
    {
      if (OAL_SemaphoreRelease(sOalSemInit) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Client: failed to release init semaphore.\n");
        lres = LIB_FAILURE;
      } // if SemaphoreRelease(init) failed
    }  // else from if no new thread appeared in between
  } // else from if SemaphoreTimedWait() failed

  return lres;
} //  GDI_Close()

// test server client contact and synchronization
//
LIB_RESULT GDI_SrvCliContactTest(int32_t aData)
{
  LIB_RESULT lres;

  // wait for Client semaphore post
  lres = OAL_SemaphoreObtain(sOalSemCli, 0xFFFFFFFF);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: client semaphore wait failed.\n");
    return LIB_FAILURE; // some failure = NOK
  }
  VDB_LOG_NOTE("Client: got client semaphore.\n");

  // write to shared memory
  char str_buf[GDI_SHM_GEN_SIZE];
  (void) sprintf(str_buf,
                 "Hello server! This is client. Data = %d",
                 aData);
  VDB_LOG_NOTE("Client: writing to shared memory: %s.\n",
               str_buf);
  // use shared memory as message buffer
  GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;

  // write to shared memory
  *msg_type = GDI_MSG_TEST;

  (void) strcpy(((char*) spSrvCliMsgBuf) + sizeof(GDI_MSG_TYPE),
                str_buf);

  // server semaphore post
  lres = OAL_SemaphoreRelease(sOalSemSrv);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: server semaphore post failed.\n");
    // todo: should probably kill server
    // try to release client semaphore for other possibly waiting processes
    if (OAL_SemaphoreRelease(sOalSemCli))
    {
      VDB_LOG_ERROR("Client: client semaphore post failed.\n");
    }
    return LIB_FAILURE;
  }
  VDB_LOG_NOTE("Client: posted server semaphore.\n");

  // server response semaphore wait
  lres = OAL_SemaphoreObtain(sOalSemResp,
                                GDI_SEM_WAIT_TIME);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: response semaphore wait failed.\n");
    // todo: should probably kill server
    // try to release client semaphore for other possibly waiting processes
    if (OAL_SemaphoreRelease(sOalSemCli))
    {
      VDB_LOG_ERROR("Client: client semaphore post failed.\n");
    }
    return LIB_FAILURE;
  }
  VDB_LOG_NOTE("Client: got response semaphore.\n");

  // read data to local client variable
  LIB_RESULT ret_data = *((LIB_RESULT*) spSrvCliMsgBuf);

  // server semaphore post
  lres = OAL_SemaphoreRelease(sOalSemSrv);
  if (lres != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: server semaphore post failed.\n");
    // todo: should probably kill server
    // try to release client semaphore for other possibly waiting processes
    if (OAL_SemaphoreRelease(sOalSemCli))
    {
      VDB_LOG_ERROR("Client: client semaphore post failed.\n");
    }
    return LIB_FAILURE;
  }
  VDB_LOG_NOTE("Client: posted server semaphore.\n");

  return ret_data;
} // GDI_SrvCliContactTest()

static inline void MessageReserve(LIB_RESULT &arLres)
{
  if (OAL_SemaphoreObtain(sOalSemCli,
                             GDI_SEM_WAIT_TIME) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: client semaphore wait failed.\n");
    arLres = LIB_FAILURE;
  } // if OAL_SemaphoreObtain() failed
} // MessageReserve()

static inline void MessageSend(LIB_RESULT &arLres)
{
  // server semaphore post
  if (OAL_SemaphoreRelease(sOalSemSrv) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: server semaphore post failed.\n");
    // try to release client semaphore for other possibly waiting processes
    if (OAL_SemaphoreRelease(sOalSemCli))
    {
      VDB_LOG_ERROR("Client: client semaphore post failed.\n");
    } // if OAL_SemaphoreRelease() failed
    arLres = LIB_FAILURE;
  } // if OAL_SempahoreRelease() failed
} // MessageSend()

static inline void ResponseWait(LIB_RESULT &arLres)
{
  // server response semaphore wait
  if (OAL_SemaphoreObtain(sOalSemResp,
                             GDI_SEM_WAIT_TIME) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("response semaphore wait failed\n");
    // todo: should probably kill server
    // try to release client semaphore for other possibly waiting processes
    if (OAL_SemaphoreRelease(sOalSemCli))
    {
      VDB_LOG_ERROR("Client: client semaphore post failed.\n");
    } // if OAL_SemaphoreRelease() failed
    arLres = LIB_FAILURE;
  } // if OAL_SemaphoreObtain() failed
} // ResponseWait()

static inline void MessageRelease(LIB_RESULT &arLres)
{
  if (OAL_SemaphoreRelease(sOalSemSrv) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: server semaphore post failed\n");
    arLres = LIB_FAILURE; // some failure = NOK
  } // if OAL_SemaphoreRelease() failed
} // MessageRelease()

LIB_RESULT GDI_ClientDeInitCall()
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as message buffer
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;

    // write message type to shared memory
    *msg_type = GDI_MSG_CLIENT_DEINIT;

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        // read response to local client variable
        if (*((LIB_RESULT*) ((int8_t*) spSrvCliMsgBuf + sizeof(GDI_MSG_TYPE)))
            != LIB_SUCCESS)
        {
          lres = LIB_FAILURE;
        } // if server returns failure

        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} //  GDI_ClientDeInitCall()

//
LIB_RESULT GDI_DisplayControlCall(GDI_DISPLAY_ID aDisplayId, bool aEnable)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);

  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_DC *dc_msg = (Gdi_Msg_DC*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_DISPLAY_CONTROL;
    dc_msg->mDisplayId = aDisplayId;
    dc_msg->mEnable = aEnable;
    dc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        // read data to local client varible
        lres = dc_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} //  GDI_DisplyControlCall()

LIB_RESULT GDI_DisplayUpdateControlCall(GDI_DISPLAY_ID aDisplayId, bool aEnable)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);

  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_DC *dc_msg = (Gdi_Msg_DC*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_DISPLAY_CONTROL_UPDATE;
    dc_msg->mDisplayId = aDisplayId;
    dc_msg->mEnable = aEnable;
    dc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        // read data to local client variable
        lres = dc_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} //  GDI_DisplayUpdateControlCall()

LIB_RESULT GDI_DisplayInfoGetCall(GDI_DISPLAY_ID aDisplayId,
                                  GDI_DisplayInfo& aReturnedDisplayInfo)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_DIG *dc_msg = (Gdi_Msg_DIG*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_DISPLAY_INFO_GET;
    dc_msg->mDid = aDisplayId;
    dc_msg->mDinfo.mActive = false;
    dc_msg->mDinfo.mAvailable = false;
    dc_msg->mDinfo.mDisplayType = GDI_DISPLAY_TYPE_INVALID;
    dc_msg->mDinfo.mHeightInPixels = 0;
    dc_msg->mDinfo.mWidthInPixels = 0;
    dc_msg->mDinfo.mRotationInDegreesX = 0;
    dc_msg->mDinfo.mRotationInDegreesY = 0;
    dc_msg->mDinfo.mRotationInDegreesZ = 0;
    dc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        aReturnedDisplayInfo = dc_msg->mDinfo;
        lres = dc_msg->mResult;

        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} // GDI_DisplayInfoGetCall()

LIB_RESULT GDI_DisplayColorFillCall(GDI_DISPLAY_ID const aDisplayId,
                                    GDI_ColorRGBA aColor)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_DCF *dc_msg = (Gdi_Msg_DCF*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_DISPLAY_COLOR_FILL;
    dc_msg->mDid = aDisplayId;
    dc_msg->mColorRGBA = aColor;
    dc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = dc_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} // GDI_DisplayColorFillCall()

LIB_RESULT GDI_DisplayBufferCopyCall(GDI_DISPLAY_ID aDisplayId,
                                     GDI_Image& aGdiImage)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);

  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_DBC *dc_msg = (Gdi_Msg_DBC*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_DISPLAY_BUFFER_COPY;
    dc_msg->mDid = aDisplayId;
    dc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        if (dc_msg->mResult != LIB_SUCCESS)
        {
          MessageRelease(lres);
          lres = LIB_FAILURE;
        } else
        {
          // server ok - continue
          // read frame buffer descriptor
          GDI_Image fbCopy;
          fbCopy.mDesc = dc_msg->mDesc;
          fbCopy.mData = (uint8_t*) spShmIdx;

          // copy data from shared memory to specified buffer
          if (fbCopy.CopyTo(aGdiImage) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("client: failed to copy display data to given image.\n");
            lres = LIB_FAILURE;
          } // if image CopyTo() failed

          // reset mData so that the memory doesn't get freed
          fbCopy.mData = NULL;

          // finalize
          MessageRelease(lres);
        } // else from if server reported failure
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} // GDI_DisplayBufferCopy()

//
LIB_RESULT GDI_WindowCreateCall(const GDI_ImageDescriptor& aGdiImageDescriptor,
                                uint32_t& aReturnedWindowHandle)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WC *wc_msg = (Gdi_Msg_WC*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_CREATE;
    wc_msg->mImgDesc = aGdiImageDescriptor;
    wc_msg->mWindowHandle = 0;
    wc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);

    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        // read data to client
				lres = wc_msg->mResult;
        if (lres == LIB_SUCCESS)
        {
          aReturnedWindowHandle = wc_msg->mWindowHandle;
        } // if server reported success

        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded
  return lres;
} //  GDI_WindowCreateCall()

//
LIB_RESULT GDI_WindowDestroyCall(uint32_t const aWindowHandle)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WD *wd_msg = (Gdi_Msg_WD*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_DESTROY;
    wd_msg->mWindowHandle = aWindowHandle;
    wd_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);

    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = wd_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  return lres;
} //  GDI_WindowDestroyCall()

//
LIB_RESULT GDI_MappingCreateCall(const GDI_W2dMapInfo &arcMappingInfo)
{
  LIB_RESULT lres = LIB_SUCCESS;
  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_MC *mc_msg = (Gdi_Msg_MC*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_MAPPING_CREATE;
    mc_msg->mW2dMapInfo = arcMappingInfo;
    mc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = mc_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  return lres;
} // GDI_MappingCreateCall()

//
LIB_RESULT GDI_MappingDestroyCall(uint32_t aWindowHandle,
                                  GDI_DISPLAY_ID aDisplayId,
                                  uint32_t aLayerId)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_MD *md_msg = (Gdi_Msg_MD*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_MAPPING_DESTROY;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mDisplayId = aDisplayId;
    md_msg->mLayerId = aLayerId;
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = md_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  return lres;
} //  GDI_MappingDestroyCall()

LIB_RESULT GDI_MappingModifyCall(const GDI_W2dMapInfo &arcMappingInfo)
{
  LIB_RESULT lres = LIB_SUCCESS;
  // wait for Client semaphore post
  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_MC *mc_msg = (Gdi_Msg_MC*) (((char*) spSrvCliMsgBuf)
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_MAPPING_MODIFY;
    mc_msg->mW2dMapInfo = arcMappingInfo;
    mc_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = mc_msg->mResult;
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  return lres;
} // GDI_MappingModifyCall()

LIB_RESULT GDI_WindowColorFillCall(uint32_t aWindowHandle,
                                   const GDI_Rectangle &arcWindowRectangle,
                                   GDI_ColorRGBA aColor)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WCF *md_msg = (Gdi_Msg_WCF*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_COLOR_FILL;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mWindowRectangle = arcWindowRectangle;
    md_msg->mColorRGBA = aColor;
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = md_msg->mResult;
        MessageSend(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  return lres;
} // GDI_WindowColorFill

LIB_RESULT GDI_WindowUpdateCall(uint32_t aWindowHandle, const GDI_Image &arcImg)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WU *md_msg = (Gdi_Msg_WU*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_UPDATE;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    // check image size
    size_t isz = arcImg.mDesc.mHeight * arcImg.mDesc.mWidthStep;
    if (isz <= GDI_SHM_IMG_DATA_SIZE)
    {
      // copy data to image data exchange shared memory
      (void) memcpy(spShmIdx,
                    arcImg.mData,
                    isz);

      MessageSend(lres);
      if (lres == LIB_SUCCESS)
      {
        ResponseWait(lres);
        if (lres == LIB_SUCCESS)
        {
          lres = md_msg->mResult;
          MessageRelease(lres);
        } // if ResponseWait() succeeded
      } // if MessageSend() succeeded
    } else
    {
      // image too big
      VDB_LOG_ERROR("Image is too big to be transfered via shared memory.\n");
      MessageRelease(lres);
      lres = LIB_FAILURE;
    } // else from if image size fits to shared memory
  } // if MessageReserve() succeeded
  return lres;
} // GDI_WindowUpdateCall

LIB_RESULT GDI_WindowUpdateRegionCall(uint32_t aWindowHandle,
                                      const GDI_Rectangle &arcWindowRectangle,
                                      const GDI_Image &arcImage)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WUR *md_msg = (Gdi_Msg_WUR*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));

    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_UPDATE_REGION;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mWindowRectangle = arcWindowRectangle;
    // fit rectangle dimensions to image
    md_msg->mWindowRectangle.mWidth = arcImage.mDesc.mWidth;
    md_msg->mWindowRectangle.mHeight = arcImage.mDesc.mHeight;
    md_msg->mImageDesc = GDI_ImageDescriptor(arcImage.mDesc.mWidth,
                                             arcImage.mDesc.mHeight,
                                             arcImage.mDesc.mPixelFormat);
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    // check input image size
    size_t isz = md_msg->mImageDesc.mWidthStep * md_msg->mImageDesc.mHeight;

    if (isz <= GDI_SHM_IMG_DATA_SIZE)
    {
      GDI_Image dst(md_msg->mImageDesc);
      dst.mData = (uint8_t*) spShmIdx;
      // copy data to image data exchange shared memory
      if (LIB_SUCCESS == arcImage.CopyTo(dst))
      {
        MessageSend(lres);
        if (lres == LIB_SUCCESS)
        {
          ResponseWait(lres);
          if (lres == LIB_SUCCESS)
          {
            lres = md_msg->mResult;
            MessageRelease(lres);
          } // if ResponseWait() succeeded
        } // if MessageSend() succeeded
      } else
      {
        VDB_LOG_ERROR("client: Image copy to shared memory failed.\n");
        MessageRelease(lres);
        lres = LIB_FAILURE;
      } // else from image copy succeeded
    } else
    {
      // image too big
      VDB_LOG_ERROR("client: Image is too big to be transfered via shared memory.\n");
      MessageRelease(lres);
      lres = LIB_FAILURE;
    } // else from if image size fits to shared memory
  } // if MessageReserve() succeeded

  return lres;
} // GDI_WindowUpdateRegionCall

#ifdef GDI_SWB
LIB_RESULT GDI_WindowBufferGetCall(uint32_t aWindowHandle,
                                   GDI_Image &arcImage)
{
  LIB_RESULT lres = LIB_SUCCESS;

  bool drop_buffer = false;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WBG *md_msg = (Gdi_Msg_WBG*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));
    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_BUFFER_GET;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = md_msg->mResult;
        if(lres == LIB_SUCCESS)
        {
#ifdef GDI_CONTIG
          uint8_t *virt_add = (uint8_t*) OAL_ContAllocGetVirtual(md_msg->mHwAddress);
					if(virt_add != NULL)
					{
            arcImage.mHwAddress = md_msg->mHwAddress;
            arcImage.mDesc = md_msg->mImageDesc;
            arcImage.mData = virt_add + md_msg->mDataShift;
          }else
          {
            VDB_LOG_ERROR("Client: Getting virtual address failed.\n");
            lres = LIB_FAILURE;
            drop_buffer = true;
          } // else from if GetVirtual() ok
#else
					uint8_t *virt_add = NULL;
					uint32_t img_sz = md_msg->mImageDesc.mHeight * md_msg->mImageDesc.mWidthStep;
					if(GDI_OSWBClient::BufferGet(img_sz, (uint32_t)md_msg->mHwAddress) == LIB_SUCCESS)
					{
						if(GDI_OSWBClient::BufferGet((uint32_t)md_msg->mHwAddress, (void**)&virt_add) == LIB_SUCCESS)
						{
							arcImage.mHwAddress = md_msg->mHwAddress;
							arcImage.mDesc = md_msg->mImageDesc;
							arcImage.mData = virt_add + md_msg->mDataShift;
						} // if get buffer address ok
						else
						{
							VDB_LOG_ERROR("Client: Getting address of shared window buffer failed.\n");
							lres = LIB_FAILURE;
							// unmap the memory and inform server
							drop_buffer = true;							
						} // else from if get buffer address ok
					} // if buffer get region ok
					else
					{
						VDB_LOG_ERROR("Client: Getting shared window buffer failed.\n");
            lres = LIB_FAILURE;
            drop_buffer = true;
					} // else from if buffer get region ok
#endif // ifdef GDI_CONTIG 
        }else
        {
          VDB_LOG_ERROR("Client: Server returned failure.\n");
        } // else from if Server succeeded
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  if(drop_buffer)
  {
    if(GDI_WindowBufferDropCall(aWindowHandle, arcImage.mHwAddress) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Client: Immediate WindowBufferDropCall() failed.\n");
    } // BufferDropCall() failed
  } // if the buffer should be dropped immediately

  return lres;
} // GDI_WindowBufferGetCall()

LIB_RESULT GDI_WindowBufferSetCall(uint32_t aWindowHandle, uint64_t aHwAddress)
{
  LIB_RESULT lres = LIB_SUCCESS;

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WBS *md_msg = (Gdi_Msg_WBS*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));
    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_BUFFER_SET;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mHwAddress = aHwAddress;
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = md_msg->mResult;
        if(lres != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Client: Server returned failure.\n");
          lres = LIB_FAILURE;
        } // if Server failed
        MessageRelease(lres);
      }// if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  return lres;
} // GDI_WindowBufferSetCall(GDI_Image)

LIB_RESULT GDI_WindowBufferDropCall(uint32_t aWindowHandle,
                                   uint64_t aHwAddress)
{
  LIB_RESULT lres = LIB_SUCCESS;
  bool unmap = true;

  // unmap from this address space
#ifdef GDI_CONTIG
  if(!OAL_ContAllocUnmapVirtual(aHwAddress))
  {
    VDB_LOG_ERROR("Client: Unmapping Contiguous memory failed.\n");
    unmap = false;
    // but anyway try to send the message
  } // if UnmapVirtual() failed
#else
	if(GDI_OSWBClient::BufferDrop((uint32_t)aHwAddress) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Client: Unmapping Contiguous memory failed.\n");
    unmap = false;
    // but anyway try to send the message
  } // if BufferDrop() failed
#endif // ifdef GDI_CONTIG

  MessageReserve(lres);
  if (lres == LIB_SUCCESS)
  {
    // use shared memory as display control message
    GDI_MSG_TYPE *msg_type = (GDI_MSG_TYPE*) spSrvCliMsgBuf;
    Gdi_Msg_WBS *md_msg = (Gdi_Msg_WBS*) ((char*) spSrvCliMsgBuf
        + sizeof(GDI_MSG_TYPE));
    // write to shared memory
    *msg_type = GDI_MSG_WINDOW_BUFFER_DROP;
    md_msg->mWindowHandle = aWindowHandle;
    md_msg->mHwAddress = aHwAddress;
    md_msg->mResult = LIB_FAILURE; // just to initialize this

    MessageSend(lres);
    if (lres == LIB_SUCCESS)
    {
      ResponseWait(lres);
      if (lres == LIB_SUCCESS)
      {
        lres = md_msg->mResult;
        if(lres != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Client: Server returned failure.\n");
        } // else from if Server succeeded
        MessageRelease(lres);
      } // if ResponseWait() succeeded
    } // if MessageSend() succeeded
  } // if MessageReserve() succeeded

  if( !unmap )
  {
    lres = LIB_FAILURE;
  } // if unmap failed

  return lres;
} // GDI_WindowBufferDropCall(GDI_Image)
#endif // ifdef GDI_SWB
