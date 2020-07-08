
/*************************************************************************
    > File Name: main.cpp
    > Author: yank
    > Mail: yanchang.li@congmutech.com 
    > Created Time: 2020年07月06日 星期一 10时14分59秒
 ************************************************************************/

/****************************************************************************
 Firstlly detect all cameras according to bdxx_cfg.txt, or enumerate cameras on
 specified i2c bus accordint to user CLI input.
 Then, user can specify a fixed device type on a certain channel by calling
 zcam_force_online_camera().
 Finally procceed a user operation on the specific channel.
*****************************************************************************/  


#include "zcam.h"
#include "string.h"

int main(int argc, char *argv[])
{
    
    //////////////////// Example.1 user CLI /////////////////////////////

    int argc1 = 0;
    char *argv1[128] = {0};

    get_cli_opt(&user_cli, argc, argv, &argc1, argv1);

    if (user_cli.log_level > 0)
        g_log_level = user_cli.log_level;
    
    DPR(D, "\t\t************* CameraWarrior *************\n"
    "\t\t BUILD on %s , %s\n\n" 
    "\t\t[f] Use -f <bus_id> to force search\n"
    "\t\t[s] Use -s ch,bus,addr,cam_name to set dev.\n"
    "\t\t[l] Use -l loglevel to set loglevel.\n"
    "\n\n", __DATE__ , __BUSER__);
    
  
    zcam_detect_on_board(user_cli.bus_enum);
    
    if (user_cli.force_dev > 0)
        zcam_force_online_camera(user_cli.force_info.ch,
            user_cli.force_info.bus,
            user_cli.force_info.slave,
            user_cli.force_info.name);

    zcam_dump_dev_info();

    zcam_process_cmds(argc1, argv1);


    //////////////////// Example.2 ZCAM API /////////////////////////////

    unsigned char val = 2;
    //int ret = CamRead(0, TARGET_ISP, 0, &val);
    //printf("val: %d, ret: %s\n", val, ret? "TRUE":"FALSE");
    
    zcam_release_all();
    
    return 0;
}
