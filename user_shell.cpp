#include "zcam.h"
#include "string.h"

int user_cmd_handler(int argc, char** argv)
{
    //DPR(DD, "user argS [%d]: %s %s  %s\n", argc, argv[0], argv[1], argv[2]);

    if (strstr(argv[0], "ispreg")) {
        
        unsigned char val;
        int reg;
    
        if (argc == 3) {
            cam_read(atoi(argv[1]), TARGET_ISP, strtol(argv[2], NULL, 16), &val);
            printf("%02x", val);

        } else if (argc == 4) {
            cam_write(atoi(argv[1]), TARGET_ISP, strtol(argv[2], NULL, 16), strtol(argv[3], NULL, 16));
            printf("%02x", val);
        }

    } else if (strstr(argv[0], "flashreg")) {
        
        unsigned char val;
        int reg;
        
        if (argc == 3) {
            cam_read(atoi(argv[1]), TARGET_FLASH, strtol(argv[2], NULL, 16), &val);
            printf("%02x", val);

        } else if (argc == 4) {
            cam_write(atoi(argv[1]), TARGET_FLASH, strtol(argv[2], NULL, 16), strtol(argv[3], NULL, 16));
            printf("%02x", val);
        }

    } else {

        PR("Error! cmds not recognized!\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{

    int argc1 = 0;
    char *argv1[128] = {0};

    get_cli_opt(&user_cli, argc, argv, &argc1, argv1);

    if (user_cli.log_level > 0)
        g_log_level = user_cli.log_level;
    
    DPR(D, "\t\t^^^^^^^^^^^^^^^^^^^^^\n\t\t"
    "[h] Use -f to force search\n\t\tUse -l 0 to be quiet!\n\n\n");
    
    zcam_detect_on_board();
    zcam_dump_dev_info();
    user_cmd_handler(argc1, argv1);
    zcam_release();
    
    return 0;
}