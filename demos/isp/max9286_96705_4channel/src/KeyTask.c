#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include"ImageStitching.h"
//void keyboard_test()
extern int parking_mode_select;
extern int parking_mode_ok;
extern int parking_mode_ok_flag;
void *KeyTask(void *ptr1)
{
    int count_sw1 = 0;
    int count_sw2 = 0;
    int count_sw3 = 0;
    int count_sw4 = 0;
    int fd=open("/dev/input/event0",O_RDWR);
    if( fd <= 0 )
    {
        printf("Can not open keyboard input file\n");
    }
   printf("This is a button  KeyTask\n");
    struct input_event *event;
    char buf[128] = {0};
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    while(1)
    {
        int ret = select(fd + 1,&rfds, NULL,NULL,NULL);
        if(ret < 0)
            continue;
        if( FD_ISSET(fd, &rfds) )
        {
            int readn = read(fd, buf, sizeof(struct input_event));
            if (readn <= 0)
            {
                printf("uart read error %d\n", readn);
                continue;
            }
            struct input_event *key_event=(struct input_event*)buf;
            if(key_event->type == EV_KEY)      //ok
            {
                if((key_event->code == 59) && !key_event->value)
                {
				cha16++;
				//console_cmd = cha16;
				if(cha16>5)
				cha16 = 0;	
				parking_mode_ok_flag = 1;
				printf("key16--mode: parking_mode_ok_flag=%d\n",parking_mode_ok_flag);	
                        	//printf("This is a button:%d %d\n", key_event->code,key_event->value);
		}
		else if((key_event->code == 60) && !key_event->value) //mode select
		{
				channel_select++;
				if(channel_select>3)
				channel_select = 0;	
			
				if(parking_mode_ok_flag == 0)
					parking_mode_select++;
				if(parking_mode_select>4)
					parking_mode_select = 1;
				
				printf("key17--camera %d  parking_mode_select=%d\n",channel_select,parking_mode_select);
		}
		else if((key_event->code == 61) && !key_event->value) //clean
		{
				parking_mode_select = 5;
				parking_mode_ok_flag = 0;
				printf("key18--no use %d\n");

		}

            }
        }
    }
}

