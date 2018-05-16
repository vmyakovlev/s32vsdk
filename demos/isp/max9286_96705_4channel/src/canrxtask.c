/* 2. 报文过滤接收程序 */
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

void *CANRXTask(void *ptr)
{
	int s, nbytes,cnt,i;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
	struct can_filter rfilter[1];
	cnt=0;
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW); //creat socket
	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr); //definition can0 device
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(s, (struct sockaddr *)&addr, sizeof(addr)); //bind
	//define receive id
	rfilter[0].can_id = 0x11;
	rfilter[0].can_mask = CAN_SFF_MASK;
	//mask
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	while(1)
	{
	     	printf("This is canrxtask loop %d\n",cnt++);
		nbytes = read(s, &frame, sizeof(frame)); //rx msg
		//print rx msg
		if(nbytes > 0)
		{
			printf("ID=0x%X DLC=%d data[0]=0x%X\n", frame.can_id,frame.can_dlc, frame.data[0]);
		}
		for(i=0;i<8;i++)
		{
			printf("0x%X  ", frame.data[i]);canrxbuf[i] = frame.data[i];
		}
			printf("\n");	
		sleep(1);
	}
	close(s);
	return 0;
}

