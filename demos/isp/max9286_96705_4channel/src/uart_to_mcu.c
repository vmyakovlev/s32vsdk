#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "stdlib.h"
#include "stdarg.h"
#include "termios.h"
#include <sys/time.h>
#include "uart_to_mcu.h"

int rader1_alarm_level;
int rader2_alarm_level;
int rader3_alarm_level;
int rader4_alarm_level;
int rader5_alarm_level;
int rader6_alarm_level;
int rader7_alarm_level;
int rader8_alarm_level;
int rader9_alarm_level;
int rader10_alarm_level;
int rader11_alarm_level;
int rader12_alarm_level;

int rader1_alarm_level_temp;
int rader2_alarm_level_temp;
int rader3_alarm_level_temp;
int rader4_alarm_level_temp;
int rader5_alarm_level_temp;
int rader6_alarm_level_temp;
int rader7_alarm_level_temp;
int rader8_alarm_level_temp;
int rader9_alarm_level_temp;
int rader10_alarm_level_temp;
int rader11_alarm_level_temp;
int rader12_alarm_level_temp;

short int parking_distance_start_x;
short int parking_distance_start_y;
short int parking_distance_end_x;
short int parking_distance_end_y;
short int parking_distance_depth;

short int parking_rect_point0_x;   
short int parking_rect_point0_y;
short int parking_rect_point1_x;
short int parking_rect_point1_y;
short int parking_rect_point2_x;
short int parking_rect_point2_y;
short int parking_rect_point3_x;
short int parking_rect_point3_y;


int car_paring_status;//泊车状态

int trail_line1_start_x;
int trail_line1_start_y;
int trail_line1_end_x;
int trail_line1_end_y;
int trail_line2_start_x;
int trail_line2_start_y;
int trail_line2_end_x;
int trail_line2_end_y;
int trail_arc_start_x;
int trail_arc_start_y;
int trail_arc_end_x;
int trail_arc_end_y;
short int trail_arc_radius ;
short int trail_arc_angle;
int direction;  //0x01 --> left   0x02 --> right

short int vehicle_speed;   //车速
short int vehicle_wheel_angle;  //转角 

int parking_mode_select;
int parking_mode_ok;
int parking_mode_ok_flag;

int gear_status_pre;
int gear_status_actual;
int gear_show_flag;

int fd_uart;
unsigned char connect_success_flag;
unsigned char txBuffer[10];//= {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};//0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99
unsigned char rxBuffer[30];
int count;
int heart_beat_rev_flag;
void *Uart_meg_thread(void *t) 
{
	int i;
	int rd_count = 0;
	//fd_set fds;
	//struct timeval tv;
	//tv.tv_sec = 0;
 	// tv.tv_usec = 0;
    struct termios tty_attributes;

    if ((fd_uart = open("/dev/ttyLF1",O_RDWR|O_NOCTTY&~O_NONBLOCK))<0)
	{
        fprintf (stderr,"Open error on %s\n", strerror(errno));
        //exit(EXIT_FAILURE);
    } 
	else 
	{
        tcgetattr(fd_uart, &tty_attributes);
 
        // c_cflag
        // Enable receiver
        tty_attributes.c_cflag |= CREAD;        
 
        // 8 data bit
        tty_attributes.c_cflag |= CS8;          
 
        // c_iflag
        // Ignore framing errors and parity errors. 
        tty_attributes.c_iflag |= IGNPAR;  
		tty_attributes.c_iflag &=~(ICRNL|IXON);
 
        // c_lflag
        // DISABLE canonical mode.
        // Disables the special characters EOF, EOL, EOL2, 
        // ERASE, KILL, LNEXT, REPRINT, STATUS, and WERASE, and buffers by lines.
 
        // DISABLE this: Echo input characters.
        tty_attributes.c_lflag &= ~(ICANON);     
 
        tty_attributes.c_lflag &= ~(ECHO);      
 
        // DISABLE this: If ICANON is also set, the ERASE character erases the preceding input  
        // character, and WERASE erases the preceding word.
        tty_attributes.c_lflag &= ~(ECHOE);     
 
        // DISABLE this: When any of the characters INTR, QUIT, SUSP, or DSUSP are received, generate the corresponding signal. 
        tty_attributes.c_lflag &= ~(ISIG);      
 
        // Minimum number of characters for non-canonical read.
        tty_attributes.c_cc[VMIN]=16;            
 
        // Timeout in deciseconds for non-canonical read.
        tty_attributes.c_cc[VTIME]=0;           
 
        // Set the baud rate
        cfsetospeed(&tty_attributes,B115200);     
        cfsetispeed(&tty_attributes,B115200);
 
        tcsetattr(fd_uart, TCSANOW, &tty_attributes);
	
		while(1)
		{		
			rd_count = read(fd_uart, rxBuffer, sizeof(rxBuffer));
			printf("rd_count = %d\n", rd_count);

          	if (rd_count > 0)
			{
				for (i=0; i<rd_count; i++)
				{
					printf("%x ", rxBuffer[i]);
				}
				printf("\n");

			}
			frame_get_data(rxBuffer, rd_count);
				
			
		}
    }   
 
    close(fd_uart);  

}

void frame_get_data(unsigned char *r_buffer, unsigned char len)
{
	mcu_soc_frame_t	mcu_frame;
	mcu_soc_frame_t	soc_frame;
	unsigned char 	t_buffer;
	
	mcu_frame.f_start = r_buffer[0];
	mcu_frame.id_flag = r_buffer[1];
    mcu_frame.d_len   = r_buffer[2];
	mcu_frame.data	  = &r_buffer[3];
	//mcu_frame.xor_flag= r_buffer[len-2];

	if(mcu_frame.f_start == F_START_MCU)
	{
		printf("id_flag = %x \n", mcu_frame.id_flag);
		
		for (int i=0; i<mcu_frame.d_len; i++)
		{
			printf("%x ", mcu_frame.data[i]);
		}
		printf("\n");	

		switch (mcu_frame.id_flag)
		{
			case ID_HANDSSHAKE_MCU:
				connect_success_flag = 1;
				txBuffer[0] = F_START_SOC;
				txBuffer[1] = ID_HANDSSHAKE_SOC;
				txBuffer[2] = 0;
				txBuffer[3] = 0;
				txBuffer[4] = 0;
				write(fd_uart, txBuffer, 5);
				printf("received:MCU hands shake signal\n");
				break;
			case ID_HEARTBEAT_MCU:
				count = 0;
				heart_beat_rev_flag = 1;
				printf("received:MCU heart beat signal\n");
				break;
			case ID_ULTRASONIC_ALARM:
				rader1_alarm_level  = r_buffer[3]&0x0f;
				rader2_alarm_level  = r_buffer[3]>>4;
				rader3_alarm_level  = r_buffer[4]&0x0f;
				rader4_alarm_level  = r_buffer[4]>>4;
				rader5_alarm_level  = r_buffer[5]&0x0f;
				rader6_alarm_level  = r_buffer[5]>>4;
				rader7_alarm_level  = r_buffer[6]&0x0f;
				rader8_alarm_level  = r_buffer[6]>>4;
				rader9_alarm_level  = r_buffer[7]&0x0f;
				rader10_alarm_level = r_buffer[7]>>4;
				rader11_alarm_level = r_buffer[8]&0x0f;
				rader12_alarm_level = r_buffer[8]>>4;
				printf("received:MCU ultrasonic alarm signal\n");
				break;
			/*
			case ID_PARKING_DISTANCE:
				parking_distance_start_x = 0;
				parking_distance_start_y = 0;
				parking_distance_end_x   = 0;
				parking_distance_end_y   = 0;
				parking_distance_depth   = 0;
				
				parking_distance_start_x = parking_distance_start_x | r_buffer[3];
				parking_distance_start_x = parking_distance_start_x << 8;
				parking_distance_start_x = parking_distance_start_x | r_buffer[4];
				parking_distance_start_y = parking_distance_start_y | r_buffer[5];
				parking_distance_start_y = parking_distance_start_y << 8;
				parking_distance_start_y = parking_distance_start_y | r_buffer[6];
				
				parking_distance_end_x = parking_distance_end_x | r_buffer[7];
				parking_distance_end_x = parking_distance_end_x << 8;
				parking_distance_end_x = parking_distance_end_x | r_buffer[8];
				parking_distance_end_y = parking_distance_end_y | r_buffer[9];
				parking_distance_end_y = parking_distance_end_y << 8;
				parking_distance_end_y = parking_distance_end_y | r_buffer[10];

				parking_distance_depth = parking_distance_depth | r_buffer[11];
				parking_distance_depth = parking_distance_depth << 8;
				parking_distance_depth = parking_distance_depth | r_buffer[12];

				printf("parking_distance_start:x=0x%hx y=0x%hx  end:x=0x%hx y=0x%hx, depth=0x%hx\n",parking_distance_start_x,parking_distance_start_y,parking_distance_end_x,parking_distance_end_y,parking_distance_depth);
				printf("parking_distance_start:x=%hd y=%hd  end:x=%hd y=%hd, depth=%hd\n",parking_distance_start_x,parking_distance_start_y,parking_distance_end_x,parking_distance_end_y,parking_distance_depth);
				printf("received:MCU parking distance signal\n");
				break;
			*/
			case ID_CAR_STATUS:
				car_paring_status =  r_buffer[3];
				printf("received:MCU car status signal\n");
				break;
			case ID_TRAIL_LINI1:
				//trail_line1_start_x = r_buffer[3];
				//trail_line1_start_y = r_buffer[5];
				//trail_line1_end_x   = r_buffer[7];
				//trail_line1_end_y   = r_buffer[9];
				printf("received:MCU trail line1 signal\n");
				break;
			case ID_TRAIL_ARC:
				trail_arc_radius = 0;
				trail_arc_angle  = 0;
				trail_arc_radius  = trail_arc_radius | r_buffer[3];
				trail_arc_radius = trail_arc_radius << 8;
				trail_arc_radius  = trail_arc_radius | r_buffer[4];
				trail_arc_angle  = trail_arc_angle | r_buffer[5];
				trail_arc_angle = trail_arc_angle << 8;
				trail_arc_angle  = trail_arc_angle | r_buffer[6];
				printf("trail_arc_radius = 0x%hx,trail_arc_angle=0x%hx\n",trail_arc_radius,trail_arc_angle);
				printf("trail_arc_radius = 0x%hd,trail_arc_angle=0x%hd\n",trail_arc_radius,trail_arc_angle);
				printf("received:MCU trail arc signal\n");
				break;
			case ID_TRAIL_LINI2:
				trail_line2_start_x = r_buffer[3];
				trail_line2_start_y = r_buffer[5];
				trail_line2_end_x   = r_buffer[7];
				trail_line2_end_y   = r_buffer[9];
				printf("received:MCU trail line2 signal\n");
				break;
			case ID_KEY_MCU:
				direction = r_buffer[3];
				vehicle_wheel_angle = 0;
				vehicle_speed = 0;
				vehicle_wheel_angle = vehicle_wheel_angle | r_buffer[4];
				vehicle_wheel_angle = vehicle_wheel_angle << 8;
				vehicle_wheel_angle = vehicle_wheel_angle | r_buffer[5];
				vehicle_speed = vehicle_speed | r_buffer[6];
				vehicle_speed = vehicle_speed << 8;
				vehicle_speed = vehicle_speed | r_buffer[7];
				printf("vehicle_wheel_angle = 0x%hx    vehicle_speed = 0x%hx\n", vehicle_wheel_angle,vehicle_speed);
				printf("received:MCU key push signal\n");
				break;	
			case ID_GEAR_MCU:
				gear_status_actual = r_buffer[3];
				gear_status_pre = r_buffer[4];
				if( gear_status_pre >0)
					gear_show_flag = 1;
				else
					gear_show_flag = 0;
				printf("gear_status_pre=%d gear_status_actual=%d\n",gear_status_pre,gear_status_actual);
				printf("received:ID_GEAR_MCU signal\n");
				break;
			case ID_PARK_RECT_POINT1:
				parking_rect_point0_x = 0;
				parking_rect_point0_y = 0;
				parking_rect_point1_x = 0;
				parking_rect_point1_y = 0;
				
				
				parking_rect_point0_x = parking_rect_point0_x | r_buffer[3];
				parking_rect_point0_x = parking_rect_point0_x << 8;
				parking_rect_point0_x = parking_rect_point0_x | r_buffer[4];
				parking_rect_point0_y = parking_rect_point0_y | r_buffer[5];
				parking_rect_point0_y = parking_rect_point0_y << 8;
				parking_rect_point0_y = parking_rect_point0_y | r_buffer[6];
				
				parking_rect_point1_x = parking_rect_point1_x | r_buffer[7];
				parking_rect_point1_x = parking_rect_point1_x << 8;
				parking_rect_point1_x = parking_rect_point1_x | r_buffer[8];
				parking_rect_point1_y = parking_rect_point1_y | r_buffer[9];
				parking_rect_point1_y = parking_rect_point1_y << 8;
				parking_rect_point1_y = parking_rect_point1_y | r_buffer[10];

				printf("parking_rect_point0:x=%hx,y=%hx Point1:x=%hx,y=%hx\n",parking_rect_point0_x,parking_rect_point0_y,parking_rect_point1_x,parking_rect_point1_y);
				break;
			case ID_PARK_RECT_POINT2:
				parking_rect_point2_x = 0;
				parking_rect_point2_y = 0;
				parking_rect_point3_x = 0;
				parking_rect_point3_y = 0;

				parking_rect_point2_x = parking_rect_point2_x | r_buffer[3];
				parking_rect_point2_x = parking_rect_point2_x << 8;
				parking_rect_point2_x = parking_rect_point2_x | r_buffer[4];
				parking_rect_point2_y = parking_rect_point2_y | r_buffer[5];
				parking_rect_point2_y = parking_rect_point2_y << 8;
				parking_rect_point2_y = parking_rect_point2_y | r_buffer[6];
				
				parking_rect_point3_x = parking_rect_point3_x | r_buffer[7];
				parking_rect_point3_x = parking_rect_point3_x << 8;
				parking_rect_point3_x = parking_rect_point3_x | r_buffer[8];
				parking_rect_point3_y = parking_rect_point3_y | r_buffer[9];
				parking_rect_point3_y = parking_rect_point3_y << 8;
				parking_rect_point3_y = parking_rect_point3_y | r_buffer[10];
				printf("parking_rect_point2:x=%hx,y=%hx Point3:x=%hx,y=%hx\n",parking_rect_point2_x,parking_rect_point2_y,parking_rect_point3_x,parking_rect_point3_y);
				break;
				break;
			default:
				break;
		}
	}	
	else
	{	
		printf("error:f_start = %x\n", mcu_frame.f_start);
	}
}

void *Uart_TX_thread(void *t) 
{
	static int cnt;
	//static int cnt2;
	
	while(1)
	{
		usleep(10000);//10ms
		if(connect_success_flag == 1)
		{
			
			if(cnt%10 == 0)  //100ms
			{
				txBuffer[0] = F_START_SOC;
				txBuffer[1] = ID_KEY_SOC;
				txBuffer[2] = 0x01;		
				txBuffer[3] = parking_mode_ok;
				txBuffer[4] = 0 - parking_mode_ok;
				write(fd_uart, txBuffer, 5);
				//printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%parking_mode_ok=%d\n",parking_mode_ok);
				if(heart_beat_rev_flag == 1) //
				{
					usleep(20000);//20ms
					txBuffer[0] = F_START_SOC;
					txBuffer[1] = ID_HEARTBEAT_SOC;
					txBuffer[2] = 0;
					txBuffer[3] = 0;
					txBuffer[4] = 0;
					write(fd_uart, txBuffer, 5);
					heart_beat_rev_flag = 0;
				}	
			}
			
			/*
			if(cnt1 == 100) //1s,send heartbest
			{
				txBuffer[0] = F_START_SOC;
				txBuffer[1] = ID_HEARTBEAT_SOC;
				txBuffer[2] = 0;
				txBuffer[3] = 0;
				txBuffer[4] = 0;
				write(fd_uart, txBuffer, 5);
			}
			*/
			cnt++;
			if(cnt>100)
			{
				cnt = 0;
			}
			
			//cnt2 = cnt1+2;
			
			count++;
			if(count>300)  //heartbeat timing
			{	
				printf("connection failed:heart beat overtime 3s\n");
				count = 0;
			}
		}
	}
}
