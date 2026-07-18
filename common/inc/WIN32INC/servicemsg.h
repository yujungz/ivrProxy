/*************************************************************************
* IVR Service messages
* 
* If you want to make change of it, please contact Liang Li zi.
*************************************************************************/

#ifndef _service_msg_H__
#define _service_msg_H__

#include "ctmsg.h"
#include "servicesymbol.h"

#define CT_objACTION        (0x0057)


/*************************************************************************
* SIA module handle messages
*************************************************************************/

#define __CTSIACmdSym(item)             CTsym_BuildCmdID( CT_objSIA, \
													  CT_vendTELSUN, \
													  (item))

#define CmdSIA_TELSUN_InstallFlow				__CTSIACmdSym(0x001)
#define CmdSIA_TELSUN_UninstallFlow				__CTSIACmdSym(0x002)
#define CmdSIA_TELSUN_TraceBegin				__CTSIACmdSym(0x003)
#define CmdSIA_TELSUN_TraceEnd					__CTSIACmdSym(0x004)
#define CmdSIA_TELSUN_GetRunningMap				__CTSIACmdSym(0x005)

#define CmdSIA_TELSUN_UrgentStop				__CTSIACmdSym(0x011)
#define CmdSIA_TELSUN_UrgentBillPing			__CTSIACmdSym(0x012)
/*  Command the session to send urgent bill ping to AAAServer
**	[SIA_TELSUN_Module, CTuint, AAAServer(GOR)]
*/
#define CmdSIA_TELSUN_UrgentStopSession			__CTSIACmdSym(0x013)
#define CmdSIA_TELSUN_StopConference			__CTSIACmdSym(0x014)
#define CmdSIA_TELSUN_JoinConference			__CTSIACmdSym(0x015)
#define CmdSIA_TELSUN_StopFlow					__CTSIACmdSym(0x016)
#define CmdSIA_TELSUN_DeleteConfCtrl			__CTSIACmdSym(0x017)
#define CmdSIA_TELSUN_LeaveConference			__CTSIACmdSym(0x018)
#define CmdSIA_TELSUN_ChangePartAttr			__CTSIACmdSym(0x019)
#define CmdSIA_TELSUN_KickOutAPart				__CTSIACmdSym(0x01a)
#define CmdSIA_TELSUN_GetPartStatus				__CTSIACmdSym(0x01b)
#define CmdSIA_TELSUN_StopCall					__CTSIACmdSym(0x01c)

#define CmdSIA_TELSUN_StartFlow					__CTSIACmdSym(0x020)
#define CmdSIA_TELSUN_StopSignalFlow			__CTSIACmdSym(0x021)
#define CmdSIA_TELSUN_PutEventMessage			__CTSIACmdSym(0x022)

////////////////////////////////////////////////////////////////

#define __CTSIARespSym(item)             CTsym_BuildRespID( CT_objSIA,\
														CT_vendTELSUN,\
														(item))

#define RespSIA_TELSUN_InstallFlow				__CTSIARespSym(0x001)
#define RespSIA_TELSUN_UninstallFlow			__CTSIARespSym(0x002)
#define RespSIA_TELSUN_TraceBegin				__CTSIARespSym(0x003)
#define RespSIA_TELSUN_TraceEnd					__CTSIARespSym(0x004)
#define RespSIA_TELSUN_GetRunningMap			__CTSIARespSym(0x005)
#define RespSIA_TELSUN_GetPartStatus			__CTSIARespSym(0x006)

#define RespSIA_TELSUN_StopFlow					__CTSIARespSym(0x016)
#define RespSIA_TELSUN_StopCall					__CTSIARespSym(0x01c)
#define RespSIA_TELSUN_StartFlow				__CTSIARespSym(0x020)
#define RespSIA_TELSUN_StopSignalFlow		    __CTSIARespSym(0x021)
#define RespSIA_TELSUN_PutEventMessage			__CTSIARespSym(0x022)

////////////////////////////////////////////////////////////////

#define __CTSIAEvtSym(item)             CTsym_BuildEvtID( CT_objSIA,\
														CT_vendTELSUN,\
														(item))

#define EvtSIA_TELSUN_TraceMessage				__CTSIAEvtSym(0x001)
#define EvtSIA_TELSUN_TraceAction				__CTSIAEvtSym(0x002)
#define EvtSIA_TELSUN_TraceRegister				__CTSIAEvtSym(0x003)
#define EvtSIA_TELSUN_TraceState				__CTSIAEvtSym(0x004)

#define EvtSIA_TELSUN_ModuleInvalid				__CTSIAEvtSym(0x010)
#define EvtSIA_TELSUN_PutFlowMessage			__CTSIAEvtSym(0x011)
#define EvtSIA_TELSUN_WaitRespond				__CTSIAEvtSym(0x012)
#define EvtSIA_TELSUN_PutSignalMessage			__CTSIAEvtSym(0x013)
#define EvtSIA_TELSUN_ExitService				__CTSIAEvtSym(0x020)

/*************************************************************************
* Flow event messages
*************************************************************************/
#define __CTActionEvtSym(item)			   CTsym_BuildEvtID( CT_objACTION,\
															 CT_vendTELSUN,\
															 (item) )

#define EvtAction_TELSUN_Connect				__CTActionEvtSym(0x001)
#define EvtAction_TELSUN_Passed					__CTActionEvtSym(0x002)
#define EvtAction_TELSUN_Failed					__CTActionEvtSym(0x003)
#define EvtAction_TELSUN_Returned				__CTActionEvtSym(0x004)
#define EvtAction_TELSUN_StopPlayBack			__CTActionEvtSym(0x005)
#define EvtAction_TELSUN_DialTerminate			__CTActionEvtSym(0x006)
#define EvtAction_TELSUN_StopDial				__CTActionEvtSym(0x007)
#define EvtAction_TELSUN_TimeOut				__CTActionEvtSym(0x008)
#define EvtAction_TELSUN_StopDigits				__CTActionEvtSym(0x009)
#define EvtAction_TELSUN_StopSendFax			__CTActionEvtSym(0x00a)
#define EvtAction_TELSUN_StopRecvFax			__CTActionEvtSym(0x00b)
#define EvtAction_TELSUN_CallTerm				__CTActionEvtSym(0x00c)
#define EvtAction_TELSUN_Record					__CTActionEvtSym(0x00d)
#define EvtAction_TELSUN_Case					__CTActionEvtSym(0x00e)
#define EvtAction_TELSUN_CallHeld				__CTActionEvtSym(0x00f)
#define EvtAction_TELSUN_Verify					__CTActionEvtSym(0x010)
#define EvtAction_TELSUN_CallCleared			__CTActionEvtSym(0x011)
#define EvtAction_TELSUN_StopFlow				__CTActionEvtSym(0x012)
#define EvtAction_TELSUN_GatewayNotExist		__CTActionEvtSym(0x013)
#define EvtAction_TELSUN_DataReceived			__CTActionEvtSym(0x014)
#define EvtAction_TELSUN_ClearCallFail			__CTActionEvtSym(0x015)
#define EvtAction_TELSUN_ClearCallOK			__CTActionEvtSym(0x016)
#define EvtAction_TELSUN_NoEvent				__CTActionEvtSym(0x017)
#define EvtAction_TELSUN_Exist					__CTActionEvtSym(0x018)
#define EvtAction_TELSUN_NeverHappen			__CTActionEvtSym(0x019)
#define EvtAction_TELSUN_JoinedConference		__CTActionEvtSym(0x01a)
#define EvtAction_TELSUN_AddSelfToConference	__CTActionEvtSym(0x01b)
#define EvtAction_TELSUN_AddOtherToConference	__CTActionEvtSym(0x01c)
#define EvtAction_TELSUN_RemoveSelfFromConference	__CTActionEvtSym(0x01d)
#define EvtAction_TELSUN_AsteriskPressed		__CTActionEvtSym(0x01e)
#define EvtAction_TELSUN_KickOutAPart			__CTActionEvtSym(0x01f)
#define EvtAction_TELSUN_NotExist				__CTActionEvtSym(0x020)
#define EvtAction_TELSUN_SignalSetFail			__CTActionEvtSym(0x021)
#define EvtAction_TELSUN_SignalSetOK			__CTActionEvtSym(0x022)
#define EvtAction_TELSUN_SignalGeted			__CTActionEvtSym(0x023)
#define EvtAction_TELSUN_ValGetFail				__CTActionEvtSym(0x024)
#define EvtAction_TELSUN_ValGetOK				__CTActionEvtSym(0x025)
#define EvtAction_TELSUN_StopCall				__CTActionEvtSym(0x026)


#define EvtAction_TELSUN_AllocResourceFail		__CTActionEvtSym(0x100)
#define EvtAction_TELSUN_AllocResourceOK		__CTActionEvtSym(0x101)
#define EvtAction_TELSUN_DeallocResourceFail	__CTActionEvtSym(0x102)
#define EvtAction_TELSUN_DeallocResourceOK		__CTActionEvtSym(0x103)
#define EvtAction_TELSUN_OpenPromptFail			__CTActionEvtSym(0x104)
#define EvtAction_TELSUN_OpenPromptOK			__CTActionEvtSym(0x105)
#define EvtAction_TELSUN_CreateGroupFail		__CTActionEvtSym(0x108)
#define EvtAction_TELSUN_CreateGroupOK			__CTActionEvtSym(0x109)
#define EvtAction_TELSUN_DestroyGroupOK			__CTActionEvtSym(0x10a)
#define EvtAction_TELSUN_DestroyGroupFail		__CTActionEvtSym(0x10b)
#define EvtAction_TELSUN_StopWatch				__CTActionEvtSym(0x10c)
#define EvtAction_TELSUN_ClosePromptFail		__CTActionEvtSym(0x10d)
#define EvtAction_TELSUN_ClosePromptOK			__CTActionEvtSym(0x10e)


#define EvtAction_TELSUN_CallEstablished		__CTActionEvtSym(0x200)
#define EvtAction_TELSUN_Alerting				__CTActionEvtSym(0x201)
#define EvtAction_TELSUN_MessageGeted			__CTActionEvtSym(0x202)
#define EvtAction_TELSUN_StopAction				__CTActionEvtSym(0x203)
#define EvtAction_TELSUN_AgentMessage			__CTActionEvtSym(0x204)
#define EvtAction_TELSUN_WaitRespond			__CTActionEvtSym(0x205)
#define EvtAction_TELSUN_ActiveFlow				__CTActionEvtSym(0x206)
#define EvtAction_TELSUN_SetActiveFlow			__CTActionEvtSym(0x207)        
#define EvtAction_TELSUN_ContinueCatchEvent		__CTActionEvtSym(0x208)        

#endif
