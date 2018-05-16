/*
 * ptw32_OLL_lock.h
 *
 * Description:
 * POSIX thread macros related to thread cancellation.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 * 
 *      Contact Email: rpj@callisto.canberra.edu.au
 * 
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef PTW32_OLL_LOCK_H
#define PTW32_OLL_LOCK_H

#include "pthread.h"
#include "sched.h"
#include "implement.h"

/*
 * C-SNZI support
 */
typedef union  ptw32_oll_counter_t_		ptw32_oll_counter_t;
typedef struct ptw32_oll_snziRoot_t_		ptw32_oll_snziRoot_t;
typedef struct ptw32_oll_snziNode_t_		ptw32_oll_snziNode_t;
typedef union  ptw32_oll_snziNodeOrRoot_t_	ptw32_oll_snziNodeOrRoot_t;
typedef struct ptw32_oll_queryResult_t_		ptw32_oll_queryResult_t;
typedef struct ptw32_oll_ticket_t_		ptw32_oll_ticket_t;
typedef struct ptw32_oll_csnzi_t_		ptw32_oll_csnzi_t;

enum
{
  ptw32_archWidth	= sizeof(size_t)*8,
  ptw32_oll_countWidth	= ptw32_archWidth-2
};

#define PTW32_OLL_MAXREADERS (((size_t)2<<(ptw32_oll_countWidth-1))-1)

union ptw32_oll_counter_t_
{
  size_t	word	: ptw32_archWidth;
  struct
  {
    /*
     * This needs to be a single word
     *
     *    ------------------------------------
     *    | STATE |  ROOT  | COUNT (readers) |
     *    ------------------------------------
     *     63 / 31  62 / 30   61 / 29 .. 0
     */
    size_t	count	: ptw32_oll_countWidth;
    size_t	root	: 1;			/* ROOT or NODE */
    size_t	state	: 1;			/* OPEN or CLOSED (root only) */
  } internal;
};

struct ptw32_oll_snziRoot_t_
{
  /*
   * "counter" must be at same offset in both
   * ptw32_oll_snziNode_t and ptw32_oll_snziRoot_t
   */
  ptw32_oll_counter_t	counter;
};

enum
{
  ptw32_oll_snziRoot_open	= 0,
  ptw32_oll_snziRoot_closed	= 1
};

enum
{
  ptw32_oll_snzi_root	= 0,
  ptw32_oll_snzi_node	= 1
};


struct ptw32_oll_queryResult_t_
{
  BOOL	nonZero;
  BOOL	open;
};

union ptw32_oll_snziNodeOrRoot_t_
{
  ptw32_oll_snziRoot_t* rootPtr;
  ptw32_oll_snziNode_t* nodePtr;
};

struct ptw32_oll_snziNode_t_
{
  /* "counter" must be at same offset in both
   * ptw32_oll_snziNode_t and ptw32_oll_snziRoot_t
   */
  ptw32_oll_counter_t		counter;
  ptw32_oll_snziNodeOrRoot_t	parentPtr;
};

struct ptw32_oll_ticket_t_
{
  ptw32_oll_snziNodeOrRoot_t	snziNodeOrRoot;
};

ptw32_oll_ticket_t ptw32_oll_ticket_null = {NULL};

struct ptw32_oll_csnzi_t_
{
  ptw32_oll_snziRoot_t	proxyRoot;
  ptw32_oll_snziNode_t	leafs[];
};

/*
 * FOLL lock support
 */

typedef struct ptw32_foll_node_t_ ptw32_foll_node_t;
typedef struct ptw32_foll_local_t_ ptw32_foll_local_t;
typedef struct ptw32_foll_rwlock_t_ ptw32_foll_rwlock_t;



struct ptw32_foll_node_t_
{
  ptw32_foll_node_t*	qNextPtr;
  ptw32_oll_csnzi_t*	csnziPtr;
  ptw32_foll_node_t*	nextPtr;
  int			kind;
  int			allocState;
  BOOL			spin;
};

struct ptw32_foll_local_t_
{
  ptw32_foll_node_t*	rNodePtr; // Default read node. Immutable
  ptw32_foll_node_t*	wNodePtr; // Write node. Immutable.
  ptw32_foll_node_t*	departFromPtr; // List node we last arrived at.
  ptw32_oll_ticket_t	ticket; // C-SNZI ticket
};

struct ptw32_foll_rwlock_t_
{
  ptw32_foll_node_t*	tailPtr;
  ptw32_foll_node_t*	rNodesPtr; // Head of reader node
};

/* Scalable RW Locks */

typedef struct ptw32_srwl_rwlock_t_ ptw32_srwl_rwlock_t;
typedef struct ptw32_srwl_node_t_ ptw32_srwl_node_t;
typedef struct ptw32_srwl_local_t_ ptw32_srwl_local_t;

enum
{
  ptw32_srwl_reader	= 0,
  ptw32_srwl_writer	= 1
};

enum
{
  ptw32_srwl_free	= 0,
  ptw32_srwl_in_use	= 1
};

struct ptw32_srwl_rwlock_t_
{
  ptw32_srwl_node_t* tailPtr;
  ptw32_srwl_node_t* readerNodePtr;
};

struct ptw32_srwl_node_t_
{
  ptw32_srwl_node_t*	qNextPtr;
  ptw32_oll_csnzi_t*	csnziPtr;
  ptw32_srwl_node_t*	nextReaderPtr;
  int			kind;		/* ptw32_srwl_reader, ptw32_srwl_writer */
  int			allocState;	/* ptw32_srwl_free, ptw32_srwl_in_use */
  BOOL			spin;
};

/*
 * When a ptw32_srwl_local_t is instantiated the "kind" of each of
 * rNode and wNode must be set as appropriate. This is the only
 * time "kind" is set.
 */
struct ptw32_srwl_local_t_
{
  ptw32_srwl_node_t*	rNodePtr;
  ptw32_srwl_node_t*	wNodePtr;
  ptw32_srwl_node_t*	departFromPtr;
  ptw32_oll_ticket_t	ticket;
};
#endif
