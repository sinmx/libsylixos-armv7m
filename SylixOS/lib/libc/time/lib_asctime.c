/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: lib_asctime.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 08 月 30 日
**
** 描        述: 系统库.

** BUG:
2010.07.10  修正 asctime_r 函数返回值.
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#include "lib_local.h"
/*********************************************************************************************************
  辅助表
*********************************************************************************************************/
const CHAR      _G_cWeek[7][4]   = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const CHAR      _G_cMonth[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", 
                                    "Aug", "Sep", "Oct", "Nov", "Dec"};
/*********************************************************************************************************
** 函数名称: asctime_r
** 功能描述: 
** 输　入  : 
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if LW_CFG_RTC_EN > 0

PCHAR  lib_asctime_r (const struct tm *ptm, PCHAR  pcBuffer)
{
    if (!ptm || !pcBuffer) {
        return  (LW_NULL);
    }

    sprintf(pcBuffer, ASSFMT, 
            _G_cWeek[ptm->tm_wday],
            _G_cMonth[ptm->tm_mon],
            ptm->tm_mday,
            ptm->tm_hour,
            ptm->tm_min, 
            ptm->tm_sec,
            ptm->tm_year + TM_YEAR_BASE);
            
    return  (pcBuffer);
}
/*********************************************************************************************************
** 函数名称: lib_asctime
** 功能描述: 
** 输　入  : 
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
PCHAR  lib_asctime (const struct tm *ptm)
{
    static CHAR     cTimeBuffer[sizeof(ASCBUF)];
    
    if (!ptm) {
        return  (LW_NULL);
    }
            
    return  (lib_asctime_r(ptm, cTimeBuffer));
}

#endif                                                                  /*  LW_CFG_RTC_EN               */
/*********************************************************************************************************
  END
*********************************************************************************************************/
