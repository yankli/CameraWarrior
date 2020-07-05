#include "zcam.h"


int main(int argc, char *argv[])
{
    get_cli_opt(&user_cli, argc, argv);

    if (user_cli.log_level > 0)
        g_log_level = user_cli.log_level;
    
    DPR(D, "\t\t^^^^^^^^^^^^^^^^^^^^^\n\t\t"
    "[h] Use -f to force search\n\t\tUse -l 0 to be quiet!\n\n\n");
    
    zcam_detect_on_board();
    zcam_dump_dev_info();
    

    #if 0

    foreach bd in boards {
        foreach ch in camset {
            foreach drv: drivers {
                if ((score = drv->probe()) > 0) {
                    zcam_dev *dev = molloc_device();
                    dev->driver = drv;
                    dev->ch = ch;
                    dev->bus = bus;
                    dev->i2c_addr = slave;
                    dev->score = score;
                    zcam_device_add_online(ch, dev);
                }
            }
        }
    }
    #endif

    return 0;
}