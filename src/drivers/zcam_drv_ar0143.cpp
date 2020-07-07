/*************************************************************************
    > File Name: zcam_drv_ar0143.cpp
    > Author: yank
    > Mail: yanchang.li@congmutech.com 
    > Created Time: 2020年07月04日 星期六 10时14分59秒
 ************************************************************************/

#include "zcam_drv.h"
#include <string>
#include <iostream>

#define veri_file (std::string(WORK_PATH) + \
    std::string("bin/ar0143_verify.sh")).c_str()

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
    int i2c_ok = 0;

    auto regmapi = get_script_regs(veri_file, "ZCREGI_");
    
    DPR(DD, "check isp regs... \n");
    
    for (auto& reg : regmapi) {
        int reg_addr = reg.second;
        unsigned char val;
        if (drv->ireg_r(drv, bus, slave, reg_addr, &val)) {
            i2c_ok++;
            regmapi[reg.first] = (unsigned int)val;
        }  

        DPR(DD, "\t%x = %02x\n",reg_addr, val);
    }
    DPR(DD, "\n");

    auto regmapf = get_script_regs(veri_file, "ZCREGF_");
    
    DPR(DD, "check flash regs... \n");
    
    for (auto& reg : regmapf) {
        int reg_addr = reg.second;
        unsigned char val;
        if (drv->freg_r(drv, bus, slave, reg_addr, &val)) {
            i2c_ok++;
            regmapf[reg.first] = (unsigned int)val;
        }  

        DPR(DD, "\t%x = %02x\n",reg_addr, val);
    }
    DPR(DD, "\n");

    if (!i2c_ok)
        return DET_NOI2C;

    char exec_ret[1024] = {0};
    std::string shell = set_script_regs(veri_file, regmapi, regmapf);
    
    std::string eval_cmd = "/bin/bash -c " + std::string("\'") + shell + std::string("\'");
    //std::string eval_cmd = veri_file;
    if (0 != shell_exec(eval_cmd.c_str(), exec_ret)) {
        ER("ar0143 evaluation error!\n");
        return DET_NOMATCH;
    } else {
        score = atoi(exec_ret);
    }
#if 0

    /* should load from ar0143_cfg.xml file */
    std::vector<unsigned int> probe_iregs;
    std::vector<unsigned int> probe_fregs;

    probe_iregs = get_cam_cfg_regs((std::string(WORK_PATH) +
        std::string("./cameras/") + std::string(drv->name) +
        std::string("_cfg.txt")).c_str(), 1);
    probe_fregs = get_cam_cfg_regs((std::string(WORK_PATH) +
        std::string("./cameras/") + std::string(drv->name) +
        std::string("_cfg.txt")).c_str(), 2);
    
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
    std::string eval_cmd = std::string(WORK_PATH) + "./bin/ar0143_verify.sh " + cmd_param;
    if (0 != shell_exec(eval_cmd.c_str(), exec_ret)) {
        ER("ar0143 evaluation error!\n");
        return DET_NOMATCH;
    } else {
        score = atoi(exec_ret);
    }
    #endif

    return score;
}

static int ar0143_get_version(struct _drv_ops* drv, int bus, int slave, char *str_ver)
{
    //TODO:

    strcpy(str_ver, "this is verion string of 0143");
    return true;
}


static int ar0143_get_userdata(struct _drv_ops* drv, int bus, int slave, char *buf)
{
    //TODO:

    strcpy(buf, "this is user string of 0143");
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