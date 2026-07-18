#ifndef MainSymbolH
#define MainSymbolH

#define CT_objTS2000OAM         (0x008f)       // define by david for TS2000 OA&M

//define by david 99-6-30 For TS2000
#define __CTTS2000OAMKeySym(item)                CTsym_Build( CT_objTS2000OAM,\
                                                              CT_vendTELSUN,\
                                                              (item) )
//告警消息的key值
#define TS2000_TELSUN_StationNum                  __CTTS2000OAMKeySym(0x000)
#define TS2000_TELSUN_ModuleNum                   __CTTS2000OAMKeySym(0x001)
#define TS2000_TELSUN_Entity                      __CTTS2000OAMKeySym(0x002)
#define TS2000_TELSUN_AlarmLevel                  __CTTS2000OAMKeySym(0x003)
#define TS2000_TELSUN_AlarmNum                    __CTTS2000OAMKeySym(0x004)
#define TS2000_TELSUN_AlarmData                   __CTTS2000OAMKeySym(0x005)

#define TS2000_TELSUN_IntValue1				   __CTTS2000OAMKeySym(0x006)
#define TS2000_TELSUN_IntValue2				   __CTTS2000OAMKeySym(0x007)
#define TS2000_TELSUN_StrValue1				   __CTTS2000OAMKeySym(0x008)
#define TS2000_TELSUN_StrValue2				   __CTTS2000OAMKeySym(0x009)
#define TS2000_TELSUN_CommandCode				   __CTTS2000OAMKeySym(0x00a)
#define TS2000_TELSUN_Data						   __CTTS2000OAMKeySym(0x00b)
#define TS2000_TELSUN_CardType                    __CTTS2000OAMKeySym(0x00c)
#define TS2000_TELSUN_TraceType                   __CTTS2000OAMKeySym(0x00d)
#define TS2000_TELSUN_TraceMsg                    __CTTS2000OAMKeySym(0x00e)
#define TS2000_TELSUN_CardNo                      __CTTS2000OAMKeySym(0x00f)
#define TS2000_TELSUN_Sender                      __CTTS2000OAMKeySym(0x010)
#define TS2000_TELSUN_TraceCommand                __CTTS2000OAMKeySym(0x011)

//*****************************************************************************
//NMAGENT使用
#define TS2000_TELSUN_RequestMode                 __CTTS2000OAMKeySym(0x031)
#define TS2000_TELSUN_RequestModuleNo             __CTTS2000OAMKeySym(0x032)
#define TS2000_TELSUN_MIBValue                    __CTTS2000OAMKeySym(0x033)

//RequestMode值
#define Mode_Get         1
#define Mode_Set         2

//ModuleNum值
#define Module_NetDevice              1

#endif