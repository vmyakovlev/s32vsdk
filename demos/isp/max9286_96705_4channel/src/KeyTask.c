#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include"ImageStitching.h"
//void keyboard_test()
void *KeyTask(void *ptr1)
{
  #if 0
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
            struct input_event *my_event=(struct input_event*)buf;
            if(my_event->type == EV_KEY)
            {
                switch( my_event->code )
                {
                    case 59:
				cha16++;
				console_cmd = cha16;
				if(cha16>5)
				cha16 = 0;	
				printf("key16--mode\n",cha16);	
                        	printf("This is a button:%d %d\n", my_event->code,my_event->value);
                           break;
			  case 60:
				channel_select++;
				if(channel_select>3)
				channel_select = 0;	
				printf("key17--camera %d\n",channel_select);
                       	break;
                    case 61:
				printf("key18--no use %d\n");	
                        break;
                    default:
                        break;

                }
            }
        }
    	}
	#endif
}

