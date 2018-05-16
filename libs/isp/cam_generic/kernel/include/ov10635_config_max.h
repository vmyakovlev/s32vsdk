/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
* \file     ov10635_config.h
* \brief    definition of OV10635 camera registers configuration with Maxim
            From OV
* \author   Pavel Bohacik
* \version  1.0
* \date     30.3.2016
* \note
****************************************************************************/

#ifndef OV10635CONFIGMAX_H
#define OV10635CONFIGMAX_H



static uint8_t OV10635_Table[] = {
		0x01, 0x03, 0x01,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x0c, 0x61,
		0x30, 0x1b, 0xff,
		0x30, 0x1c, 0xff,
		0x30, 0x1a, 0xff,
		0x30, 0x11, 0x42,
		0x69, 0x00, 0x0c,
		0x69, 0x01, 0x11,
		0x35, 0x03, 0x10,
		0x30, 0x25, 0x03,
		0x30, 0x03, 0x20,
		0x30, 0x04, 0x21,
		0x30, 0x05, 0x20,
		0x30, 0x06, 0x91,
		0x36, 0x00, 0x74,
		0x36, 0x01, 0x2b,
		0x36, 0x12, 0x00,
		0x36, 0x11, 0x67,
		0x36, 0x33, 0xca,
		0x36, 0x02, 0x2f,
		0x36, 0x03, 0x00,
		0x36, 0x30, 0x28,
		0x36, 0x31, 0x16,
		0x37, 0x14, 0x10,
		0x37, 0x1d, 0x01,
		0x43, 0x00, 0x3A, 
		0x30, 0x07, 0x01,
		0x30, 0x24, 0x01,
		0x30, 0x20, 0x0b,
		0x37, 0x02, 0x20,
		0x37, 0x03, 0x48,
		0x37, 0x04, 0x32,
		0x37, 0x09, 0xa8,
		0x37, 0x09, 0xa8,
		0x37, 0x0c, 0xc7,
		0x37, 0x0d, 0x80,
		0x37, 0x12, 0x00,
		0x37, 0x13, 0x20,
		0x37, 0x15, 0x04,
		0x38, 0x1d, 0x40,
		0x38, 0x1c, 0x00,
		0x38, 0x22, 0x50,
		0x38, 0x24, 0x50,
		0x38, 0x15, 0x8c,
		0x38, 0x04, 0x05,
		0x38, 0x05, 0x1f,
		0x38, 0x00, 0x00,
		0x38, 0x01, 0x00,
		0x38, 0x06, 0x03,
		0x38, 0x07, 0x29,
		0x38, 0x02, 0x00,
		0x38, 0x03, 0x04,
		0x38, 0x08, 0x05,
		0x38, 0x09, 0x00,
		0x38, 0x0a, 0x03,
		0x38, 0x0b, 0x20,
		0x38, 0x0c, 0x07,
		0x38, 0x0d, 0x71,
		0x6e, 0x42, 0x03,
		0x6e, 0x43, 0x48,
		0x38, 0x0e, 0x03,
		0x38, 0x0f, 0x48,
		0x38, 0x13, 0x02,
		0x38, 0x11, 0x10,
		0x38, 0x1f, 0x0c,
		0x38, 0x28, 0x03,
		0x38, 0x29, 0x10,
		0x38, 0x2a, 0x10,
		0x38, 0x2b, 0x10,
		0x36, 0x21, 0x64,
		0x50, 0x05, 0x08,
		0x56, 0xd5, 0x00,
		0x56, 0xd6, 0x80,
		0x56, 0xd7, 0x00,
		0x56, 0xd8, 0x00,
		0x56, 0xd9, 0x00,
		0x56, 0xda, 0x80,
		0x56, 0xdb, 0x00,
		0x56, 0xdc, 0x00,
		0x56, 0xe8, 0x00,
		0x56, 0xe9, 0x7f,
		0x56, 0xea, 0x00,
		0x56, 0xeb, 0x7f,
		0x51, 0x00, 0x00,
		0x51, 0x01, 0x80,
		0x51, 0x02, 0x00,
		0x51, 0x03, 0x80,
		0x51, 0x04, 0x00,
		0x51, 0x05, 0x80,
		0x51, 0x06, 0x00,
		0x51, 0x07, 0x80,
		0x51, 0x08, 0x00,
		0x51, 0x09, 0x00,
		0x51, 0x0a, 0x00,
		0x51, 0x0b, 0x00,
		0x51, 0x0c, 0x00,
		0x51, 0x0d, 0x00,
		0x51, 0x0e, 0x00,
		0x51, 0x0f, 0x00,
		0x51, 0x10, 0x00,
		0x51, 0x11, 0x80,
		0x51, 0x12, 0x00,
		0x51, 0x13, 0x80,
		0x51, 0x14, 0x00,
		0x51, 0x15, 0x80,
		0x51, 0x16, 0x00,
		0x51, 0x17, 0x80,
		0x51, 0x18, 0x00,
		0x51, 0x19, 0x00,
		0x51, 0x1a, 0x00,
		0x51, 0x1b, 0x00,
		0x51, 0x1c, 0x00,
		0x51, 0x1d, 0x00,
		0x51, 0x1e, 0x00,
		0x51, 0x1f, 0x00,
		0x56, 0xd0, 0x00,
		0x50, 0x06, 0x24,
		0x56, 0x08, 0x0d,
		0x52, 0xd7, 0x06,
		0x52, 0x8d, 0x08,
		0x52, 0x93, 0x12,
		0x52, 0xd3, 0x12,
		0x52, 0x88, 0x06,
		0x52, 0x89, 0x20,
		0x52, 0xc8, 0x06,
		0x52, 0xc9, 0x20,
		0x52, 0xcd, 0x04,
		0x53, 0x81, 0x00,
		0x53, 0x82, 0xff,
		0x55, 0x89, 0x76,
		0x55, 0x8a, 0x47,
		0x55, 0x8b, 0xef,
		0x55, 0x8c, 0xc9,
		0x55, 0x8d, 0x49,
		0x55, 0x8e, 0x30,
		0x55, 0x8f, 0x67,
		0x55, 0x90, 0x3f,
		0x55, 0x91, 0xf0,
		0x55, 0x92, 0x10,
		0x55, 0xa2, 0x6d,
		0x55, 0xa3, 0x55,
		0x55, 0xa4, 0xc3,
		0x55, 0xa5, 0xb5,
		0x55, 0xa6, 0x43,
		0x55, 0xa7, 0x38,
		0x55, 0xa8, 0x5f,
		0x55, 0xa9, 0x4b,
		0x55, 0xaa, 0xf0,
		0x55, 0xab, 0x10,
		0x55, 0x81, 0x52,
		0x53, 0x00, 0x01,
		0x53, 0x01, 0x00,
		0x53, 0x02, 0x00,
		0x53, 0x03, 0x0e,
		0x53, 0x04, 0x00,
		0x53, 0x05, 0x0e,
		0x53, 0x06, 0x00,
		0x53, 0x07, 0x36,
		0x53, 0x08, 0x00,
		0x53, 0x09, 0xd9,
		0x53, 0x0a, 0x00,
		0x53, 0x0b, 0x0f,
		0x53, 0x0c, 0x00,
		0x53, 0x0d, 0x2c,
		0x53, 0x0e, 0x00,
		0x53, 0x0f, 0x59,
		0x53, 0x10, 0x00,
		0x53, 0x11, 0x7b,
		0x53, 0x12, 0x00,
		0x53, 0x13, 0x22,
		0x53, 0x14, 0x00,
		0x53, 0x15, 0xd5,
		0x53, 0x16, 0x00,
		0x53, 0x17, 0x13,
		0x53, 0x18, 0x00,
		0x53, 0x19, 0x18,
		0x53, 0x1a, 0x00,
		0x53, 0x1b, 0x26,
		0x53, 0x1c, 0x00,
		0x53, 0x1d, 0xdc,
		0x53, 0x1e, 0x00,
		0x53, 0x1f, 0x02,
		0x53, 0x20, 0x00,
		0x53, 0x21, 0x24,
		0x53, 0x22, 0x00,
		0x53, 0x23, 0x56,
		0x53, 0x24, 0x00,
		0x53, 0x25, 0x85,
		0x53, 0x26, 0x00,
		0x53, 0x27, 0x20,
		0x56, 0x09, 0x01,
		0x56, 0x0a, 0x40,
		0x56, 0x0b, 0x01,
		0x56, 0x0c, 0x40,
		0x56, 0x0d, 0x00,
		0x56, 0x0e, 0xfa,
		0x56, 0x0f, 0x00,
		0x56, 0x10, 0xfa,
		0x56, 0x11, 0x02,
		0x56, 0x12, 0x80,
		0x56, 0x13, 0x02,
		0x56, 0x14, 0x80,
		0x56, 0x15, 0x01,
		0x56, 0x16, 0x2c,
		0x56, 0x17, 0x01,
		0x56, 0x18, 0x2c,
		0x56, 0x3b, 0x01,
		0x56, 0x3c, 0x01,
		0x56, 0x3d, 0x01,
		0x56, 0x3e, 0x01,
		0x56, 0x3f, 0x03,
		0x56, 0x40, 0x03,
		0x56, 0x41, 0x03,
		0x56, 0x42, 0x05,
		0x56, 0x43, 0x09,
		0x56, 0x44, 0x05,
		0x56, 0x45, 0x05,
		0x56, 0x46, 0x05,
		0x56, 0x47, 0x05,
		0x56, 0x51, 0x00,
		0x56, 0x52, 0x80,
		0x52, 0x1a, 0x01,
		0x52, 0x1b, 0x03,
		0x52, 0x1c, 0x06,
		0x52, 0x1d, 0x0a,
		0x52, 0x1e, 0x0e,
		0x52, 0x1f, 0x12,
		0x52, 0x20, 0x16,
		0x52, 0x23, 0x02,
		0x52, 0x25, 0x04,
		0x52, 0x27, 0x08,
		0x52, 0x29, 0x0c,
		0x52, 0x2b, 0x12,
		0x52, 0x2d, 0x18,
		0x52, 0x2f, 0x1e,
		0x52, 0x41, 0x04,
		0x52, 0x42, 0x01,
		0x52, 0x43, 0x03,
		0x52, 0x44, 0x06,
		0x52, 0x45, 0x0a,
		0x52, 0x46, 0x0e,
		0x52, 0x47, 0x12,
		0x52, 0x48, 0x16,
		0x52, 0x4a, 0x03,
		0x52, 0x4c, 0x04,
		0x52, 0x4e, 0x08,
		0x52, 0x50, 0x0c,
		0x52, 0x52, 0x12,
		0x52, 0x54, 0x18,
		0x52, 0x56, 0x1e,
		0x46, 0x06, 0x07,
		0x46, 0x07, 0x71,
		0x46, 0x0a, 0x02,
		0x46, 0x0b, 0x70,
		0x46, 0x0c, 0x00,
		0x46, 0x20, 0x0e,
		0x47, 0x00, 0x04,
		0x47, 0x01, 0x00,
		0x47, 0x02, 0x01,
		
		0x40, 0x04, 0x04,
		0x40, 0x05, 0x18,
		0x40, 0x01, 0x06,
		0x40, 0x50, 0x22,
		0x40, 0x51, 0x24,
		0x40, 0x52, 0x02,
		0x40, 0x57, 0x9c,
		0x40, 0x5a, 0x00,
		0x42, 0x02, 0x02,
		0x30, 0x23, 0x10,
		0x01, 0x00, 0x01,
		0x01, 0x00, 0x01,
		0x6f, 0x10, 0x07,
		0x6f, 0x11, 0x82,
		0x6f, 0x12, 0x04,
		0x6f, 0x13, 0x00,
		0x6f, 0x14, 0x1f,
		0x6f, 0x15, 0xdd,
		0x6f, 0x16, 0x04,
		0x6f, 0x17, 0x04,
		0x6f, 0x18, 0x36,
		0x6f, 0x19, 0x66,
		0x6f, 0x1a, 0x04,
		0x6f, 0x1b, 0x08,
		0x6f, 0x1c, 0x0c,
		0x6f, 0x1d, 0xe7,
		0x6f, 0x1e, 0x04,
		0x6f, 0x1f, 0x0c,
		0xd0, 0x00, 0x19,
		0xd0, 0x01, 0xa0,
		0xd0, 0x02, 0x00,
		0xd0, 0x03, 0x01,
		0xd0, 0x04, 0xa9,
		0xd0, 0x05, 0xad,
		0xd0, 0x06, 0x10,
		0xd0, 0x07, 0x40,
		0xd0, 0x08, 0x44,
		0xd0, 0x09, 0x00,
		0xd0, 0x0a, 0x68,
		0xd0, 0x0b, 0x00,
		0xd0, 0x0c, 0x15,
		0xd0, 0x0d, 0x00,
		0xd0, 0x0e, 0x00,
		0xd0, 0x0f, 0x00,
		0xd0, 0x10, 0x19,
		0xd0, 0x11, 0xa0,
		0xd0, 0x12, 0x00,
		0xd0, 0x13, 0x01,
		0xd0, 0x14, 0xa9,
		0xd0, 0x15, 0xad,
		0xd0, 0x16, 0x12,
		0xd0, 0x17, 0x30,
		0xd0, 0x18, 0x44,
		0xd0, 0x19, 0x00,
		0xd0, 0x1a, 0x68,
		0xd0, 0x1b, 0x00,
		0xd0, 0x1c, 0x15,
		0xd0, 0x1d, 0x00,
		0xd0, 0x1e, 0x00,
		0xd0, 0x1f, 0x00,
		0xd0, 0x20, 0x19,
		0xd0, 0x21, 0xa0,
		0xd0, 0x22, 0x00,
		0xd0, 0x23, 0x01,
		0xd0, 0x24, 0xa9,
		0xd0, 0x25, 0xad,
		0xd0, 0x26, 0x13,
		0xd0, 0x27, 0x18,
		0xd0, 0x28, 0x44,
		0xd0, 0x29, 0x00,
		0xd0, 0x2a, 0x68,
		0xd0, 0x2b, 0x00,
		0xd0, 0x2c, 0x15,
		0xd0, 0x2d, 0x00,
		0xd0, 0x2e, 0x00,
		0xd0, 0x2f, 0x00,
		0xd0, 0x30, 0x19,
		0xd0, 0x31, 0xa0,
		0xd0, 0x32, 0x00,
		0xd0, 0x33, 0x01,
		0xd0, 0x34, 0xa9,
		0xd0, 0x35, 0xad,
		0xd0, 0x36, 0x13,
		0xd0, 0x37, 0x3c,
		0xd0, 0x38, 0x44,
		0xd0, 0x39, 0x00,
		0xd0, 0x3a, 0x68,
		0xd0, 0x3b, 0x00,
		0xd0, 0x3c, 0x15,
		0xd0, 0x3d, 0x00,
		0xd0, 0x3e, 0x00,
		0xd0, 0x3f, 0x00,
		0xd0, 0x40, 0x18,
		0xd0, 0x41, 0x60,
		0xd0, 0x42, 0x00,
		0xd0, 0x43, 0x01,
		0xd0, 0x44, 0x9c,
		0xd0, 0x45, 0x80,
		0xd0, 0x46, 0x00,
		0xd0, 0x47, 0x00,
		0xd0, 0x48, 0xa8,
		0xd0, 0x49, 0x63,
		0xd0, 0x4a, 0x06,
		0xd0, 0x4b, 0x4c,
		0xd0, 0x4c, 0xd8,
		0xd0, 0x4d, 0x03,
		0xd0, 0x4e, 0x20,
		0xd0, 0x4f, 0x00,
		0xd0, 0x50, 0x18,
		0xd0, 0x51, 0x60,
		0xd0, 0x52, 0x00,
		0xd0, 0x53, 0x01,
		0xd0, 0x54, 0xa8,
		0xd0, 0x55, 0x63,
		0xd0, 0x56, 0x02,
		0xd0, 0x57, 0xa4,
		0xd0, 0x58, 0x84,
		0xd0, 0x59, 0xa3,
		0xd0, 0x5a, 0x00,
		0xd0, 0x5b, 0x00,
		0xd0, 0x5c, 0x18,
		0xd0, 0x5d, 0x60,
		0xd0, 0x5e, 0x00,
		0xd0, 0x5f, 0x01,
		0xd0, 0x60, 0xa8,
		0xd0, 0x61, 0x63,
		0xd0, 0x62, 0x03,
		0xd0, 0x63, 0xf0,
		0xd0, 0x64, 0x8c,
		0xd0, 0x65, 0x85,
		0xd0, 0x66, 0x00,
		0xd0, 0x67, 0x6e,
		0xd0, 0x68, 0x98,
		0xd0, 0x69, 0x63,
		0xd0, 0x6a, 0x00,
		0xd0, 0x6b, 0x00,
		0xd0, 0x6c, 0xe5,
		0xd0, 0x6d, 0x83,
		0xd0, 0x6e, 0x20,
		0xd0, 0x6f, 0x00,
		0xd0, 0x70, 0x10,
		0xd0, 0x71, 0x00,
		0xd0, 0x72, 0x00,
		0xd0, 0x73, 0x4b,
		0xd0, 0x74, 0x18,
		0xd0, 0x75, 0x80,
		0xd0, 0x76, 0x80,
		0xd0, 0x77, 0x06,
		0xd0, 0x78, 0x18,
		0xd0, 0x79, 0x60,
		0xd0, 0x7a, 0x00,
		0xd0, 0x7b, 0x01,
		0xd0, 0x7c, 0xa8,
		0xd0, 0x7d, 0x63,
		0xd0, 0x7e, 0x01,
		0xd0, 0x7f, 0x00,
		0xd0, 0x80, 0x94,
		0xd0, 0x81, 0x85,
		0xd0, 0x82, 0x00,
		0xd0, 0x83, 0x70,
		0xd0, 0x84, 0x94,
		0xd0, 0x85, 0x63,
		0xd0, 0x86, 0x00,
		0xd0, 0x87, 0x02,
		0xd0, 0x88, 0xe4,
		0xd0, 0x89, 0x83,
		0xd0, 0x8a, 0x20,
		0xd0, 0x8b, 0x00,
		0xd0, 0x8c, 0x10,
		0xd0, 0x8d, 0x00,
		0xd0, 0x8e, 0x00,
		0xd0, 0x8f, 0x44,
		0xd0, 0x90, 0x18,
		0xd0, 0x91, 0x80,
		0xd0, 0x92, 0x80,
		0xd0, 0x93, 0x06,
		0xd0, 0x94, 0x8c,
		0xd0, 0x95, 0x65,
		0xd0, 0x96, 0x00,
		0xd0, 0x97, 0x76,
		0xd0, 0x98, 0xbc,
		0xd0, 0x99, 0x23,
		0xd0, 0x9a, 0x00,
		0xd0, 0x9b, 0x00,
		0xd0, 0x9c, 0x0c,
		0xd0, 0x9d, 0x00,
		0xd0, 0x9e, 0x00,
		0xd0, 0x9f, 0x41,
		0xd0, 0xa0, 0xa8,
		0xd0, 0xa1, 0x64,
		0xd0, 0xa2, 0x38,
		0xd0, 0xa3, 0x29,
		0xd0, 0xa4, 0x18,
		0xd0, 0xa5, 0x80,
		0xd0, 0xa6, 0x80,
		0xd0, 0xa7, 0x06,
		0xd0, 0xa8, 0xa8,
		0xd0, 0xa9, 0x64,
		0xd0, 0xaa, 0x38,
		0xd0, 0xab, 0x24,
		0xd0, 0xac, 0x8c,
		0xd0, 0xad, 0x63,
		0xd0, 0xae, 0x00,
		0xd0, 0xaf, 0x00,
		0xd0, 0xb0, 0xa4,
		0xd0, 0xb1, 0x63,
		0xd0, 0xb2, 0x00,
		0xd0, 0xb3, 0x40,
		0xd0, 0xb4, 0xbc,
		0xd0, 0xb5, 0x23,
		0xd0, 0xb6, 0x00,
		0xd0, 0xb7, 0x00,
		0xd0, 0xb8, 0x10,
		0xd0, 0xb9, 0x00,
		0xd0, 0xba, 0x00,
		0xd0, 0xbb, 0x0a,
		0xd0, 0xbc, 0x18,
		0xd0, 0xbd, 0xe0,
		0xd0, 0xbe, 0x80,
		0xd0, 0xbf, 0x06,
		0xd0, 0xc0, 0x9c,
		0xd0, 0xc1, 0xc0,
		0xd0, 0xc2, 0x00,
		0xd0, 0xc3, 0x40,
		0xd0, 0xc4, 0xa8,
		0xd0, 0xc5, 0x64,
		0xd0, 0xc6, 0x55,
		0xd0, 0xc7, 0x86,
		0xd0, 0xc8, 0xa8,
		0xd0, 0xc9, 0xa4,
		0xd0, 0xca, 0x55,
		0xd0, 0xcb, 0x87,
		0xd0, 0xcc, 0xd8,
		0xd0, 0xcd, 0x03,
		0xd0, 0xce, 0x30,
		0xd0, 0xcf, 0x00,
		0xd0, 0xd0, 0xa8,
		0xd0, 0xd1, 0x64,
		0xd0, 0xd2, 0x55,
		0xd0, 0xd3, 0x88,
		0xd0, 0xd4, 0xd8,
		0xd0, 0xd5, 0x05,
		0xd0, 0xd6, 0x30,
		0xd0, 0xd7, 0x00,
		0xd0, 0xd8, 0xd8,
		0xd0, 0xd9, 0x03,
		0xd0, 0xda, 0x30,
		0xd0, 0xdb, 0x00,
		0xd0, 0xdc, 0x18,
		0xd0, 0xdd, 0xe0,
		0xd0, 0xde, 0x80,
		0xd0, 0xdf, 0x06,
		0xd0, 0xe0, 0xa9,
		0xd0, 0xe1, 0x07,
		0xd0, 0xe2, 0x3d,
		0xd0, 0xe3, 0x05,
		0xd0, 0xe4, 0x8c,
		0xd0, 0xe5, 0x68,
		0xd0, 0xe6, 0x00,
		0xd0, 0xe7, 0x00,
		0xd0, 0xe8, 0xb8,
		0xd0, 0xe9, 0x63,
		0xd0, 0xea, 0x00,
		0xd0, 0xeb, 0x18,
		0xd0, 0xec, 0xb8,
		0xd0, 0xed, 0x63,
		0xd0, 0xee, 0x00,
		0xd0, 0xef, 0x98,
		0xd0, 0xf0, 0xbc,
		0xd0, 0xf1, 0x03,
		0xd0, 0xf2, 0x00,
		0xd0, 0xf3, 0x00,
		0xd0, 0xf4, 0x10,
		0xd0, 0xf5, 0x00,
		0xd0, 0xf6, 0x00,
		0xd0, 0xf7, 0x10,
		0xd0, 0xf8, 0xa9,
		0xd0, 0xf9, 0x67,
		0xd0, 0xfa, 0x67,
		0xd0, 0xfb, 0x02,
		0xd0, 0xfc, 0xb8,
		0xd0, 0xfd, 0xa3,
		0xd0, 0xfe, 0x00,
		0xd0, 0xff, 0x19,
		0xd1, 0x00, 0x8c,
		0xd1, 0x01, 0x8b,
		0xd1, 0x02, 0x00,
		0xd1, 0x03, 0x00,
		0xd1, 0x04, 0xa9,
		0xd1, 0x05, 0xa7,
		0xd1, 0x06, 0x67,
		0xd1, 0x07, 0x03,
		0xd1, 0x08, 0xb8,
		0xd1, 0x09, 0xc4,
		0xd1, 0x0a, 0x00,
		0xd1, 0x0b, 0x08,
		0xd1, 0x0c, 0x8c,
		0xd1, 0x0d, 0x6d,
		0xd1, 0x0e, 0x00,
		0xd1, 0x0f, 0x00,
		0xd1, 0x10, 0xb8,
		0xd1, 0x11, 0x85,
		0xd1, 0x12, 0x00,
		0xd1, 0x13, 0x98,
		0xd1, 0x14, 0xe0,
		0xd1, 0x15, 0x63,
		0xd1, 0x16, 0x30,
		0xd1, 0x17, 0x04,
		0xd1, 0x18, 0xe0,
		0xd1, 0x19, 0x64,
		0xd1, 0x1a, 0x18,
		0xd1, 0x1b, 0x00,
		0xd1, 0x1c, 0xa4,
		0xd1, 0x1d, 0x83,
		0xd1, 0x1e, 0xff,
		0xd1, 0x1f, 0xff,
		0xd1, 0x20, 0xb8,
		0xd1, 0x21, 0x64,
		0xd1, 0x22, 0x00,
		0xd1, 0x23, 0x48,
		0xd1, 0x24, 0xd8,
		0xd1, 0x25, 0x0b,
		0xd1, 0x26, 0x18,
		0xd1, 0x27, 0x00,
		0xd1, 0x28, 0xd8,
		0xd1, 0x29, 0x0d,
		0xd1, 0x2a, 0x20,
		0xd1, 0x2b, 0x00,
		0xd1, 0x2c, 0x9c,
		0xd1, 0x2d, 0x60,
		0xd1, 0x2e, 0x00,
		0xd1, 0x2f, 0x00,
		0xd1, 0x30, 0xd8,
		0xd1, 0x31, 0x08,
		0xd1, 0x32, 0x18,
		0xd1, 0x33, 0x00,
		0xd1, 0x34, 0x9c,
		0xd1, 0x35, 0x80,
		0xd1, 0x36, 0x00,
		0xd1, 0x37, 0x70,
		0xd1, 0x38, 0xa8,
		0xd1, 0x39, 0x67,
		0xd1, 0x3a, 0x38,
		0xd1, 0x3b, 0x22,
		0xd1, 0x3c, 0xa8,
		0xd1, 0x3d, 0xc7,
		0xd1, 0x3e, 0x38,
		0xd1, 0x3f, 0x43,
		0xd1, 0x40, 0xd8,
		0xd1, 0x41, 0x03,
		0xd1, 0x42, 0x20,
		0xd1, 0x43, 0x00,
		0xd1, 0x44, 0xa8,
		0xd1, 0x45, 0xa7,
		0xd1, 0x46, 0x38,
		0xd1, 0x47, 0x42,
		0xd1, 0x48, 0x9c,
		0xd1, 0x49, 0x80,
		0xd1, 0x4a, 0x00,
		0xd1, 0x4b, 0x00,
		0xd1, 0x4c, 0x9c,
		0xd1, 0x4d, 0x84,
		0xd1, 0x4e, 0x00,
		0xd1, 0x4f, 0x01,
		0xd1, 0x50, 0x8c,
		0xd1, 0x51, 0x65,
		0xd1, 0x52, 0x00,
		0xd1, 0x53, 0x00,
		0xd1, 0x54, 0xbc,
		0xd1, 0x55, 0x03,
		0xd1, 0x56, 0x00,
		0xd1, 0x57, 0x00,
		0xd1, 0x58, 0x10,
		0xd1, 0x59, 0x00,
		0xd1, 0x5a, 0x00,
		0xd1, 0x5b, 0x0c,
		0xd1, 0x5c, 0xa4,
		0xd1, 0x5d, 0x84,
		0xd1, 0x5e, 0x00,
		0xd1, 0x5f, 0xff,
		0xd1, 0x60, 0x8c,
		0xd1, 0x61, 0x66,
		0xd1, 0x62, 0x00,
		0xd1, 0x63, 0x00,
		0xd1, 0x64, 0xbc,
		0xd1, 0x65, 0x03,
		0xd1, 0x66, 0x00,
		0xd1, 0x67, 0x00,
		0xd1, 0x68, 0x10,
		0xd1, 0x69, 0x00,
		0xd1, 0x6a, 0x00,
		0xd1, 0x6b, 0x09,
		0xd1, 0x6c, 0xbc,
		0xd1, 0x6d, 0x04,
		0xd1, 0x6e, 0x00,
		0xd1, 0x6f, 0x02,
		0xd1, 0x70, 0x9c,
		0xd1, 0x71, 0x80,
		0xd1, 0x72, 0x00,
		0xd1, 0x73, 0x00,
		0xd1, 0x74, 0x8c,
		0xd1, 0x75, 0x65,
		0xd1, 0x76, 0x00,
		0xd1, 0x77, 0x00,
		0xd1, 0x78, 0x9c,
		0xd1, 0x79, 0x84,
		0xd1, 0x7a, 0x00,
		0xd1, 0x7b, 0x01,
		0xd1, 0x7c, 0xbc,
		0xd1, 0x7d, 0x03,
		0xd1, 0x7e, 0x00,
		0xd1, 0x7f, 0x00,
		0xd1, 0x80, 0x0f,
		0xd1, 0x81, 0xff,
		0xd1, 0x82, 0xff,
		0xd1, 0x83, 0xf8,
		0xd1, 0x84, 0xa4,
		0xd1, 0x85, 0x84,
		0xd1, 0x86, 0x00,
		0xd1, 0x87, 0xff,
		0xd1, 0x88, 0xbc,
		0xd1, 0x89, 0x04,
		0xd1, 0x8a, 0x00,
		0xd1, 0x8b, 0x02,
		0xd1, 0x8c, 0x0f,
		0xd1, 0x8d, 0xff,
		0xd1, 0x8e, 0xff,
		0xd1, 0x8f, 0xf1,
		0xd1, 0x90, 0x9c,
		0xd1, 0x91, 0x84,
		0xd1, 0x92, 0x00,
		0xd1, 0x93, 0x01,
		0xd1, 0x94, 0x00,
		0xd1, 0x95, 0x00,
		0xd1, 0x96, 0x00,
		0xd1, 0x97, 0x1d,
		0xd1, 0x98, 0x18,
		0xd1, 0x99, 0x60,
		0xd1, 0x9a, 0x80,
		0xd1, 0x9b, 0x06,
		0xd1, 0x9c, 0xa8,
		0xd1, 0x9d, 0x64,
		0xd1, 0x9e, 0x38,
		0xd1, 0x9f, 0x29,
		0xd1, 0xa0, 0xa8,
		0xd1, 0xa1, 0xa4,
		0xd1, 0xa2, 0x40,
		0xd1, 0xa3, 0x08,
		0xd1, 0xa4, 0x8c,
		0xd1, 0xa5, 0x63,
		0xd1, 0xa6, 0x00,
		0xd1, 0xa7, 0x00,
		0xd1, 0xa8, 0xa8,
		0xd1, 0xa9, 0xe4,
		0xd1, 0xaa, 0x38,
		0xd1, 0xab, 0x2a,
		0xd1, 0xac, 0xa8,
		0xd1, 0xad, 0xc4,
		0xd1, 0xae, 0x40,
		0xd1, 0xaf, 0x09,
		0xd1, 0xb0, 0xd8,
		0xd1, 0xb1, 0x05,
		0xd1, 0xb2, 0x18,
		0xd1, 0xb3, 0x00,
		0xd1, 0xb4, 0x8c,
		0xd1, 0xb5, 0x67,
		0xd1, 0xb6, 0x00,
		0xd1, 0xb7, 0x00,
		0xd1, 0xb8, 0xa8,
		0xd1, 0xb9, 0xa4,
		0xd1, 0xba, 0x38,
		0xd1, 0xbb, 0x24,
		0xd1, 0xbc, 0xd8,
		0xd1, 0xbd, 0x06,
		0xd1, 0xbe, 0x18,
		0xd1, 0xbf, 0x00,
		0xd1, 0xc0, 0x8c,
		0xd1, 0xc1, 0x65,
		0xd1, 0xc2, 0x00,
		0xd1, 0xc3, 0x00,
		0xd1, 0xc4, 0xa4,
		0xd1, 0xc5, 0x63,
		0xd1, 0xc6, 0x00,
		0xd1, 0xc7, 0x40,
		0xd1, 0xc8, 0xbc,
		0xd1, 0xc9, 0x23,
		0xd1, 0xca, 0x00,
		0xd1, 0xcb, 0x00,
		0xd1, 0xcc, 0x13,
		0xd1, 0xcd, 0xff,
		0xd1, 0xce, 0xff,
		0xd1, 0xcf, 0xc4,
		0xd1, 0xd0, 0xa8,
		0xd1, 0xd1, 0x64,
		0xd1, 0xd2, 0x6e,
		0xd1, 0xd3, 0x44,
		0xd1, 0xd4, 0xa8,
		0xd1, 0xd5, 0xa4,
		0xd1, 0xd6, 0x55,
		0xd1, 0xd7, 0x86,
		0xd1, 0xd8, 0x8c,
		0xd1, 0xd9, 0x63,
		0xd1, 0xda, 0x00,
		0xd1, 0xdb, 0x00,
		0xd1, 0xdc, 0xa8,
		0xd1, 0xdd, 0xc4,
		0xd1, 0xde, 0x6e,
		0xd1, 0xdf, 0x45,
		0xd1, 0xe0, 0xa8,
		0xd1, 0xe1, 0xe4,
		0xd1, 0xe2, 0x55,
		0xd1, 0xe3, 0x87,
		0xd1, 0xe4, 0xd8,
		0xd1, 0xe5, 0x05,
		0xd1, 0xe6, 0x18,
		0xd1, 0xe7, 0x00,
		0xd1, 0xe8, 0x8c,
		0xd1, 0xe9, 0x66,
		0xd1, 0xea, 0x00,
		0xd1, 0xeb, 0x00,
		0xd1, 0xec, 0xa8,
		0xd1, 0xed, 0xa4,
		0xd1, 0xee, 0x6e,
		0xd1, 0xef, 0x46,
		0xd1, 0xf0, 0xd8,
		0xd1, 0xf1, 0x07,
		0xd1, 0xf2, 0x18,
		0xd1, 0xf3, 0x00,
		0xd1, 0xf4, 0xa8,
		0xd1, 0xf5, 0x84,
		0xd1, 0xf6, 0x55,
		0xd1, 0xf7, 0x88,
		0xd1, 0xf8, 0x8c,
		0xd1, 0xf9, 0x65,
		0xd1, 0xfa, 0x00,
		0xd1, 0xfb, 0x00,
		0xd1, 0xfc, 0xd8,
		0xd1, 0xfd, 0x04,
		0xd1, 0xfe, 0x18,
		0xd1, 0xff, 0x00,
		0xd2, 0x00, 0x03,
		0xd2, 0x01, 0xff,
		0xd2, 0x02, 0xff,
		0xd2, 0x03, 0xb8,
		0xd2, 0x04, 0x18,
		0xd2, 0x05, 0xe0,
		0xd2, 0x06, 0x80,
		0xd2, 0x07, 0x06,
		0xd2, 0x08, 0xa8,
		0xd2, 0x09, 0x83,
		0xd2, 0x0a, 0x6e,
		0xd2, 0x0b, 0x43,
		0xd2, 0x0c, 0xa8,
		0xd2, 0x0d, 0xe3,
		0xd2, 0x0e, 0x38,
		0xd2, 0x0f, 0x0f,
		0xd2, 0x10, 0x8c,
		0xd2, 0x11, 0x84,
		0xd2, 0x12, 0x00,
		0xd2, 0x13, 0x00,
		0xd2, 0x14, 0xa8,
		0xd2, 0x15, 0xa3,
		0xd2, 0x16, 0x38,
		0xd2, 0x17, 0x0e,
		0xd2, 0x18, 0xa8,
		0xd2, 0x19, 0xc3,
		0xd2, 0x1a, 0x6e,
		0xd2, 0x1b, 0x42,
		0xd2, 0x1c, 0xd8,
		0xd2, 0x1d, 0x07,
		0xd2, 0x1e, 0x20,
		0xd2, 0x1f, 0x00,
		0xd2, 0x20, 0x8c,
		0xd2, 0x21, 0x66,
		0xd2, 0x22, 0x00,
		0xd2, 0x23, 0x00,
		0xd2, 0x24, 0xd8,
		0xd2, 0x25, 0x05,
		0xd2, 0x26, 0x18,
		0xd2, 0x27, 0x00,
		0xd2, 0x28, 0x44,
		0xd2, 0x29, 0x00,
		0xd2, 0x2a, 0x48,
		0xd2, 0x2b, 0x00,
		0xd2, 0x2c, 0x15,
		0xd2, 0x2d, 0x00,
		0xd2, 0x2e, 0x00,
		0xd2, 0x2f, 0x00,
		0xd2, 0x30, 0x9c,
		0xd2, 0x31, 0x21,
		0xd2, 0x32, 0xff,
		0xd2, 0x33, 0xfc,
		0xd2, 0x34, 0xd4,
		0xd2, 0x35, 0x01,
		0xd2, 0x36, 0x48,
		0xd2, 0x37, 0x00,
		0xd2, 0x38, 0x18,
		0xd2, 0x39, 0x60,
		0xd2, 0x3a, 0x00,
		0xd2, 0x3b, 0x01,
		0xd2, 0x3c, 0xa8,
		0xd2, 0x3d, 0x63,
		0xd2, 0x3e, 0x07,
		0xd2, 0x3f, 0x80,
		0xd2, 0x40, 0x8c,
		0xd2, 0x41, 0x63,
		0xd2, 0x42, 0x00,
		0xd2, 0x43, 0x68,
		0xd2, 0x44, 0xbc,
		0xd2, 0x45, 0x03,
		0xd2, 0x46, 0x00,
		0xd2, 0x47, 0x00,
		0xd2, 0x48, 0x10,
		0xd2, 0x49, 0x00,
		0xd2, 0x4a, 0x00,
		0xd2, 0x4b, 0x0c,
		0xd2, 0x4c, 0x15,
		0xd2, 0x4d, 0x00,
		0xd2, 0x4e, 0x00,
		0xd2, 0x4f, 0x00,
		0xd2, 0x50, 0x07,
		0xd2, 0x51, 0xff,
		0xd2, 0x52, 0xda,
		0xd2, 0x53, 0x00,
		0xd2, 0x54, 0x15,
		0xd2, 0x55, 0x00,
		0xd2, 0x56, 0x00,
		0xd2, 0x57, 0x00,
		0xd2, 0x58, 0x18,
		0xd2, 0x59, 0x60,
		0xd2, 0x5a, 0x80,
		0xd2, 0x5b, 0x06,
		0xd2, 0x5c, 0xa8,
		0xd2, 0x5d, 0x63,
		0xd2, 0x5e, 0xc4,
		0xd2, 0x5f, 0xb8,
		0xd2, 0x60, 0x8c,
		0xd2, 0x61, 0x63,
		0xd2, 0x62, 0x00,
		0xd2, 0x63, 0x00,
		0xd2, 0x64, 0xbc,
		0xd2, 0x65, 0x23,
		0xd2, 0x66, 0x00,
		0xd2, 0x67, 0x01,
		0xd2, 0x68, 0x10,
		0xd2, 0x69, 0x00,
		0xd2, 0x6a, 0x00,
		0xd2, 0x6b, 0x25,
		0xd2, 0x6c, 0x9d,
		0xd2, 0x6d, 0x00,
		0xd2, 0x6e, 0x00,
		0xd2, 0x6f, 0x00,
		0xd2, 0x70, 0x00,
		0xd2, 0x71, 0x00,
		0xd2, 0x72, 0x00,
		0xd2, 0x73, 0x0b,
		0xd2, 0x74, 0xb8,
		0xd2, 0x75, 0xe8,
		0xd2, 0x76, 0x00,
		0xd2, 0x77, 0x02,
		0xd2, 0x78, 0x07,
		0xd2, 0x79, 0xff,
		0xd2, 0x7a, 0xd6,
		0xd2, 0x7b, 0x8c,
		0xd2, 0x7c, 0x15,
		0xd2, 0x7d, 0x00,
		0xd2, 0x7e, 0x00,
		0xd2, 0x7f, 0x00,
		0xd2, 0x80, 0x18,
		0xd2, 0x81, 0x60,
		0xd2, 0x82, 0x80,
		0xd2, 0x83, 0x06,
		0xd2, 0x84, 0xa8,
		0xd2, 0x85, 0x63,
		0xd2, 0x86, 0xc4,
		0xd2, 0x87, 0xb8,
		0xd2, 0x88, 0x8c,
		0xd2, 0x89, 0x63,
		0xd2, 0x8a, 0x00,
		0xd2, 0x8b, 0x00,
		0xd2, 0x8c, 0xbc,
		0xd2, 0x8d, 0x23,
		0xd2, 0x8e, 0x00,
		0xd2, 0x8f, 0x01,
		0xd2, 0x90, 0x10,
		0xd2, 0x91, 0x00,
		0xd2, 0x92, 0x00,
		0xd2, 0x93, 0x1b,
		0xd2, 0x94, 0x9d,
		0xd2, 0x95, 0x00,
		0xd2, 0x96, 0x00,
		0xd2, 0x97, 0x00,
		0xd2, 0x98, 0xb8,
		0xd2, 0x99, 0xe8,
		0xd2, 0x9a, 0x00,
		0xd2, 0x9b, 0x02,
		0xd2, 0x9c, 0x9c,
		0xd2, 0x9d, 0xc0,
		0xd2, 0x9e, 0x00,
		0xd2, 0x9f, 0x00,
		0xd2, 0xa0, 0x18,
		0xd2, 0xa1, 0xa0,
		0xd2, 0xa2, 0x80,
		0xd2, 0xa3, 0x06,
		0xd2, 0xa4, 0xe0,
		0xd2, 0xa5, 0x67,
		0xd2, 0xa6, 0x30,
		0xd2, 0xa7, 0x00,
		0xd2, 0xa8, 0xa8,
		0xd2, 0xa9, 0xa5,
		0xd2, 0xaa, 0xce,
		0xd2, 0xab, 0xb0,
		0xd2, 0xac, 0x19,
		0xd2, 0xad, 0x60,
		0xd2, 0xae, 0x00,
		0xd2, 0xaf, 0x01,
		0xd2, 0xb0, 0xa9,
		0xd2, 0xb1, 0x6b,
		0xd2, 0xb2, 0x06,
		0xd2, 0xb3, 0x14,
		0xd2, 0xb4, 0xe0,
		0xd2, 0xb5, 0x83,
		0xd2, 0xb6, 0x28,
		0xd2, 0xb7, 0x00,
		0xd2, 0xb8, 0x9c,
		0xd2, 0xb9, 0xc6,
		0xd2, 0xba, 0x00,
		0xd2, 0xbb, 0x01,
		0xd2, 0xbc, 0xe0,
		0xd2, 0xbd, 0x63,
		0xd2, 0xbe, 0x18,
		0xd2, 0xbf, 0x00,
		0xd2, 0xc0, 0x8c,
		0xd2, 0xc1, 0x84,
		0xd2, 0xc2, 0x00,
		0xd2, 0xc3, 0x00,
		0xd2, 0xc4, 0xe0,
		0xd2, 0xc5, 0xa3,
		0xd2, 0xc6, 0x58,
		0xd2, 0xc7, 0x00,
		0xd2, 0xc8, 0xa4,
		0xd2, 0xc9, 0xc6,
		0xd2, 0xca, 0x00,
		0xd2, 0xcb, 0xff,
		0xd2, 0xcc, 0xb8,
		0xd2, 0xcd, 0x64,
		0xd2, 0xce, 0x00,
		0xd2, 0xcf, 0x18,
		0xd2, 0xd0, 0xbc,
		0xd2, 0xd1, 0x46,
		0xd2, 0xd2, 0x00,
		0xd2, 0xd3, 0x03,
		0xd2, 0xd4, 0x94,
		0xd2, 0xd5, 0x85,
		0xd2, 0xd6, 0x00,
		0xd2, 0xd7, 0x00,
		0xd2, 0xd8, 0xb8,
		0xd2, 0xd9, 0x63,
		0xd2, 0xda, 0x00,
		0xd2, 0xdb, 0x98,
		0xd2, 0xdc, 0xe0,
		0xd2, 0xdd, 0x64,
		0xd2, 0xde, 0x18,
		0xd2, 0xdf, 0x00,
		0xd2, 0xe0, 0x0f,
		0xd2, 0xe1, 0xff,
		0xd2, 0xe2, 0xff,
		0xd2, 0xe3, 0xf0,
		0xd2, 0xe4, 0xdc,
		0xd2, 0xe5, 0x05,
		0xd2, 0xe6, 0x18,
		0xd2, 0xe7, 0x00,
		0xd2, 0xe8, 0x9c,
		0xd2, 0xe9, 0x68,
		0xd2, 0xea, 0x00,
		0xd2, 0xeb, 0x01,
		0xd2, 0xec, 0xa5,
		0xd2, 0xed, 0x03,
		0xd2, 0xee, 0x00,
		0xd2, 0xef, 0xff,
		0xd2, 0xf0, 0xbc,
		0xd2, 0xf1, 0x48,
		0xd2, 0xf2, 0x00,
		0xd2, 0xf3, 0x01,
		0xd2, 0xf4, 0x0f,
		0xd2, 0xf5, 0xff,
		0xd2, 0xf6, 0xff,
		0xd2, 0xf7, 0xea,
		0xd2, 0xf8, 0xb8,
		0xd2, 0xf9, 0xe8,
		0xd2, 0xfa, 0x00,
		0xd2, 0xfb, 0x02,
		0xd2, 0xfc, 0x18,
		0xd2, 0xfd, 0x60,
		0xd2, 0xfe, 0x00,
		0xd2, 0xff, 0x01,
		0xd3, 0x00, 0xa8,
		0xd3, 0x01, 0x63,
		0xd3, 0x02, 0x06,
		0xd3, 0x03, 0x14,
		0xd3, 0x04, 0x07,
		0xd3, 0x05, 0xff,
		0xd3, 0x06, 0xe4,
		0xd3, 0x07, 0x6d,
		0xd3, 0x08, 0x9c,
		0xd3, 0x09, 0x83,
		0xd3, 0x0a, 0x00,
		0xd3, 0x0b, 0x10,
		0xd3, 0x0c, 0x85,
		0xd3, 0x0d, 0x21,
		0xd3, 0x0e, 0x00,
		0xd3, 0x0f, 0x00,
		0xd3, 0x10, 0x44,
		0xd3, 0x11, 0x00,
		0xd3, 0x12, 0x48,
		0xd3, 0x13, 0x00,
		0xd3, 0x14, 0x9c,
		0xd3, 0x15, 0x21,
		0xd3, 0x16, 0x00,
		0xd3, 0x17, 0x04,
		0xd3, 0x18, 0x18,
		0xd3, 0x19, 0x60,
		0xd3, 0x1a, 0x00,
		0xd3, 0x1b, 0x01,
		0xd3, 0x1c, 0x9c,
		0xd3, 0x1d, 0x80,
		0xd3, 0x1e, 0xff,
		0xd3, 0x1f, 0xff,
		0xd3, 0x20, 0xa8,
		0xd3, 0x21, 0x63,
		0xd3, 0x22, 0x09,
		0xd3, 0x23, 0xef,
		0xd3, 0x24, 0xd8,
		0xd3, 0x25, 0x03,
		0xd3, 0x26, 0x20,
		0xd3, 0x27, 0x00,
		0xd3, 0x28, 0x18,
		0xd3, 0x29, 0x60,
		0xd3, 0x2a, 0x80,
		0xd3, 0x2b, 0x06,
		0xd3, 0x2c, 0xa8,
		0xd3, 0x2d, 0x63,
		0xd3, 0x2e, 0xc9,
		0xd3, 0x2f, 0xef,
		0xd3, 0x30, 0xd8,
		0xd3, 0x31, 0x03,
		0xd3, 0x32, 0x20,
		0xd3, 0x33, 0x00,
		0xd3, 0x34, 0x44,
		0xd3, 0x35, 0x00,
		0xd3, 0x36, 0x48,
		0xd3, 0x37, 0x00,
		0xd3, 0x38, 0x15,
		0xd3, 0x39, 0x00,
		0xd3, 0x3a, 0x00,
		0xd3, 0x3b, 0x00,
		0xd3, 0x3c, 0x18,
		0xd3, 0x3d, 0x80,
		0xd3, 0x3e, 0x00,
		0xd3, 0x3f, 0x01,
		0xd3, 0x40, 0xa8,
		0xd3, 0x41, 0x84,
		0xd3, 0x42, 0x0a,
		0xd3, 0x43, 0x12,
		0xd3, 0x44, 0x8c,
		0xd3, 0x45, 0x64,
		0xd3, 0x46, 0x00,
		0xd3, 0x47, 0x00,
		0xd3, 0x48, 0xbc,
		0xd3, 0x49, 0x03,
		0xd3, 0x4a, 0x00,
		0xd3, 0x4b, 0x00,
		0xd3, 0x4c, 0x13,
		0xd3, 0x4d, 0xff,
		0xd3, 0x4e, 0xff,
		0xd3, 0x4f, 0xfe,
		0xd3, 0x50, 0x15,
		0xd3, 0x51, 0x00,
		0xd3, 0x52, 0x00,
		0xd3, 0x53, 0x00,
		0xd3, 0x54, 0x44,
		0xd3, 0x55, 0x00,
		0xd3, 0x56, 0x48,
		0xd3, 0x57, 0x00,
		0xd3, 0x58, 0x15,
		0xd3, 0x59, 0x00,
		0xd3, 0x5a, 0x00,
		0xd3, 0x5b, 0x00,
		0xd3, 0x5c, 0x00,
		0xd3, 0x5d, 0x00,
		0xd3, 0x5e, 0x00,
		0xd3, 0x5f, 0x00,
		0xd3, 0x60, 0x00,
		0xd3, 0x61, 0x00,
		0xd3, 0x62, 0x00,
		0xd3, 0x63, 0x00,
		0x6f, 0x0e, 0x33,
		0x6f, 0x0f, 0x33,
		0x46, 0x0e, 0x08,
		0x46, 0x0f, 0x01,
		0x46, 0x10, 0x00,
		0x46, 0x11, 0x01,
		0x46, 0x12, 0x00,
		0x46, 0x13, 0x01,
		0x46, 0x05, 0x00,
		0x46, 0x08, 0x00,
		0x46, 0x09, 0x08,
		0x68, 0x04, 0x00,
		0x68, 0x05, 0x06,
		0x68, 0x06, 0x00,
		0x51, 0x20, 0x00,
		0x35, 0x10, 0x00,
		0x35, 0x04, 0x00,
		0x68, 0x00, 0x00,
		0x6f, 0x0d, 0x0f,
		0x50, 0x00, 0xff,
		0x50, 0x01, 0xbf,
		0x50, 0x02, 0x7e,
		0x50, 0x3d, 0x00,
		0xc4, 0x50, 0x01,
		0xc4, 0x52, 0x04,
		0xc4, 0x53, 0x00,
		0xc4, 0x54, 0x00,
		0xc4, 0x55, 0x00,
		0xc4, 0x56, 0x00,
		0xc4, 0x57, 0x00,
		0xc4, 0x58, 0x00,
		0xc4, 0x59, 0x00,
		0xc4, 0x5b, 0x00,
		0xc4, 0x5c, 0x00,
		0xc4, 0x5d, 0x00,
		0xc4, 0x5e, 0x00,
		0xc4, 0x5f, 0x00,
		0xc4, 0x60, 0x00,
		0xc4, 0x61, 0x01,
		0xc4, 0x62, 0x01,
		0xc4, 0x64, 0x88,
		0xc4, 0x65, 0x00,
		0xc4, 0x66, 0x8a,
		0xc4, 0x67, 0x00,
		0xc4, 0x68, 0x86,
		0xc4, 0x69, 0x00,
		0xc4, 0x6a, 0x40,
		0xc4, 0x6b, 0x50,
		0xc4, 0x6c, 0x30,
		0xc4, 0x6d, 0x28,
		0xc4, 0x6e, 0x60,
		0xc4, 0x6f, 0x40,
		0xc4, 0x7c, 0x01,
		0xc4, 0x7d, 0x38,
		0xc4, 0x7e, 0x00,
		0xc4, 0x7f, 0x00,
		0xc4, 0x80, 0x00,
		0xc4, 0x81, 0xff,
		0xc4, 0x82, 0x00,
		0xc4, 0x83, 0x40,
		0xc4, 0x84, 0x00,
		0xc4, 0x85, 0x18,
		0xc4, 0x86, 0x00,
		0xc4, 0x87, 0x18,
		0xc4, 0x88, 0x34,
		0xc4, 0x89, 0x00,
		0xc4, 0x8a, 0x34,
		0xc4, 0x8b, 0x00,
		0xc4, 0x8c, 0x00,
		0xc4, 0x8d, 0x04,
		0xc4, 0x8e, 0x00,
		0xc4, 0x8f, 0x04,
		0xc4, 0x90, 0x07,
		0xc4, 0x92, 0x20,
		0xc4, 0x93, 0x08,
		0xc4, 0x98, 0x02,
		0xc4, 0x99, 0x00,
		0xc4, 0x9a, 0x02,
		0xc4, 0x9b, 0x00,
		0xc4, 0x9c, 0x02,
		0xc4, 0x9d, 0x00,
		0xc4, 0x9e, 0x02,
		0xc4, 0x9f, 0x60,
		0xc4, 0xa0, 0x03,
		0xc4, 0xa1, 0x00,
		0xc4, 0xa2, 0x04,
		0xc4, 0xa3, 0x00,
		0xc4, 0xa4, 0x00,
		0xc4, 0xa5, 0x10,
		0xc4, 0xa6, 0x00,
		0xc4, 0xa7, 0x40,
		0xc4, 0xa8, 0x00,
		0xc4, 0xa9, 0x80,
		0xc4, 0xaa, 0x0d,
		0xc4, 0xab, 0x00,
		0xc4, 0xac, 0x0f,
		0xc4, 0xad, 0xc0,
		0xc4, 0xb4, 0x01,
		0xc4, 0xb5, 0x01,
		0xc4, 0xb6, 0x00,
		0xc4, 0xb7, 0x01,
		0xc4, 0xb8, 0x00,
		0xc4, 0xb9, 0x01,
		0xc4, 0xba, 0x01,
		0xc4, 0xbb, 0x00,
		0xc4, 0xbc, 0x01,
		0xc4, 0xbd, 0x60,
		0xc4, 0xbe, 0x02,
		0xc4, 0xbf, 0x33,
		0xc4, 0xc8, 0x03,
		0xc4, 0xc9, 0xd0,
		0xc4, 0xca, 0x0e,
		0xc4, 0xcb, 0x00,
		0xc4, 0xcc, 0x10,
		0xc4, 0xcd, 0x18,
		0xc4, 0xce, 0x10,
		0xc4, 0xcf, 0x18,
		0xc4, 0xd0, 0x04,
		0xc4, 0xd1, 0x80,
		0xc4, 0xe0, 0x04,
		0xc4, 0xe1, 0x02,
		0xc4, 0xe2, 0x01,
		0xc4, 0xe4, 0x10,
		0xc4, 0xe5, 0x20,
		0xc4, 0xe6, 0x30,
		0xc4, 0xe7, 0x40,
		0xc4, 0xe8, 0x50,
		0xc4, 0xe9, 0x60,
		0xc4, 0xea, 0x70,
		0xc4, 0xeb, 0x80,
		0xc4, 0xec, 0x90,
		0xc4, 0xed, 0xa0,
		0xc4, 0xee, 0xb0,
		0xc4, 0xef, 0xc0,
		0xc4, 0xf0, 0xd0,
		0xc4, 0xf1, 0xe0,
		0xc4, 0xf2, 0xf0,
		0xc4, 0xf3, 0x80,
		0xc4, 0xf4, 0x00,
		0xc4, 0xf5, 0x20,
		0xc4, 0xf6, 0x02,
		0xc4, 0xf7, 0x00,
		0xc4, 0xf8, 0x04,
		0xc4, 0xf9, 0x0b,
		0xc4, 0xfa, 0x00,
		0xc4, 0xfb, 0x01,
		0xc4, 0xfc, 0x01,
		0xc4, 0xfd, 0x00,
		0xc4, 0xfe, 0x04,
		0xc4, 0xff, 0x02,
		0xc5, 0x00, 0x48,
		0xc5, 0x01, 0x74,
		0xc5, 0x02, 0x58,
		0xc5, 0x03, 0x80,
		0xc5, 0x04, 0x05,
		0xc5, 0x05, 0x80,
		0xc5, 0x06, 0x03,
		0xc5, 0x07, 0x80,
		0xc5, 0x08, 0x01,
		0xc5, 0x09, 0xc0,
		0xc5, 0x0a, 0x01,
		0xc5, 0x0b, 0xa0,
		0xc5, 0x0c, 0x01,
		0xc5, 0x0d, 0x2c,
		0xc5, 0x0e, 0x01,
		0xc5, 0x0f, 0x0a,
		0xc5, 0x10, 0x00,
		0xc5, 0x11, 0x01,
		0xc5, 0x12, 0x01,
		0xc5, 0x13, 0x80,
		0xc5, 0x14, 0x04,
		0xc5, 0x15, 0x00,
		0xc5, 0x18, 0x03,
		0xc5, 0x19, 0x48,
		0xc5, 0x1a, 0x07,
		0xc5, 0x1b, 0x70,
		0xc2, 0xe0, 0x00,
		0xc2, 0xe1, 0x51,
		0xc2, 0xe2, 0x00,
		0xc2, 0xe3, 0xd6,
		0xc2, 0xe4, 0x01,
		0xc2, 0xe5, 0x5e,
		0xc2, 0xe9, 0x01,
		0xc2, 0xea, 0x7a,
		0xc2, 0xeb, 0x90,
		0xc2, 0xed, 0x00,
		0xc2, 0xee, 0x7a,
		0xc2, 0xef, 0x64,
		0xc3, 0x08, 0x00,
		0xc3, 0x09, 0x00,
		0xc3, 0x0a, 0x00,
		0xc3, 0x0c, 0x00,
		0xc3, 0x0d, 0x01,
		0xc3, 0x0e, 0x00,
		0xc3, 0x0f, 0x00,
		0xc3, 0x10, 0x01,
		0xc3, 0x11, 0x60,
		0xc3, 0x12, 0xff,
		0xc3, 0x13, 0x08,
		0xc3, 0x14, 0x01,
		0xc3, 0x15, 0x7f,
		0xc3, 0x16, 0xff,
		0xc3, 0x17, 0x0b,
		0xc3, 0x18, 0x00,
		0xc3, 0x19, 0x0c,
		0xc3, 0x1a, 0x00,
		0xc3, 0x1b, 0xe0,
		0xc3, 0x1c, 0x00,
		0xc3, 0x1d, 0x14,
		0xc3, 0x1e, 0x00,
		0xc3, 0x1f, 0xc5,
		0xc3, 0x20, 0xff,
		0xc3, 0x21, 0x4b,
		0xc3, 0x22, 0xff,
		0xc3, 0x23, 0xf0,
		0xc3, 0x24, 0xff,
		0xc3, 0x25, 0xe8,
		0xc3, 0x26, 0x00,
		0xc3, 0x27, 0x46,
		0xc3, 0x28, 0xff,
		0xc3, 0x29, 0xd2,
		0xc3, 0x2a, 0xff,
		0xc3, 0x2b, 0xe4,
		0xc3, 0x2c, 0xff,
		0xc3, 0x2d, 0xbb,
		0xc3, 0x2e, 0x00,
		0xc3, 0x2f, 0x61,
		0xc3, 0x30, 0xff,
		0xc3, 0x31, 0xf9,
		0xc3, 0x32, 0x00,
		0xc3, 0x33, 0xd9,
		0xc3, 0x34, 0x00,
		0xc3, 0x35, 0x2e,
		0xc3, 0x36, 0x00,
		0xc3, 0x37, 0xb1,
		0xc3, 0x38, 0xff,
		0xc3, 0x39, 0x64,
		0xc3, 0x3a, 0xff,
		0xc3, 0x3b, 0xeb,
		0xc3, 0x3c, 0xff,
		0xc3, 0x3d, 0xe8,
		0xc3, 0x3e, 0x00,
		0xc3, 0x3f, 0x48,
		0xc3, 0x40, 0xff,
		0xc3, 0x41, 0xd0,
		0xc3, 0x42, 0xff,
		0xc3, 0x43, 0xed,
		0xc3, 0x44, 0xff,
		0xc3, 0x45, 0xad,
		0xc3, 0x46, 0x00,
		0xc3, 0x47, 0x66,
		0xc3, 0x48, 0x01,
		0xc3, 0x49, 0x00,
		0x67, 0x00, 0x04,
		0x67, 0x01, 0x7b,
		0x67, 0x02, 0xfd,
		0x67, 0x03, 0xf9,
		0x67, 0x04, 0x3d,
		0x67, 0x05, 0x71,
		0x67, 0x06, 0x78,
		0x67, 0x08, 0x05,
		0x6f, 0x06, 0x6f,
		0x6f, 0x07, 0x00,
		0x6f, 0x0a, 0x6f,
		0x6f, 0x0b, 0x00,
		0x6f, 0x00, 0x03,
		0xc3, 0x4c, 0x01,
		0xc3, 0x4d, 0x00,
		0xc3, 0x4e, 0x46,
		0xc3, 0x4f, 0x55,
		0xc3, 0x50, 0x00,
		0xc3, 0x51, 0x40,
		0xc3, 0x52, 0x00,
		0xc3, 0x53, 0xff,
		0xc3, 0x54, 0x04,
		0xc3, 0x55, 0x08,
		0xc3, 0x56, 0x01,
		0xc3, 0x57, 0xef,
		0xc3, 0x58, 0x30,
		0xc3, 0x59, 0x01,
		0xc3, 0x5a, 0x64,
		0xc3, 0x5b, 0x46,
		0xc3, 0x5c, 0x00,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x42, 0xf0,
		0x30, 0x1b, 0xf0,
		0x30, 0x1c, 0xf0,
		0x30, 0x1a, 0xf0,
		0x47, 0x09, 0x10,	
		0xce, 0xb0, 0x00,
		0xce, 0xb1, 0x00,
		0xce, 0xb2, 0x00,
		0xce, 0xb3, 0x00,
		0xce, 0xb4, 0x00,
		0xce, 0xb5, 0x00,
		0xce, 0xb6, 0x00,
		0xce, 0xb7, 0x00,
		0xc4, 0xbc, 0x01,
		0xc4, 0xbd, 0x60

}; 

static uint8_t Ov10635_FsinTable[] = {
		0x38, 0x32, 0x01,
		0x38, 0x33, 0x1A,
		0x38, 0x34, 0x03,
		0x38, 0x35, 0x48,
		0x30, 0x2E, 0x01
    }; 

#endif /* OV10635CONFIGMAX_H */
