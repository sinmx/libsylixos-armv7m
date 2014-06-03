/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: k_value.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2006 年 12 月 12 日
**
** 描        述: 这是系统特殊变量附值量定义。

** BUG
2007.11.04  将 LW_TRUE 定义为 (BOOL)(-1)
2009.04.04  将 LW_TRUE 定义为 1, 以保持最大的兼容性.
2009.07.11  加入毫秒到系统时钟个数的转换.
2010.05.05  对 LW_TRUE 不再使用 (BOOL) 强制转型.
*********************************************************************************************************/

#ifndef  __K_VALUE_H
#define  __K_VALUE_H

/*********************************************************************************************************
  BOOL
*********************************************************************************************************/
#define LW_FALSE                        (0)                             /*  0x0                         */
#define LW_TRUE                         (1)                             /*  0x1                         */

#ifndef TRUE
#define TRUE                            LW_TRUE
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
#define FALSE                           LW_FALSE
#endif                                                                  /*  FALSE                       */
/*********************************************************************************************************
  NULL
*********************************************************************************************************/
#ifdef __cplusplus
#define LW_NULL                         0                               /*  C++ 下的 NULL               */
#ifndef NULL
#define NULL                            0
#endif                                                                  /*  NULL                        */

#else

#define LW_NULL                         ((PVOID)0)                      /*  C   下的 NULL               */
#ifndef NULL
#define NULL                            ((PVOID)0)
#endif                                                                  /*  NULL                        */
#endif
/*********************************************************************************************************
  ROUND_UP
*********************************************************************************************************/

#define ROUND_UP(x, align)              (size_t)(((size_t)(x) +  (align - 1)) & ~(align - 1))
#define ROUND_DOWN(x, align)            (size_t)( (size_t)(x) & ~(align - 1))
#define ALIGNED(x, align)               (((size_t)(x) & (align - 1)) == 0)

/*********************************************************************************************************
  ID_SELF
*********************************************************************************************************/

#ifndef __SYLIXOS_KERNEL
#define LW_ID_SELF                      API_ThreadIdSelf()
#else
#define LW_ID_SELF                      Lw_Thread_Self()
#endif
#define LW_HANDLE_SELF                  LW_ID_SELF

/*********************************************************************************************************
  micro-second
*********************************************************************************************************/
#define LW_MSECOND_TO_TICK_1(ulMs)      \
        (((ulMs * CLOCKS_PER_SEC) / 1000) ? ((ulMs * CLOCKS_PER_SEC) / 1000) : (1))
         
#define LW_MSECOND_TO_TICK_0(ulMs)      \
        ((ulMs * CLOCKS_PER_SEC) / 1000)

#endif                                                                  /*  __K_VALUE_H                 */
/*********************************************************************************************************
  END
*********************************************************************************************************/
