/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: pmDev.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2014 �� 07 �� 19 ��
**
** ��        ��: ��Դ�����豸�ӿ�.
*********************************************************************************************************/

#ifndef __PMDEV_H
#define __PMDEV_H

/*********************************************************************************************************
  �ü�����
*********************************************************************************************************/
#if LW_CFG_POWERM_EN > 0

/*********************************************************************************************************
  ����������ýӿ�
*********************************************************************************************************/

LW_API INT  API_PowerMDevInit(PLW_PM_DEV  pmdev,  PLW_PM_ADAPTER  pmadapter, 
                              UINT        uiChan, PLW_PMD_FUNCS   pmdfunc);
LW_API INT  API_PowerMDevTerm(PLW_PM_DEV  pmdev);
LW_API INT  API_PowerMDevOn(PLW_PM_DEV  pmdev);
LW_API INT  API_PowerMDevOff(PLW_PM_DEV  pmdev);

#define pmDevInit       API_PowerMDevInit
#define pmDevTerm       API_PowerMDevTerm
#define pmDevOn         API_PowerMDevOn
#define pmDevOff        API_PowerMDevOff

/*********************************************************************************************************
  ���º����ں�ʹ��
*********************************************************************************************************/

VOID  pmDevSuspend(VOID);
VOID  pmDevResume(VOID);

#endif                                                                  /*  LW_CFG_POWERM_EN            */
#endif                                                                  /*  __PMDEV_H                   */
/*********************************************************************************************************
  END
*********************************************************************************************************/