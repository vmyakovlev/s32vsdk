
#include "t32term.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __STANDALONE__
#include <oal.h>
#include "asm_gcc_arm64.h"
#endif
#pragma import(__use_no_semihosting)


int T32_Term_Errno;

#define FLAGS_MINUS        (0x01)
#define FLAGS_PLUS         (0x02)
#define FLAGS_SPACE        (0x04)
#define FLAGS_ZERO         (0x08)
#define FLAGS_POUND        (0x10)
#define IS_FLAG_MINUS(a)   (a & FLAGS_MINUS)
#define IS_FLAG_PLUS(a)    (a & FLAGS_PLUS)
#define IS_FLAG_SPACE(a)   (a & FLAGS_SPACE)
#define IS_FLAG_ZERO(a)    (a & FLAGS_ZERO)
#define IS_FLAG_POUND(a)   (a & FLAGS_POUND)

#ifdef  __STANDALONE__

int T32_Term_Getchar(void)
{
   unsigned char imsg[4];

   T32_Term_GetBlock(imsg,1);

   return imsg[0];
}


void T32_Term_Puts(char const* buffer)
{
   int i;

   for (i = 0; buffer[i] ; i++);

   T32_Term_PutBlock((unsigned char *) buffer, i);
}


static void T32_Term_PutPacket(unsigned char * data, int len)
{
   int i;
   unsigned char sum;

   data[-2] = 2;
   data[-1] = len;
   sum = len;
   for ( i = 0 ; i < len ; i++)
      sum += data[i];
   data[len] = sum;
   data[len+1] = 3;
   T32_Term_PutBlock( data-2, len+4 );
}


static int T32_Term_GetPacket(unsigned char * data)
{
   int i, len;
   unsigned char sum;

   T32_Term_GetBlock( data, 2 );
   if (data[0] != 2)
      return -1;
   len = data[1];

   T32_Term_GetBlock( data, len+2 );
   if (data[len+1] != 3)
      return -1;

   sum = len;
   for ( i = 0 ; i < len ; i++)
      sum += data[i];

   if ( sum != data[i] )
      return -1;

   T32_Term_Errno = data[0];

   return len;
}


int T32_Term_OpenFile(char const* fname, int mode)
{
   int i;
   unsigned char msg[256];

   msg[2] = 0x01;
   msg[3] = mode;
   msg[4] = 0;
   for (i = 0; fname[i] ; i++)
      msg[5+i] = fname[i];
   T32_Term_PutPacket( msg+2, i+3 );

   if (T32_Term_GetPacket( msg ) == -1)
      return -1;
   if (msg[0])
      return -1;
   return msg[1];
}


int T32_Term_CloseFile(int handle)
{
   unsigned char msg[256];

   msg[2] = 0x02;
   msg[3] = handle;
   T32_Term_PutPacket( msg+2, 2 );

   if (T32_Term_GetPacket( msg ) == -1)
      return -1;
   if (msg[0])
      return -1;
   return 0;
}


int T32_Term_ReadFile(int handle, char * buffer, int len)
{
   int i, blen, rlen;
   unsigned char msg[256];

   rlen = 0;
   while (len > 0) {
      blen = len;
      if (blen > 240)
         blen = 240;

      msg[2] = 0x03;
      msg[3] = handle;
      msg[4] = blen;
      T32_Term_PutPacket( msg+2, 3 );

      if ((blen = T32_Term_GetPacket( msg )) == -1)
         return -1;
      if (msg[0])
         return -1;

      blen--;
      if (blen == 0)
         break;

      for ( i = 0 ; i < blen ; i++ )
         buffer[i] = msg[i+1];

      buffer += blen;
      len -= blen;
      rlen += blen;
   }

   return rlen;
}


int T32_Term_WriteFile(int handle, char * buffer, int len)
{
   int i, blen, wlen;
   unsigned char msg[256];

   wlen = 0;
   while (len > 0) {
      blen = len;
      if (blen > 240)
         blen = 240;

      msg[2] = 0x04;
      msg[3] = handle;
      for ( i = 0 ; i < blen ; i++ )
         msg[i+4] = buffer[i];
      T32_Term_PutPacket( msg+2, 2+blen );

      if (T32_Term_GetPacket( msg ) == -1)
         return -1;
      if (msg[0])
         return -1;

      blen = msg[1];
      if (blen == 0)
         break;

      buffer += blen;
      len -= blen;
      wlen += blen;
   }

   return wlen;
}


int T32_Term_SeekFile(int handle, long pos)
{
   unsigned char msg[256];

   msg[2] = 0x05;
   msg[3] = handle;
   msg[4] = (unsigned char) (pos);
   msg[5] = (unsigned char) (pos>>8);
   msg[6] = (unsigned char) (pos>>16);
   msg[7] = (unsigned char) (pos>>24);

   T32_Term_PutPacket( msg+2, 6 );

   if (T32_Term_GetPacket( msg ) == -1)
      return -1;
   if (msg[0])
      return -1;

   return ((long)msg[1])|(((long)msg[2])<<8)|(((long)msg[3])<<16)|(((long)msg[4])<<24);
}


long T32_Term_TellFile(int handle)
{
   unsigned char msg[256];

   msg[2] = 0x06;
   msg[3] = handle;

   T32_Term_PutPacket( msg+2, 2 );

   if (T32_Term_GetPacket( msg ) == -1)
      return -1;
   if (msg[0])
      return -1;

   return ((long)msg[1])|(((long)msg[2])<<8)|(((long)msg[3])<<16)|(((long)msg[4])<<24);
}


long T32_Term_TellSizeFile(int handle)
{
   unsigned char msg[256];

   msg[2] = 0x07;
   msg[3] = handle;

   T32_Term_PutPacket( msg+2, 2 );

   if (T32_Term_GetPacket( msg ) == -1)
      return -1;
   if (msg[0])
      return -1;

   return ((long)msg[1])|(((long)msg[2])<<8)|(((long)msg[3])<<16)|(((long)msg[4])<<24);
}


int T32_Term_GetTempName(char * name)
{
   int i, blen;
   unsigned char msg[256];

   msg[2] = 0x08;
   
   T32_Term_PutPacket( msg+2, 1 );

   if ((blen = T32_Term_GetPacket( msg )) == -1)
      return -1;
   if (msg[0])
      return -1;

   blen--;
   
   for ( i = 0 ; i < blen ; i++ )
      name[i] = msg[i+1];
   name[i] = '\0';

   return 1;
}


/****************************************************************************
 * Function:      DEBUG_putchar
 * Description:   This function writes 1 character to a specific output port.
 *                That port is currently a usart on J2210, uart on J2211 
 *                or the Miniwin console.
 * Parameters:
 *    char        character to output to port.
 * Return Value/Updated Parameters:
 *    None
 ****************************************************************************/
void DEBUG_putchar(char Ch)
{
   char aString[2] = {0,};
   aString[0] = Ch;   
   T32_Term_Puts(aString);
}

/****************************************************************************
 * Function:      DEBUG_PutString
 * Description:   Print a character string string.
 * Parameters:
 *    char *      string to output to port.
 * Return Value/Updated Parameters:
 *    None
 ****************************************************************************/
void DEBUG_put_string(char *string)
{
   T32_Term_Puts(string);
}

/****************************************************************************
 *  printf and support routines
 ****************************************************************************/
/****************************************************************************
 * Function:      DEBUG_PutRepChar
 * Description:   Print a character multiple times.
 * Parameters:
 *    char        character to output.
 *    int         number if times to output the character.
 * Return Value/Updated Parameters:
 *    None
 ****************************************************************************/
void DEBUG_PutRepChar(char c, int count)
{
   while (count--)
   {
      DEBUG_putchar(c);
   } // End while.
} // End of the DEBUG_PutRepChar method.
 
/****************************************************************************
 * Function:      DEBUG_PutStringReverse
 * Description:   Print a string in reverse order.
 * Parameters:
 *    char *      character string to send to the output.
 *    int         location with in string to start printing.
 * Return Value/Updated Parameters:
 *    None
 ****************************************************************************/
void DEBUG_PutStringReverse(char *s, int index)
{
   while ((index--) > 0)
   {
      DEBUG_putchar(s[index]);
   }
}


/****************************************************************************
 * Function:      DEBUG_printf
 * Description:   Version of printf. Supports most printf capabilities.
 * Parameters:
 *    char *      sting to print
 *    ...         variable list of arguments.
 * Return Value/Updated Parameters:
 *    None
 ****************************************************************************/
typedef __builtin_va_list va_list;
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)


/****************************************************************************
 * Function:      file_print_enable
 * Description:   enable DEBUG_printf to file out.
 * Parameters:
 *    char *      filename 
 *    int         the maximu size of file 
 * Return file handler 
 *    None
 ****************************************************************************/


   static int   T32_fhandle            = 0;
   static int   T32_file_printf_enable = 0;
   static void *T32_pwrite_string_handle = 0;
   static char *T32_pwrite_string      = 0;
   static int   T32_file_size          = 1024*100;
   static char *T32_pwrite_index       = 0;
   static int   T32_wbyte              = 0;

   int T32_file_print_enable(const char* filename, int max_file_size)
   {
      int result = 0;
      unsigned long writebyte = 0;
      unsigned long readbyte  = 0 ;
      
      if (max_file_size == 0)
      {
         max_file_size = T32_file_size; 
      }
      else
      {
         T32_file_size = max_file_size;
      }   
      T32_pwrite_string_handle = OAL_MemoryAllocFlag(max_file_size, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS|OAL_MEMORY_FLAG_ZERO);
      T32_pwrite_index = T32_pwrite_string = (char*)OAL_MemoryReturnAddress(T32_pwrite_string_handle, ACCESS_CH_WB);
      T32_wbyte = 0;
      if (T32_pwrite_index != 0)
      {
         T32_fhandle = T32_fopen (filename, T32_TERM_O_CREATE_TRUNC | T32_TERM_O_RDWR | T32_TERM_O_ASCII);
         if (T32_fhandle != 0)
         {
            T32_file_printf_enable = 1;
         }
      }
      return T32_fhandle;
   }
   int T32_file_print_disable()
   {
      int result = 0;
      unsigned long writebyte = 0;
      writebyte = T32_fwrite(T32_pwrite_string, T32_wbyte, 1, T32_fhandle);
      result = T32_fclose(T32_fhandle);
      OAL_MemoryFree(T32_pwrite_string_handle);
      T32_file_printf_enable = 0;
   }

int _putchar(int c)
{
   if (T32_file_printf_enable == 1)
   {
      *T32_pwrite_index++ = (char) (c & 0xff);
      T32_wbyte++;
      if (T32_wbyte >= (int) ((float) T32_file_size * 0.9))
      {
         unsigned long writebyte = 0;
         writebyte = T32_fwrite(T32_pwrite_string, T32_wbyte, 1, T32_fhandle);
         T32_wbyte = 0;
         T32_pwrite_index = T32_pwrite_string;
      }
      DEBUG_putchar((char) c);
   }
   else
   {
      DEBUG_putchar((char) c);
   }


   return 0;
}


/********************************************************************/
static int
_debug_print_mknumstr (char *numstr, void *nump, int neg, int radix)
{
   int a,b,c;
   unsigned int ua,ub,uc;

   int nlen;
   char *nstrp;

   nlen = 0;
   nstrp = numstr;
   *nstrp++ = '\0';

   if (neg)
   {
      a = *((int *)nump);
      if (a == 0)
      {
         *nstrp = '0';
         ++nlen;
         goto done;
      }
      while (a != 0)
      {
         b = a / radix;
         c = a - (b * radix);
         if (c < 0)
         {
            c = ~c + 1 + '0';
         }
         else
         {
            c = c + '0';
         }
         a = b;
         *nstrp++ = (char)c;
         ++nlen;
      }
   }
   else
   {
      ua = *((unsigned int *)nump);
      if (ua == 0)
      {
         *nstrp = '0';
         ++nlen;
         goto done;
      }
      while (ua != 0)
      {
         ub = ua / (unsigned int)radix;
         uc = ua - (ub * (unsigned int)radix);
         if (uc < 10)
         {
            uc = uc + '0';
         }
         else
         {
            uc = uc - 10 + 'A';
         }
         ua = ub;
         *nstrp++ = (char)uc;
         ++nlen;
      }
   }
   done:
   return nlen;
}

/********************************************************************/
static void
_pad_zero (int curlen, int field_width)
{
   int i;

   for (i = curlen; i < field_width; i++)
   {
      DEBUG_putchar('0');
   }
}

/********************************************************************/
static void
_pad_space (int curlen, int field_width)
{
   int i;

   for (i = curlen; i < field_width; i++)
   {
      DEBUG_putchar(' ');
   }
}

/********************************************************************/
int
_debug_sprint_fmt (const char *aString, const char *fmt, va_list ap)
{
   /* va_list ap; */
   char *p;
   char *s = (char*) aString;
   int c = 0;

   char vstr[33];
   char *vstrp;
   int vlen = 0;

   int done = 0;
   int count = 0;

   int   flags_used = 0;
   int   field_width = 0;

   int   ival = 0;
   int schar, dschar;
   int *ivalp;
   char *sval;
   int cval = 0;
   unsigned int uval = 0;
   int i = 0;

   /*
    * Start parsing apart the format string and display appropriate
    * formats and data.
    */
   for (p = (char *)fmt; (c = *p); p++)
   {
      /*
       * All formats begin with a '%' marker.  Special chars like
       * '\n' or '\t' are normally converted to the appropriate
       * character by the __compiler__.  Thus, no need for this
       * routine to account for the '\' character.
       */
      if (c != '%')
      {
         /*
          * This needs to be replaced with something like
          * 'out_char()' or call an OS routine.
          */
         *s++ = (c);

         /*
          * By using 'continue', the next iteration of the loop
          * is used, skipping the code that follows.
          */
         continue;
      }

      /*
       * First check for specification modifier flags.
       */
      flags_used = 0;
      done = false;
      while (!done)
      {
         switch (/* c = */ *++p)
         {
            case '-':
               flags_used |= FLAGS_MINUS;
               break;
            case '+':
               flags_used |= FLAGS_PLUS;
               break;
            case ' ':
               flags_used |= FLAGS_SPACE;
               break;
            case '0':
               flags_used |= FLAGS_ZERO;
               break;
            case '#':
               flags_used |= FLAGS_POUND;
               break;
            default:
               /* we've gone one char too far */
               --p;
               done = true;
               break;
         }
      }

      /*
       * Next check for minimum field width.
       */
      field_width = 0;
      done = false;
      while (!done)
      {
         switch (c = *++p)
         {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
               field_width = (field_width * 10) + (c - '0');
               break;
            default:
               /* we've gone one char too far */
               --p;
               done = true;
               break;
         }
      }
      /*
       * update check for specification modifier flags
       */
      
      if (!(IS_FLAG_ZERO(flags_used)) && (field_width != 0))
      {
         flags_used |= FLAGS_SPACE;
      }

      /*
       * Next check for the width and precision field separator.
       */
      if (/* (c = *++p) */ *++p == '.')
      {
         /* precision_used = true; */

         /*
          * Must get precision field width, if present.
          */
         /* precision_width = 0; */
         done = false;
         while (!done)
         {
            switch (/* c = uncomment if used below */ *++p)
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  break;
               default:
                  /* we've gone one char too far */
                  --p;
                  done = true;
                  break;
            }
         }
      }
      else
      {
         /* we've gone one char too far */
         --p;
      }

      /*
       * Check for the length modifier.
       */
      /* length_modifier = 0; */
      switch (/* c = */ *++p)
      {
         case 'h':
            /* length_modifier |= LENMOD_h; */
            break;
         case 'l':
            /* length_modifier |= LENMOD_l; */
            break;
         case 'L':
            /* length_modifier |= LENMOD_L; */
            break;
         default:
            /* we've gone one char too far */
            --p;
            break;
      }

      /*
       * Now we're ready to examine the format.
       */
      switch (c = *++p)
      {
         case 'd':
         case 'i':
            uval = va_arg(ap, int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,true,10);
            vstrp = &vstr[vlen];

            if (ival < 0)
            {
               schar = '-';
               ++vlen;
            }
            else
            {
               if (IS_FLAG_PLUS(flags_used))
               {
                  schar = '+';
                  ++vlen;
               }
               else
               {
                  if (IS_FLAG_SPACE(flags_used))
                  {
                     schar = ' ';
                     ++vlen;
                  }
                  else
                  {
                     schar = 0;
                  }
               }
            }
            dschar = false;
         
            /*
             * do the ZERO pad.
             */
            if (IS_FLAG_ZERO(flags_used))
            {
               if (schar)
                  *s++ = (schar);
               dschar = true;
               for (i = vlen; i < field_width; i++)
               {
                   *s++ = ('0');
               }
               vlen = field_width;
            }
            else
            {
               if (!IS_FLAG_MINUS(flags_used))
               {
                  for (i = vlen; i < field_width; i++)
                  {
                      *s++ = (' ');
                  }
        
                  if (schar)
                     *s++ = (schar);
                  dschar = true;
               }
            }
         
            /* the string was built in reverse order, now display in */
            /* correct order */
            if (!dschar && schar)
            {
               *s++ = (schar);
            }
            goto cont_xd;

         case 'x':
         case 'X':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,&uval,false,16);
            vstrp = &vstr[vlen];

            dschar = false;
            if (IS_FLAG_ZERO(flags_used))
            {
               if (IS_FLAG_POUND(flags_used))
               {
                  *s++ = ('0');
                  *s++ = ('x');
                  /*vlen += 2;*/
                  dschar = true;
               }
               for (i = vlen; i < field_width; i++)
               {
                   *s++ = ('0');
               }
               vlen = field_width;
            }
            else
            {
               if (!IS_FLAG_MINUS(flags_used))
               {
                  if (IS_FLAG_POUND(flags_used))
                  {
                     vlen += 2;
                  }
                  for (i = vlen; i < field_width; i++)
                  {
                      *s++ = (' ');
                  }
                  if (IS_FLAG_POUND(flags_used))
                  {
                     *s++ = ('0');
                     *s++ = ('x');
                     dschar = true;
                  }
               }
            }

            if ((IS_FLAG_POUND(flags_used)) && !dschar)
            {
               *s++ = ('0');
               *s++ = ('x');
               vlen += 2;
            }
            goto cont_xd;

         case 'o':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,8);
            goto cont_u;
         case 'b':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,2);
            goto cont_u;
         case 'p':

         uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,16);
            goto cont_u;
         case 'u':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,10);

            cont_u:
               vstrp = &vstr[vlen];

               if (IS_FLAG_ZERO(flags_used))
               {
                  for (i = vlen; i < field_width; i++)
                  {
                      *s++ = ('0');
                  }
                  vlen = field_width;
               }
               else
               {
                  if (!IS_FLAG_MINUS(flags_used))
                  {
                     for (i = vlen; i < field_width; i++)
                     {
                         *s++ = (' ');
                     }
                  }
               }

            cont_xd:
               while (*vstrp)
                  *s++ = (*vstrp--);

               if (IS_FLAG_MINUS(flags_used))
               {
                  for (i = vlen; i < field_width; i++)
                  {
                      *s++ = (' ');
                  }
               }
            break;

         case 'c':
            cval = va_arg(ap, int);
            *s++ = (cval);
            break;
         case 's':
            sval = va_arg(ap, char *);
            if (sval)
            {
               vlen = strlen(sval);
               if (!IS_FLAG_MINUS(flags_used))
               {
                  for (i = vlen; i < field_width; i++)
                  {
                      *s++ = (' ');
                  }
               }
               while (*sval)
                  *s++ = (*sval++);
               if (IS_FLAG_MINUS(flags_used))
               {
                  for (i = vlen; i < field_width; i++)
                  {
                      *s++ = (' ');
                  }
               }
            }
            break;
         case 'n':
            ivalp = va_arg(ap, int *);
            *ivalp = count;
            break;
         default:
            *s++ = (c);
            break;
      }
   }
   *s = 0;
}
/********************************************************************/
int
_debug_print_fmt (const char *fmt, va_list ap)
{
   /* va_list ap; */
   char *p;
   int c;

   char vstr[33];
   char *vstrp;
   int vlen;

   int done;
   int count = 0;

   int   flags_used;
   int   field_width;

   int   ival;
   int schar, dschar;
   int *ivalp;
   char *sval;
   int cval;
   unsigned int uval;

   /*
    * Start parsing apart the format string and display appropriate
    * formats and data.
    */
   for (p = (char *)fmt; (c = *p); p++)
   {
      /*
       * All formats begin with a '%' marker.  Special chars like
       * '\n' or '\t' are normally converted to the appropriate
       * character by the __compiler__.  Thus, no need for this
       * routine to account for the '\' character.
       */
      if (c != '%')
      {
         /*
          * This needs to be replaced with something like
          * 'out_char()' or call an OS routine.
          */
         _putchar(c);

         /*
          * By using 'continue', the next iteration of the loop
          * is used, skipping the code that follows.
          */
         continue;
      }

      /*
       * First check for specification modifier flags.
       */
      flags_used = 0;
      done = false;
      while (!done)
      {
         switch (/* c = */ *++p)
         {
            case '-':
               flags_used |= FLAGS_MINUS;
               break;
            case '+':
               flags_used |= FLAGS_PLUS;
               break;
            case ' ':
               flags_used |= FLAGS_SPACE;
               break;
            case '0':
               flags_used |= FLAGS_ZERO;
               break;
            case '#':
               flags_used |= FLAGS_POUND;
               break;
            default:
               /* we've gone one char too far */
               --p;
               done = true;
               break;
         }
      }

      /*
       * Next check for minimum field width.
       */
      field_width = 0;
      done = false;
      while (!done)
      {
         switch (c = *++p)
         {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
               field_width = (field_width * 10) + (c - '0');
               break;
            default:
               /* we've gone one char too far */
               --p;
               done = true;
               break;
         }
      }
      /*
       * update check for specification modifier flags
       */
      
      if (!(IS_FLAG_ZERO(flags_used)) && (field_width != 0))
      {
         flags_used |= FLAGS_SPACE;
      }

      /*
       * Next check for the width and precision field separator.
       */
      if (/* (c = *++p) */ *++p == '.')
      {
         /* precision_used = true; */

         /*
          * Must get precision field width, if present.
          */
         /* precision_width = 0; */
         done = false;
         while (!done)
         {
            switch (/* c = uncomment if used below */ *++p)
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  break;
               default:
                  /* we've gone one char too far */
                  --p;
                  done = true;
                  break;
            }
         }
      }
      else
      {
         /* we've gone one char too far */
         --p;
      }

      /*
       * Check for the length modifier.
       */
      /* length_modifier = 0; */
      switch (/* c = */ *++p)
      {
         case 'h':
            /* length_modifier |= LENMOD_h; */
            break;
         case 'l':
            /* length_modifier |= LENMOD_l; */
            break;
         case 'L':
            /* length_modifier |= LENMOD_L; */
            break;
         default:
            /* we've gone one char too far */
            --p;
            break;
      }

      /*
       * Now we're ready to examine the format.
       */
      switch (c = *++p)
      {
         case 'd':
         case 'i':
            uval = va_arg(ap, int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,true,10);
            vstrp = &vstr[vlen];

            if (ival < 0)
            {
               schar = '-';
               ++vlen;
            }
            else
            {
               if (IS_FLAG_PLUS(flags_used))
               {
                  schar = '+';
                  ++vlen;
               }
               else
               {
                  if (IS_FLAG_SPACE(flags_used))
                  {
                     schar = ' ';
                     ++vlen;
                  }
                  else
                  {
                     schar = 0;
                  }
               }
            }
            dschar = false;
         
            /*
             * do the ZERO pad.
             */
            if (IS_FLAG_ZERO(flags_used))
            {
               if (schar)
                  _putchar(schar);
               dschar = true;
         
               _pad_zero(vlen, field_width);
               vlen = field_width;
            }
            else
            {
               if (!IS_FLAG_MINUS(flags_used))
               {
                  _pad_space (vlen, field_width);
         
                  if (schar)
                     _putchar(schar);
                  dschar = true;
               }
            }
         
            /* the string was built in reverse order, now display in */
            /* correct order */
            if (!dschar && schar)
            {
               _putchar(schar);
            }
            goto cont_xd;

         case 'x':
         case 'X':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,&uval,false,16);
            vstrp = &vstr[vlen];

            dschar = false;
            if (IS_FLAG_ZERO(flags_used))
            {
               if (IS_FLAG_POUND(flags_used))
               {
                  _putchar('0');
                  _putchar('x');
                  /*vlen += 2;*/
                  dschar = true;
               }
               _pad_zero (vlen, field_width);
               vlen = field_width;
            }
            else
            {
               if (!IS_FLAG_MINUS(flags_used))
               {
                  if (IS_FLAG_POUND(flags_used))
                  {
                     vlen += 2;
                  }
                  _pad_space (vlen, field_width);
                  if (IS_FLAG_POUND(flags_used))
                  {
                     _putchar('0');
                     _putchar('x');
                     dschar = true;
                  }
               }
            }

            if ((IS_FLAG_POUND(flags_used)) && !dschar)
            {
               _putchar('0');
               _putchar('x');
               vlen += 2;
            }
            goto cont_xd;

         case 'o':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,8);
            goto cont_u;
         case 'b':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,2);
            goto cont_u;
         case 'p':

         uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,16);
            goto cont_u;
         case 'u':
            uval = va_arg(ap, unsigned int);
            vlen = _debug_print_mknumstr(vstr,(void*)&uval,false,10);

            cont_u:
               vstrp = &vstr[vlen];

               if (IS_FLAG_ZERO(flags_used))
               {
                  _pad_zero (vlen, field_width);
                  vlen = field_width;
               }
               else
               {
                  if (!IS_FLAG_MINUS(flags_used))
                  {
                     _pad_space (vlen, field_width);
                  }
               }

            cont_xd:
               while (*vstrp)
                  _putchar(*vstrp--);

               if (IS_FLAG_MINUS(flags_used))
               {
                  _pad_space (vlen, field_width);
               }
            break;

         case 'c':
            cval = va_arg(ap, int);
            _putchar(cval);
            break;
         case 's':
            sval = va_arg(ap, char *);
            if (sval)
            {
               vlen = strlen(sval);
               if (!IS_FLAG_MINUS(flags_used))
               {
                  _pad_space (vlen, field_width);
               }
               while (*sval)
                  _putchar(*sval++);
               if (IS_FLAG_MINUS(flags_used))
               {
                  _pad_space (vlen, field_width);
               }
            }
            break;
         case 'n':
            ivalp = va_arg(ap, int *);
            *ivalp = count;
            break;
         default:
            _putchar(c);
            break;
      }
   }
}

/****************************************************************************
 * Function:      DEBUG_printf
 * Description:   Version of printf. Supports most printf capabilities.
 * Parameters:
 *    char *      sting to print
 *    ...         variable list of arguments. doesn't support floating point
 * Return Value/Updated Parameters:
 *    None
 ****************************************************************************/
int DEBUG_printf(const char* fmt, ...)
{
   va_list ap;
   int rvalue;
   /*
    * Initialize the pointer to the variable length argument list.
    */
   va_start(ap, fmt);
   rvalue = _debug_print_fmt(fmt, ap);
   /*
    * Cleanup the variable length argument list.
    */
   va_end(ap);
   return rvalue;
}

int DEBUG_sprintf(const char * s, const char* fmt, ...)
{
   va_list ap;
   int rvalue;
   /*
    * Initialize the pointer to the variable length argument list.
    */
   va_start(ap, fmt);
   rvalue = _debug_sprint_fmt(s, fmt, ap);
   /*
    * Cleanup the variable length argument list.
    */
   va_end(ap);
   return rvalue;
}

static size_t fposition = 0;

int T32_fopen (char const* fname, int mode) 
{  
   int handle = T32_Term_OpenFile(fname, mode);
   fposition = T32_Term_TellFile(handle);
   return (handle);
}
int T32_fclose(int handle)
{
   return (T32_Term_CloseFile(handle));
}
unsigned long T32_fread(void *buffer, unsigned long size, unsigned long count,int handle)
{
   return (T32_Term_ReadFile(handle, (char *) buffer, (int) (size * count)));
} 

unsigned long T32_fwrite( const void *buffer, unsigned long size, unsigned long count, int handle)
{
   return (T32_Term_WriteFile(handle, (char *) buffer, (int) (size * count)));
}

int T32_fseek(int handle, long offset, int origin)
{
   long pos = 0;
   switch (origin)
   {
      case 0:     // SEEK_SET
         // seek from begining of file position
         pos = T32_Term_SeekFile(handle, 0) - fposition + offset;
         break;
      case 1:     // SEEK_CUR
         // seek from current file position
         pos = offset; 
         break;
      case 2:     // SEEK_END 
         //seek from end offset should be from end of file.
         pos = T32_Term_SeekFile(handle, 0x8000000) + offset; // max file size is 2GB
         break;
      default:
         break;         
   }
   return (T32_Term_SeekFile(handle, pos));
}


/****************************************************************************
 * Function:      ftell
 * Description:   return current file position indicator.
 * Parameters:
 *    int         file handle
 * Return         current file postion :
 *
 ****************************************************************************/
long T32_ftell(int handle )
{
   return (T32_Term_TellFile(handle));
}

#else /// __LINUX__

int T32_file_print_enable(const char* filename, int max_file_size)
{
   return 0;
}
int T32_file_print_disable()
{
   return 0;
}
int DEBUG_printf(const char* fmt, ...)
{
    return 0;
}
int DEBUG_sprintf(const char * s, const char* fmt, ...)
{
   return 0;
}

void T32_Term_PutBlock(const unsigned char * data, int size)
{
   //empty function
}
void T32_Term_GetBlock(unsigned char * data, int size)
{
   //empty function
}

int T32_Term_Getchar(void)
{
   //empty function
}

void T32_Term_Puts(char const* buffer)
{
   //empty function
}

int T32_fopen (char const* fname, int mode)
{
   return -1;
}

int T32_fclose(int handle)
{
   return -1;
}

unsigned long T32_fread(void *buffer, unsigned long size, unsigned long count,int handle)
{
   return -1;   
}

unsigned long T32_fwrite( const void *buffer, unsigned long size, unsigned long count, int handle)
{
   return -1;
}

int T32_fseek(int handle, long offset, int origin)
{
   return 0;
}

long T32_ftell(int handle)
{
   return 0;
}

#endif


