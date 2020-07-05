
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

int ZAPI zcam_detect_on_board(void);
void ZAPI zcam_dump_dev_info(void);
void ZAPI zcam_dump_driver_info(void);
void ZAPI zcam_release(void);
int ZAPI cam_read(int ch, TARGET_TYPE t, int addr, unsigned char* val);
int ZAPI cam_write(int ch, TARGET_TYPE t, int addr, unsigned char val);
int ZAPI cam_fburn(int ch, int addr, int size, unsigned char* buf);
int ZAPI cam_fdump(int ch, int addr, int size, unsigned char* buf);
int ZAPI cam_version(int ch, char *buf);
int ZAPI cam_userdata(int ch, char *buf);

typedef int (*CMDHDL) (int argc, char *argv[]);

typedef struct _work_item{
    char *sub_cmd;
    CMDHDL handler;
} WORK_ITEM, *PWORK_ITEM;

int ZAPI zcam_process_cmds(int argc, char *argv[]);

#endif