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
* \file APU_ACF.cpp
* \brief APU_lib I/O data descriptor implementation
* \author Igor Aleksandrowicz
* \version
* \date 10-February-2014
****************************************************************************/


#include <cstring>
#include <climits>
#include <sstream>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include "oal.h"
#include <oal_protected.h>
#include "global_errors.h"
#include <set>
using std::stringstream;

/****************************************************************************
* functions
****************************************************************************/
std::set<void *> allocated_addresses;

void *OAL_MemoryReturnAddress(void *pBuffer, OAL_MEMORY_ACCESS access) {
	if (allocated_addresses.find(pBuffer) == allocated_addresses.end())
		return NULL;
	if (access == ACCESS_PHY || access ==  ACCESS_NCH_NB  || access == ACCESS_CH_WB)
		return pBuffer;
	else 
		return NULL;
}

void *OAL_MemoryReturnAddressPhysical(void *pBuffer, OAL_MEMORY_ACCESS access){
	if (access == ACCESS_PHY || access ==  ACCESS_NCH_NB  || access == ACCESS_CH_WB)
		return pBuffer;
	else 
		return NULL;
}

int32_t OAL_MemoryFree(void* mem) {
	allocated_addresses.erase(mem);
	free(mem);
	return 0;
}

void *OAL_MemoryAllocFlag(uint32_t sz, OAL_MEMORY_FLAG flags){
	void *ptr = malloc(sz);
	if ((ptr != NULL) && (flags & OAL_MEMORY_FLAG_ZERO))
	{
		(void)memset(ptr, 0, sz);
	}

	allocated_addresses.insert(ptr);
	return ptr;
}

void* OAL_MemoryAlloc(uint32_t size)
{
	return OAL_MemoryAllocFlag(size, 0);
}


int32_t OAL_Initialize() { //no use
	return LIB_SUCCESS;
}

int32_t OAL_Deinitialize(){
	return LIB_SUCCESS;
}


void OAL_MemoryFlush(void * /*start*/, uint32_t /*size*/){
	//Just pass
}
void OAL_MemoryFlushAndInvalidate(void * /*start*/, uint32_t /*size*/){
	//Just pass
}
void OAL_MemoryInvalidate(void * /*start*/, uint32_t /*size*/){
	//Just pass
}
void OAL_MemoryFlushAndInvalidateAll (void){
	//Just pass
}

void OAL_MemoryUnmap(void *)
{
}

/////////////////////////////////////////////////////////////////////////
// Function returns total size in bytes being managed by OAL Memory.
/////////////////////////////////////////////////////////////////////////
int64_t OAL_MemorySizeTotal(void)
{
  return -1;
}

/////////////////////////////////////////////////////////////////////////
// Function returns free size in bytes remaining in OAL Memory.
/////////////////////////////////////////////////////////////////////////
int64_t OAL_MemorySizeFree(void)
{
  return -1;
}



//void OAL_MemoryAllocReset(void) {
	// ???
//};

//LIB_RESULT OAL_MemoryAllocConnect(void){
	//???
	//return LIB_SUCCESS;
//}


/*
//TODO: really level 1 cache?
static const size_t CACHELINE_SIZE =
(size_t)sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
static const size_t PAGE_SIZE = (size_t)sysconf(_SC_PAGESIZE);


size_t AlignmentGet(OAL_MEMORY_FLAG aFlags)
{
	size_t ret = 0;

	if ((aFlags & OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)) ==
		OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE))
	{
		ret = CACHELINE_SIZE;
	}
	else if ((aFlags & OAL_MEMORY_FLAG_ALIGN(ALIGN2_PAGE)) ==
		OAL_MEMORY_FLAG_ALIGN(ALIGN2_PAGE))
	{
		ret = PAGE_SIZE;
	}
	else if ((aFlags & OAL_MEMORY_FLAG_ALIGN(BYTE_N)) ==
		OAL_MEMORY_FLAG_ALIGN(BYTE_N))
	{
		size_t alignment = (size_t)((aFlags & 0x00FF0000) >> 16);
		ret = alignment;
	}

	return ret;
}

#ifdef __cplusplus
extern "C" {
#endif

	

	unsigned long OAL_MemoryGetNumAllocations()
	{
		return OAL_ContAllocGetNumAllocations();
	}

	unsigned long OAL_MemoryGetNumMappings()
	{
		return OAL_ContAllocGetNumMappings();
	}




#ifdef __cplusplus
}
#endif

//pthread_mutex_lock(&gCMBRegister.mMutex);
//pthread_mutex_unlock(&gCMBRegister.mMutex);



//CPU cache function
void CleanAndInvalidateDCacheByAddress(char *, char *)
{
	CleanAndInvalidateDCacheAll();
}

//CPU cache function
void InvalidateDCacheByAddress(char *, char *)
{
	CleanAndInvalidateDCacheAll();
}

//CPU cache function
void OAL_MemoryFlush(void *, uint32_t)
{
	CleanAndInvalidateDCacheAll();
}

//CPU cache function
void OAL_MemoryFlushAndInvalidate(void *, uint32_t)
{
	CleanAndInvalidateDCacheAll();
}

//CPU cache function
void OAL_MemoryInvalidate(void *, uint32_t)
{
	CleanAndInvalidateDCacheAll();
}

//CPU cache function
void OAL_MemoryFlushAndInvalidateAll(void)
{
	CleanAndInvalidateDCacheAll();
}




*/
