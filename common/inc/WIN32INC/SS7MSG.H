#ifndef _SS7MSG_H_
#define _SS7MSG_H_

#include "ctmsg.h"

#define __CTSS7CmdSym(item) CTsym_BuildCmdID( CT_objSS7,\
                                                      CT_vendTELSUN,\
                                                      (item) )
#define __CTSS7RespSym(item) CTsym_BuildRespID( CT_objSS7,\
                                                      CT_vendTELSUN,\
                                                      (item) )
#define __CTSS7EvtSym(item) CTsym_BuildEvtID( CT_objSS7,\
                                                      CT_vendTELSUN,\
												(item) )

#define CmdSS7_TELSUN_CallSetupRequest		 __CTSS7CmdSym(0x000)
#define CmdSS7_TELSUN_CallAccepted			 __CTSS7CmdSym(0x001)
#define CmdSS7_TELSUN_CallConnected			 __CTSS7CmdSym(0x002)
#define CmdSS7_TELSUN_ReleaseCallRequest	 __CTSS7CmdSym(0x004)
#define CmdSS7_TELSUN_CallEventRequest		 __CTSS7CmdSym(0x007)

#define CmdSS7_TELSUN_SS7DataConfig			__CTSS7CmdSym(0x008)
#define CmdSS7_TELSUN_SS7DataQuery			__CTSS7CmdSym(0x009)
#define CmdSS7_TELSUN_SS7StatusQuery		__CTSS7CmdSym(0x00a)
#define CmdSS7_TELSUN_SS7Operate			__CTSS7CmdSym(0x00b)
#define CmdSS7_TELSUN_MessageTracing		__CTSS7CmdSym(0x00c)
#define CmdSS7_TELSUN_SS7Measurement		__CTSS7CmdSym(0x00d)
#define CmdSS7_TELSUN_SystemAlarmMgr		__CTSS7CmdSym(0x00e)
#define CmdSS7_TELSUN_AppRegister			__CTSS7CmdSym(0x010)
#define CmdSS7_TELSUN_AppUnRegister			__CTSS7CmdSym(0x011)
#define CmdSS7_TELSUN_BlockChannel          __CTSS7CmdSym(0x012)
#define CmdSS7_TELSUN_UnblockChannel        __CTSS7CmdSym(0x013) 

#define RespSS7_TELSUN_CallSetupRequest		 __CTSS7RespSym(0x000)
#define RespSS7_TELSUN_CallAccepted			 __CTSS7RespSym(0x001)
#define RespSS7_TELSUN_CallConnected			 __CTSS7RespSym(0x002)
#define RespSS7_TELSUN_ReleaseCallRequest	 __CTSS7RespSym(0x004)
#define RespSS7_TELSUN_CallEventRequest		 __CTSS7RespSym(0x007)

#define RespSS7_TELSUN_SS7DataConfig		__CTSS7RespSym(0x008)
#define RespSS7_TELSUN_SS7DataQuery			__CTSS7RespSym(0x009)
#define RespSS7_TELSUN_SS7StatusQuery		__CTSS7RespSym(0x00a)
#define RespSS7_TELSUN_SS7Operate			__CTSS7RespSym(0x00b)
#define RespSS7_TELSUN_MessageTracing		__CTSS7RespSym(0x00c)
#define RespSS7_TELSUN_SS7Measurement		__CTSS7RespSym(0x00d)
#define RespSS7_TELSUN_SystemAlarmMgr		__CTSS7RespSym(0x00e)
#define RespSS7_TELSUN_AppRegister			__CTSS7RespSym(0x010)
#define RespSS7_TELSUN_AppUnRegister		__CTSS7RespSym(0x011)
#define RespSS7_TELSUN_BlockChannel         __CTSS7RespSym(0x012)
#define RespSS7_TELSUN_UnblockChannel       __CTSS7RespSym(0x013)


#define EvtSS7_TELSUN_ChannelBlocked			__CTSS7EvtSym(0x001)
#define EvtSS7_TELSUN_ChannelUnBlocked			__CTSS7EvtSym(0x002)
#define EvtSS7_TELSUN_CallReleased				__CTSS7EvtSym(0x005)
#define EvtSS7_TELSUN_CallEventIndication		__CTSS7EvtSym(0x006)
#define EvtSS7_TELSUN_SystemAlarmInd			__CTSS7EvtSym(0x00f)
#define EvtSS7_TELSUN_MtpPrivitive				__CTSS7EvtSym(0x010)
#define EvtSS7_TELSUN_GroupBlock                __CTSS7EvtSym(0x011)
#define EvtSS7_TELSUN_GroupUnblock              __CTSS7EvtSym(0x012)
#define EvtSS7_TELSUN_StateQuery                __CTSS7EvtSym(0x013)
#define EvtSS7_TELSUN_TCPDisCnnt                __CTSS7EvtSym(0x014)
#define EvtSS7_TELSUN_TCPCnntCmplt              __CTSS7EvtSym(0x015)
#define EvtSS7_TELSUN_TCPReset                  __CTSS7EvtSym(0x016)
#define EvtSS7_TELSUN_TCPBreak                  __CTSS7EvtSym(0x017)
#define EvtSS7_TELSUN_GRS		                __CTSS7EvtSym(0x018)
#endif
