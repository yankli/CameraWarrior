#ifndef __ZCAM_UTIL_H_
#define __ZCAM_UTIL_H_

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

extern int g_log_level;

#define D       4
#define DD      5
#define DDD     6
#define DDDD    7

#define PR_(cond,fmt,arg...) do { \
if(cond) \
    printf("[ZCAM] " fmt,##arg); \
} while(0)


#define DPR(cur,fmt,arg...) do { \
if (cur <= DD) \
    PR_((cur <= g_log_level), fmt, ##arg); \
else \
    PR_((cur <= g_log_level),"%s,%d " fmt, __FILE__, __LINE__, ##arg); \
} while(0)

#define PR(fmt,arg...) do { \
    PR_((1),fmt,##arg); \
}while(0)


struct CLI_PARAM {

    int log_level;
    int force_bus_enum;

public:
    CLI_PARAM(): log_level(-1), force_bus_enum(-1){}
};

typedef struct _bd_cfg {
    int ch;
    int bus;
    int slave;

public:    
    _bd_cfg(): ch(-1), bus(-1),slave(-1) {}
    _bd_cfg(int c, int b, int s): ch(c), bus(b),slave(s) {}
}BD_CFG, *PBD_CFG;

typedef int SCORE;

enum {
    DET_NOI2C   = -1,
    DET_NOMATCH = 0,
};

static inline int get_cli_opt(struct CLI_PARAM *inst, int argc, char *argv[],
    int *argco, char *argvo[])
{
	int i = 0;
	int old = 0;
    *argco = argc;
    int idx = 0;
    argvo[0] = argv[0];

    while(i < argc) {
        old = i;
		if (!strcmp(argv[i], "-l")) {
			if (argc > i + 1) {
				i += 1;
				inst->log_level = atoi(argv[i]);
                g_log_level = inst->log_level;
				DPR(D, "user set log level:%u\n", inst->log_level);
                *argco = *argco - 2;
			}
		}

		if (!strcmp(argv[i], "-f")) {
                i += 1;
                inst->force_bus_enum = atoi(argv[i]);
				DPR(D, "Force to enumerate on bus:%u\n", inst->force_bus_enum);
                *argco = *argco - 2;
		}

        i++;
        if (i < argc)
            argvo[++idx] = argv[i];

	}
    DPR(D, "\n\n\n");

    return 0;
}

static int shell_exec(const char *command, char *result_buf)
{
    FILE *fp;
    int rc;

    fp = popen(command, "r");
    if(NULL == fp) {
        PR("popen exec error!! invalid script file\n");
        return -1;
    }

    DPR(DD, "execcmd: %s ...\n", command);
    char tmp_ret[4096];

    while(fgets(tmp_ret, sizeof(tmp_ret), fp) != NULL) {
        strcat(result_buf, tmp_ret);
    }
    
    rc = pclose(fp);
    if(rc) {
        PR("[!!!] Exec error!! PLEASE CHECK commands!!: -->\n\t[%s] \n<--\n", command);
        return -1;
    }
	return rc;
}

static std::vector<unsigned int> get_cam_cfg_regs(const char *file, int line_num)
{
    std::vector<unsigned int> regs;
    std::ifstream cfg_ifs(file);
    if (!cfg_ifs) {
        regs.clear();
        PR("open config file %s error!!\n", file);
        return regs;
    }

    std::string str_regs;
    int line = 0;

    while(std::getline(cfg_ifs, str_regs)) {
        line++;
        if (line == line_num) {
            std::stringstream ss(str_regs);
            std::string reg;
            while (ss >> reg) {
                regs.push_back(strtol(reg.c_str(), NULL, 16));
            }
            break;
        }
    };
    cfg_ifs.close();

    return regs;
}


static std::vector<BD_CFG> get_board_cfg(void)
{
    std::vector<BD_CFG> bd_infos;
    std::string bd_name;

    std::ifstream bdifs("/etc/zm_board_name");
    if (bdifs) {
        std::getline(bdifs, bd_name);
    }
    bdifs.close();
    std::string bd_string = std::string("./boards/bd_")
        + bd_name + std::string("_cfg.txt");
    
    std::ifstream cfg_ifs(bd_string.c_str());
    DPR(D, "open board config file [%s] [%s]\n",
        cfg_ifs? "OK":"FAIL", bd_string.c_str());
    if (!cfg_ifs) {
        bd_infos.clear();
        return bd_infos;
    }
   
    
    std::string str_bd;
    int line = 0;
    int ch = 0;
    
    while(std::getline(cfg_ifs, str_bd)) {
        line++;
    
        std::stringstream ss(str_bd);
        std::string bus;
        std::string slave;
        
        ss >> bus >> slave;
        BD_CFG bd_cfg;
        bd_cfg.bus = atoi(bus.c_str());
        bd_cfg.slave = strtol(slave.c_str(), NULL, 16);
        bd_cfg.ch = ch++;
        
        bd_infos.push_back(bd_cfg);
    };

    return bd_infos;
}

#endif