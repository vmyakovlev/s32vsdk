/**
 @file             fsl_printf.c
 @version          0.0.0.0

 @brief            Module serves to printing debug messages
 @details

 Project           M4_ETH_PROJECT_NAME
 Platform          M4_ETH_PLATFORM

 SWVersion         M4_ETH_RELEASE_VERSION


 Copyright (c) 2014-2016 Freescale Semiconductor Inc.
 Copyright (c) 2016 NXP Semiconductors
 All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

/*==================================================================================================
                                         MISRA VIOLATIONS
==================================================================================================*/
/*
 * @page misra_violations MISRA-C:2004 violations
 *
 * @section fsl_printf_REF_1
 * Violates MISRA 2004 Advisory Rule 6.3, typedefs that indicate size and signedness should be used
 * in place of the basic numerical types.
 * Basic numerical types are used where it is required by standard printf API and also in processing
 * of those variables. And also where text is processed, there char may be used as natural type for
 * text.
 * 
 * @section fsl_printf_REF_2
 * Violates MISRA 2004 Required Rule 17.2, Pointer subtraction shall only be applied to pointers
 * that address elements of the same array.
 * False detection. Those pointers point to the same array.
 *
 * @section fsl_printf_REF_3
 * Violates MISRA 2004 Advisory Rule 11.3, A cast should not be performed between a pointer type
 * and an integral type.
 * In this case the cast is required by API of another module. It shall be checked that the pointer
 * fits into the uint32_t. Currently there is no check because s32v234 has its own implementation
 * of printf and all other platforms are 32bit. So the pointer fits there.
 * 
 * @section fsl_printf_REF_4
 * Violates MISRA 2004 Advisory Rule 11.3, A cast should not be performed between a pointer type
 * and an integral type.
 * In this case there is runtime check for the pointer size, so the conversion is safe.
 * 
 * @section fsl_printf_REF_5
 * Violates MISRA 2004 Required Rule 9.1, All automatic variables shall have been assigned a value
 * before being used.
 * False detection. Those variables have been assigned a value before being used. This happened in
 * the function where pointer to those variables were passed or in the same function above.
 *
 * @section fsl_printf_REF_6
 * Violates MISRA 2004 Required Rule 13.7, Boolean operations whose results are invariant shall
 * not be permitted.
 * Since preprocessor does not recognize sizeof operator, it is much safer to use compiler
 * condition and let compiler remove unnecessary code than to use preprocessor condition with
 * manually written sizeof values.
 *
 * @section fsl_printf_REF_7
 * Violates MISRA 2004 Required Rule 14.5, The continue statement shall not be used.
 * Use of the continue statement is much more straight forward, because it precisely matched the
 * need to start another iteration from beginning. Use of different construction would make the
 * code unclear and difficult to read.
 *
 * @section fsl_printf_REF_8
 * Violates MISRA 2004 Required Rule 1.1, All code shall conform to ISO/IEC 9899:1990 "Programming
 * languages - C", amended and corrected by ISO/IEC 9899/COR1:1995, ISO/IEC 9899/AMD1:1995, and
 * ISO/IEC 9899/COR2:1996.
 * False detection. Identifiers va_start and va_end are defined in "Programming language - C" since
 * version C89.
 * 
 * @section fsl_printf_REF_9
 * Violates MISRA 2004 Required Rule 16.1, Functions shall not be defined with a variable number
 * of arguments.
 * This is implementation of function printf. Its API enforces the use of variable number of
 * arguments. This function was implemented, because several 3rd party software packages depend
 * on it.
 * 
 * @section fsl_printf_REF_10
 * Violates MISRA 2004 Advisory Rule 11.4, A cast should not be performed between a pointer to 
 * object type and a different pointer to object type.
 * This is a cast from pointer to char to pointer to uint8_t, which is safe, because both pointed
 * types have same size. Just signedness may change, which is the intention.
 * 
 * @section fsl_printf_REF_11
 * Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or 
 * unspecified behaviour.
 * Addressing violations of kind: "Taking address of near auto variable". 
 * Appearing in function calls, passing as argument. This argument is being used only during
 * the function is executed, typically to write to (and even initialize) pointed variable. The
 * pointer and all of its copies are forgotten once the function returns, so there is no issue
 * with lifetime of the pointed variable.
 * 
 * @section fsl_printf_REF_12
 * Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or 
 * unspecified behaviour.
 * Addressing violations of kind: "Taking address of near auto variable". 
 * Appearing in initialization or assignment of variable. Both the variable and pointer to it
 * have same life time. There is no issue with that.
 * 
 * @section fsl_printf_REF_13
 * Violates MISRA 2004 Required Rule 18.4, Unions shall not be used.
 * Access to bit representation of double type is needed. Union is used to get the bits. Cast
 * may not be used since it causes unwanted conversion of the value. All other ways of
 * accessing the bits are less transparent and not safer. Correctness of bit representation
 * (i.e. the binary format) is checked in tests.
 * 
 * @section fsl_printf_REF_14
 * Violates MISRA 2004 Required Rule 14.3, Before preprocessing, a null statement shall only
 * occur on a line by itself; it may be followed by a comment provided that the first
 * character following the null statement is a white-space character.
 * False detection. There is no null statement.
 * 
 * @section fsl_printf_REF_15
 * Violates MISRA 2004 Required Rule 17.4, Array indexing shall be the only allowed form of pointer
 * arithmetic.
 * Rule is violated because the pointer can not be defined as an array in this case.
 * Pointer can be assigend to point to a different memory which is not possible with arrays.
 * To avoid the violation the mode functions would have to be redesigned witch would make
 * the code less readable and less effective too.
 * 
 * @section fsl_printf_REF_16
 * Violates MISRA 2004 Required Rule 13.3, Floating-point expressions shall not be tested for equality
 * or inequality.
 * In this case the check for equality to zero was not the main intension. The code in the condition
 * can process any value as long as it is either greater or less than zero, no matter how close to zero,
 * which is what the conditon checks. Anyway, in this case the comparison is made to constant
 * which is well defined even in floating point representation, so it is safe. 
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "fsl_printf.h"
#include "fsl_printf_cfg.h"

/* Used for variable arguments number processing */
#include <stdarg.h>

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
#define FLAG_NONE                    0U
#define FLAG_MINUS                   1U  /* Left aligned position of - or + */
#define FLAG_PLUS                    2U  /* Add '+' or '-' */
#define FLAG_SPACE                   4U  /* Add ' ' or '-' */
#define FLAG_HASH                    8U  /* Add 0x to hexa, always add . to floating point */
#define FLAG_ZERO                   16U  /* Pad with leading zeros to get the minimal width */
#define FLAG_UNSUPPORTED           128U  /* Output only ?? and perform no other action - this type is not supported
                                            the rest of the arguments will be most probably corrupted */

#define WIDTH_NONE                   (-1) /* Output width not specified */
#define WIDTH_STAR                   (-2) /* The next argument specifies the width */

#define PRECISION_NONE               (-1)
#define PRECISION_STAR               (-2)

#define LENGTH_NONE                  0U
#define LENGTH_HH                    1U
#define LENGTH_H                     2U
#define LENGTH_LITTLE_L              3U
#define LENGTH_LITTLE_LL             4U
#define LENGTH_J                     5U
#define LENGTH_Z                     6U
#define LENGTH_T                     7U
#define LENGTH_LARGE_L               8U

#define SPECIFIER_NONE               0U

#define TYPE_NONE                    0U
#define TYPE_INT8                    1U
#define TYPE_UINT8                   2U
#define TYPE_INT16                   3U
#define TYPE_UINT16                  4U
#define TYPE_INT32                   5U
#define TYPE_UINT32                  6U
#define TYPE_INT64                   7U
#define TYPE_UINT64                  8U
#define TYPE_DOUBLE                  9U
#define TYPE_LONG_DOUBLE            10U
#define TYPE_STRING                 11U
#define TYPE_POINTER                12U

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/*  This union is needed to be able to access bits of double values.
    Cast may not be used, it converts the value. */
typedef union
/** @violates @ref fsl_printf_REF_13 MISRA 2004 Required Rule 18.4 */
{
    float64_t dValue;
    uint64_t  u64Bits;
} tunDoubleToBits;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
static uint32_t Minimum(uint32_t u32Value1, uint32_t u32Value2);
static uint8_t* StrReplaceSint32(sint32_t s32Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
static uint8_t* StrReplaceSint64(sint64_t s64Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
#endif
static uint8_t* StrReplaceUint32(uint32_t u32Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
static uint8_t * StrReplaceUint64(uint64_t u64Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
#endif
static uint8_t* StrReplaceOctal32(uint32_t u32Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
static uint8_t* StrReplaceOctal64(uint64_t u64Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
#endif
static uint8_t* StrReplaceHexa32(uint32_t u32Value, uint8_t *pu8DestStr, const uint8_t pu8Hexa[], FormatSpecifierType formatSpec);
#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
static uint8_t* StrReplaceHexa64(uint64_t u64Value, uint8_t *pu8DestStr, const uint8_t pu8Hexa[], FormatSpecifierType formatSpec);
#endif
static uint8_t* StrReplaceChar(uint8_t u8Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
static uint8_t* StrReplaceString(const uint8_t pcou8Value[], uint8_t *pu8DestStr, FormatSpecifierType formatSpec);
static uint8_t* ConvertBufWithWidthPrecision(const uint8_t pcou8Buf[], uint16_t u16BufLen, uint8_t pu8DestStr[], FormatSpecifierType formatSpec);
static const uint8_t* ParseFormatSpecifier(const uint8_t pcou8FormatStr[],FormatSpecifierType *formatSpec);
#if TRUE == FSL_PRINTF_CFG_FLOAT_SUPPORT
static void ConvertDoubleToScientific(float64_t dNum, char_t *pcNegative, sint16_t *ps16Exp, uint8_t *pu8Bdp, uint32_t *pu32Adp);
#endif /* FSL_PRINTF_CFG_FLOAT_SUPPORT */
/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
#if TRUE == FSL_PRINTF_CFG_FLOAT_SUPPORT
/*
* @brief Converts the floating point number to scientific format
* @param[in] dNum - Number to convert
* @param[out] pcNegative sign flag either '-' when the number is negative or ' '(space) when it is non-negative
* @param[out] ps16Exp - Exponent value (signed)
* @param[out] pu8Bdp - Part before decimal point (only one digit 0-9)
* @param[out] pu32Adp - Part after decimal point (9 digits including leading 0s which must be printed out)
* @details Function returns integer numbers to be printed out to form the scientific format (%e) for floating
*         point numbers output.
*         The converted numbers shall be displayed by the following formatting string
*         printf("%c%1u.%09ue%02d", negative, bdp, adp, exp);
* @warning Function does not handle NaN (not a number) and infinity numbers, also 0 although it is a special
*          case is converted to the same format as other numbers.
* @note This function involves floating point divisions and multiplications which may be done several times
*       which may consume quite a lot of CPU cycles on some cores.
*/
static void ConvertDoubleToScientific(float64_t dNum, char_t *pcNegative, sint16_t *ps16Exp, uint8_t *pu8Bdp, uint32_t *pu32Adp)
{
    *ps16Exp = 0; /* We have to do this at each case */

    /** @violates @ref fsl_printf_REF_16 MISRA 2004 Required Rule 13.3 */
    if((dNum < (0.0)) || (dNum > (0.0)))
    {
        /* Get the sign and convert dNum to absolute value */
        if(dNum < 0.0)
        {   /* - */
            dNum = -dNum;
            *pcNegative = '-';
        }
        else /* dNum > 0.0 */
        { /* + */
            *pcNegative = ' ';
        }
        
        /* Get the exponent and normalize number
          - this is done gradually to improve the performance */
        while(dNum >= 1e64)
        {
            dNum /= 1e64;
            /* This syntax is used to avoid compiler warnings */
            *ps16Exp = (sint16_t)(*ps16Exp+64);
        }
        while(dNum >= 1e16)
        {
            dNum /= 1e16;
            *ps16Exp = (sint16_t)(*ps16Exp+16);
        }
        while(dNum >= 1e4)
        {
            dNum /= 1e4;
            *ps16Exp = (sint16_t)(*ps16Exp+4);
        }
        while(dNum >= 1e1)
        {
            dNum /= 1e1;
            *ps16Exp = (sint16_t)(*ps16Exp+1);
        }
    
        while(dNum <= 1e-64)
        {
            dNum *= 1e64;
            *ps16Exp = (sint16_t)(*ps16Exp-64);
        }
        while(dNum <= 1e-16)
        {
            dNum *= 1e16;
            *ps16Exp = (sint16_t)(*ps16Exp-16);
        }
        while(dNum <= 1e-4)
        {
            dNum *= 1e4;
            *ps16Exp = (sint16_t)(*ps16Exp-4);
        }
        while(dNum < 1.0)
        {
            dNum *= 10.0;
            *ps16Exp = (sint16_t)(*ps16Exp-1);
        }
        
        /* Part before decimal point */
        *pu8Bdp = (uint8_t) dNum;
        
        /* Part after decimal point */
        dNum = dNum - (float64_t)(*pu8Bdp);
        dNum *= 1e9;
        *pu32Adp = (uint32_t) dNum;
    }
    else /* dNum == 0 - special case */
    {
        *pcNegative = ' ';
        *pu8Bdp = 0U;
        *pu32Adp = 0U;
    }
}
#endif /* FSL_PRINTF_CFG_FLOAT_SUPPORT */


/**
* @brief     Function returns minimum of two variables
* @details
* @param[in] u32Value1       first value to compare
* @param[in] u32Value1       second value to compare
* @return                    value of the variable with lower value
*/
static uint32_t Minimum(uint32_t u32Value1, uint32_t u32Value2)
{
    uint32_t u32Minimum;

    if (u32Value1 < u32Value2)
    {
        u32Minimum = u32Value1;
    }
    else
    {
        u32Minimum = u32Value2;
    }

    return u32Minimum;
}

/**
* @brief     Signed to String
* @details   Converts value of the signed variable to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceSint32(sint32_t s32Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t u8CurNum;
    sint32_t s32Tmp;
    uint32_t u32Tmp;
    uint8_t au8Buffer[10U];
    uint16_t u16NumCntr = 0U;
    uint8_t *pu8ModifiedDestStr;

    if (0 > s32Value)
    {
        /* Change sign */
        s32Tmp = -s32Value;
        u32Tmp = (uint32_t)s32Tmp;
    }
    else
    {
        /* Do not change sign */
        u32Tmp = (uint32_t)s32Value;
    }

    do
    {
        u8CurNum = (uint8_t)(u32Tmp % 10U); /* Cannot be greater than 10 */
        au8Buffer[u16NumCntr] = (uint8_t)(u8CurNum + '0');
        u16NumCntr++;
        u32Tmp /= 10U;
    } while (0U != u32Tmp);

    if (0 > s32Value)
    {
        /* Write - sign to buffer */
        au8Buffer[u16NumCntr] = (uint8_t)'-';
        u16NumCntr++;
    }
    else
    {   /* Positive number */
        if(FLAG_PLUS == (formatSpec.u8Flags & FLAG_PLUS))
        {   /* Plus sign shall be added */
            au8Buffer[u16NumCntr] = (uint8_t)'+';
            u16NumCntr++;
        }
        if(FLAG_SPACE == (formatSpec.u8Flags & FLAG_SPACE))
        {   /* Space shall be added */
            au8Buffer[u16NumCntr] = (uint8_t)' ';
            u16NumCntr++;
        }
    }

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}


#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
/**
* @brief     Signed to String
* @details   Converts value of the signed variable to the string
* @param[in] u64Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceSint64(sint64_t s64Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t u8CurNum;
    uint64_t u64Tmp;
    sint64_t s64Tmp;
    uint8_t au8Buffer[21U];
    uint16_t u16NumCntr = 0U;
    uint8_t *pu8ModifiedDestStr;
    uint32_t u32CurTmp;
    uint32_t u32i;

    if (0 > s64Value)
    {
        /* Change sign */
        s64Tmp = -s64Value;
        u64Tmp = (uint64_t)s64Tmp;
    }
    else
    {
        /* Do not change sign */
        u64Tmp = (uint64_t)s64Value;
    }

    do
    {
        u32CurTmp = (uint32_t)(u64Tmp % 1000000000UL);
        u64Tmp /= 1000000000UL;
        for(u32i = 0U; u32i < 9U; u32i++)
        {   /* Produce 9 digits */
            u8CurNum = (uint8_t)(u32CurTmp % 10U);
            au8Buffer[u16NumCntr] = (uint8_t)(u8CurNum + '0');
            u16NumCntr++;
            u32CurTmp /= 10U;
            if( (0U == u32CurTmp) && (0U == u64Tmp) )
            {   /* This is the last piece, skip leading zeros */
                break;
            }
        }
    } while (0U != u64Tmp);


    if (0 > s64Value)
    {
        /* Write - sign to buffer */
        au8Buffer[u16NumCntr] = (uint8_t)'-';
        u16NumCntr++;
    }
    else
    {   /* Positive number */
        if(FLAG_PLUS == (formatSpec.u8Flags & FLAG_PLUS))
        {   /* Plus sign shall be added */
            au8Buffer[u16NumCntr] = (uint8_t)'+';
            u16NumCntr++;
        }
        if(FLAG_SPACE == (formatSpec.u8Flags & FLAG_SPACE))
        {   /* Space shall be added */
            au8Buffer[u16NumCntr] = (uint8_t)' ';
            u16NumCntr++;
        }
    }

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}
#endif

/**
* @brief     Unsigned to String
* @details   Converts value of the unsigned variable to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceUint32(uint32_t u32Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t u8CurNum;
    uint32_t u32Tmp = u32Value;
    uint8_t au8Buffer[10U];
    uint16_t u16NumCntr = 0U;
    uint8_t *pu8ModifiedDestStr;

    /* Extract variable value */
    do
    {
        u8CurNum = (uint8_t)(u32Tmp % 10U);

        au8Buffer[u16NumCntr] = (uint8_t)(u8CurNum + '0');
        u16NumCntr++;
        u32Tmp /= 10U;
    } while (0U != u32Tmp);

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}

#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
/**
* @brief     Unsigned to String
* @details   Converts value of the unsigned variable to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceUint64(uint64_t u64Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t u8CurNum;
    uint64_t u64Tmp = u64Value;
    uint8_t au8Buffer[21U];
    uint16_t u16NumCntr = 0U;
    uint8_t *pu8ModifiedDestStr;
    uint32_t u32CurTmp;
    uint32_t u32i;

    /* Extract variable value */
    do
    {
        u32CurTmp = (uint32_t)(u64Tmp % 1000000000UL);
        u64Tmp /= 1000000000UL;
        for(u32i = 0U; u32i < 9U; u32i++)
        {   /* Produce 9 digits */
            u8CurNum = (uint8_t)(u32CurTmp % 10U);
            au8Buffer[u16NumCntr] = (uint8_t)(u8CurNum + '0');
            u16NumCntr++;
            u32CurTmp /= 10U;
            if( (0U == u32CurTmp) && (0U == u64Tmp) )
            {   /* This is the last piece, skip leading zeros */
                break;
            }
        }
    } while (0U != u64Tmp);

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}
#endif


/**
* @brief     Octal to String
* @details   Converts value of the variable to octal format and than to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceOctal32(uint32_t u32Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t au8Buffer[11U];
    uint16_t u16NumCntr = 0U;
    uint8_t u8CurNum;
    uint32_t u32Tmp = u32Value;
    uint8_t *pu8ModifiedDestStr;

    do
    {
        u8CurNum = (uint8_t)((uint8_t)u32Tmp & 0x7U);
        au8Buffer[u16NumCntr] = (uint8_t)(u8CurNum + '0');
        u16NumCntr++;
        u32Tmp >>= 3U;
    } while (0U != u32Tmp);

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}

#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
/**
* @brief     Octal to String
* @details   Converts value of the variable to octal format and than to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceOctal64(uint64_t u64Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t au8Buffer[22U];
    uint16_t u16NumCntr = 0U;
    uint8_t u8CurNum;
    uint64_t u64Tmp = u64Value;
    uint8_t *pu8ModifiedDestStr;

    do
    {
        u8CurNum = (uint8_t)((uint8_t)u64Tmp & 0x7U);
        au8Buffer[u16NumCntr] = (uint8_t)(u8CurNum + '0');
        u16NumCntr++;
        u64Tmp >>= 3U;
    } while (0U != u64Tmp);

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}
#endif

/**
* @brief     Hexa to String
* @details   Converts value of the variable to hexa format and than to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] *pu8Hexa        array with hexa characters
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceHexa32(uint32_t u32Value, uint8_t *pu8DestStr, const uint8_t pu8Hexa[], FormatSpecifierType formatSpec)
{
    uint8_t au8Buffer[8U];
    uint16_t u16NumCntr = 0U;
    uint8_t u8CurNum;
    uint32_t u32Tmp = u32Value;
    uint8_t *pu8ModifiedDestStr;

    do
    {
        u8CurNum = pu8Hexa[u32Tmp & 0xFU];
        au8Buffer[u16NumCntr] = u8CurNum;
        u16NumCntr++;
        u32Tmp >>= 4U;
    } while (0U != u32Tmp);

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}

#if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
/**
* @brief     Hexa to String
* @details   Converts value of the variable to hexa format and than to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] *pu8Hexa        array with hexa characters
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceHexa64(uint64_t u64Value, uint8_t *pu8DestStr, const uint8_t pu8Hexa[], FormatSpecifierType formatSpec)
{
    uint8_t au8Buffer[16U];
    uint16_t u16NumCntr = 0U;
    uint8_t u8CurNum;
    uint64_t u64Tmp = u64Value;
    uint8_t *pu8ModifiedDestStr;

    do
    {
        u8CurNum = pu8Hexa[u64Tmp & 0xFU];
        au8Buffer[u16NumCntr] = u8CurNum;
        u16NumCntr++;
        u64Tmp >>= 4U;
    } while (0U != u64Tmp);

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16NumCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}
#endif
/**
* @brief     Char to String
* @details   Converts the char variable to the string
* @param[in] u32Value        value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceChar(uint8_t u8Value, uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t au8Buffer[1U];
    uint8_t *pu8ModifiedDestStr;

    au8Buffer[0] = u8Value;

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, 1U, pu8DestStr, formatSpec);

    /* Format "variable string" based on format specifier */
    return pu8ModifiedDestStr;
}


/**
* @brief     String to String
* @details   Converts the string variable to the string
* @param[in] pcou8Value      value to convert to string
* @param[in] *pu8DestStr     result destination
* @param[in] formatSpec      format specifier precision
* @return                    pointer to the current position in buffer with replaced characters
*/
static uint8_t * StrReplaceString(const uint8_t pcou8Value[], uint8_t *pu8DestStr, FormatSpecifierType formatSpec)
{
    uint8_t au8Buffer[STR_BUF_HALF_SIZE];
    uint16_t u16CharCntr = 0U;
    uint8_t *pu8ModifiedDestStr;
    uint16_t u16Index = 0U;
    
    if (NULL == pcou8Value)
    {
        /** @violates @ref fsl_printf_REF_10 MISRA 2004 Advisory Rule 11.4 */
        pcou8Value = (uint8_t*)"(NULL)";
    }

    for (u16Index=0U; (uint8_t)'\0' != pcou8Value[u16Index]; u16Index++)
    {
        /* Buffer overflow - do not support substitution longer than STR_BUF_HALF_SIZE bytes */
        if (u16CharCntr > (uint16_t)(STR_BUF_HALF_SIZE-1U))
        {
            break;
        }
        au8Buffer[u16CharCntr] = pcou8Value[u16Index];
        u16CharCntr++;
    }

    /* Format "variable string" based on format specifier */
    /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
    pu8ModifiedDestStr = ConvertBufWithWidthPrecision(au8Buffer, u16CharCntr, pu8DestStr, formatSpec);

    /* Return current position in destination string */
    return pu8ModifiedDestStr;
}


/**
* @brief     Format the output according to the informations extracted from format specifier.
* @details   Function converts string passed through *pcou8Buf parameter to the format defined by formatSpec
*            structure and save the result to the *pu8DestStr.
* @param[in]  *pcou8Buf         string which should be formatted
* @param[in]  u16BufLen       string buffer length
* @param[out] *pu8DestStr     destination of the formatted string
* @param[in]  formatSpec      structure with desired format informations
* @return                     pointer to the current position in buffer with replaced characters
*/
static uint8_t * ConvertBufWithWidthPrecision(const uint8_t pcou8Buf[], uint16_t u16BufLen, uint8_t pu8DestStr[], FormatSpecifierType formatSpec)
{
    sint16_t s16X;
    sint32_t s32X;
    uint16_t u16Index = 0U;

    /* Signed, unsigned, octal, hexa numbers */
    if (((uint8_t)'d' == formatSpec.u8Specifier) || ((uint8_t)'i' == formatSpec.u8Specifier) ||
        ((uint8_t)'u' == formatSpec.u8Specifier) || ((uint8_t)'o' == formatSpec.u8Specifier) ||
        ((uint8_t)'x' == formatSpec.u8Specifier) || ((uint8_t)'X' == formatSpec.u8Specifier) )
    {
        sint32_t s32Spaces = 0;
        sint32_t s32Zeros = 0;
        uint16_t u16NumLenWithoutSign = u16BufLen;

        if( ((uint8_t)'-' == pcou8Buf[u16BufLen-1U]) ||
            (((uint8_t)'+' == pcou8Buf[u16BufLen-1U]) || ((uint8_t)' ' == pcou8Buf[u16BufLen-1U]))
          )
        {
            u16NumLenWithoutSign = (uint16_t)(u16BufLen-1U);
        }

        /* Test if the width was set (macros WIDTH_NONE and WIDTH_STAR)*/
        if (formatSpec.s16Width > 0)
        {
            s32Spaces = (sint32_t)formatSpec.s16Width - (sint32_t)u16BufLen;
        }

        /* Test if the precision was set (macros PRECISION_NONE and PRECISION_STAR)*/
        if (formatSpec.s16Precision > 0)
        {
            s32Zeros = (sint32_t)formatSpec.s16Precision - (sint32_t)u16NumLenWithoutSign;
        }

        /* Number of spaces is reduced by zeros */
        if ((s32Spaces > 0) && (s32Zeros > 0))
        {
            s32Spaces -= s32Zeros;
        }

        /* Print spaces (nothing will be printed for negative number of spaces) */
        for (s32X = 0; s32X < s32Spaces; s32X++)
        {
            pu8DestStr[u16Index] = (uint8_t)' ';
            u16Index++;
        }

        /* Print minus sign if necessary */
        if ((uint8_t)'-' == pcou8Buf[u16BufLen-1U])
        {
            pu8DestStr[u16Index] = (uint8_t)'-';
            u16Index++;
        }

        /* Print zeros (nothing will be printed for negative number of zeros) */
        for (s32X = 0; s32X < s32Zeros; s32X++)
        {
            pu8DestStr[u16Index] = (uint8_t)'0';
            u16Index++;
        }

        /* Print the number */
        for (s16X = ((sint16_t)(u16NumLenWithoutSign-1)); s16X >= 0; s16X--)
        {
            pu8DestStr[u16Index] = pcou8Buf[s16X];
            u16Index++;
        }
    }
    /* String */
    else if ((uint8_t)'s' == formatSpec.u8Specifier)
    {
        uint16_t u16Limit = 0U;     /* Number of string characters which should be printed */
        sint32_t s32Spaces = 0;     /* Number of spaces which should be printed */

        /* Test if the precision was set (macros PRECISION_NONE and PRECISION_STAR)*/
        if (formatSpec.s16Precision >= 0)
        {
            u16Limit = (uint16_t)Minimum((uint32_t)(sint32_t)formatSpec.s16Precision, (uint32_t)u16BufLen);
        }
        else
        {
            u16Limit = u16BufLen;
        }

        /* Test if the width was set (macros WIDTH_NONE and WIDTH_STAR)*/
        if (formatSpec.s16Width >= 0)
        {
            s32Spaces = (sint32_t)formatSpec.s16Width - (sint32_t)u16Limit;
        }
        else
        {
            s32Spaces = 0;
        }

        /* Print spaces (nothing will be printed for negative number of spaces) */
        for (s32X = 0; s32X < s32Spaces; s32X++)
        {
            pu8DestStr[u16Index] = (uint8_t)' ';
            u16Index++;
        }

        /* Print string (or its part)*/
        for (s32X = 0; s32X < (sint32_t)u16Limit; s32X++)
        {
            pu8DestStr[u16Index] = pcou8Buf[s32X];
            u16Index++;
        }
    }
    /* Char */
    else if ((uint8_t)'c' == formatSpec.u8Specifier)
    {
        if (formatSpec.s16Width > (sint16_t)u16BufLen)
        {
            for (s16X = (sint16_t)(formatSpec.s16Width - (sint16_t)u16BufLen); s16X > 0; s16X--)
            {
                pu8DestStr[u16Index] = (uint8_t)' ';
                u16Index++;
            }
        }

        for(s16X = ((sint16_t)(u16BufLen - 1)); s16X >= 0; s16X--)
        {
            pu8DestStr[u16Index] = pcou8Buf[s16X];
            u16Index++;
        }
    }
    /* Pointer */
    else if ((uint8_t)'p' == formatSpec.u8Specifier)
    {
        sint32_t s32Spaces = 0;
        sint32_t s32Zeros = 0;

        /* Test if the width was set (macros WIDTH_NONE and WIDTH_STAR)*/
        if (formatSpec.s16Width > 0)
        {
            s32Spaces = (sint32_t)formatSpec.s16Width - ((sint32_t)u16BufLen + 2);
        }

        /* Test if the precision was set (macros PRECISION_NONE and PRECISION_STAR)*/
        if (formatSpec.s16Precision > 0)
        {
            s32Zeros = (sint32_t)formatSpec.s16Precision - ((sint32_t)u16BufLen + 2);
        }

        /* Number of spaces is reduced by zeros */
        if ((s32Spaces > 0) && (s32Zeros > 0))
        {
            s32Spaces -= s32Zeros;
        }

        /* Print spaces (nothing will be printed for negative number of spaces) */
        for (s32X = 0; s32X < s32Spaces; s32X++)
        {
            pu8DestStr[u16Index] = (uint8_t)' ';
            u16Index++;
        }

        /* Print 0x before pointer value */
        pu8DestStr[u16Index] = (uint8_t)'0';
        u16Index++;
        pu8DestStr[u16Index] = (uint8_t)'x';
        u16Index++;

        /* Print zeros (nothing will be printed for negative number of zeros) */
        for (s32X = 0; s32X < s32Zeros; s32X++)
        {
            pu8DestStr[u16Index] = (uint8_t)'0';
            u16Index++;
        }

        /* Print the number */
        for (s16X = ((sint16_t)(u16BufLen-1)); s16X >= 0; s16X--)
        {
            pu8DestStr[u16Index] = pcou8Buf[s16X];
            u16Index++;
        }
    }
    else
    {
        /* Nothing to be done */
    }

    /* Return current position in destination string */
    return &pu8DestStr[u16Index];
}


/**
* @brief      Parse format specifiers found in the input string.
* @details    Function is called whenever the fsl_printf function find % character in the input string.
*             The parsing of the format specifier is performed and extracted informations are stored to the formatSpec
*             structure.
* @param[in]  *pcou8FormatStr   String with the format specifier
* @param[out] *formatSpec       Pointer to the structure where data extracted from the format specifier are saved
* @return                       pointer to the current position in input string
*/
static const uint8_t * ParseFormatSpecifier(const uint8_t pcou8FormatStr[], FormatSpecifierType *formatSpec)
{
    uint8_t u8TypeWidth = 0U;
    uint16_t u16ResultWidth = 0U;
    uint16_t u16ResultPrecision = 0U;
    uint16_t u16Index = 0U;
    /* Format specifier structure initialization */
    formatSpec->u8Flags = FLAG_NONE;
    formatSpec->s16Width = WIDTH_NONE;
    formatSpec->s16Precision = PRECISION_NONE;
    formatSpec->u8Length = LENGTH_NONE;
    formatSpec->u8Specifier = SPECIFIER_NONE;
    formatSpec->u8Type = TYPE_NONE;

    /* Move to the symbol behind % */
    u16Index++;

    /* Parse flags */
    while(1)
    {
        if (pcou8FormatStr[u16Index] == (uint8_t)'-')
        {
            formatSpec->u8Flags |= FLAG_MINUS;
            u16Index++;
        }
        else if (pcou8FormatStr[u16Index] == (uint8_t)'+')
        {
            formatSpec->u8Flags |= FLAG_PLUS;
            u16Index++;
        }
        else if (pcou8FormatStr[u16Index] == (uint8_t)' ')
        {
            formatSpec->u8Flags |= FLAG_SPACE;
            u16Index++;
        }
        else if (pcou8FormatStr[u16Index] == (uint8_t)'#')
        {
            formatSpec->u8Flags |= FLAG_HASH;
            u16Index++;
        }
        else if (pcou8FormatStr[u16Index] == (uint8_t)'0')
        {
            formatSpec->u8Flags |= FLAG_ZERO;
            u16Index++;
        }
        else
        {
            break;
        }
    }

    /* Parse width */
    if (pcou8FormatStr[u16Index] == (uint8_t)'*')
    {
        formatSpec->s16Width = WIDTH_STAR;
        u16Index++;
    }
    else if ((pcou8FormatStr[u16Index] >= (uint8_t)'0') && (pcou8FormatStr[u16Index] <= (uint8_t)'9'))
    {
        /* Convert string to uint16_t */
        while ((pcou8FormatStr[u16Index] >= (uint8_t)'0') && (pcou8FormatStr[u16Index] <= (uint8_t)'9'))
        {
            u16ResultWidth = (uint16_t)(u16ResultWidth*10U);
            u16ResultWidth = (uint16_t)(u16ResultWidth + (uint16_t)(pcou8FormatStr[u16Index] - '0'));
            u16Index++;
        }

        /* Store result to the struct */
        formatSpec->s16Width = (sint16_t)u16ResultWidth;
    }
    else
    {
        /* Nothing to be done */
    }

    /* Parse precision */
    if (pcou8FormatStr[u16Index] == (uint8_t)'.')
    {
        u16Index++;

        if (pcou8FormatStr[u16Index] == (uint8_t)'*')
        {
            formatSpec->s16Precision = PRECISION_STAR;
        }
        else
        {
            /* Convert string to uint16_t */
            while ((pcou8FormatStr[u16Index] >= (uint8_t)'0') && (pcou8FormatStr[u16Index] <= (uint8_t)'9'))
            {
                u16ResultPrecision = (uint16_t)(u16ResultPrecision * 10U);
                u16ResultPrecision = (uint16_t)(u16ResultPrecision + (uint16_t)(pcou8FormatStr[u16Index] - '0'));
                u16Index++;
            }

            formatSpec->s16Precision = (sint16_t)u16ResultPrecision;
        }
    }

    /* Parse length */
    if ((uint8_t)'h' == pcou8FormatStr[u16Index])
    {
        formatSpec->u8Length = LENGTH_H;
        u16Index++;
        /** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
        u8TypeWidth = (uint8_t)sizeof(short);


        if ((uint8_t)'h' == pcou8FormatStr[u16Index])
        {
            formatSpec->u8Length = LENGTH_HH;
            u16Index++;
            u8TypeWidth = (uint8_t)sizeof(char_t);
        }
    }
    else if ((uint8_t)'l' == pcou8FormatStr[u16Index] )
    {
        formatSpec->u8Length = LENGTH_LITTLE_L;
        u16Index++;
        /** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
        u8TypeWidth = (uint8_t)sizeof(long int); /* Either 4 or 8 */

        if ((uint8_t)'l' == pcou8FormatStr[u16Index])
        {
            /* The long long (64bit) values are sometimes not supported */
            formatSpec->u8Length = LENGTH_LITTLE_LL;
            u16Index++;
            /** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
            u8TypeWidth = (uint8_t)sizeof(long long int);
            #if FALSE == FSL_PRINTF_CFG_LLINT_SUPPORT
                formatSpec->u8Flags |= FLAG_UNSUPPORTED;
            #endif
        }
    }
    else if ((uint8_t)'j' == pcou8FormatStr[u16Index])
    {
        /* The intmax_t or uintmax_t values are not supported */
        formatSpec->u8Length = LENGTH_J;
        u16Index++;
        u8TypeWidth = 0U;
        formatSpec->u8Flags |= FLAG_UNSUPPORTED;

    }
    else if ((uint8_t)'z' == pcou8FormatStr[u16Index])
    {
        /* The size_t values are not supported */
        formatSpec->u8Length = LENGTH_Z;
        u16Index++;
        u8TypeWidth = 0U;
        formatSpec->u8Flags |= FLAG_UNSUPPORTED;
    }
    else if ((uint8_t)'t' == pcou8FormatStr[u16Index])
    {
        /* The ptrdiff_t values are not supported */
        formatSpec->u8Length = LENGTH_T;
        u16Index++;
        u8TypeWidth = 0U;
        formatSpec->u8Flags |= FLAG_UNSUPPORTED;
    }
    else if ((uint8_t)'L' == pcou8FormatStr[u16Index])
    {
        /* The long double (typ. 64 or 80 bits) 80 bits values are not supported */
        formatSpec->u8Length = LENGTH_LARGE_L;
        u16Index++;
        /** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
        u8TypeWidth = (uint8_t)sizeof(long double);
        formatSpec->u8Flags |= FLAG_UNSUPPORTED;
    }
    else
    {
        /* Default length */
    }

    formatSpec->u8Specifier = pcou8FormatStr[u16Index];
    /* Parse specifier */
    switch((char_t)pcou8FormatStr[u16Index])
    {
        case 'd':
        case (uint8_t)'i':
            if(0U == u8TypeWidth) /* The default -> Integer */
            {
                /** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
                u8TypeWidth = (uint8_t)sizeof(int);
            }

            if (1U == u8TypeWidth)
            {
                formatSpec->u8Type = TYPE_INT8;
            }
            else if (2U == u8TypeWidth)
            {
                formatSpec->u8Type = TYPE_INT16;
            }
            else if (4U == u8TypeWidth)
            {
                formatSpec->u8Type = TYPE_INT32;
            }
            else  /* 8U == u8TypeWidth */
            {
                formatSpec->u8Type = TYPE_INT64;
            }
            break;
        case (uint8_t)'u':
        case (uint8_t)'o':
        case (uint8_t)'x':
        case (uint8_t)'X':
            if(0U == u8TypeWidth) /* The default -> Integer */
            {
                /** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
                u8TypeWidth = (uint8_t)sizeof(int);
            }

            if (1U == u8TypeWidth)
            {
                formatSpec->u8Type = TYPE_UINT8;
            }
            else if (2U == u8TypeWidth)
            {
                formatSpec->u8Type = TYPE_UINT16;
            }
            else if (4U == u8TypeWidth)
            {
                formatSpec->u8Type = TYPE_UINT32;
            }
            else  /* 8U == u8TypeWidth */
            {
                formatSpec->u8Type = TYPE_UINT64;
            }
            break;
        case (uint8_t)'a':
        case (uint8_t)'A':
        case (uint8_t)'f':
        case (uint8_t)'F':
        case (uint8_t)'e':
        case (uint8_t)'E':
        case (uint8_t)'g':
        case (uint8_t)'G':
            #if FALSE == FSL_PRINTF_CFG_FLOAT_SUPPORT
                /* Floating point numbers not supported */
                formatSpec->u8Flags |= FLAG_UNSUPPORTED;
            #else
                /* Hexadecimal floating point output is not supported for now */
                if(((uint8_t)'a' == pcou8FormatStr[u16Index]) || ((uint8_t)'A' == pcou8FormatStr[u16Index]))
                {
                    formatSpec->u8Flags |= FLAG_UNSUPPORTED;
                }
            #endif
            if (0U == u8TypeWidth)
            {   /* Check for default length (0U) */
                formatSpec->u8Type = TYPE_DOUBLE;
            }
            else
            {
                formatSpec->u8Type = TYPE_LONG_DOUBLE;
            }
            break;
        case (uint8_t)'c':
            formatSpec->u8Type = TYPE_INT8;
            break;
        case (uint8_t)'s':
            formatSpec->u8Type = TYPE_STRING;
            break;
        case (uint8_t)'p':
            formatSpec->u8Type = TYPE_POINTER;
            break;
        case (uint8_t)'n':
            formatSpec->u8Type = 0U; /* TODO */
            break;
        case (uint8_t)'%':
            formatSpec->u8Type = TYPE_NONE;
            break;
        default:
            formatSpec->u8Specifier = SPECIFIER_NONE;
            break;
    }

    /* Return pointer to the current position in input string */
    return &pcou8FormatStr[u16Index];
}


/**
 * @brief  Not needed at the moment.
 */
void fsl_printf_init(void)
{
    ;
}


/**
* @brief     Print data to the interface selected in configuration.
* @details   Function is used to print data through another module, which is joined in configuration.
*            It serves for printing debug messages.
* @param[in] *pcou8Str String which should be printed. It can contain format specifiers which will be substituted by
*                      the values of the variables that are passed to the function through variable count of parameters.
*                      Format is same as in standard function printf. Conversion specifiers are either: 
*                      - supported: diuoxXcsp%
*                      - with configurable support: fFeEgG (They are ignored if the support is disabled)
*                      - always ignored: aA
*                      - not recognized: all others (not recognized conversion specifiers will cause the conversion to abort)
*                      - Support of "long long" is configurable - it is ignored if it is disabled.
*                      - Long float is ignored.
*/
/** @violates @ref fsl_printf_REF_9 MISRA 2004 Required Rule 16.1 */
/** @violates @ref fsl_printf_REF_1 MISRA 2004 Advisory Rule 6.3 */
void fsl_printf(const char_t *pcocStr, ...)
{
    static const uint8_t au8HEXA[16] = { (uint8_t)'0', (uint8_t)'1', (uint8_t)'2', (uint8_t)'3',
                                         (uint8_t)'4', (uint8_t)'5', (uint8_t)'6', (uint8_t)'7',
                                         (uint8_t)'8', (uint8_t)'9', (uint8_t)'A', (uint8_t)'B',
                                         (uint8_t)'C', (uint8_t)'D', (uint8_t)'E', (uint8_t)'F'
                                       };
    static const uint8_t au8Hexa[16] = { (uint8_t)'0', (uint8_t)'1', (uint8_t)'2', (uint8_t)'3',
                                         (uint8_t)'4', (uint8_t)'5', (uint8_t)'6', (uint8_t)'7',
                                         (uint8_t)'8', (uint8_t)'9', (uint8_t)'a', (uint8_t)'b',
                                         (uint8_t)'c', (uint8_t)'d', (uint8_t)'e', (uint8_t)'f'
                                       };
    /** @violates @ref fsl_printf_REF_10 MISRA 2004 Advisory Rule 11.4 */
    const uint8_t *pcou8StrActChar = (const uint8_t*)pcocStr;  /* Pointer to actual character */
#if (TRUE == FSL_PRINT_CFG_PUT_BUFF_ON_STACK)
    /*  Buffer with replaced special characters - the output buffer.
        When this definition is selected, then fsl_printf is re-entrant but pay attention
        to stack size. There might be several instances of this buffer stored on the stack
        in some cases. */
    uint8_t au8StrRplc[STR_BUF_SIZE];
#else
    /*  Buffer with replaced special characters - the output buffer.
        When this definition is selected, then fsl_printf is NOT re-entrant. */
    static uint8_t au8StrRplc[STR_BUF_SIZE];
#endif
    /** @violates @ref fsl_printf_REF_12 MISRA 2004 Required Rule 1.2 */
    uint8_t *pu8StrRplc = au8StrRplc; /* Pointer to buffer with replaced characters */
    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
    uint8_t *pu8StrRplcPrintTrshld = pu8StrRplc + (STR_BUF_SIZE/4U); /* Threshold to empty buffer*/
    boolean bLong = FALSE;            /* Indicates that long variant of the type is used */
    boolean bAbort = FALSE;
    uint32_t u32NextArg = 0U;         /* Variable to store function parameter */
#if TRUE == FSL_PRINTF_CFG_FLOAT_SUPPORT
    float64_t dNextArg;               /* Variable to store function parameter */
#endif
    uint64_t u64NextArg = 0U;         /* Variable to store function parameter */
    void *pvNextArg = (void*)0U;      /* Variable to store function parameter */
    FormatSpecifierType formatSpec;   /* Keeps information about print format */
    va_list arguments;

    /* Set variable arguments start position */
    /** @violates @ref fsl_printf_REF_5 MISRA 2004 Required Rule 9.1 */
    /** @violates @ref fsl_printf_REF_8 MISRA 2004 Required Rule 1.1 */
    va_start (arguments, pcocStr);

    /* Extract number of special characters from string */
    while(0U != *pcou8StrActChar)
    {
        /* Format specifier character found */
        if ((uint8_t)'%' == *pcou8StrActChar)
        {
            bLong = FALSE;
            /* Parse format specifier at current position */
            /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
            pcou8StrActChar = ParseFormatSpecifier(pcou8StrActChar, &formatSpec);

            /* We must read the argument in each case */
            switch(formatSpec.u8Type)
            {   /*  Two assumptions were made here:                                                            */
                /*  1. Integer is 32bit wide                                                                   */
                /*  2. Integer promotion works as defined in C99 and in MISRA (same definition).               */
                /*  Both must be verified in tests for each platform, compiler, and compiler options set used. */
                case TYPE_INT8:           /* Promoted to int */
                case TYPE_UINT8:          /* Promoted to int */
                case TYPE_INT16:          /* Promoted to int */
                case TYPE_UINT16:         /* Promoted to int */
                case TYPE_INT32:          /* int */
                case TYPE_UINT32:         /* int */
                    u32NextArg = (uint32_t)va_arg(arguments, unsigned int);
                    break;
                case TYPE_STRING:         /* Pointer in fact */
                case TYPE_POINTER:        /* Pointer (32 or 64 bit) */
                    /** @violates @ref fsl_printf_REF_3 MISRA 2004 Advisory Rule 11.3 */
                    pvNextArg = (void *)va_arg(arguments, void *);
                    break;
                case TYPE_INT64:          /* long long */
                case TYPE_UINT64:         /* long long */
                    /* We must read it out even if this type is not supported */
                    u64NextArg = (uint64_t)va_arg(arguments, unsigned long long);
                    bLong = TRUE;
                    break;
#if TRUE == FSL_PRINTF_CFG_FLOAT_SUPPORT
                case TYPE_DOUBLE:
                    dNextArg = (float64_t)va_arg(arguments, double);
                    break;
                case TYPE_LONG_DOUBLE:
                    /* We must read it out even if this type is not supported */
                    (void)va_arg(arguments, long double);
                    break;
#endif
                case TYPE_NONE:
                    /* Nothing to read from va_arg */
                    break;
                default:
                    /* Type was not recognized. If this happens then undefined behaviour may occur. */
                    /* Inform user about the problem: */
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    *pu8StrRplc = (uint8_t)'(';  pu8StrRplc++;
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    *pu8StrRplc = (uint8_t)'e';  pu8StrRplc++;
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    *pu8StrRplc = (uint8_t)'r';  pu8StrRplc++;
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    *pu8StrRplc = (uint8_t)'r';  pu8StrRplc++;
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    *pu8StrRplc = (uint8_t)')';  pu8StrRplc++;
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    *pu8StrRplc = (uint8_t)'\n'; pu8StrRplc++;
                    /* Parsing should stop to avoid undefined behaviour */
                    bAbort = TRUE;
                    break;

            }
            if(TRUE == bAbort)
            {   /* Abort processing of the input, exit this loop */
                break;
            }

            if(FLAG_UNSUPPORTED == (formatSpec.u8Flags & FLAG_UNSUPPORTED))
            {
                /* Output ?? when the type is unsupported */
                *pu8StrRplc = (uint8_t)'?';
                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                pu8StrRplc++;
                *pu8StrRplc = (uint8_t)'?';
                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                pu8StrRplc++;
                /* Move to next character */
                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                pcou8StrActChar++;
                /** @violates @ref fsl_printf_REF_7 MISRA 2004 Required Rule 14.5 */
                continue;
            }

            /* Check character behind % */
            switch((char_t)formatSpec.u8Specifier)
            {
                case 'i':
                case 'd':
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    if(FALSE == bLong)
                    {
                    #endif
                        pu8StrRplc = StrReplaceSint32((sint32_t)u32NextArg, pu8StrRplc, formatSpec);
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    }
                    else
                    {
                        pu8StrRplc = StrReplaceSint64((sint64_t)u64NextArg, pu8StrRplc, formatSpec);
                    }
                    #endif
                    break;
                case 'u':
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    if(FALSE == bLong)
                    {
                    #endif
                        pu8StrRplc = StrReplaceUint32(u32NextArg, pu8StrRplc, formatSpec);
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    }
                    else
                    {
                        pu8StrRplc = StrReplaceUint64(u64NextArg, pu8StrRplc, formatSpec);
                    }
                    #endif
                    break;
                case 'o':
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    if(FALSE == bLong)
                    {
                    #endif
                        pu8StrRplc = StrReplaceOctal32(u32NextArg, pu8StrRplc, formatSpec);
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    }
                    else
                    {
                        pu8StrRplc = StrReplaceOctal64(u64NextArg, pu8StrRplc, formatSpec);
                    }
                    #endif
                    break;
                case 'x':
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    if(FALSE == bLong)
                    {
                    #endif
                        pu8StrRplc = StrReplaceHexa32(u32NextArg, pu8StrRplc, au8Hexa, formatSpec);
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    }
                    else
                    {
                        pu8StrRplc = StrReplaceHexa64(u64NextArg, pu8StrRplc, au8Hexa, formatSpec);
                    }
                    #endif
                    break;
                case 'X':
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    if(FALSE == bLong)
                    {
                    #endif
                        pu8StrRplc = StrReplaceHexa32(u32NextArg, pu8StrRplc, au8HEXA, formatSpec);
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    }
                    else
                    {
                        pu8StrRplc = StrReplaceHexa64(u64NextArg, pu8StrRplc, au8HEXA, formatSpec);
                    }
                    #endif
                    break;
                case 'c':
                    pu8StrRplc = StrReplaceChar((uint8_t)u32NextArg, pu8StrRplc, formatSpec);
                    break;
                case 's':
                    pu8StrRplc = StrReplaceString((uint8_t*)pvNextArg, pu8StrRplc, formatSpec);
                    break;
                case 'p':
                    /** @violates @ref fsl_printf_REF_6 MISRA 2004 Required Rule 13.7 */
                    if(4U == sizeof(void*))
                    {
                        /** @violates @ref fsl_printf_REF_4 MISRA 2004 Advisory Rule 11.3 */
                        pu8StrRplc = StrReplaceHexa32((uint32_t)(uaddr_t)pvNextArg, pu8StrRplc, au8HEXA, formatSpec);
                    }
                    #if TRUE == FSL_PRINTF_CFG_LLINT_SUPPORT
                    else
                    {
                        /** @violates @ref fsl_printf_REF_4 MISRA 2004 Advisory Rule 11.3 */
                        pu8StrRplc = StrReplaceHexa64((uint64_t)pvNextArg, pu8StrRplc, au8HEXA, formatSpec);
                    }
                    #endif
                    
                    break;
#if TRUE == FSL_PRINTF_CFG_FLOAT_SUPPORT
                case 'F': /* Letter case selection is not supported - so the next case is the same */
                case 'f': /* Floats promoted to double thus use the next case */
                case 'G': /* Letter case selection is not supported - so the next case is the same */
                case 'g': /* Appropriate format to be selected internally - done: the next one  */
                case 'E': /* Letter case selection is not supported - so the next case is the same */
                case 'e':
                    {   /* Block started to declare additional variables for this special case */
                        tunDoubleToBits unDouble; /* Union is needed to access bits (cast makes conversion instead) */
                        uint64_t u64Exponent;
                        uint16_t u16Exponent;

                        /** @violates @ref fsl_printf_REF_5 MISRA 2004 Required Rule 9.1 */
                        unDouble.dValue = dNextArg;
                        /* Check for special values encoded in exponent */
                        /* Valid for double defined in IEEE 754 */
                        u64Exponent = unDouble.u64Bits >> 52U;
                        u16Exponent = (uint16_t)u64Exponent;
                        u16Exponent &= 0x7FFU;
                        if(0x7FFU == u16Exponent)
                        {   /* Either inf or NaN */
                            if(0U == (unDouble.u64Bits & 0xFFFFFFFFFFFFFULL)) /* if mantis == zero */
                            {   /* Infinity */
                                if(0U != (unDouble.u64Bits & 0x8000000000000000ULL)) /* if sign bit is set */
                                {   /* Write minus */
                                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                    *pu8StrRplc = (uint8_t)'-';   pu8StrRplc++;
                                }
                                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                *pu8StrRplc = (uint8_t)'I';   pu8StrRplc++;
                                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                *pu8StrRplc = (uint8_t)'N';   pu8StrRplc++;
                                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                *pu8StrRplc = (uint8_t)'F';   pu8StrRplc++;
                            }
                            else
                            {   /* NaN */
                                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                *pu8StrRplc = (uint8_t)'N';   pu8StrRplc++;
                                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                *pu8StrRplc = (uint8_t)'a';   pu8StrRplc++;
                                /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                                *pu8StrRplc = (uint8_t)'N';   pu8StrRplc++;
                            }
                        }
                        else
                        {   /* Normal number or signed zero or subnormal number */
                            char_t cSign;
                            uint8_t  u8Bdp;
                            uint32_t u32Adp;
                            sint16_t s16Exp;
                            FormatSpecifierType rFS;

                            /** @violates @ref fsl_printf_REF_5 MISRA 2004 Required Rule 9.1 */
                            /** @violates @ref fsl_printf_REF_11 MISRA 2004 Required Rule 1.2 */
                            ConvertDoubleToScientific(dNextArg, &cSign, &s16Exp, &u8Bdp, &u32Adp);
    
                            /* Currently no support for any user selectable formatting for floating point */
                            /* Add sign */
                            *pu8StrRplc = (uint8_t)cSign;
                            /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                            pu8StrRplc++;
                            /* Add part before decimal point */
                            rFS.u8Flags = FLAG_NONE;
                            rFS.s16Width = 1;
                            rFS.s16Precision = PRECISION_NONE;
                            rFS.u8Length = LENGTH_NONE;
                            rFS.u8Specifier = (uint8_t)'u';
                            rFS.u8Type = TYPE_UINT8;
                            pu8StrRplc = StrReplaceUint32((uint32_t)u8Bdp, pu8StrRplc, rFS);
                            /* Add dot */
                            *pu8StrRplc = (uint8_t)'.';
                            /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                            pu8StrRplc++;
                            /* Add part after decimal point */
                            rFS.s16Width = 9; /* 9 digits */
                            rFS.s16Precision = 9;
                            rFS.u8Flags = FLAG_ZERO; /* Include leading zeros */
                            rFS.u8Type = TYPE_UINT32;
                            pu8StrRplc = StrReplaceUint32(u32Adp, pu8StrRplc, rFS);
                            /* Add 'e' character */
                            *pu8StrRplc = (uint8_t)'e';
                            /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                            pu8StrRplc++;
                            /* Add the exponent */
                            rFS.s16Width = 3; /* 3 digits */
                            rFS.s16Precision = 3;
                            rFS.u8Specifier = (uint8_t)'i';
                            rFS.u8Flags = FLAG_ZERO | FLAG_PLUS; /* Include leading zeros, always output + or - */
                            rFS.u8Type = TYPE_INT16;
                            pu8StrRplc = StrReplaceSint32((sint32_t)s16Exp, pu8StrRplc, rFS);
                            /* We are finished */
                        }
                    }
                    break;
#endif
                case '%':
                    *pu8StrRplc = *pcou8StrActChar;
                    /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
                    pu8StrRplc++;
                    break;
                default:
                        /* Nothing to be done. */
                    break;
            }
        }
        /* Normal character */
        else
        {
            *pu8StrRplc = *pcou8StrActChar;
            /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
            pu8StrRplc++;
        }

        /* Time to print string to the output */
        /** @violates @ref fsl_printf_REF_2 MISRA 2004 Required Rule 17.2 */
        if (pu8StrRplc > pu8StrRplcPrintTrshld)
        {
            /* Add string end character to string */
            *pu8StrRplc = (uint8_t)'\0';

            /* Print content of the buffer */
            /** @violates @ref fsl_printf_REF_3 MISRA 2004 Advisory Rule 11.3 */
            /** @violates @ref fsl_printf_REF_14 MISRA 2004 Required Rule 14.3 */
            FSL_PRINTF_CFG_DO_OUTPUT(au8StrRplc);

            /* Set the buffer pointer to its beginning */
            /** @violates @ref fsl_printf_REF_12 MISRA 2004 Required Rule 1.2 */
            pu8StrRplc = au8StrRplc;
        }

        /* Move to next character */
        /** @violates @ref fsl_printf_REF_15 MISRA 2004 Required Rule 17.4 */
        pcou8StrActChar++;
    }
    /** @violates @ref fsl_printf_REF_8 MISRA 2004 Required Rule 1.1 */
    va_end (arguments);

    /* Add string end character to string */
    *pu8StrRplc = (uint8_t)'\0';

    /* Print content of the buffer */
    /** @violates @ref fsl_printf_REF_3 MISRA 2004 Advisory Rule 11.3 */
    /** @violates @ref fsl_printf_REF_14 MISRA 2004 Required Rule 14.3 */
    FSL_PRINTF_CFG_DO_OUTPUT(au8StrRplc);
}

#ifdef __cplusplus
}
#endif