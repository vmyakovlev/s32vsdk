#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include"ImageStitching.h"

void *CANTXTask(void *ptr)
{     
   #if 1
	int s, nbytes,i,cnt;
	char * str = ptr;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame[2] = {{0}};
	cnt = 0;
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);//creat socket
	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr); //definition can0 
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(s, (struct sockaddr *)&addr, sizeof(addr));//bind can0 
	//the task is only send can message
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
	//creat can message
	frame[0].can_id = 0x135;
	frame[0]. can_dlc = 8;
	for(i=0;i<8;i++)
	frame[0].data[i] = 2*i-1;
	
	frame[1].can_id = 0x246;
	frame[1]. can_dlc = 8;
	for(i=0;i<8;i++)
	frame[1].data[i] = 2*i;
	//send can message loop
	
      while(1)
	{
		nbytes = write(s, &frame[0], sizeof(frame[0])); //send frame[0]
		if(nbytes != sizeof(frame[0]))
		{
			printf("Send Error frame[0]\n!");
			break; //send error and exit
		}
		sleep(1);
		nbytes = write(s, &frame[1], sizeof(frame[1])); //send  frame[1]
		if(nbytes != sizeof(frame[0]))
		{
			printf("Send Error frame[1]\n!");
			break;
		}
		printf("This is cantxtask loop %d\n!",cnt++);
		sleep(1);
	}
	close(s);
	return 0;
	#endif
}




