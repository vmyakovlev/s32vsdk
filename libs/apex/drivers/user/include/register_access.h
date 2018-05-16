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

#ifndef REGISTERACCESS_H
#define REGISTERACCESS_H

#include <stdint.h>


/*******************************/
#ifdef __GNUC__
/*******************************/


template <typename AccessT, uint32_t Offset, uint32_t Width> class BitField
{
private:

   static const AccessT mask = (1ULL << Width) - 1;
   volatile AccessT raw;

public:

   inline AccessT& blindWrite ( const AccessT& val ) volatile __attribute__((always_inline))
   {
      raw = (val & mask) << Offset;
      return (AccessT&)raw;
   }

   inline AccessT& operator= ( const AccessT& val ) volatile __attribute__((always_inline))
   {
      raw = (raw & ~(mask << Offset)) | ((val & mask) << Offset);
      return (AccessT&)raw;
   }

   inline operator AccessT() const volatile __attribute__((always_inline))
   {
      return (raw >> Offset) & mask;
   }
};


#define reg32read(reg,field) (reg.asBitField.field)

#define reg32write0(reg,val)                 \
      ({                                    \
         reg = (val);                        \
      })


#define reg32write1(reg,field,val)           \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field = (val);           \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write2(reg,field1,val1,         \
                        field2,val2)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write3(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write4(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write5(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write6(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write7(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write8(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7,         \
                        field8,val8)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write9(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7,         \
                        field8,val8,         \
                        field9,val9)         \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         tmp.asBitField.field9 = (val9);         \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write10(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10)      \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         tmp.asBitField.field9 = (val9);         \
         tmp.asBitField.field10 = (val10);       \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write11(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11)      \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         tmp.asBitField.field9 = (val9);         \
         tmp.asBitField.field10 = (val10);       \
         tmp.asBitField.field11 = (val11);       \
         reg.as32BitValue = tmp.as32BitValue;    \
      })

#define reg32write12(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11,      \
                         field12,val12)      \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         tmp.asBitField.field9 = (val9);         \
         tmp.asBitField.field10 = (val10);       \
         tmp.asBitField.field11 = (val11);       \
         tmp.asBitField.field12 = (val12);       \
         reg.as32BitValue = tmp.as32BitValue;    \
      })


#define reg32write13(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11,      \
                         field12,val12,      \
                         field13,val13)      \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         tmp.asBitField.field9 = (val9);         \
         tmp.asBitField.field10 = (val10);       \
         tmp.asBitField.field11 = (val11);       \
         tmp.asBitField.field12 = (val12);       \
         tmp.asBitField.field13 = (val13);       \
         reg.as32BitValue = tmp.as32BitValue;    \
      })


#define reg32write14(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11,      \
                         field12,val12,      \
                         field13,val13,      \
                         field14,val14)      \
      ({                                     \
         typeof(reg) tmp;                        \
         tmp.as32BitValue = reg.as32BitValue;    \
         tmp.asBitField.field1 = (val1);         \
         tmp.asBitField.field2 = (val2);         \
         tmp.asBitField.field3 = (val3);         \
         tmp.asBitField.field4 = (val4);         \
         tmp.asBitField.field5 = (val5);         \
         tmp.asBitField.field6 = (val6);         \
         tmp.asBitField.field7 = (val7);         \
         tmp.asBitField.field8 = (val8);         \
         tmp.asBitField.field9 = (val9);         \
         tmp.asBitField.field10 = (val10);       \
         tmp.asBitField.field11 = (val11);       \
         tmp.asBitField.field12 = (val12);       \
         tmp.asBitField.field13 = (val13);       \
         tmp.asBitField.field14 = (val14);       \
         reg.as32BitValue = tmp.as32BitValue;    \
      })


/*******************************/
#else
/*******************************/


#define reg32read(reg,field) (reg.asBitField.field)

#define reg32write0(reg,val)                 \
      do{                                    \
         reg = (val);                        \
      }while(0)

#define reg32write1(reg,field,val)           \
      do{                                    \
         reg.asBitField.field = (val);       \
      }while(0)

#define reg32write2(reg,field1,val1,         \
                        field2,val2)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
      }while(0)

#define reg32write3(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
      }while(0)

#define reg32write4(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
      }while(0)

#define reg32write5(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
      }while(0)

#define reg32write6(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
      }while(0)

#define reg32write7(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
      }while(0)

#define reg32write8(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7,         \
                        field8,val8)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
      }while(0)

#define reg32write9(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7,         \
                        field8,val8,         \
                        field9,val9)         \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
         reg.asBitField.field9 = (val9);     \
      }while(0)

#define reg32write10(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10)      \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
         reg.asBitField.field9 = (val9);     \
         reg.asBitField.field10 = (val10);   \
      }while(0)

#define reg32write11(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11)      \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
         reg.asBitField.field9 = (val9);     \
         reg.asBitField.field10 = (val10);   \
         reg.asBitField.field11 = (val11);   \
      }while(0)
#define reg32write12(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11,      \
                         field12,val12)      \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
         reg.asBitField.field9 = (val9);     \
         reg.asBitField.field10 = (val10);   \
         reg.asBitField.field11 = (val11);   \
         reg.asBitField.field12 = (val12);   \
      }while(0)


#define reg32write13(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11,      \
                         field12,val12,      \
                         field13,val13)      \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
         reg.asBitField.field9 = (val9);     \
         reg.asBitField.field10 = (val10);   \
         reg.asBitField.field11 = (val11);   \
         reg.asBitField.field12 = (val12);   \
         reg.asBitField.field13 = (val13);   \
      }while(0)



#define reg32write14(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,      \
                         field11,val11,      \
                         field12,val12)      \
                         field13,val13)      \
                         field14,val14)      \
      do{                                    \
         reg.asBitField.field1 = (val1);     \
         reg.asBitField.field2 = (val2);     \
         reg.asBitField.field3 = (val3);     \
         reg.asBitField.field4 = (val4);     \
         reg.asBitField.field5 = (val5);     \
         reg.asBitField.field6 = (val6);     \
         reg.asBitField.field7 = (val7);     \
         reg.asBitField.field8 = (val8);     \
         reg.asBitField.field9 = (val9);     \
         reg.asBitField.field10 = (val10);   \
         reg.asBitField.field11 = (val11);   \
         reg.asBitField.field12 = (val12);   \
         reg.asBitField.field13 = (val13);   \
         reg.asBitField.field14 = (val14);   \
      }while(0)


/*
#define reg32write2(reg,field1,val1,         \
                        field2,val2)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write3(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write4(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write5(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write6(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write7(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  tmp.asBitField.field7 = (val7);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write8(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7,         \
                        field8,val8)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  tmp.asBitField.field7 = (val7);            \
  tmp.asBitField.field8 = (val8);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write9(reg,field1,val1,         \
                        field2,val2,         \
                        field3,val3,         \
                        field4,val4,         \
                        field5,val5,         \
                        field6,val6,         \
                        field7,val7,         \
                        field8,val8,         \
                        field9,val9)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  tmp.asBitField.field7 = (val7);            \
  tmp.asBitField.field8 = (val8);            \
  tmp.asBitField.field9 = (val9);            \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write10(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  tmp.asBitField.field7 = (val7);            \
  tmp.asBitField.field8 = (val8);            \
  tmp.asBitField.field9 = (val9);            \
  tmp.asBitField.field10 = (val10);          \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write11(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,         \
                         field11,val11)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  tmp.asBitField.field7 = (val7);            \
  tmp.asBitField.field8 = (val8);            \
  tmp.asBitField.field9 = (val9);            \
  tmp.asBitField.field10 = (val10);          \
  tmp.asBitField.field11 = (val11);          \
  reg.as32BitValue = tmp.as32BitValue;       \
})

#define reg32write12(reg,field1,val1,        \
                         field2,val2,        \
                         field3,val3,        \
                         field4,val4,        \
                         field5,val5,        \
                         field6,val6,        \
                         field7,val7,        \
                         field8,val8,        \
                         field9,val9,        \
                         field10,val10,         \
                         field11,val11,         \
                         field12,val12)         \
({                                  \
  typeof(reg) tmp;                        \
  tmp.as32BitValue = reg.as32BitValue;       \
  tmp.asBitField.field1 = (val1);            \
  tmp.asBitField.field2 = (val2);            \
  tmp.asBitField.field3 = (val3);            \
  tmp.asBitField.field4 = (val4);            \
  tmp.asBitField.field5 = (val5);            \
  tmp.asBitField.field6 = (val6);            \
  tmp.asBitField.field7 = (val7);            \
  tmp.asBitField.field8 = (val8);            \
  tmp.asBitField.field9 = (val9);            \
  tmp.asBitField.field10 = (val10);          \
  tmp.asBitField.field11 = (val11);          \
  tmp.asBitField.field12 = (val12);          \
  reg.as32BitValue = tmp.as32BitValue;       \
})
 */

/*******************************/
#endif
/*******************************/


/*******************************/
// Some common implementations...
/*******************************/
#if 1 // C99 compatible implementation!

// _REG32_NUM_ARGS(...) evaluates the literal number of the passed-in arguments.
#define                      _REG32_NUM_ARGS2(X,                A32,B32,A31,B31,A30,B30,A29,B29,A28,B28,A27,B27,A26,B26,A25,B25,A24,B24,A23,B23,A22,B22,A21,B21,A20,B20,A19,B19,A18,B18,A17,B17,A16,B16,A15,B15,A14,B14,A13,B13,A12,B12,A11,B11,A10,B10,A9,B9,A8,B8,A7,B7,A6,B6,A5,B5,A4,B4,A3,B3,A2,B2,A1,B1,A0,B0,N,...) N
#define _REG32_NUM_ARGS(...) _REG32_NUM_ARGS2(0, ##__VA_ARGS__ , 33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0,0)

#define _REG32WRITE_N3(reg,N,...)  reg32write ## N(reg, ##__VA_ARGS__)
#define _REG32WRITE_N2(reg,N,...) _REG32WRITE_N3(reg,N, ##__VA_ARGS__)
#define  REG32WRITE(reg,...)      _REG32WRITE_N2(reg,_REG32_NUM_ARGS(__VA_ARGS__), ##__VA_ARGS__)

#else // Old GNU compatible implementation!

// _REG32_NUM_ARGS(...) evaluates the literal number of the passed-in arguments.
#define                    _REG32_NUM_ARGS2(X,             A32,B32,A31,B31,A30,B30,A29,B29,A28,B28,A27,B27,A26,B26,A25,B25,A24,B24,A23,B23,A22,B22,A21,B21,A20,B20,A19,B19,A18,B18,A17,B17,A16,B16,A15,B15,A14,B14,A13,B13,A12,B12,A11,B11,A10,B10,A9,B9,A8,B8,A7,B7,A6,B6,A5,B5,A4,B4,A3,B3,A2,B2,A1,B1,A0,B0,N,args...) N
#define _REG32_NUM_ARGS(args...) _REG32_NUM_ARGS2(0, args , 33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0,0)

#define _REG32WRITE_N3(reg,N,args...)  reg32write ## N(reg, args)
#define _REG32WRITE_N2(reg,N,args...) _REG32WRITE_N3(reg,N, args)
#define  REG32WRITE(reg,args...)      _REG32WRITE_N2(reg,_REG32_NUM_ARGS(args), args)

#endif

/*
 * Note that reg32write() macro could be used as below.
 *
 * reg32write(regName, value) <-- Use this when regName is 32bit variable which is not a bit field.
 *    ie) reg32write(dfcRegisters.mCnsCFlush, 0xFFFFFFFF);
 *    ie) reg32write(dfcRegisters.mCnsCCfg0.as32BitValue, 0xFFFFFFFF);
 * reg32write(regName, field, value)
 *    ie) reg32write(dfcRegisters.mCnsCCfg0, mEnable, 1);
 * reg32write(regName, field1, value1, field2, value2)
 * reg32write(regName, field1, value1, field2, value2, field3, value3)
 * and so on...
 *
 * Regardless of number of passing parameters, you could use this unified macro for up to 32 field/value set.
 */
#define  reg32write REG32WRITE

//#undef _REG32_NUM_ARGS
//#undef _REG32_NUM_ARGS2
//#undef _REG32WRITE_N3
//#undef _REG32WRITE_N2

// Read and modify with AND operation, then write back
#define reg32readandwrite(reg, field, val) reg32write(reg, field, reg32read(reg, field)&(val))

// Read and modify with OR operation, then write back
#define reg32readorwrite(reg, field, val) reg32write(reg, field, reg32read(reg, field)|(val))


#endif /* REGISTERACCESS_H */
