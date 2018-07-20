#ifndef __UART_TO_MCU_H__
#define __UART_TO_MCU_H__



typedef struct {
	unsigned char 	f_start;
	unsigned char	  id_flag;
	unsigned char  	d_len;
	unsigned char	  *data;
	unsigned char	  xor_flag;
}mcu_soc_frame_t;



#define F_START_MCU			    0x10
#define F_START_SOC			    0x01

#define ID_HANDSSHAKE_MCU	    0x10
#define ID_HANDSSHAKE_SOC	    0x01
#define ID_HEARTBEAT_MCU        0x20
#define ID_HEARTBEAT_SOC        0x02
#define ID_ULTRASONIC_ALARM	    0x22
//#define ID_PARKING_DISTANCE     0x26
#define ID_CAR_STATUS		    0x28
#define ID_TRAIL_LINI1          0x2A
#define ID_TRAIL_ARC      	    0x2B
#define ID_TRAIL_LINI2      	0x2C
#define ID_KEY_MCU        		0xC2	
#define ID_KEY_SOC        		0x30
#define ID_GEAR_MCU				0x31
#define ID_PARK_RECT_POINT1		0x26
#define ID_PARK_RECT_POINT2		0x27



#ifdef __cplusplus
extern "C" {
#endif
 void *Uart_meg_thread(void *t);
 void *Uart_TX_thread(void *t) ;


#ifdef __cplusplus
}
#endif



#endif
