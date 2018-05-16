/****************************************************************************/
/*                                                                          */
/* file:     sr2rom.c                                                       */
/* content:  S-Records-to-SAMI ROM file format converter                    */
/* format:   ANSI C                                                         */
/* author:   Thomas Lüdeke                                                  */
/* created:  26.01.1999                                                     */
/* modified: 16.02.2001 added width input and types > 2                     */
/*           07.06.2001 added S0,S5,S7-S9 (ignored)                         */
/*                      delete result file if an error occured              */
/*           07.11.2002 added adr = 0 : no fill, stop at last occupied addr */
/*                      added statistics summary                            */
/*                      improved command line parsing                       */
/*                                                                          */
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_RAM_SIZE 128 * 1024 /* 128 K addresses */

#define Prog_Desc   "S-Record to ROM Content File Converter"
#define Prog_Vers   "0.4"

FILE *sfp = 0 ;            /* source-file pointer */
FILE *ofp = 0 ;            /* output-file pointer */
char tmp[256];
char *pattern[] = {"none","zig-zag","zeroes"} ;
char *format[] = {"bin","hex"} ;

long char2int(char c);
void center_print(char *txt,int cp,int sp,int up) ;
void print_usage(char *prog) ;

int main(int argc, char *argv[])
 {
  char c ;
  int adr_len ;
  long len ;
  long dat[MAX_RAM_SIZE] ;
  char msk[MAX_RAM_SIZE / 8] ;
  char mod[MAX_RAM_SIZE / 8] ;
  long adr = 0 ;
  char fill = 1 ;
  char quiet = 0 ;
  long adr_cur = 0 ;
  long adr_max = 0 ;
  long nibbles = 0 ;
  int i, j, k, l ;
  char *p ;
  char typ ;
  int wid = 32 ;
  int form = 0 ;
  char *prog ;
  //int sfn = 0, ofn = 0 ;
  char ofmt = 0 ;

/* remove path from program name */

  p = argv[0] ;
  prog = p ;
  c = *p ;
  while (c != '\0')
   {
    prog = p ;
    while ((c != '\0') && (c != '/'))
     {
      p++ ;
      c = *p ;
     }
    if (c == '/')
     {
      p++ ;
      c = *p ;
     }
   }

/* parse command line */

  i = 1 ;
  j = 0 ;
  while (i < argc)
   {
    if (!strcmp(argv[i],"-s"))                   /* address size */
     {
      i++ ;
      sscanf(argv[i],"%ld",&adr) ;
     }
    else if (!strcmp(argv[i],"-w"))              /* bit width */
     {
      i++ ;
      sscanf(argv[i],"%d",&wid) ;
     }
    else if (!strcmp(argv[i],"-f"))              /* format */
     {
      i++ ;
      for (k = 0; k < sizeof(format)/4; k++)
        if (!strcmp(argv[i],format[k]))
         {
          ofmt = k ;
          break ;
         }
      if (k == sizeof(format)/4)
       {
        printf ("%s: Error - invalid output format '%s'.\n",prog,argv[i]) ;
        print_usage(prog) ;
        return(7) ;
       }
     }
    else if (!strcmp(argv[i],"-p"))              /* fill pattern */
     {
      i++ ;
      for (k = 0; k < sizeof(pattern)/4; k++)
        if (!strcmp(argv[i],pattern[k]))
         {
          fill = k ;
          break ;
         }
      if (k == sizeof(pattern)/4)
       {
        printf ("%s: Error - invalid fill pattern '%s'.\n",prog,argv[i]) ;
        print_usage(prog) ;
        return(6) ;
       }
     }
    else if (!strcmp(argv[i],"-h"))              /* print usage info */
     {
      print_usage(prog) ;
      return(0) ;
     }
    else if (!strcmp(argv[i],"-q"))              /* quiet mode */
      quiet = 1 ;
    else if (!j)                                 /* input file name */
     {
      j++ ;
      //sfn = i ;
      if (!(sfp = fopen(argv[i],"r")))
       {
        printf ("%s: Error - can't open %s for reading.\n",prog,argv[i]) ;
        return(1) ;
       }
     }
    else if (j == 1)                             /* output file name */
     {
      j++ ;
      //ofn = i ;
      if (!(ofp = fopen(argv[i],"w")))
       {
        printf ("%s: Error - can't open %s for writing.\n",prog,argv[i]) ;
        return(2) ;
       }
     }
    else if (j == 2)                             /* address size (old format) */
     {
      j++ ;
      sscanf(argv[i],"%ld",&adr) ;
     }
    else if (j == 3)                             /* bit width (old format) */
     {
      j++ ;
      sscanf(argv[i],"%d",&wid) ;
     }
    else if (j == 4)                             /* format */
     {
      j++ ;
      for (k = 0; k < sizeof(format)/4; k++)
        if (!strcmp(argv[i],format[k]))
         {
          ofmt = k ;
          break ;
         }
      if (k == sizeof(format)/4)
       {
        printf ("%s: Error - invalid output format '%s'.\n",prog,argv[i]) ;
        print_usage(prog) ;
        return(7) ;
       }
     }
    else
     {
      printf ("%s: Error - unknown argument '%s'.\n",prog,argv[i]) ;
      print_usage(prog) ;
      return(4) ;
     }
    i++ ;
   }

  if (adr > MAX_RAM_SIZE)
   {
    printf("%s: Error - maximum memory size (%d) exceeded.\n",prog,MAX_RAM_SIZE) ;
    return(5) ;
   }

  if (!adr)
    fill = 0 ;

  for (i=(wid-1); i; i>>=1)
    form++ ;
  form -= 2 ;

/* generate runtime banner */

  if (!quiet)
   {
    printf("\n") ;
    center_print(prog,24,1,1) ;
    printf("\n") ;
    center_print(Prog_Desc,24,0,0) ;
    sprintf(tmp,"Version %s",Prog_Vers) ;
    center_print(tmp,24,0,0) ;
    sprintf(tmp,"Created 1999-2002 by Thomas Lüdeke") ;
    center_print(tmp,24,0,0) ;
    printf("\n") ;
   }

/* init data with fill pattern (if adr > 0) */

  if (!quiet && fill)
    printf("  - initializing memory with fill pattern\n") ;

  for (i=0; i<MAX_RAM_SIZE/8; i++)
    mod[i] = 0 ;

  for (i=adr; i<MAX_RAM_SIZE/8; i++)
    msk[i] = 0 ;

  j = 0 ;
  k = 0 ;
  if (fill == 1)                    /* zig-zag pattern */
    for (i=0; i<adr; i++)
     {
      dat[i] = 1 << j ;
      msk[i>>3] |= 1 << (i & 7) ;
      if (k)
        if (j)
          j-- ;
        else
         {
          j++ ;
          k = 0 ;
         }
      else
        if (j == (wid-1))
         {
          j-- ;
          k = 1 ;
         }
        else
          j++ ;
     }
  else if (fill == 2)               /* zeroes pattern */
    for (i=0; i<adr; i++)
     {
      dat[i] = 0 ;
      msk[i>>3] |= 1 << (i & 7) ;
     }

/* read srecord */

  if (!quiet)
    printf("  - reading s-record file\n") ;
  c = fgetc(sfp) ;
  while (c != EOF)
   {
    while ((c != 'S') && (c != EOF))
      c = fgetc(sfp) ;
    if (c != EOF)
     {
      c = fgetc(sfp) ;
      typ = c ;
      c = fgetc(sfp) ;
      len = char2int(c) ;
      c = fgetc(sfp) ;
      len =  (len << 4) + char2int(c) ;
      switch (typ)
       {
	case '0': adr_len = 0;  /* ignore S0 records - this are meaningless headers! */
                  len = 0;
                  break;
        case '1': adr_len = 4 ;
                  len = (len - 3) * 2 ;
                  break ;
        case '2': adr_len = 6 ;
                  len = (len - 4) * 2 ;
                  break ;
        case '3': adr_len = 8 ;
                  len = (len - 5) * 2 ;
                  break ;
        case '5': adr_len = 0;  /* ignore S5 records - number of encountered S1-S3 records */
                  len = 0;
                  break;
        case '7':
        case '8':
        case '9': adr_len = 0;  /* termination record - ignored */
                  len = 0;
                  break;
        default:  printf ("%s: Error - unsuported S-record type '%c'.\n",prog,typ) ;
              /* if an error occurs delete the result file ! */
                  fclose (ofp);
                  sprintf (tmp, "rm %s", argv[2]);
                  system (tmp);
                  return(3) ;
       }
      c = fgetc(sfp) ;
      adr_cur = 0 ;
      for (i=0; i< adr_len; i++)
       {
        adr_cur = (adr_cur << 4) + char2int(c) ;
        c = fgetc(sfp) ;
       }
      adr_cur <<= 1 ;
      j = 0 ;
      while ((c != '\n') && (j < len))
       {
        k = adr_cur >> form ;
        l = 4 * ((1 << form) - (adr_cur % (1 << form)) - 1 ) ;
        dat[k] = (dat[k] & ~(0xf << l)) | (char2int(c) << l) ;
        if ((~mod[k>>3] >> (k & 7)) & 1)
          nibbles++ ;
        j++ ;
        c = fgetc(sfp) ;
        msk[k>>3] |= 1 << (k & 7) ;
        mod[k>>3] |= 1 << (k & 7) ;
        adr_cur++ ;
        if (k > adr_max)
          adr_max = k ;
       }
     }
   }
  fclose(sfp) ;
  adr_max++ ;

/* print dat */

  if (!quiet)
    printf("  - writing memory content file\n") ;
  if (!adr)
    adr = adr_max ;
	
/* print file header */
fprintf(ofp, "##############################################################################\n");
fprintf(ofp, "#\n");
fprintf(ofp, "# This is an auto generated File, don't change manually.\n");
fprintf(ofp, "#\n");
fprintf(ofp, "# Freescale Confidential Proprietary\n");
fprintf(ofp, "#\n");
fprintf(ofp, "# Copyright (c) 2014 Freescale Semiconductor;\n");
fprintf(ofp, "# All Rights Reserved\n");
fprintf(ofp, "#\n");
fprintf(ofp, "##############################################################################\n");
fprintf(ofp, "#\n");
fprintf(ofp, "# THIS SOFTWARE IS PROVIDED BY FREESCALE \"AS IS\" AND ANY EXPRESSED OR\n");
fprintf(ofp, "# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES\n");
fprintf(ofp, "# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n");
fprintf(ofp, "# IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,\n");
fprintf(ofp, "# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n");
fprintf(ofp, "# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n");
fprintf(ofp, "# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n");
fprintf(ofp, "# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n");
fprintf(ofp, "# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING\n");
fprintf(ofp, "# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF\n");
fprintf(ofp, "# THE POSSIBILITY OF SUCH DAMAGE.\n");
fprintf(ofp, "#\n");
fprintf(ofp, "##############################################################################\n\n");

fprintf(ofp, "unsinged char kmem_array[] = {\n");
  for (i=0; i<adr; i++)
   {
    if (!ofmt)
     {
	  fprintf(ofp,"0x") ;
      for (j=(wid-1); j>=0; j--)
        if ((msk[i>>3]>>(i&7))&1)
          fprintf(ofp,"%ld",(dat[i]>>j)&1) ;
        else
          fprintf(ofp,"x") ;
     }
    else
     {
      dat[i] &= 0xffffffff >> (32-wid) ;
	  fprintf(ofp,"0x") ;
      for (j=((1<<form)-1); j>=0; j--)
        if ((msk[i>>3]>>(i&7))&1)
          fprintf(ofp,"%lx",(dat[i]>>(4*j))&0xf) ;
        else
          fprintf(ofp,"x") ;
     }
    if (i < (adr-1)) {
	  if ((i+1)%16) {
		fprintf(ofp, ", ") ;
	  }
	  else {
		fprintf(ofp,",\n") ;
	  }
	}
   }
   
  fprintf(ofp, " };");
  fclose(ofp) ;

/* generate statistics */

  if (!quiet)
   {
    printf("\n") ;
    printf("  - Converter Statistics:\n") ;
    printf("    memory size:                : %ld x %d (%ld bytes)\n",adr,wid,adr*wid/8) ;
    printf("    occupied memory size (bytes): %ld (%4.1f%%)\n",nibbles*2,(((float)nibbles)*200.0)/(adr*(wid/8))) ;
    printf("    written memory size (bytes) : %ld (%4.1f%%)\n",adr_max*(wid/8),(((float)adr_max)*100.0)/adr) ;
    printf("    fill pattern                : %s\n",pattern[(int)fill]) ;
    printf("    output format               : %s\n",format[(int)ofmt]) ;
    printf("\n") ;
   }

  return(0) ;
 }

long char2int(char c)
 {
  switch (c)
   {
    case '0' : return(0) ;
    case '1' : return(1) ;
    case '2' : return(2) ;
    case '3' : return(3) ;
    case '4' : return(4) ;
    case '5' : return(5) ;
    case '6' : return(6) ;
    case '7' : return(7) ;
    case '8' : return(8) ;
    case '9' : return(9) ;
    case 'a' :
    case 'A' : return(10) ;
    case 'b' :
    case 'B' : return(11) ;
    case 'c' :
    case 'C' : return(12) ;
    case 'd' :
    case 'D' : return(13) ;
    case 'e' :
    case 'E' : return(14) ;
    case 'f' :
    case 'F' : return(15) ;
    default  : return(0) ;
   }
 }

void center_print(char *txt,int cp,int sp,int up)
 {
  int i,j ;

  for (i=0; i<(cp-((strlen(txt)*(sp+1))/2)); ++i)
    printf(" ") ;
  for (i=0; i<strlen(txt); i++)
   {
    for (j=0; j<sp; j++)
      printf(" ") ;
    if (up)
      printf("%c",toupper(txt[i])) ;
    else
      printf("%c",txt[i]) ;
   }
  printf("\n") ;
 }

void print_usage(char *prog)
 {
  int i ;

  printf("Usage: %s INFILE OUTFILE [ADDR_SIZE] [WIDTH] [FORMAT] [OPTION] ...\n",prog) ;
  printf("Convert s-record to ROM content file.\n\n") ;
  printf("  -s addr_size   size of address space in words (if addr_size is '0', no fill is done)\n") ;
  printf("  -w width       width of word in bits\n") ;
  printf("  -p pattern     fill pattern:\n") ;
  for (i = 0; i < sizeof(pattern)/4; i++)
    printf("                    %s\n",pattern[i]) ;
  printf("  -f format      output format:\n") ;
  for (i = 0; i < sizeof(format)/4; i++)
    printf("                    %s\n",format[i]) ;
  printf("  -q             suppress messages to stdout\n") ;
  printf("  -h             display this help and exit\n") ;
 }
