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
** 文   件   名: lwip_netifnum.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 07 月 30 日
**
** 描        述: lwip 网口计数器改进.
                 lwip netif_add() 总是增加网络接口计数器, 但 netif_remove() 并没有处理.
                 
** BUG:
2011.02.13  netif_remove_hook() 中加入对 npf detach 的操作, 确保 attach 与 detach 成对操作.
2011.03.10  将 _G_ulNetIfLock 开放, posix net/if.h 需要此锁.
2011.07.04  加入对路由表的回调操作.
2013.04.16  netif_remove_hook 需要卸载 DHCP 数据结构.
2013.09.24  移除网络接口加入对 auto ip 的回收.
2014.03.22  可以通过索引号, 快速得到 netif.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#define  __NETIF_MAIN_FILE
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if LW_CFG_NET_EN > 0
#include "lwip/mem.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/err.h"
#include "lwip_route.h"
#include "lwip_if.h"
/*********************************************************************************************************
  网络接口数量宏定义
*********************************************************************************************************/
#define __LW_NETIF_MAX_NUM              10
#define __LW_NETIF_USED                 1
#define __LW_NETIF_UNUSED               0
/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
static struct netif        *_G_pnetifArray[__LW_NETIF_MAX_NUM];
static UINT                 _G_uiNetifNum = 0;
/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
#if LW_CFG_NET_NPF_EN > 0
VOID  __npfNetifRemoveHook(struct netif  *pnetif);
#endif                                                                  /*  LW_CFG_NET_NPF_EN > 0       */
/*********************************************************************************************************
** 函数名称: netif_add_hook
** 功能描述: 创建网络接口回调函数, 返回网络接口号 (网络上下文中调用)
** 输　入  : pvNetif     网络接口
** 输　出  : ERR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  netif_add_hook (PVOID  pvNetif)
{
    struct netif  *netif = (struct netif *)pvNetif;
    INT            i;
    
    if (_G_ulNetifLock == 0) {
        _G_ulNetifLock =  API_SemaphoreMCreate("netif_lock", LW_PRIO_DEF_CEILING, 
                                               LW_OPTION_DELETE_SAFE |
                                               LW_OPTION_INHERIT_PRIORITY |
                                               LW_OPTION_OBJECT_GLOBAL, LW_NULL);
    }
    
    LWIP_NETIF_LOCK();                                                  /*  进入临界区                  */
    for (i = 0; i < __LW_NETIF_MAX_NUM; i++) {
        if (_G_pnetifArray[i] == LW_NULL) {
            _G_pnetifArray[i] =  netif;
            netif->num        =  (UINT8)i;
            _G_uiNetifNum++;
            break;
        }
    }
    LWIP_NETIF_UNLOCK();                                                /*  退出临界区                  */
    
    if (i < __LW_NETIF_MAX_NUM) {
        rt_netif_add_hook(netif);                                       /*  更新路由表有效标志          */
        return  (ERR_OK);
    
    } else {
        return  (ERR_USE);
    }
}
/*********************************************************************************************************
** 函数名称: netif_remove_hook
** 功能描述: 删除网络接口回调函数. (网络上下文中调用)
** 输　入  : pvNetif     网络接口
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  netif_remove_hook (PVOID  pvNetif)
{
    struct netif  *netif = (struct netif *)pvNetif;
    INT            iNum  = (INT)netif->num;
    
    rt_netif_remove_hook(netif);                                        /*  更新路由表有效标志          */
    
    if (iNum < __LW_NETIF_MAX_NUM) {
        LWIP_NETIF_LOCK();                                              /*  进入临界区                  */
        _G_pnetifArray[iNum] = LW_NULL;
        _G_uiNetifNum--;
        LWIP_NETIF_UNLOCK();                                            /*  退出临界区                  */
    }
    
#if LW_CFG_NET_NPF_EN > 0
    __npfNetifRemoveHook(netif);
#endif                                                                  /*  LW_CFG_NET_NPF_EN > 0       */

#if LW_CFG_LWIP_DHCP > 0
    if ((netif->flags & NETIF_FLAG_DHCP) && (netif->dhcp)) {
        dhcp_stop(netif);                                               /*  关闭 DHCP 回收 UDP 控制块   */
        dhcp_cleanup(netif);                                            /*  回收 DHCP 内存              */
    }
#endif                                                                  /*  LW_CFG_LWIP_DHCP > 0        */

#if LW_CFG_LWIP_AUTOIP > 0
    if (netif->autoip) {
        mem_free(netif->autoip);                                        /*  回收 AUTOIP 内存            */
        netif->autoip = LW_NULL;
    }
#endif                                                                  /*  LW_CFG_LWIP_AUTOIP > 0       */
}
/*********************************************************************************************************
** 函数名称: netif_get_num
** 功能描述: 获得网络接口数量.
** 输　入  : NONE
** 输　出  : 网络接口数量
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT netif_get_num (VOID)
{
    UINT    uiNum;

    LWIP_NETIF_LOCK();                                                  /*  进入临界区                  */
    uiNum = _G_uiNetifNum;
    LWIP_NETIF_UNLOCK();                                                /*  退出临界区                  */

    return  (uiNum);
}
/*********************************************************************************************************
** 函数名称: netif_get_by_index
** 功能描述: 通过 index 获得网络接口结构. (没有加锁)
** 输　入  : uiIndex       index
** 输　出  : 网络接口
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PVOID netif_get_by_index (UINT uiIndex)
{
    if (uiIndex < __LW_NETIF_MAX_NUM) {
        return  (_G_pnetifArray[uiIndex]);
    }
    
    return  (LW_NULL);
}

#endif                                                                  /*  LW_CFG_NET_EN               */
/*********************************************************************************************************
  END
*********************************************************************************************************/
