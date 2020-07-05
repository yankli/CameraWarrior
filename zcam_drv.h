#ifndef __ZCAM_DRV_H_
#define __ZCAM_DRV_H_

#include <stdio.h>
#include <string.h>
#include "zcam_util.h"


typedef struct _drv_ops {
    char name[64];
    int (*ireg_r)(struct _drv_ops* drv, int bus, int slave, unsigned int reg, unsigned char* val);
    int (*ireg_w)(struct _drv_ops* drv, int bus, int slave, unsigned int reg, unsigned char val);
    int (*freg_r)(struct _drv_ops* drv, int bus, int slave, unsigned int reg, unsigned char* val);
    int (*freg_w)(struct _drv_ops* drv, int bus, int slave, unsigned int reg, unsigned char val);
    int (*fdump)(struct _drv_ops* drv, int bus, int slave, int start, int bytes, unsigned char* buf);
    int (*fburn)(struct _drv_ops* drv, int bus, int slave, int start, int bytes, unsigned char* buf);
    int (*probe)(struct _drv_ops* drv, int bus, int slave);
    
    int (*get_version)(struct _drv_ops* drv, int bus, int slave, char* str_ver);
    int (*get_userdata)(struct _drv_ops* drv, int bus, int slave, char* data);
} DRV_OPS, *PDRV_OPS;


int zcam_drv_register(DRV_OPS *pdrv);

/************************************************/
/* 1. macro definition */

#define MK_INIT_FUNC(mod) int zcam_drv_##mod##_init() \
{ \
    DPR(DDD, "zcam driver: ["#mod "] try to register.\n"); \
    zcam_drv_register(&zcam_drv_ops_##mod); \
    return 0; \
}


#define REF_INIT_FUNC(mod) zcam_drv_##mod##_init

#define BOOTING_CALL_FUNC(mod,__func__) static void* drv_obj_##mod = (void *)__func__()

#define BOOTING_CALL_MOD(mod) MK_INIT_FUNC(mod) \
    BOOTING_CALL_FUNC(mod, REF_INIT_FUNC(mod))



#endif
