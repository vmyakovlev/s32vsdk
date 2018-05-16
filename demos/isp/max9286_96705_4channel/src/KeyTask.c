#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include"ImageStitching.h"

void *KeyTask(void *ptr1)  //zhy  
{

	FILE *p=NULL;
	FILE *p16 =NULL;
///	FILE *p17=NULL;
	char i[] = {0}; 	
	char i16[]={0};
///	char i17[]={0};

	p = fopen("/sys/class/gpio/export","w");
	fprintf(p,"%d",18);
	fclose(p);

	p16=fopen("/sys/class/gpio/export","w");
	fprintf(p16,"%d",16);
	fclose(p16);

	///p17=fopen("/sys/class/gpio/export","w");
	///fprintf(p17,"%d",17);
	///fclose(p17);


	p = fopen("/sys/class/gpio/gpio18/direction","w");
	fprintf(p,"in");
	fclose(p);	
	
	p16 = fopen("/sys/class/gpio/gpio16/direction","w");
	fprintf(p16,"in");
	fclose(p16);	
	
	///p17 = fopen("/sys/class/gpio/gpio17/direction","w");
	///fprintf(p17,"in");
	///fclose(p17);	
	
	while(1)
	{
		p = fopen("/sys/class/gpio/gpio18/value","r");
		fseek(p , 0 , 0);

		p16 = fopen("/sys/class/gpio/gpio16/value","r");
		fseek(p16 , 0 , 0);

	///	p17 = fopen("/sys/class/gpio/gpio17/value","r");
	///	fseek(p17 , 0 , 0);
		
		#if 0
		fscanf(p,"%s",i);
		#else
		fread(i , 1, 1 ,p);
		fread(i16, 1, 1 ,p16);
	///	fread(i17, 1, 1 ,p17);
		#endif

		key = i[0];
		key16 = i16[0];
	///	key17 = i17[0];
		
		if(key ==0x30)
			channel_select++;
		if(channel_select>3)
			channel_select = 0;	

		if(key16 ==0x30)
			cha16++;
		console_cmd = cha16;
		if(cha16>5)
			cha16 = 0;	
		
	///	if(cha17 ==0x30)
	///		cha17++;
	///	if(cha17>3)
	///		cha17 = 0;			
		printf("key16,key18 = %d, %d\n",cha16,channel_select);		
		sleep(1);
		fclose(p);
		fclose(p16);
	///	fclose(p17);
	}
	return 0;

}
