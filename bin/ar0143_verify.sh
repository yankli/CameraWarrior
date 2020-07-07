#########################################################################
# File Name: ar0143_verify.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: 2020年07月04日 星期六 23时59分19秒
#########################################################################
#!/bin/bash

#ZCREGI_9c20
#ZCREGI_9c20

vreg1=ZCREGI_9c20
vreg2=ZCREGI_9c21
let "ret=${vreg1}&${vreg2}"
echo ${ret}
exit 0
