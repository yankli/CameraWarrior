#include "zcam.h"
#include "string.h"

int main(int argc, char *argv[])
{
    
    int argc1 = 0;
    char *argv1[128] = {0};

    get_cli_opt(&user_cli, argc, argv, &argc1, argv1);

    if (user_cli.log_level > 0)
        g_log_level = user_cli.log_level;
    
    DPR(D, "\t\t************* HELP *************\n"
    "\t\t[f] Use -f <bus_id> to force search\n"
    "\t\t[s] Use -s ch,bus,addr,cam_name to set dev.\n"
    "\t\t[l] Use -l loglevel to set loglevel.\n"
    "\n\n");
    
    zcam_detect_on_board(user_cli.bus_enum);
    
    if (user_cli.force_dev > 0)
        zcam_force_online_camera(user_cli.force_info.ch,
            user_cli.force_info.bus,
            user_cli.force_info.slave,
            user_cli.force_info.name);

    zcam_dump_dev_info();

    zcam_process_cmds(argc1, argv1);
    
    zcam_release_all();
    
    return 0;
}