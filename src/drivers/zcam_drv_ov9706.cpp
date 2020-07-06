/*************************************************************************
    > File Name: zcam_drv_ov9706.cpp
    > Author: yank
    > Mail: yanchang.li@congmutech.com 
    > Created Time: 2020年07月04日 星期六 10时14分59秒
 ************************************************************************/

#include "zcam_drv.h"
#include <string>


static int ov9706_ispreg_rd(struct _drv_ops* drv, int bus, int slave, 
    unsigned int reg, unsigned char* val)
{
    *val = (unsigned char)0xff;
    DPR(DDD, "#\n");
    return true;
}

static int ov9706_ispreg_wr(struct _drv_ops* drv, int bus, int slave,
    unsigned int reg, unsigned char val)
{
    //TODO: low level write.
    DPR(DDD, "#\n");
    return true;
}

static int ov9706_flashreg_rd(struct _drv_ops* drv, int bus, int slave,
    unsigned int reg, unsigned char* val)
{
    *val = (unsigned char)0xff;
    DPR(DDD, "#\n");
    return true;
}

static int ov9706_flashreg_wr(struct _drv_ops* drv, int bus, int slave,
    unsigned int reg, unsigned char val)
{
    //TODO: low level write.
    DPR(DDD, "#\n");
    return true;
}

static int ov9706_flash_dump(struct _drv_ops* drv, int bus, int slave,
    int start, int bytes, unsigned char *buf)
{
    memset(buf, 0xab, bytes);
    DPR(DDD, "#\n");
    return true;
}

static int ov9706_flash_burn(struct _drv_ops* drv, int bus, int slave,
    int f_addr, int bytes, unsigned char *buf)
{
    //TODO: low level burn from buf to flash.
    DPR(DDD, "#\n");
    return true;
}

static SCORE ov9706_probe(struct _drv_ops* drv, int bus, int slave)
{
    int score = DET_NOMATCH;

    return score;
}

static int ov9706_get_version(struct _drv_ops* drv, int bus, int slave, char *str_ver)
{
    return true;
}


static int ov9706_get_userdata(struct _drv_ops* drv, int bus, int slave, char *buf)
{

    return true;
}


DRV_OPS zcam_drv_ops_ov9706 = {
    "ov9706",
    ov9706_ispreg_rd,
    ov9706_ispreg_wr,
    ov9706_flashreg_rd,
    ov9706_flashreg_wr,
    ov9706_flash_dump,
    ov9706_flash_burn,
    ov9706_probe,
    ov9706_get_version,
    ov9706_get_version,
};

BOOTING_CALL_MOD(ov9706);