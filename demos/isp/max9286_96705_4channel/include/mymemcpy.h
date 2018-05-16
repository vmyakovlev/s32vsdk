/*
 * mymemcpy.h
 *
 * ARM NEON optimized memcpy routine for ARM-v8 Architecture
 *
 * Function shamelessly copied from Linaro
 *
 * M.Staudenmaier
*/

#ifndef _MYMEMCPY_H
#define _MYMEMCPY_H

#ifdef __cplusplus
extern "C" 
{
#endif
void *mymemcpy(void *dest, const void *src, size_t n);

#ifdef __cplusplus
}
#endif
	
#endif
