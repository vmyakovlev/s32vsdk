/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
#ifndef T32TREM_H
#define T32TREM_H

#ifndef APEX2_EMULATE
int DEBUG_printf(const char *f, ...);     // variable arguments
int DEBUG_sprintf(const char* s, const char *f, ...);     // variable arguments
#else
#ifdef DEBUG_PRINT
#define DEBUG_printf printf_s
#define DEBUG_sprintf sprintf_s
#else
#define DEBUG_printf printf  //printf_s
#define DEBUG_sprintf sprintf //sprintf_s
#endif
#endif
int T32_file_print_enable(const char* filename, int max_file_size = 1024*100);
int T32_file_print_disable();

#define T32_TERM_O_OPEN_EXISTING   0x00
#define T32_TERM_O_CREATE_TRUNC    0x04
#define T32_TERM_O_CREATE_APPEND   0x08

#define T32_TERM_O_RDONLY          0x00
#define T32_TERM_O_WRONLY          0x02
#define T32_TERM_O_RDWR            0x02

#define T32_TERM_O_ASCII           0x00
#define T32_TERM_O_BINARY          0x01

#define T32_TERM_SEEK_SET 0
#define T32_TERM_SEEK_CUR 1
#define T32_TERM_SEEK_END 2

void T32_Term_PutBlock(const unsigned char * data, int size);
void T32_Term_GetBlock(unsigned char * data, int size);

int T32_Term_Getchar(void);
void T32_Term_Puts(char const* buffer);


/****************************************************************************
 * Function:      fopen
 * Description:   close file handle.
 * Parameters:
 *    char const* fname - file name
 *    int    mode
 *             T32_TERM_O_OPEN_EXISTING   0x00
 *             T32_TERM_O_CREATE_TRUNC    0x04
 *             T32_TERM_O_CREATE_APPEND   0x08
 *
 *             T32_TERM_O_RDONLY          0x00
 *             T32_TERM_O_WRONLY          0x02
 *             T32_TERM_O_RDWR            0x02
 *
 *             T32_TERM_O_ASCII           0x00 
 *             T32_TERM_O_BINARY          0x01
 *
 * Return    file handle 
 *
 ****************************************************************************/
int T32_fopen (char const* fname, int mode); 

/****************************************************************************
 * Function:      fclose
 * Description:   close file handle.
 * Parameters:
 *    int    handle - file handle
 * Return    0 success
 *          -1 failure 
 *
 ****************************************************************************/
int T32_fclose(int handle);

/****************************************************************************
 * Function:      fread
 * Description:   read data from file hndle into buffer
 * Parameters:
 *    const void *buffer - pointer to the array where the read objects are stored
 *    unsigned long size - size of each object in bytes
 *    unsigned long count - the number of the objects to be read
 *    int    handle - file handle
 * Return    number of bytes read otherwise -1 
 *
 ****************************************************************************/
unsigned long T32_fread(void *buffer, unsigned long size, unsigned long count,int handle);

/****************************************************************************
 * Function:      fwrite
 * Description:   write buffer data to file handle 
 * Parameters:
 *    const void *buffer - pointer to the array where the read objects are stored
 *    unsigned long size - size of each object in bytes
 *    unsigned long count - the number of the objects to be read
 *    int    handle - file handle
 * Return         number of bytes written otherwise -1
 *
 ****************************************************************************/
unsigned long T32_fwrite( const void *buffer, unsigned long size, unsigned long count, int handle);

/****************************************************************************
 * Function:      fseek
 * Description:   return current file position by offset and origin.
 * Parameters:
 *    int         file handle
 *    long        offset from spesific origin
 *    int         T32_TERM_SEEK_SET, T32_TERM_SEEK_CUR, T32_TERM_SEEK_END
 * Return         file position
 *
 ****************************************************************************/
int T32_fseek(int handle, long offset, int origin);

/****************************************************************************
 * Function:      ftell
 * Description:   return current file position indicator.
 * Parameters:
 *    int         file handle
 * Return         current file postion :
 *
 ****************************************************************************/
long T32_ftell(int handle);

#endif /* T32TREM_H */
