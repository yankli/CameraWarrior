/*************************************************************************
    > File Name: user_cmds.cpp
    > Author: yank
    > Mail: yanchang.li@congmutech.com 
    > Created Time: 2020年07月04日 星期六 10时14分59秒
 ************************************************************************/

#include "zcam.h"
#include "string.h"
#include "stdio.h"
#include "unistd.h"

static int __cmd_ispreg_proc(int argc, char** argv);
static int __cmd_flashreg_proc(int argc, char** argv);
static int __cmd_flashburn_proc(int argc, char** argv);
static int __cmd_flashdump_proc(int argc, char** argv);
static int __cmd_version_proc(int argc, char** argv);
static int __cmd_userdata_proc(int argc, char** argv);

///////////////////////////////////////////////////////////////////////////

static WORK_ITEM work_table[] = {
    {   "ispreg",      __cmd_ispreg_proc      },
    {   "flashreg",    __cmd_flashreg_proc    },
    {   "flashburn",    __cmd_flashburn_proc    },
    {   "flashdump",    __cmd_flashdump_proc    },
    {   "zcv",          __cmd_version_proc    },
    {   "zcu",          __cmd_userdata_proc    },
    {0}
};


int ZAPI zcam_process_cmds(int argc, char *argv[])
{
    PWORK_ITEM work = work_table;
    
    while (work && work->handler) {
        if (strstr(argv[0], work->sub_cmd))
            return work->handler(argc, argv);
        work++;
    }

    ER("user subcmd is not found!\n");

    return false;
}



///////////////////////////////////////////////////////////////////////////

static int __cmd_ispreg_proc(int argc, char** argv)
{
    unsigned char val;
    int reg;
    int rc = false;

    if (argc == 3) {
        rc = CamRead(atoi(argv[1]), TARGET_ISP, strtol(argv[2], NULL, 16), &val);
        printf("%02x", val);
    } else if (argc == 4) {
        rc = CamWrite(atoi(argv[1]), TARGET_ISP, strtol(argv[2], NULL, 16), strtol(argv[3], NULL, 16));
        printf("%02x", strtol(argv[3], NULL, 16));
    }

    return rc;
}

int __cmd_flashreg_proc(int argc, char** argv)
{
    unsigned char val;
    int reg;
    int rc = false;

    if (argc == 3) {
        rc = CamRead(atoi(argv[1]), TARGET_FLASH, strtol(argv[2], NULL, 16), &val);
        printf("%02x", val);
    } else if (argc == 4) {
        rc = CamWrite(atoi(argv[1]), TARGET_FLASH, strtol(argv[2], NULL, 16), strtol(argv[3], NULL, 16));
        printf("%02x", strtol(argv[3], NULL, 16));
    }

    return rc;
}

int __cmd_flashburn_proc(int argc, char** argv)
{
    char buf[0x100000];
    int reg;
    int rc = false;
    int bytes;

    if (argc == 5) {
        std::ifstream ifs(argv[4],  std::ios::binary);
        
        if (ifs) {
            ifs.seekg( 0, std::ios::end);
		    int fsize = ifs.tellg();
            ifs.seekg( 0, std::ios::beg);
            bytes = std::min((int)strtol(argv[3], NULL, 16), fsize);
            
            ifs.read(buf, bytes);
        }
        ifs.close();
        
        rc = CamFburn(atoi(argv[1]), strtol(argv[2], NULL, 16), bytes, (unsigned char *)buf);
        return rc;
    }
    
    return rc;
}

int __cmd_flashdump_proc(int argc, char** argv)
{
    char buf[0x100000] = {0};
    int reg;
    int rc = false;
    

    if (argc >= 4) {
        rc = CamFdump(atoi(argv[1]), strtol(argv[2], NULL, 16), strtol(argv[3], NULL, 16), (unsigned char *)buf);
        write(1, buf, strtol(argv[3], NULL, 16));
    }
    
    if (argc == 5) {
        std::ofstream ofs(argv[4], std::ios::binary);
        if (ofs) {
            ofs.write(buf,  strtol(argv[3], NULL, 16));
        }
        ofs.close();
    }
    
    return rc;
}

int __cmd_version_proc(int argc, char** argv)
{
    char ver_str[1024] = {0};
    int reg;
    int rc = false;

    if (argc == 2) {
        rc = CamVersion(atoi(argv[1]), ver_str);
        printf("%s", ver_str);   
    }
    
    return rc;
}

int __cmd_userdata_proc(int argc, char** argv)
{
    char ver_str[1024] = {0};
    int reg;
    int rc = false;

    if (argc == 2) {
        rc = CamUserdata(atoi(argv[1]), ver_str);
        printf("%s", ver_str);   
    }
    
    return rc;
}