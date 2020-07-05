
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
int ZAPI cam_read(int ch, TARGET_TYPE t, int addr, unsigned char* val);
int ZAPI cam_write(int ch, TARGET_TYPE t, int addr, unsigned char val);
int ZAPI cam_fburn(int ch, int addr, int size, unsigned char* buf);
int ZAPI cam_fdump(int ch, int addr, int size, unsigned char* buf);
int ZAPI cam_version(int ch, int addr, char *buf);
int ZAPI cam_userdata(int ch, int addr, char *buf);

#endif