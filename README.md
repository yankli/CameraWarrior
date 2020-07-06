###             Camera Warrior
###             author: Yank li
###    email: yanchang.li@zongmutech.com
###
######################################################################

This is a scrum camera utilities to accommodate different types of camera
sensors. This tool provides universal CLI to operate camera sensors read,
write, burn, dump, etc.

1.    [Camera Discovering Ways]

    1.) Enumerate all devices with I2C address from 0 t0 127 automatically.
    2.) Load devices from specific board config file, which is indicated
        by board name in /etcc/zm_board_name.
    3.) User set camera device with channel, i2c, and type by force.

    for method 1. and 2., CameraWarrior need to invoke a shell script, usually
    named  bin/xxx_verify.sh, to evaluate whether current device is a xxx camera.
    The standard output characters of script will be treated as a match score.
    a larger positive number means a closer match than a less value. 0 means
    i2c dev found but not a known camera type, vice versa, minus value means
    even i2c dev not found.
    
    [NOTES] 
        the environment variable ZCAMPATH will affect the configuration file
        search path.
        

2.    [Usage]

    1.) Commandline mode. severals soft links to camera_warrior constitutes the
        util bundile.
    
    ├── camera_warrior
    ├── flashburn -> camera_warrior
    ├── flashdump -> camera_warrior
    ├── flashreg -> camera_warrior
    ├── ispreg -> camera_warrior
    ├── zcu -> camera_warriorer
    └── zcv -> camera_warrior

    commands plus a option parameter can be handled.

    xx -l <loglevel>
        log level setting.

    xx -b <bus>
        set the i2c bus id to enumerate by force

    xx -s ch,i2cbus,i2cslave,camera_name
        set the device on channel <ch> manually.


    2.) User app can also make use of this utility by linking against
        libzcam.so. APIs provided:


        int ZAPI zcam_detect_on_board(int bus);
        void ZAPI zcam_release_all(void);
        int ZAPI zcam_force_online_camera(int ch, int bus, int slave, char* type);
        int ZAPI CamRead(int ch, TARGET_TYPE t, int addr, unsigned char* val);
        int ZAPI CamWrite(int ch, TARGET_TYPE t, int addr, unsigned char val);
        int ZAPI CamFburn(int ch, int addr, int size, unsigned char* buf);
        int ZAPI CamFdump(int ch, int addr, int size, unsigned char* buf);
        int ZAPI CamVersion(int ch, char *buf);
        int ZAPI CamUserdata(int ch, char *buf);



