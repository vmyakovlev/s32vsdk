#include "config.h"
#include "i2c.h"
#include "HDMI.h"

#ifdef OLD_EVB
  #define HDMI_I2C I2C2
#else  
  #define HDMI_I2C I2C1
#endif // ifdef OLD_EVB

void HDMI_configuration(void) {

  // --------------------------------------------------------------------
  // 1280*720@60Hz, HDMI Output, RGB 4:4:4 24-Bit Input
  // 20131211 Silicon Image
  // ----------------------------------------------------------------------

  // Initial Settings
  // ----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C,0xc7,0x00);	// Program HW TPI mode
  I2C8_TX(HDMI_I2C,0x1e,0x00);// Power up Chip

#ifdef HDMI_RES_1280X720  
  // Program 1280x720@60Hz Resolution, 74.25MHz
  // ----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C, 0x00, 0x01);	// 74.25MHz input clock, Low byte
  I2C8_TX(HDMI_I2C, 0x01, 0x1D);	// 74.25MHz input clock, High byte
  I2C8_TX(HDMI_I2C, 0x02, 0x70);	// 60Hz vertical timing, Low byte
  I2C8_TX(HDMI_I2C, 0x03, 0x17);	// 60Hz vertical timing, High byte
  I2C8_TX(HDMI_I2C, 0x04, 0x72);	// Total 1650 pixel per line, Low byte
  I2C8_TX(HDMI_I2C, 0x05, 0x06);	// Total 1650 pixels per line, High byte
  I2C8_TX(HDMI_I2C, 0x06, 0xEE);	// Total 750 lines per frame, Low byte
  I2C8_TX(HDMI_I2C, 0x07, 0x02);	// Total 750 lines per frame , High byte
#else
#ifdef HDMI_RES_1920X1080    // #ifdef HDMI_RES_1280x720 
  // Program 1920x1080@60Hz Resolution, 148.50MHz
  // ----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C, 0x00, 0x02);  // 148.50MHz input clock, Low byte
  I2C8_TX(HDMI_I2C, 0x01, 0x3A);  // 148.50MHz input clock, High byte
  I2C8_TX(HDMI_I2C, 0x02, 0x70);  // 60Hz vertical timing, Low byte
  I2C8_TX(HDMI_I2C, 0x03, 0x17);  // 60Hz vertical timing, High byte
  I2C8_TX(HDMI_I2C, 0x04, 0x98);  // Total 2200 pixel per line, Low byte
  I2C8_TX(HDMI_I2C, 0x05, 0x08);  // Total 2200 pixel per line, Low byte
  I2C8_TX(HDMI_I2C, 0x06, 0x65);  // Total 1125 lines per frame, Low byte
  I2C8_TX(HDMI_I2C, 0x07, 0x04);  // Total 1125 lines per frame, Low byte
#else 
#ifdef HDMI_RES_1280X1024
  // Program 1280X1024@60Hz VESA Resolution, 108.000MHz
  // ----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C, 0x00, 0x94);  // 109.00MHz input clock, Low byte	0x2A94 => 10900
  I2C8_TX(HDMI_I2C, 0x01, 0x2A);  // 109.00MHz input clock, High byte
  I2C8_TX(HDMI_I2C, 0x02, 0x70);  // 60Hz vertical timing, Low byte		0x1770 => 6000
  I2C8_TX(HDMI_I2C, 0x03, 0x17);  // 60Hz vertical timing, High byte
  I2C8_TX(HDMI_I2C, 0x04, 0xB0);  // Total 1712 pixel per line, Low byte
  I2C8_TX(HDMI_I2C, 0x05, 0x06);  // Total 1712 pixel per line, High byte
  I2C8_TX(HDMI_I2C, 0x06, 0x27);  // Total 1063 lines per frame, Low byte
  I2C8_TX(HDMI_I2C, 0x07, 0x04);  // Total 1063 lines per frame, High byte*/
#else
  // Program 1920x1080@60Hz Resolution, 148.50MHz
  // ----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C, 0x00, 0x02);  // 148.50MHz input clock, Low byte
  I2C8_TX(HDMI_I2C, 0x01, 0x3A);  // 148.50MHz input clock, High byte
  I2C8_TX(HDMI_I2C, 0x02, 0x70);  // 60Hz vertical timing, Low byte
  I2C8_TX(HDMI_I2C, 0x03, 0x17);  // 60Hz vertical timing, High byte
  I2C8_TX(HDMI_I2C, 0x04, 0x98);  // Total 2200 pixel per line, Low byte
  I2C8_TX(HDMI_I2C, 0x05, 0x08);  // Total 2200 pixel per line, Low byte
  I2C8_TX(HDMI_I2C, 0x06, 0x65);  // Total 1125 lines per frame, Low byte
  I2C8_TX(HDMI_I2C, 0x07, 0x04);  // Total 1125 lines per frame, Low byte
#endif // else from #ifdef HDMI_RES_1280X1024
#endif // else from #ifdef HDMI_RES_1920X1080
#endif // else from #ifdef HDMI_RES_1280X720  

  // Setup input/output mode
  // -----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C, 0x08, 0x60);	// 24-bit input, TMDS Clock = Input Clock
  I2C8_TX(HDMI_I2C, 0x09, 0x00);	// Input is RGB
  I2C8_TX(HDMI_I2C, 0x0a, 0x00);	// Output is RGB

  // Setup AVI Infoframe & update input/output mode registers in HW
  // -----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C,0x0C,0x43);// AVI_DBYTE0 = Checksum = 0x43
  I2C8_TX(HDMI_I2C,0x0D,0x00); // AVI_DBYTE1 = 0x00 -- Y=00(RGB), A0=0(act vid indicator OFF), B=00(no bar data), S=00(no scan data)
  I2C8_TX(HDMI_I2C,0x0E,0x28); // AVI_DBYTE2 = 0x28 -- C=00(nodata), M=10(16:9), R=1000(same as aspect ratio)
  I2C8_TX(HDMI_I2C,0x0F,0x0); // AVI_DBYTE3 = 0x00 -- ITC=0(nodata), EC=000(xvYCC601), Q=00(default), SC=00(no known scaling)
  I2C8_TX(HDMI_I2C,0x10,0x4); // AVI_DBYTE4 = 0x04 -- VIC = Code 4, 128HDMI_I2C0p60Hz
  I2C8_TX(HDMI_I2C,0x11,0x0); // AVI_DBYTE5 = 0x00 -- PR=0000(no pixel repetition)
  I2C8_TX(HDMI_I2C,0x12,0x0); // AVI_DBYTE6 = 0x00
  I2C8_TX(HDMI_I2C,0x13,0x0); // AVI_DBYTE7 = 0x00
  I2C8_TX(HDMI_I2C,0x14,0x0); // AVI_DBYTE8 = 0x00
  I2C8_TX(HDMI_I2C,0x15,0x0); // AVI_DBYTE9 = 0x00
  I2C8_TX(HDMI_I2C,0x16,0x0); // AVI_DBYTE10 = 0x00
  I2C8_TX(HDMI_I2C,0x17,0x0); // AVI_DBYTE11 = 0x00
  I2C8_TX(HDMI_I2C,0x18,0x0); // AVI_DBYTE12 = 0x00
  I2C8_TX(HDMI_I2C,0x19,0x0); // AVI_DBYTE13 = 0x00

  // -------------- AUDIO DISABLED -----------------------------------
  I2C8_TX(HDMI_I2C, 0x26, 0x00);	// Audio disabled
  I2C8_TX(HDMI_I2C, 0xBF, 0x02);	// Disable Sending of Audio-Infoframe

  // Setup output mode & Power-on
  // -----------------------------------------------------------------------
  I2C8_TX(HDMI_I2C, 0x1A, 0x01); 	// Output Mode = HDMI, TMDS On
  // -----------------------------------------------------------------------
  // -----------------------------------------------------------------------
}
