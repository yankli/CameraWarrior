/*************************************************************************
    > File Name: main.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2020年07月04日 星期六 10时14分59秒
 ************************************************************************/
#include "zcam_util.h"
#include "zcam_drv.h"
#include "zcam_dev.h"
#include "zcam.h"


/**************************************************/

/* 1.   use macro to define a func, or a class, then call them up.
        after refine, this part can be processed simutaneously with BOOTING CALL,

        MK_INIT_FUNC(ar0143) then BOOTING_CALL_FUNC(__func__), which can be merged
        into one macro: BOOTING_CALL_MOD(ar0143)
 */


/* 2.   call func by macro in each drv file, to make generator func and inject it
        into global init function list */

/* 3.   in main file, enumerate drv generator function by name string, and invoke
        each generator to populate drv objects */


#define MAX_DRV_NUM 64
#define MAX_DEV_NUM 256

int g_log_level = GLOG;

static DRV_OPS g_drv_ops_tbl[MAX_DRV_NUM] = {0};
static int g_drv_idx = 0;

static PCAM_DEV g_dev_tbl[MAX_DEV_NUM] = {0};
static int g_dev_idx = 0;
struct CLI_PARAM user_cli;

int zcam_drv_register(DRV_OPS *pdrv)
{
    if (!pdrv)
        return -1;

    for (int i = 0; i < MAX_DRV_NUM; i++) {
        if (!strcmp(pdrv->name, g_drv_ops_tbl[i].name))
            return 0;
    }
    
    if (g_drv_idx == MAX_DRV_NUM)
        return -1;
    
    g_drv_ops_tbl[g_drv_idx++] = *pdrv;
    DPR(DDD,"zcam driver: [%s] registered at[%d], done.\n", pdrv->name, g_drv_idx);
    
    return 0;
}


PDRV_OPS zcam_drv_get(char *name)
{
    if (!name)
        return NULL;

    for (int i = 0; i < MAX_DRV_NUM; i++) {
        if (!strcmp(name, g_drv_ops_tbl[i].name))
            return &g_drv_ops_tbl[i];
    }

    return NULL;
}

PDRV_OPS zcam_drv_get(int idx)
{
    if (idx >= MAX_DRV_NUM)
        return NULL;

    return &g_drv_ops_tbl[idx];
}


int ZAPI zcam_dev_unregister(int ch)
{
    if (g_dev_tbl[ch] && g_dev_tbl[ch]->match_score >= 0) {
        DPR(D, "-- dev [%d] released!!\n", ch);
        delete(g_dev_tbl[ch]);
        g_dev_tbl[ch] = NULL;
        g_dev_idx--;
        return true;
    }

    return false;
}


int zcam_dev_register(int ch, PCAM_DEV newdev)
{
    int rc = false;
    if (newdev->ch < 0 || newdev->ch > MAX_DEV_NUM - 1
        || newdev->i2c_addr < 0)
        return false;

    if (g_dev_tbl[ch] == NULL) {
        DPR(D, "**new dev [%d] populated!\n", ch);
        g_dev_tbl[ch] = newdev;
        g_dev_idx++;
    } else if (newdev->match_score > g_dev_tbl[ch]->match_score) {
        DPR(D, "**new dev [%d] replaced!!\n", ch);
        delete(g_dev_tbl[ch]);
        g_dev_tbl[ch] = newdev;
    }

    return true;
}


PCAM_DEV zcam_dev_get(int ch)
{
    if (!g_dev_tbl[ch] || !g_dev_tbl[ch]->drv)
        return NULL;

    return g_dev_tbl[ch];
}



/*********************************************************************/

int ZAPI zcam_drv_count()
{
    return g_drv_idx;
}


int ZAPI zcam_dev_count()
{
    return g_dev_idx;
}


void ZAPI zcam_dump_driver_info()
{
    for (int i = 0; i < zcam_drv_count(); i++) {
        PDRV_OPS drv = zcam_drv_get(i);
        if (drv)
            DPR(D, "[+] supported camera types: [%s]\n", drv->name);
    }
}

void ZAPI zcam_dump_dev_info(void)
{
    for (int ch = 0; ch < MAX_DEV_NUM; ch++) {
        PCAM_DEV dev = g_dev_tbl[ch];
        if (dev)
            DPR(D, "[+] dev_%d at[%d@%x]: [%s]\n",
            dev->ch, dev->i2c_bus, dev->i2c_addr, dev->dev_type);
    }
}


int ZAPI zcam_detect_on_board(void)
{
    zcam_dump_driver_info();
    
    std::vector<BD_CFG> boards;
    if (user_cli.force_bus_enum >= 0) {
        DPR(D, "Enumeration on all possible boards address!\n");
        int ch = 0;
        for (int s = 0; s <= 0xff/2; s++) {
            boards.emplace_back(ch, user_cli.force_bus_enum, s);
            ch++;
        }
    } else {
        DPR(D, "try to use board config file...\n");
        boards = get_board_cfg();   
    }
    
    DPR(D, "\n\nxxxx [%d cameras configured, enumerating...] xxxx\n",
            boards.size());
        
    if (boards.size() > 0) {
        for (auto& bd : boards)
            for (int i = 0; i < zcam_drv_count(); i++) {
                PDRV_OPS drv = zcam_drv_get(i);
                if (drv) {
                    int score = drv->probe(drv, bd.bus, bd.slave);
                    if (score > DET_NOMATCH) {
                        DPR(D, "[+++] [%s] <-match-> [%d@%x] score:%d\n",
                        drv->name, bd.bus, bd.slave, score);
                        CAM_DEV *new_dev = new CAM_DEV;
                        new_dev->i2c_bus = bd.bus;
                        new_dev->i2c_addr = bd.slave;
                        new_dev->ch = bd.ch;

                        strcpy(new_dev->dev_type, drv->name);
                        new_dev->match_score = score;
                        new_dev->drv = drv;
                        zcam_dev_register(bd.ch, new_dev);

                    } else if (score == DET_NOMATCH) {
                        DPR(DD, "[...] [%s] not match dev[%d@%x]\n", drv->name,
                            bd.bus, bd.slave); 
                        CAM_DEV *new_dev = new CAM_DEV;
                        new_dev->i2c_bus = bd.bus;
                        new_dev->i2c_addr = bd.slave;
                        new_dev->ch = bd.ch;

                        strcpy(new_dev->dev_type, "UNKNOWN");
                        new_dev->match_score = DET_NOMATCH;
                        new_dev->drv = NULL;
                        zcam_dev_register(bd.ch, new_dev);

                    } else if(score <= DET_NOI2C){
                        DPR(D, "[---] No i2c dev at [%d@%x]\n", bd.bus, bd.slave); 
                    } 
                }
            }
    }

    DPR(D, "\t\t[enumeration done.]\n\n");

}


int ZAPI cam_read(int ch, TARGET_TYPE t, int addr, unsigned char* val)
{
    int rc = false;
    PCAM_DEV pdev = zcam_dev_get(ch);

    if (!pdev) {
        PR("[!ERROR!] Device not exist!\n");
        return false;
    }
    switch(t) {
        case TARGET_ISP:
            rc = pdev->drv->ireg_r(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, addr, val);
            break;
        case TARGET_FLASH:
            rc = pdev->drv->freg_r(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, addr, val);
            break;

        default:
            break;
    }
    
    return rc;
}


int ZAPI cam_write(int ch, TARGET_TYPE t, int addr, unsigned char val)
{
    int rc = false;
    PCAM_DEV pdev = zcam_dev_get(ch);

    if (!pdev) {
        PR("[!ERROR!] Device not exist!\n");
        return false;
    }
    switch(t) {
        case TARGET_ISP:
            rc = pdev->drv->ireg_w(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, addr, val);
            break;
        case TARGET_FLASH:
            rc = pdev->drv->freg_w(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, addr, val);
            break;

        default:
            break;
    }
    
    return rc;
}


int ZAPI cam_fburn(int ch, int addr, int size, unsigned char* buf)
{
    int rc = false;
    PCAM_DEV pdev = zcam_dev_get(ch);

    if (!pdev) {
        PR("[!ERROR!] Device not exist!\n");
        return false;
    }

    return pdev->drv->fburn(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, addr, size, buf);
}
   

int ZAPI cam_fdump(int ch, int addr, int size, unsigned char* buf)
{
    int rc = false;
    PCAM_DEV pdev = zcam_dev_get(ch);

    if (!pdev) {
        PR("[!ERROR!] Device not exist!\n");
        return false;
    }

    return pdev->drv->fdump(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, addr, size, buf);
}


int ZAPI cam_version(int ch, int addr, char *buf)
{
    int rc = false;
    PCAM_DEV pdev = zcam_dev_get(ch);

    if (!pdev) {
        PR("[!ERROR!] Device not exist!\n");
        return false;
    }
    return pdev->drv->get_version(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, buf);

}
   

int ZAPI cam_userdata(int ch, int addr, char *buf)
{
    int rc = false;
    PCAM_DEV pdev = zcam_dev_get(ch);

    if (!pdev) {
        PR("[!ERROR!] Device not exist!\n");
        return false;
    }
    return pdev->drv->get_userdata(pdev->drv, pdev->i2c_bus, pdev->i2c_addr, buf);
}
   

   

   

   
