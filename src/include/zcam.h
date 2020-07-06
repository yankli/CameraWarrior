
/*************************************************************************
    > File Name: zcam.h
    > Author: yank
    > Mail: yanchang.li@congmutech.com 
    > Created Time: 2020年07月04日 星期六 10时14分59秒
 ************************************************************************/

#ifndef __ZCAM_API_H
#define __ZCAM_API_H
#include "zcam_util.h"

#define ZAPI 
enum TARGET_TYPE {
    TARGET_INVALID = 0,
    TARGET_ISP = 1,
    TARGET_FLASH = 2,
    TARGET_CCI = 3,
    TARGET_SIZE
};

extern struct CLI_PARAM user_cli;

int ZAPI zcam_detect_on_board(int bus);
void ZAPI zcam_release_all(void);
int ZAPI zcam_force_online_camera(int ch, int bus, int slave, char* type);
int ZAPI CamRead(int ch, TARGET_TYPE t, int addr, unsigned char* val);
int ZAPI CamWrite(int ch, TARGET_TYPE t, int addr, unsigned char val);
int ZAPI CamFburn(int ch, int addr, int size, unsigned char* buf);
int ZAPI CamFdump(int ch, int addr, int size, unsigned char* buf);
int ZAPI CamVersion(int ch, char *buf);
int ZAPI CamUserdata(int ch, char *buf);
void ZAPI zcam_dump_dev_info(void);
void ZAPI zcam_dump_driver_info(void);

typedef int (*CMDHDL) (int argc, char *argv[]);

typedef struct _work_item{
    char *sub_cmd;
    CMDHDL handler;
} WORK_ITEM, *PWORK_ITEM;

int ZAPI zcam_process_cmds(int argc, char *argv[]);

#endif