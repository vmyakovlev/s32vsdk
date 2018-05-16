/* $Id: global_table.c 120532 2009-01-29 01:07:20Z afeldman $ */
#include "INTEGRITY.h"
#include "boottable.h"

/* This global table will be filled in during the Integrate phase with	      */
/* information about the AddressSpaces, Tasks, and Objects that are to be     */
/* created.  If you do not plan to use Integrate, you may omit this file from */
/* the kernel, and the boot table code will then not be included.	      */

GlobalTable TheGlobalTable = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

/*
 * If you want to hard code your board's network configuration into the
 * kernel, you can uncomment the following #define and edit the values
 * below to contain the correct values for your board.
 *
 * You may also add a define in the builder options and define the 
 * other parameters as defined below.
 *
 * Note:  If you do uncomment the define (or otherwise define the
 * NetDevs structure), it overrides any saved (i.e. in flash) settings you
 * might have, even if you enter a zero in a field.
 *
 * More information is available in the INTEGRITY Networking Guide, in the
 * chapter "Network Configuration."
 */

/* #define HARD_CODE_NETWORK_CONFIGURATION */

#ifdef HARD_CODE_NETWORK_CONFIGURATION

#define IPAddress(a,b,c,d) ((a)<<24 | (b)<<16 | (c)<<8 | (d))

#ifndef ETHERADDR
#define ETHERADDR 0,0,0,0,0,0
#endif
#ifndef IP1
#define IP1 0
#define IP2 0
#define IP3 0
#define IP4 0
#endif
#ifndef NM1
#define NM1 0
#define NM2 0
#define NM3 0
#define NM4 0
#endif
#ifndef GW1
#define GW1 0
#define GW2 0
#define GW3 0
#define GW4 0
#endif
#ifndef NS1
#define NS1 0
#define NS2 0
#define NS3 0
#define NS4 0
#endif
#ifndef HOSTNAME
#define HOSTNAME "INTEGRITY1"
#endif
/* set the above IP macros to zero if using DHCP */
/* #define NETFLAGS NETINFO_USE_DHCP */
#ifndef NETFLAGS
#define NETFLAGS 0 /* by default, we won't use DHCP */
#endif

/*    NetInfo structure is defined in <INTEGRITY.h>  */
struct NetInfo NetDevs[5] = {
    {
    /* Ethernet Address */
    /* Defining ETHERADDR to 0x80,0x01,0x02,0x03,0x04,0x05 would */
    /* (along with HARD_CODE_NETWORK_CONFIGURATION), set the ethernet */
    /* address.  Remember to escape the commas with backslashes if you */
    /* use the GUI builder to do this. */
    ETHERADDR,

    /* Define the IP address in 4 digits... IP1..IP4 */
    IPAddress(IP1, IP2, IP3, IP4),

    /* Ditto for Gateway GW1..GW4 */
    IPAddress(GW1, GW2, GW3, GW4), /* Gateway */
    /* Ditto for NetMask NM1..NM4 */
    IPAddress(NM1, NM2, NM3, NM4),
    0, /* internal use,  should be zero */
    /* Define the NameServer address in 4 digits... NS1..NS4 */
    IPAddress(NS1, NS2, NS3, NS4),

    /* Hostname of target system */
    HOSTNAME,

    /* NetFlags: 0 or NETINFO_USE_DHCP */
    NETFLAGS

    /* remaining fields uninitialized (0) */
    }
};

/* The following value is used by VME boards using LBP */
int hardcode_board_location = 1;

#endif /* HARD_CODE_NETWORK_CONFIGURATION */
