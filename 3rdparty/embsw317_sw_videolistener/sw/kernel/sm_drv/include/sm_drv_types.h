/**
  @addtogroup       dxgrSMKERNELMOD
  @{

  @file             sm_drv_types.h
  @brief            IOCTL interface definition for the firmware driver
  @details

  Copyright (c) 2016 NXP Semiconductors
  
  All Rights Reserved.
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
 
#ifndef __SM_DRV_TYPES_H
#define __SM_DRV_TYPES_H

typedef struct {
    long long s64_val0;
    long long s64_val1;
} sm_drv_sc_param_t;

#define SM_DRV_MAGIC                    229

#define SM_DRV_IOCTL_INIT               _IOR(SM_DRV_MAGIC, 0, sm_drv_sc_param_t *)
#define SM_DRV_IOCTL_START              _IOR(SM_DRV_MAGIC, 1, sm_drv_sc_param_t *)
#define SM_DRV_IOCTL_STOP               _IOR(SM_DRV_MAGIC, 2, sm_drv_sc_param_t *)
#define SM_DRV_IOCTL_SET_CFG            _IOWR(SM_DRV_MAGIC, 3, sm_drv_sc_param_t *)
#define SM_DRV_IOCTL_GET_CFG            _IOWR(SM_DRV_MAGIC, 4, sm_drv_sc_param_t *)
#define SM_DRV_IOCTL_REG_SIG            _IOW(SM_DRV_MAGIC, 5, pid_t *)
#define SM_DRV_IOCTL_UNREG_SIG          _IOW(SM_DRV_MAGIC, 6, pid_t *)
#define SM_DRV_IOCTL_ENABLE_EVENTS      _IOW(SM_DRV_MAGIC, 7, sm_drv_sc_param_t *)

#endif /* __SM_DRV_TYPES_H */
/** @}*/
