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

#ifndef ASMGCCARM64_H
#define ASMGCCARM64_H

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef	unsigned long size_t;

#define DEFINE_SYSOP_TYPE_FUNC(_op, _type)		\
static inline void _op ## _type(void)			\
{							\
	__asm__ (#_op " " #_type);			\
}

#define DEFINE_SYSOP_FUNC(_op)				\
static inline void _op(void)				\
{							\
	__asm__ (#_op);					\
}


#define _DEFINE_SYSREG_READ_FUNC(_name, _reg_name)	\
static inline uint64_t read_ ## _name(void)		\
{							\
	uint64_t v;					\
	__asm__ ("mrs %0, " #_reg_name : "=r" (v));	\
	return v;					\
}

#define _DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)		\
static inline void write_ ## _name(uint64_t v)		\
{							\
	__asm__ ("msr " #_reg_name ", %0" : : "r" (v));	\
}

#define _DEFINE_SYSREG_WRITE_CONST_FUNC(_name, _reg_name)		\
static inline void write_ ## _name(const uint64_t v)	\
{							\
	__asm__ ("msr " #_reg_name ", %0" : : "i" (v));	\
}

/* Define read function for system register */
#define DEFINE_SYSREG_READ_FUNC(_name) 			\
	_DEFINE_SYSREG_READ_FUNC(_name, _name)

/* Define read & write function for system register */
#define DEFINE_SYSREG_RW_FUNCS(_name)			\
	_DEFINE_SYSREG_READ_FUNC(_name, _name)		\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _name)

/* Define read & write function for renamed system register */
#define DEFINE_RENAME_SYSREG_RW_FUNCS(_name, _reg_name)	\
	_DEFINE_SYSREG_READ_FUNC(_name, _reg_name)	\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)

/* Define write function for special system registers */
#define DEFINE_SYSREG_WRITE_CONST_FUNC(_name)		\
	_DEFINE_SYSREG_WRITE_CONST_FUNC(_name, _name)

DEFINE_SYSOP_TYPE_FUNC(dsb, sy)
DEFINE_SYSOP_FUNC(isb)
#define dsb()			dsbsy() 

#endif /* ASMGCCARM64_H */