#include <stdio.h> 
#include <string.h>
#include <termios.h>
#include<unistd.h>
#include"string.h"
#include"ImageStitching.h"
#include<fcntl.h>
void *TerminalTask(void *ptr)  
{
       	char ch;
	char str1[25];
	char filename[100]= "";
        char *str;
	int tty = open("/dev/tty", O_RDONLY);
	struct termios newt, oldt;
	tcgetattr(tty, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr(tty, TCSANOW, &newt);	
	while(1)
	{
		read(tty, &ch, 2);
		if (ch == 'q') {
		    tcsetattr(tty, TCSANOW, &oldt);
		    fprintf(stderr, "Quit\n", ch);
		} 
		else if(ch=='0')
		{
			console_cmd = 0;
			channel_select++;
			if(channel_select>3)
				channel_select=0;
		}
		else if(ch=='1')
		{
			console_cmd = 1;
			channel_select++;
			if(channel_select>3)
				channel_select=0;
		}
		else if(ch=='2')
			console_cmd = 2;
		else if(ch=='3')
		{
			console_cmd = 3;
			channel_select++;
			if(channel_select>3)
				channel_select=0;
		}
		else if(ch=='4')
		{
			console_cmd = 4;		
			channel_select++;
			if(channel_select>3)
				channel_select=0;
		}
		else if(ch == '5') 
			console_cmd = 5;
		else if(ch == '6') 
			console_cmd = 6;
		else if(ch == '7') 
			console_cmd = 7;
		else if(ch == '8') 
			console_cmd = 8;
		else if(ch == '9') 
			console_cmd = 9;

		else if(ch=='s'){
			*filename = NULL;
		       fprintf(stderr, "[%c]\n", ch);
			snap_number++;
			printf("%d,%d\n",channel_select,snap_number);//for test
			if(channel_select==0)
			str = "/root/front";	
		       else if(channel_select==1)
			str = "/root/back";	
		       else if(channel_select==2)
			str = "/root/left";
		        else if(channel_select==3)
			str = "/root/right";					
			sprintf(str1, "%d" , snap_number);
			strcat(filename,str);
		       strcat(filename,str1);
			strcat(filename,".yuv");	
			printf("%s\n",filename);				
			saveframe(filename , SINGLE_BUFFER, SIG_WIDTH * SIG_HEIGHT * 2, 1);
		}
		else	if (ch == 'e'){
			 //fprintf(stderr, "[%c]\n", ch);
			 printf( " Exit output window !\n");
		}
		else{
		};	
		sleep(1);		
	}
	return 0;
}
