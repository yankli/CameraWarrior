#include "zcam.h"
#include "string.h"

int main(int argc, char *argv[])
{

    int argc1 = 0;
    char *argv1[128] = {0};

    get_cli_opt(&user_cli, argc, argv, &argc1, argv1);

    if (user_cli.log_level > 0)
        g_log_level = user_cli.log_level;
    
    DPR(D, "\t\t^^^^^^^^^^^^^^^^^^^^^\n"
    "\t\t[h] Use -f to force search\n\t\tUse -l 0 to be quiet!\n\n\n");
    
    zcam_detect_on_board();
    zcam_dump_dev_info();
    zcam_process_cmds(argc1, argv1);
    zcam_release();
    
    return 0;
}