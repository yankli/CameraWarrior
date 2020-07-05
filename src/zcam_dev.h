/*************************************************************************
    > File Name: zcam_dev.h
    > Author: yank
    > Mail: yanchang.li@congmutech.com 
    > Created Time: 2020年07月04日 星期六 10时14分59秒
 ************************************************************************/
#ifndef __ZCAM_DEV_H_
#define __ZCAM_DEV_H_

#include "zcam_drv.h"

typedef struct _zcam_dev {
    int i2c_bus;
    int i2c_addr;

    int ch;
    char dev_type[64];
    int match_score;

    PDRV_OPS drv;

    public:
    _zcam_dev():match_score(0), drv(NULL), i2c_addr(-1){}

} CAM_DEV, *PCAM_DEV;



#endif
