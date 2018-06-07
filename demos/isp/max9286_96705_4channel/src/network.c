#include <network.h>

int networkstatus = 1;

//**************function declaration******

int net_detect(char* net_name);
unsigned int tcp_receive_srv(int s,unsigned int frame_num);

void* net_status_check(void *t)
{

	while(1)
	{
		usleep(500000);
		net_detect("eth0"); 
	}	
}
	
int net_detect(char* net_name)	
{  
		int skfd = 0;  
		struct ifreq ifr;  
		struct sockaddr_in *pAddr = NULL;  
		skfd = socket(AF_INET, SOCK_DGRAM, 0);	
		if(skfd < 0)  
		{  
			printf("%s:%d Open socket error!\n", __FILE__, __LINE__);  
			return -1;	
		}  
		strcpy(ifr.ifr_name, net_name);  
		if(ioctl(skfd, SIOCGIFFLAGS, &ifr) <0 )  
		{  
			printf("%s:%d IOCTL error!\n", __FILE__, __LINE__);  
			printf("Maybe ethernet inferface %s is not valid!", ifr.ifr_name);	
			close(skfd);  
			return -1;	
		}  
		if(ifr.ifr_flags & IFF_RUNNING)  
		{  
			printf("%s is OK!\n", ifr.ifr_name); 
			networkstatus = 1;
		}  
		else  
		{  
			printf("%s is NO!\n", ifr.ifr_name); 
			networkstatus = 0;
		}  
		if(ioctl(skfd,SIOCGIFADDR,&ifr)<0)	
		{  
			printf("SIOCGIFADDR IOCTL error!\n");  
			close(skfd);  
			return -1;	
		}  
		pAddr = (struct sockaddr_in *)&(ifr.ifr_addr);	
		//printf("ip addr :[%s]\n", inet_ntoa(pAddr->sin_addr));  
		if(ioctl(skfd,SIOCGIFHWADDR,&ifr)<0)  
		{  
			printf("SIOCGIFHWADDR IOCTL error!\n");  
			close(skfd);  
			return -1;	
		}  
		/*printf("mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n",  
			(unsigned char)ifr.ifr_hwaddr.sa_data[0],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[1],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[2],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[3],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[4],  
			(unsigned char)ifr.ifr_hwaddr.sa_data[5]);	*/
		close(skfd);  
		return 0;  
}  

///////////////////////////////////////////////////////////////////////////
void* net_tr_thread(void *t)
{
	
	void * ss;
    int i,j,size,bytes;
    int count,tmp[2];
    unsigned int totalBytes;
    unsigned int send_frame_num;
    unsigned int recv_frame_num_arry[2];
    size=16*1024;
    totalBytes=0;
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT1);
    int server_socket = socket(PF_INET,SOCK_STREAM,0);
	
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }
    { 
      int opt =1;
      setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    }
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT1); 
        exit(1);
    }
    if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }

      //  struct sockaddr_in client_addr;
      //  socklen_t length = sizeof(client_addr);
	printf("net_tr_thread ok!\n");
    while (1) //
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
 
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
           	 printf("Server Accept Failed!\n");
           	 break;
        }
		if (networkstatus == 1)
		{
            i = (int)recv( new_server_socket, (char *)&recv_frame_num_arry[0], 2*sizeof(int), 0 );
    	    send_frame_num=recv_frame_num_arry[0];
   	     	GetNewPicFrame=recv_frame_num_arry[1];
	      	printf("send_frame_num=%d,GetNewPicFrame=%d ,i=%d\n",send_frame_num,GetNewPicFrame,i);
	    	if(GetNewPicFrame==1)
	    	{
			
				GetNewPicFrame=0;
     			memcpy(CAPTURE_MEM, buf_camfront, 1280*720*2);
				memcpy(&CAPTURE_MEM[1280*720*2],buf_camback,1280*720*2);
     			memcpy(&CAPTURE_MEM[1280*720*4],buf_camleft,1280*720*2);
     			memcpy(&CAPTURE_MEM[1280*720*6],buf_camright,1280*720*2);
	    	}
			/*
        	else if(GetNewPicFrame==2)   
	    	{
				GetNewPicFrame=0;
				//stitching_block2();
				//memcpy(CAPTURE_MEM,SVM_BUFFER2,300*480*2);
			}
	    	else if(GetNewPicFrame==3) 
	    	{
				GetNewPicFrame=0;
	        	//Resize_Interpolation_Front(Lut_front_u,remap_lut_front,SINGLE_CAMERA);
				memcpy(CAPTURE_MEM,buf_single_front,360*410*2); //Front
        	}
	    	else if(GetNewPicFrame==4)  
	    	{
				GetNewPicFrame=0;
				memcpy(CAPTURE_MEM,buf_single_back,360*410*2); //Back
			}
	   	    else if(GetNewPicFrame==5)    // Left
	    	{
		 		GetNewPicFrame=0;
	        	//Resize_Interpolation_Front(Lut_front_u,remap_lut_front,SINGLE_CAMERA);
				memcpy(CAPTURE_MEM,buf_single_left,480*410*2);
        	}
	    	else if(GetNewPicFrame==6)  //Right
	    	{
				GetNewPicFrame=0;
				memcpy(CAPTURE_MEM,buf_single_right,480*410*2);
        	}
			else if(GetNewPicFrame==8)  
	    	{
	    		GetNewPicFrame=0;
				//memcpy((unsigned char *)(&CAPTURE_MEM[CAPTURE_OFFSET]), (unsigned char *)(&CAR_LOGO[128*304*2-1024]),1024);
				//memcpy(CAPTURE_MEM,ECU_VERSION,ECU_VERSION_LEN);	
			}
			*/
       	  	for (count = 0; count < send_frame_num; count ++) 
          	{

             	if( i== 2*sizeof(int) )
             	{
           	    	bytes = send( new_server_socket, (char *)&CAPTURE_MEM[size*count], size, 0 );
           	    	totalBytes = totalBytes + bytes;
           	    	if( bytes <= 0 ) 
            	    {
                		printf("send failed\n");
                        return ss;
           	     	}
                  	i = (int)recv( new_server_socket, (char *)&tmp, 2*sizeof(int), 0 );
             	}
	        	 else
             	{
                   printf("send failed\n");
		           return ss;
             	}
  	      	}
         	 printf("Transmitted %d frames of %d bytes for a total of %u\n", count, size, totalBytes);
       }
   }
}
/////////////////////////////////////////////////////////////////////////////
void* net_rv_thread(void *t)
{
	void * ss;
    int ii;
    FILE *fp_app;
    unsigned int data_length;
    unsigned int recv_frame_num;
    unsigned int recv_frame_num_arry[2];
 
    //设置一\u017e鰏ocket地址\u0153峁箂erver_addr,\u017d矸衿鱥nternet地址, 端口
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段内\u017d媲哪谌萑可柚梦?
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT2);
 
    //\u017d\u017d\u0153\u0161用于internet的流协议(TCP)socket,用server_socket\u017d矸衿鱯ocket
    int server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }
    { 
      int opt =1;
      setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    }
     
    //把socket和socket地址\u0153峁沽灯鹄\u017d
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT2); 
        exit(1);
    }
 
    //server_socket用于\u0152嗵?    if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }

    // struct sockaddr_in client_addr;
    // socklen_t length = sizeof(client_addr);

    while (1) //
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
       
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }
        if (networkstatus == 1)
        {  
	       ii = (int)recv( new_server_socket, (char *)&recv_frame_num_arry[0], 2*sizeof(int), 0 );
           recv_frame_num=recv_frame_num_arry[0];
           CaptureNewFrame=recv_frame_num_arry[1];
	       printf("recv_frame_num=%d,CaptureNewFrame=%d ,i=%d\n",recv_frame_num,CaptureNewFrame,ii);

	       if(CaptureNewFrame==5)			//\u4e0b\u8f7d\u5e94\u7528\u7a0b\u5e8f
	       {
          	   data_length = tcp_receive_srv(new_server_socket,recv_frame_num);
	 	       if((fp_app=fopen("/Yulan/sdk/App.zip","w+"))==NULL)
			   {
                	 printf("open App.zip error!\n");
			         return ss;
			   }
         	   if (fwrite(FALSHBURN_MEM,data_length,1,fp_app) != 1)
			   {
               		 printf("write App.zip error!\n");
			   }
          	   fclose(fp_app);
			   system("unzip -o /Yulan/sdk/App.zip -d /Yulan/sdk/");
			   system("mv -f /Yulan/sdk/sdktest /Yulan/bin/");
			   system("mv -f /Yulan/sdk/libsdk.so /lib32/");
			   system("sync");
			   //flag_appload_ok = 1;  // APP load sucess ,set 1
			   
           }   
       	    else if(CaptureNewFrame==6)   //\u4e0b\u8f7d\u5168\u666f\u67e5\u627e\u8868\u3001\u5355\u8def\u67e5\u627e\u8868\u3001\u5012\u8f66\u8f85\u52a9\u7ebf\u3001\u5c0f\u8f66logo
		   {
		
		       data_length = tcp_receive_srv(new_server_socket,recv_frame_num);
	 	       if((fp_app=fopen("/Yulan/sdk/ftemp.bin","w+"))==NULL)
			   {
                	printf("open lut.bin error!\n");
			        return ss;
			   }
         	   if (fwrite(FALSHBURN_MEM,data_length,1,fp_app) != 1)
			   {
               		 printf("write lut.bin error!\n");
			   }
          	  fclose(fp_app);
			  system("sync");
			  //flag_lut_ok =1;	// LUT load sucess ,set 1
           }
			else if(CaptureNewFrame==9)
		   {	
				printf("wyf:load prar.bin\n");
				data_length = tcp_receive_srv(new_server_socket,recv_frame_num);
				if((fp_app=fopen("/Yulan/sdk/Para.bin","w+"))==NULL)
			   {
                	printf("open Para.bin error!\n");
			        return ss;
			   }
				if (fwrite(FALSHBURN_MEM,data_length,1,fp_app) != 1)
			   {
               		 printf("write Para.bin error!\n");
			   }
          	  fclose(fp_app);
			  system("sync");
		   }
       }
    }
}

unsigned int tcp_receive_srv(int s,unsigned int frame_num)
{
    int i,size,bytes,count;
    unsigned int totalBytes;  
    char      pBuf[8192];					
    size=8192;// \u6bcf\u5e27\u957f\u5ea6\u56fa\u5b9a\u4f4d8192
    totalBytes=0;

   for (count = 0; count < frame_num; count++  ) //\u5904\u7406\u6240\u6709\u7684\u63a5\u6536\u5e27
   {
        i = 0;
        while (i < (int)size)
        {
            // \u6709\u6570\u636e\u5728\u52a8\u6001\u8fde\u63a5\u4e0a
            bytes = recv( s, pBuf+i,size-i, 0 );
            // \u5982\u679c\u8fde\u63a5\u7ed3\u675f\u6216\u8005\u51fa\u9519 break \u63a8\u51fa
            if (bytes <= 0) 
	     	{
				printf(" recv error!\n");                	
				break;
	     	}
 	     	i += bytes;
        }
		memcpy(&FALSHBURN_MEM[totalBytes],pBuf,i);
		totalBytes += i;
		//\u56de\u4f20\u4fe1\u606f
		bytes = send( s, (char *)&size, 4, 0 );
   }
    printf ("Received packets of %d bytes for a total of %u bytes \n",size, totalBytes);
	return totalBytes;
}


