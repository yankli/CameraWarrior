#include "zcam_drv.h"
#include <string>
#include <iostream>


static int ar0143_ispreg_rd(struct _drv_ops* drv, int bus, int slave, 
    unsigned int reg, unsigned char* val)
{
    *val = (unsigned char)0xff;
    DPR(DDD, "#\n");
    return true;
}

static int ar0143_ispreg_wr(struct _drv_ops* drv, int bus, int slave,
    unsigned int reg, unsigned char val)
{
    //TODO: low level write.
    DPR(DDD, "#\n");
    return true;
}

static int ar0143_flashreg_rd(struct _drv_ops* drv, int bus, int slave,
    unsigned int reg, unsigned char* val)
{
    *val = (unsigned char)0xff;
    DPR(DDD, "#\n");
    return true;
}

static int ar0143_flashreg_wr(struct _drv_ops* drv, int bus, int slave,
    unsigned int reg, unsigned char val)
{
    //TODO: low level write.
    DPR(DDD, "#\n");
    return true;
}

static int ar0143_flash_dump(struct _drv_ops* drv, int bus, int slave,
    int start, int bytes, unsigned char *buf)
{
    memset(buf, 0xff, bytes);
    DPR(DDD, "#\n");
    return true;
}

static int ar0143_flash_burn(struct _drv_ops* drv, int bus, int slave,
    int f_addr, int bytes, unsigned char *buf)
{
    //TODO: low level burn from buf to flash.
    DPR(DDD, "#\n");
    /* if( f_addr < 0) 
            f_addr = load_burn_info(xx.cfg);
    */
    if( f_addr < 0) 
            f_addr = 0xff00;
    
    return true;
}

static SCORE ar0143_probe(struct _drv_ops* drv, int bus, int slave)
{
    int score = DET_NOMATCH;

    /* should load from ar0143_cfg.xml file */
    std::vector<unsigned int> probe_iregs;
    std::vector<unsigned int> probe_fregs;
    
    probe_iregs = get_cam_cfg_regs((std::string("./cameras/") + std::string(drv->name) + std::string("_cfg.txt")).c_str(), 1);
    probe_fregs = get_cam_cfg_regs((std::string("./cameras/") + std::string(drv->name) + std::string("_cfg.txt")).c_str(), 2);
    
    DPR(DDD, "ar0143 iregs size %d\n", probe_iregs.size());
    DPR(DDD, "ar0143 fregs size %d\n", probe_fregs.size());

    std::string cmd_param = "";
    int i2c_ok = 0;
    for (auto & reg : probe_iregs) {
        unsigned char val;
        if (drv->ireg_r(drv, bus, slave, reg, &val)) {
            char val_c[8] = {0};
            snprintf(val_c, sizeof(val_c), "0x%02x ", (int)val);
            cmd_param += std::string(val_c);
            i2c_ok++;
        }  
    }

    for (auto & reg : probe_fregs) {
        unsigned char val;
        if (drv->freg_r(drv, bus, slave, reg, &val)) {
            char val_c[8] = {0};
            snprintf(val_c, sizeof(val_c), "0x%02x ", (int)val);
            cmd_param += std::string(val_c);
            i2c_ok++;
        }       
    }

    if (!i2c_ok)
        return DET_NOI2C;

    char exec_ret[1024] = {0};
    std::string eval_cmd = "./bin/ar0143_verify.sh " + cmd_param;
    if (0 != shell_exec(eval_cmd.c_str(), exec_ret)) {
        PR("ar0143 evaluation error!\n");
        return DET_NOMATCH;
    } else {
        score = atoi(exec_ret);
    }

    return score;
}

static int ar0143_get_version(struct _drv_ops* drv, int bus, int slave, char *str_ver)
{
    return true;
}


static int ar0143_get_userdata(struct _drv_ops* drv, int bus, int slave, char *buf)
{

    return true;
}


DRV_OPS zcam_drv_ops_ar0143 = {
    "ar0143",
    ar0143_ispreg_rd,
    ar0143_ispreg_wr,
    ar0143_flashreg_rd,
    ar0143_flashreg_wr,
    ar0143_flash_dump,
    ar0143_flash_burn,
    ar0143_probe,
    ar0143_get_version,
    ar0143_get_version,
};

BOOTING_CALL_MOD(ar0143);