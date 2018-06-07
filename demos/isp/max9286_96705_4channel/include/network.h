#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <string.h>        // for bzero
#include <linux/netlink.h>  
#include <linux/rtnetlink.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>  
#include <linux/if.h> 
#include <pthread.h>
#include <stdio.h>


#define HELLO_WORLD_SERVER_PORT1    1000
#define HELLO_WORLD_SERVER_PORT2    1001
#define LENGTH_OF_LISTEN_QUEUE 20
//******global variable******************
//extern volatile int networkstatus;
//int networkstatus1= 0;

int GetNewPicFrame;
int CaptureNewFrame;
unsigned char CAPTURE_MEM[1280*720*2*4];
char FALSHBURN_MEM[9*1024*1024];

unsigned char* buf_camfront; //[1280*720*2];
unsigned char* buf_camback;  //[1280*720*2];
unsigned char* buf_camright; //[1280*720*2];
unsigned char* buf_camleft;  //[1280*720*2];

char buf_single_front[480*410*2];
char buf_single_right[480*410*2];
char buf_single_back[480*410*2];
char buf_single_left[480*410*2];

#ifdef __cplusplus
extern "C" {
#endif
 
void* net_status_check(void *t);
void* net_tr_thread(void *t);
void* net_rv_thread(void *t);
 


#ifdef __cplusplus
}
#endif


#endif 
