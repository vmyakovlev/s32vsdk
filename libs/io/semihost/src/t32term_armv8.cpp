
/**************************************************************************

   ARM11, Cortex-R, Cortex-A DCC communication variant (DCC3 or DCC method)
   
**************************************************************************/

#define DCC3
#define AARCH64 __a64__    // Your A64/AArch64 compiler define

#ifdef DCC3

//=============================================================================
// DCC3 method

#ifdef AARCH64

int A64ReadMDSCR_EL1()
{
#ifndef __INTEGRITY__
   __asm 
   (
      "MRS x0, MDSCR_EL1"
   );
#endif
}
int A64ReadDBGDTR_EL0()
{
#ifndef __INTEGRITY__
   __asm    
   (
      "MRS x0, DBGDTR_EL0"
   );
#endif
}

int A64WriteDBGDTR_EL0(int ch)
{
#ifndef __INTEGRITY__
   __asm 
   (
      "MSR DBGDTR_EL0, x0"
   );
#endif
}

#endif

volatile void T32_Term_PutBlock(const unsigned char * data, int size)
{
    int             len;
    volatile unsigned long   ch, k;

    while (size > 0) {
   len = size;
   if (len > 3)
       len = 3;
   ch = data[0] | (data[1] << 8) | (data[2]) << 16 | ((len - 1) << 24);
   
#ifdef AARCH64
   do /* Wait for Terminal Ready */
   {
      k = A64ReadMDSCR_EL1();
   } while (k & 0x20000000);  // TXfull
   
   A64WriteDBGDTR_EL0(ch);
#else
   do /* Wait for Terminal Ready */
   {
      __asm
      (
         "MRC   p14, 0, k, c0, c1"
      );
   } while (k & 0x20000000);
   __asm
   (
      "MCR   p14, 0, ch, c0, c5"
   );
#endif
   data += len;
   size -= len;
    }
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
    static int      bufsize = 0;
    static int      bufindex;
    static unsigned char buf[4];
    int             i, j;
    unsigned long   ch;

    while (size > 0) 
    {
      if (!bufsize) 
      {
      
#ifdef AARCH64
          do /* Wait for input byte */
          {
            ch = A64ReadMDSCR_EL1();
          } while (!(ch & 0x40000000));  // RXfull
          
         ch = A64ReadDBGDTR_EL0();
#else
          do /* Wait for input byte */
          {
            __asm
            (
               "MRC   p14, 0, ch, c0, c1"
            );
         } while (!(ch & 0x40000000));
         __asm
         (
            "MRC   p14, 0, ch, c0, c5"
         );
#endif
       
       buf[0] = ch & 0xff;
       buf[1] = (ch >> 8) & 0xff;
       buf[2] = (ch >> 16) & 0xff;
       bufsize = (ch >> 24) + 1;
       bufindex = 0;
   }
   i = bufsize;
   if (i > size)
       i = size;

   for (j = 0; j < i; j++)
       data[j] = buf[bufindex + j];

   bufindex += i;
   bufsize -= i;
   data += i;
   size -= i;
    }
}

#else

//=============================================================================
// DCC method

void T32_Term_PutBlock(const unsigned char * data, int size)
{
   register unsigned int k, len;

   while (size > 0)
   {
      do /* Wait for Terminal Ready */
      {
         __asm
         (
            "MRC   p14, 0, k, c0, c1"
         );
      }while (k & 0x20000000);
      k = *data;
      __asm
      (
         "MCR   p14, 0, k, c0, c5"
      );
      size--;
      data++;
   }
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
   register unsigned int k, len;

   while (size > 0)
   {
      do /* Wait for input byte */
      {
         __asm
         (
            "MRC   p14, 0, k, c0, c1"
         );
      }while (!(k & 0x40000000));
      __asm
      (
         "MRC   p14, 0, k, c0, c5"
      );
      *data = k;
      size--;
      data++;
   }
}

#endif
