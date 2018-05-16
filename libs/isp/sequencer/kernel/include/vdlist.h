/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
* \file     vdlist.h
* \brief    definition of double linked list of void*
* \author   Tomas Babinec
* \version  0.1
* \date     26.6.2015
* \note
****************************************************************************/

#ifndef VDLIST_H
#define VDLIST_H

#ifndef __KERNEL__
  #include <stdint.h>
#endif // #ifndef __KERNEL__

#ifdef __cplusplus 
extern "C" { 
#endif

//****************************************************************************
// types
//****************************************************************************

/****************************************************************************/
/** Double linked list node.
*
* Can contain arbitrary data storred as void*.
*****************************************************************************/
typedef struct VDList_Node
{
  struct VDList_Node *mpPrev;
  struct VDList_Node *mpNext;
  void *mpData;  
} VDList_Node_t;

/****************************************************************************/
/** Double linked list.
*
* Keeps pointers for first&last node and current number of nodes in the list.
*****************************************************************************/
typedef struct VDList
{
  struct VDList_Node *mpHead;
  struct VDList_Node *mpTail;
  uint32_t            mLength;
} VDList_t;

//****************************************************************************

static inline void VDList_NodeInit(struct VDList_Node *apNode, void *apData)
{
  apNode->mpPrev  = NULL;
  apNode->mpNext  = NULL;
  apNode->mpData  = apData;
} // VDList_Init()

//****************************************************************************

static inline void VDList_Init(struct VDList *apList)
{
  apList->mpHead  = NULL;
  apList->mpTail  = NULL;
  apList->mLength = 0;
} // VDList_Init()
  
//****************************************************************************

static inline void VDList_HeadPush(struct VDList *apList, struct VDList_Node *apNode)
{
  if(apList->mpHead == NULL)
  {
    apList->mpTail  = apNode;
    apNode->mpNext  = NULL;    
  } // if list empty
  else
  {
    apNode->mpNext  = apList->mpHead;
    apList->mpHead->mpPrev = apNode;
  } // if list not empty
  apNode->mpPrev  = NULL;
  apList->mpHead  = apNode;
  
  apList->mLength++;
} // VDList_HeadPush()

//****************************************************************************

static inline void VDList_TailPush(struct VDList *apList, struct VDList_Node *apNode)
{
  if(apList->mpTail == NULL)
  {
    apList->mpHead  = apNode;
    apNode->mpPrev  = NULL;    
  } // if list empty
  else
  {
    apNode->mpPrev  = apList->mpTail;
    apList->mpTail->mpNext = apNode;
  } // if list not empty
  apNode->mpNext  = NULL;
  apList->mpTail  = apNode;
  
  apList->mLength++;
} // VDList_TailPush()

//****************************************************************************

static inline struct VDList_Node* VDList_HeadPop(struct VDList *apList)
{
  struct VDList_Node *lpRetNode = NULL;
  
  if(apList->mLength > 0)
  {
    lpRetNode = apList->mpHead;
    apList->mpHead = apList->mpHead->mpNext;
    if(apList->mpHead != NULL)
    {
      apList->mpHead->mpPrev = NULL;
    } // if some nodes left
    else
    {
      apList->mpTail = NULL;
    } // nothing left
    apList->mLength--;
      
    lpRetNode->mpPrev = NULL;
    lpRetNode->mpNext = NULL;    
  }// if list not empty
  return lpRetNode;
} // VDList_HeadPop()

//****************************************************************************

static inline struct VDList_Node* VDList_TailPop(struct VDList *apList)
{
  struct VDList_Node *lpRetNode = NULL;
  
  if(apList->mLength > 0)
  {
    lpRetNode = apList->mpTail;
    apList->mpTail = apList->mpTail->mpPrev;
    if(apList->mpTail != NULL)
    {
      apList->mpTail->mpNext = NULL;
    } // if some nodes left
    else
    {
      apList->mpHead = NULL;
    } // nothing left
    apList->mLength--;
    
    lpRetNode->mpPrev = NULL;
    lpRetNode->mpNext = NULL;    
  }// if list not empty
  return lpRetNode;
} // VDList_TailPop()

//****************************************************************************

static inline void VDList_Rem(struct VDList *apList, struct VDList_Node *apNode)
{  
  if(apList->mLength > 0)
  {
    if(apNode->mpPrev != NULL)
    {
      apNode->mpPrev->mpNext = apNode->mpNext;
    } // if not first node
    else
    {
      apList->mpHead = apNode->mpNext;
    } // else from if not first node
    
    if(apNode->mpNext != NULL)
    {
      apNode->mpNext->mpPrev = apNode->mpPrev;
    } // else from if not last node    
    else
    {
      apList->mpTail = apNode->mpPrev;
    } // if last node    
    
    apList->mLength--;
  }// if list not empty
} // VDList_TailPop()

//****************************************************************************

static inline int32_t VDList_Empty(struct VDList *apList)
{
  return (int32_t)(apList->mpHead == NULL);
} // VDList_Empty()

//****************************************************************************

static inline uint32_t VDList_Length(struct VDList *apList)
{
  return apList->mLength;
} // VDList_Length()

//****************************************************************************


#ifdef __cplusplus 
} 
#endif 

#endif /* VDLIST_H */