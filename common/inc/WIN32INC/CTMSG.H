#ifndef __CTMSG_H__
#define __CTMSG_H__

#include "ctsymbol.h"

#define CT_msgCOMMAND 	      0x00    //message type is Command
#define CT_msgRESPOND 	      0x01    //message type is response
#define CT_msgEVENT   	      0x02    //message type is event

#define MSG_TYPE_OFFSET	      30
#define MSG_TYPE_MASK  	      0x03
#define MSG_OBJECT_OFFSET     20
#define MSG_OBJECT_MASK       0x03FF
#define MSG_VENDOR_OFFSET     10
#define MSG_VENDOR_MASK       0x03FF
#define MSG_ITEM_OFFSET       0
#define MSG_ITEM_MASK	      0X03FF

#define CTsym_BuildMsgID( msg, obj, vender, item) \
	  (CTsymbol)( ( (MSG_TYPE_MASK & (msg)) << MSG_TYPE_OFFSET ) |\
		      ( (MSG_OBJECT_MASK & (obj)) << MSG_OBJECT_OFFSET) |\
		      ( (MSG_VENDOR_MASK & (vender)) << MSG_VENDOR_OFFSET) |\
		      ( MSG_ITEM_MASK & (item) ) )

#define CTsym_BuildCmdID( obj, vender, item ) \
	CTsym_BuildMsgID( CT_msgCOMMAND, obj, vender, item)

#define CTsym_BuildRespID( obj, vender, item ) \
	CTsym_BuildMsgID( CT_msgRESPOND, obj, vender, item)

#define CTsym_BuildEvtID( obj, vender, item ) \
	CTsym_BuildMsgID( CT_msgEVENT, obj, vender, item)

#define GETMSGTYPE( msgID )  ( ( (msgID) >> MSG_TYPE_OFFSET) & MSG_TYPE_MASK)
#define GETOBJECT( msgID )   ( ( (msgID) >> MSG_OBJECT_OFFSET) & MSG_OBJECT_MASK)
#define GETVENDOR( msgID )   ( ( (msgID) >> MSG_VENDOR_OFFSET) & MSG_VENDOR_MASK)
#define GETITEM( msgID )     ( ( (msgID) >> MSG_ITEM_OFFSET)   & MSG_ITEM_MASK)


/******************************************************************************
 *    Definitions of public message.
 *****************************************************************************/
#define __CTPUBLICCmdTELSUNSym(item)	   CTsym_BuildCmdID( CT_objANY,\
							     CT_vendTELSUN,\
							     (item) )

#define CmdPUBLIC_TELSUN_PollInterval	   __CTPUBLICCmdTELSUNSym(0x00)

#define __CTPUBLICRespTELSUNSym(item)	   CTsym_BuildRespID( CT_objANY,\
							      CT_vendTELSUN,\
							      (item) )
#define RespPUBLIC_TELSUN_PollInterval	   __CTPUBLICRespTELSUNSym(0x00)

#define __CTPUBLICEvtTELSUNSym(item)	   CTsym_BuildEvtID( CT_objANY,\
							      CT_vendTELSUN,\
							      (item) )
#define EvtPUBLIC_TELSUN_ModuleActivate	   __CTPUBLICEvtTELSUNSym(0x01)
#define EvtPUBLIC_TELSUN_ModuleDeactivate  __CTPUBLICEvtTELSUNSym(0x02)
#define EvtPUBLIC_TELSUN_StationDisconnect __CTPUBLICEvtTELSUNSym(0x03)
#define EvtPUBLIC_TELSUN_CommonAlert       __CTPUBLICEvtTELSUNSym(0x04)
#define EvtPUBLIC_TELSUN_Shutdown          __CTPUBLICEvtTELSUNSym(0x05)
//Walker,1998-06
#define EvtPUBLIC_TELSUN_TimeOut           __CTPUBLICEvtTELSUNSym(0x06)
#define EvtPUBLIC_TELSUN_LoadConfig        __CTPUBLICEvtTELSUNSym(0x07)
#define EvtPUBLIC_TELSUN_PollInterval	   __CTPUBLICEvtTELSUNSym(0x08)
#define EvtPUBLIC_TELSUN_Alarm             __CTPUBLICEvtTELSUNSym(0x09)
//Gang Zhao, 1998-11
#define EvtPUBLIC_TELSUN_TimeOutClientPoll __CTPUBLICEvtTELSUNSym(0x0A)

#define EvtPUBLIC_TELSUN_LoadProfile       __CTPUBLICEvtTELSUNSym(0x0B)
#define EvtPUBLIC_TELSUN_Error             __CTPUBLICEvtTELSUNSym(0x0C)
#define EvtPUBLIC_TELSUN_StationConnect	   __CTPUBLICEvtTELSUNSym(0x0D)
#define EvtPUBLIC_TELSUN_UDPServerShutdown __CTPUBLICEvtTELSUNSym(0x0E)



//**********************************************************************
//CONFIG send messages
//**********************************************************************

//(1)Messages' type is COMMAND; CONFIG->COMM----------------------------
#define __CTCONFIGCmdExcelSym(item) CTsym_BuildCmdID( CT_objCONFIG,\
						      CT_vendEXCEL,\
						      (item) )

#define CmdCONFIG_EXCEL_IpAddr                      __CTCONFIGCmdExcelSym(0x0101)
#define CmdCONFIG_EXCEL_AnswerSuperModeID           __CTCONFIGCmdExcelSym(0x00bb)
#define CmdCONFIG_EXCEL_AssignEXSHost_Slave         __CTCONFIGCmdExcelSym(0x006e)
#define CmdCONFIG_EXCEL_AssignLogicNodeID           __CTCONFIGCmdExcelSym(0x0010)
#define CmdCONFIG_EXCEL_AssignLogicSpanID           __CTCONFIGCmdExcelSym(0x00a8)
#define CmdCONFIG_EXCEL_BusyOutFlagConfig           __CTCONFIGCmdExcelSym(0x00d3)
#define CmdCONFIG_EXCEL_CallProgressClassConfig     __CTCONFIGCmdExcelSym(0x00b3)
#define CmdCONFIG_EXCEL_CallProgressPatternConfig   __CTCONFIGCmdExcelSym(0x00b2)
#define CmdCONFIG_EXCEL_CCSRedundancyConfig         __CTCONFIGCmdExcelSym(0x005b)
#define CmdCONFIG_EXCEL_ClearSysSoftware            __CTCONFIGCmdExcelSym(0x000c)
#define CmdCONFIG_EXCEL_DistantReleaseModeConfig    __CTCONFIGCmdExcelSym(0x00b8)
#define CmdCONFIG_EXCEL_DownloadBeginSRecord        __CTCONFIGCmdExcelSym(0x00a2)
#define CmdCONFIG_EXCEL_DownloadBeginBRecord        __CTCONFIGCmdExcelSym(0x009b)
#define CmdCONFIG_EXCEL_DownloadBRecord             __CTCONFIGCmdExcelSym(0x009c)
#define CmdCONFIG_EXCEL_DownloadComplete            __CTCONFIGCmdExcelSym(0x00a4)
#define CmdCONFIG_EXCEL_DownloadSrecord             __CTCONFIGCmdExcelSym(0x00a3)
#define CmdCONFIG_EXCEL_DSPSIMMConfig               __CTCONFIGCmdExcelSym(0x00c0)
#define CmdCONFIG_EXCEL_E1SpanConfig                __CTCONFIGCmdExcelSym(0x00d8)
#define CmdCONFIG_EXCEL_EXNETRingConfig             __CTCONFIGCmdExcelSym(0x0074)
#define CmdCONFIG_EXCEL_EXSNodeConfig               __CTCONFIGCmdExcelSym(0x007f)
#define CmdCONFIG_EXCEL_Filter_TimerConfig          __CTCONFIGCmdExcelSym(0x0012)
#define CmdCONFIG_EXCEL_FlashTimingConfig           __CTCONFIGCmdExcelSym(0x0016)
#define CmdCONFIG_EXCEL_InPulsParamterConfig        __CTCONFIGCmdExcelSym(0x0028)
#define CmdCONFIG_EXCEL_InseizeListConfig           __CTCONFIGCmdExcelSym(0x0029)
#define CmdCONFIG_EXCEL_LocalReleaseModeConfig      __CTCONFIGCmdExcelSym(0x0021)
#define CmdCONFIG_EXCEL_LoopTimingConfig            __CTCONFIGCmdExcelSym(0x004a)
#define CmdCONFIG_EXCEL_OutseizeListConfig          __CTCONFIGCmdExcelSym(0x002a)
#define CmdCONFIG_EXCEL_PCMFormatConfig             __CTCONFIGCmdExcelSym(0x00d9)
#define CmdCONFIG_EXCEL_PollIntervalConfig          __CTCONFIGCmdExcelSym(0x009f)
#define CmdCONFIG_EXCEL_PPLAsign                    __CTCONFIGCmdExcelSym(0x00d1)
#define CmdCONFIG_EXCEL_PPLAuditConfig              __CTCONFIGCmdExcelSym(0x00dc)
#define CmdCONFIG_EXCEL_PPLCofig                    __CTCONFIGCmdExcelSym(0x00d7)
#define CmdCONFIG_EXCEL_PPLCreate                   __CTCONFIGCmdExcelSym(0x00d4)
#define CmdCONFIG_EXCEL_PPLDelete                   __CTCONFIGCmdExcelSym(0x00da)
#define CmdCONFIG_EXCEL_PPLTableDownload            __CTCONFIGCmdExcelSym(0x00d6)
#define CmdCONFIG_EXCEL_PPLTimerConfig              __CTCONFIGCmdExcelSym(0x00cf)
#define CmdCONFIG_EXCEL_PPLTransmitSignalConfig     __CTCONFIGCmdExcelSym(0x00d2)
#define CmdCONFIG_EXCEL_ReceiveSignalConfig         __CTCONFIGCmdExcelSym(0x0015)
#define CmdCONFIG_EXCEL_RecordAnnounDelete          __CTCONFIGCmdExcelSym(0x0054)
#define CmdCONFIG_EXCEL_RecordAnnounDownload        __CTCONFIGCmdExcelSym(0x0053)
#define CmdCONFIG_EXCEL_ResetConfig                 __CTCONFIGCmdExcelSym(0x000b)
#define CmdCONFIG_EXCEL_ServiceStateConfig          __CTCONFIGCmdExcelSym(0x000a)
#define CmdCONFIG_EXCEL_SpanFilterConfig            __CTCONFIGCmdExcelSym(0x00cd)
#define CmdCONFIG_EXCEL_SS7CICConfig                __CTCONFIGCmdExcelSym(0x006a)
#define CmdCONFIG_EXCEL_SS7ISUPMessFormatConfig     __CTCONFIGCmdExcelSym(0x006b)
#define CmdCONFIG_EXCEL_SS7SignalLinkConfig         __CTCONFIGCmdExcelSym(0x005e)
#define CmdCONFIG_EXCEL_SS7SignalLinkSetConfig      __CTCONFIGCmdExcelSym(0x005d)
#define CmdCONFIG_EXCEL_SS7SignalRouteConfig        __CTCONFIGCmdExcelSym(0x005f)
#define CmdCONFIG_EXCEL_SS7SignalStackConfig        __CTCONFIGCmdExcelSym(0x005c)
#define CmdCONFIG_EXCEL_StandbyLineCardConfig       __CTCONFIGCmdExcelSym(0x0023)
#define CmdCONFIG_EXCEL_StartDialConfig             __CTCONFIGCmdExcelSym(0x0013)
#define CmdCONFIG_EXCEL_SynchronPriorListConfig     __CTCONFIGCmdExcelSym(0x0006)
#define CmdCONFIG_EXCEL_SystemConfig                __CTCONFIGCmdExcelSym(0x00af)
#define CmdCONFIG_EXCEL_T1SpanConfig                __CTCONFIGCmdExcelSym(0x00a9)
#define CmdCONFIG_EXCEL_TagConfig                   __CTCONFIGCmdExcelSym(0x00d0)
#define CmdCONFIG_EXCEL_TimeSet                     __CTCONFIGCmdExcelSym(0x00b5)
#define CmdCONFIG_EXCEL_TransCadencePatternConfig   __CTCONFIGCmdExcelSym(0x0030)
#define CmdCONFIG_EXCEL_TrunkTypeConfig             __CTCONFIGCmdExcelSym(0x0011)
#define CmdCONFIG_EXCEL_TransSignalConfig           __CTCONFIGCmdExcelSym(0x0014)
#define CmdCONFIG_EXCEL_TransToneConfig             __CTCONFIGCmdExcelSym(0x0031)
#define CmdCONFIG_EXCEL_PPLTableInitiateDownLoad    __CTCONFIGCmdExcelSym(0x00d5)

////////////////////////////////////////////////////////////////////////////////
//(2)Messages' type is COMMAND; CONFIG->SWITCH-------------------------------
#define __CTCONFIGCmdSym(item) CTsym_BuildCmdID(CT_objCONFIG,\
						CT_vendTELSUN,\
						(item) )

#define CmdCONFIG_TELSUN_ConfigConform              __CTCONFIGCmdSym(0x0000)


////////////////////////////////////////////////////////////////////////////////
//(3)Messages' type is RESPONSE; CONFIG->MAINT--------------------------------
#define __CTCONFIGRespSym(item) CTsym_BuildRespID(CT_objCONFIG,\
						  CT_vendTELSUN,\
						  (item) )

#define RespCONFIG_TELSUN_AnswerSuperModeConfig     __CTCONFIGRespSym(0x00bb)
#define RespCONFIG_TELSUN_AssignEXSHostOrSlave      __CTCONFIGRespSym(0x006e)
#define RespCONFIG_TELSUN_AssignLogicalNodeID       __CTCONFIGRespSym(0x0001)
#define RespCONFIG_TELSUN_AssignLogicalSpanID       __CTCONFIGRespSym(0x00a8)
#define RespCONFIG_TELSUN_BChannelConfig            __CTCONFIGRespSym(0x00c8)
#define RespCONFIG_TELSUN_BusyOutFlagConfig         __CTCONFIGRespSym(0x00d3)
#define RespCONFIG_TELSUN_CallProgressAnalysisClassConfig \
						    __CTCONFIGRespSym(0x00b3)
#define RespCONFIG_TELSUN_CallProgressAnalysisPatternConfig \
						    __CTCONFIGRespSym(0x00b2)
#define RespCONFIG_TELSUN_CCSRedundancyConfig       __CTCONFIGRespSym(0x005b)
#define RespCONFIG_TELSUN_ClearSystemSoftware  	    __CTCONFIGRespSym(0x000c)
#define RespCONFIG_TELSUN_DChannelAssign            __CTCONFIGRespSym(0x00c4)
#define RespCONFIG_TELSUN_DChannelConfig            __CTCONFIGRespSym(0x00c7)
#define RespCONFIG_TELSUN_DChannelDeassign          __CTCONFIGRespSym(0x00c5)
#define RespCONFIG_TELSUN_DChannelFaciliListConfig  __CTCONFIGRespSym(0x00c6)
#define RespCONFIG_TELSUN_DistantReleaseModeConfig  __CTCONFIGRespSym(0x00b8)
#define RespCONFIG_TELSUN_DownloadBeginBRecord      __CTCONFIGRespSym(0x009b)
#define RespCONFIG_TELSUN_DownloadBeginSRecord      __CTCONFIGRespSym(0x00a2)
#define RespCONFIG_TELSUN_DownloadBRecord           __CTCONFIGRespSym(0x009c)
#define RespCONFIG_TELSUN_DownloadComplete          __CTCONFIGRespSym(0x00a4)
#define RespCONFIG_TELSUN_DownloadSRecord           __CTCONFIGRespSym(0x00a3)
#define RespCONFIG_TELSUN_DSPSIMMConfig             __CTCONFIGRespSym(0x00c0)
#define RespCONFIG_TELSUN_E1SpanConfig              __CTCONFIGRespSym(0x00d8)
#define RespCONFIG_TELSUN_EXNETRingConfig           __CTCONFIGRespSym(0x0074)
#define RespCONFIG_TELSUN_EXSNodeConfig             __CTCONFIGRespSym(0x007f)
#define RespCONFIG_TELSUN_Filter_timerConfig        __CTCONFIGRespSym(0x0012)
#define RespCONFIG_TELSUN_FlashTimingConfig         __CTCONFIGRespSym(0x0016)
#define RespCONFIG_TELSUN_InpulsingParametersConfig __CTCONFIGRespSym(0x0028)
#define RespCONFIG_TELSUN_InseizeInstructListConfig __CTCONFIGRespSym(0x0029)
#define RespCONFIG_TELSUN_LocalReleaseModeConfig    __CTCONFIGRespSym(0x0021)
#define RespCONFIG_TELSUN_LoopTimingConfig          __CTCONFIGRespSym(0x004a)
#define RespCONFIG_TELSUN_OutseizeInstructListConfig \
						    __CTCONFIGRespSym(0x002a)
#define RespCONFIG_TELSUN_PCMEncodeFormatConfig     __CTCONFIGRespSym(0x00d9)
#define RespCONFIG_TELSUN_PollIntervalConfig        __CTCONFIGRespSym(0x009f)
#define RespCONFIG_TELSUN_PPLAsign                  __CTCONFIGRespSym(0x00d1)
#define RespCONFIG_TELSUN_PPLAuditConfig            __CTCONFIGRespSym(0x00dc)
#define RespCONFIG_TELSUN_PPLConfig                 __CTCONFIGRespSym(0x00d7)
#define RespCONFIG_TELSUN_PPLCreat                  __CTCONFIGRespSym(0x00d4)
#define RespCONFIG_TELSUN_PPLDelete                 __CTCONFIGRespSym(0x00da)
#define RespCONFIG_TELSUN_PPLTableDownload          __CTCONFIGRespSym(0x00d6)
#define RespCONFIG_TELSUN_PPLTableInitiateDownload  __CTCONFIGRespSym(0x00d5)
#define RespCONFIG_TELSUN_PPLTimerConfig            __CTCONFIGRespSym(0x00cf)
#define RespCONFIG_TELSUN_PPLTransmitSignalConfig   __CTCONFIGRespSym(0x00d2)
#define RespCONFIG_TELSUN_ReceiveSignalConfig       __CTCONFIGRespSym(0x0015)
#define RespCONFIG_TELSUN_RecordAnnounceDelete      __CTCONFIGRespSym(0x0054)
#define RespCONFIG_TELSUN_RecordAnnounceDownload    __CTCONFIGRespSym(0x0053)
#define RespCONFIG_TELSUN_RecordAnnounceDownloadInit \
						    __CTCONFIGRespSym(0x0052)
#define RespCONFIG_TELSUN_ResetConfig               __CTCONFIGRespSym(0x00c4)
#define RespCONFIG_TELSUN_ServiceStateConfig        __CTCONFIGRespSym(0x000a)
#define RespCONFIG_TELSUN_SpanFilterConfig          __CTCONFIGRespSym(0x00cd)
#define RespCONFIG_TELSUN_SS7CICConfig              __CTCONFIGRespSym(0x006a)
#define RespCONFIG_TELSUN_SS7ISUPMessageFormatConfig \
						    __CTCONFIGRespSym(0x006b)
#define RespCONFIG_TELSUN_SS7SignalLinkConfig       __CTCONFIGRespSym(0x005e)
#define RespCONFIG_TELSUN_SS7SignalLinkSetConfig    __CTCONFIGRespSym(0x005d)
#define RespCONFIG_TELSUN_SS7SignalRouteConfig      __CTCONFIGRespSym(0x005f)
#define RespCONFIG_TELSUN_SS7SignalStackConfig      __CTCONFIGRespSym(0x005c)
#define RespCONFIG_TELSUN_StandbyLindCardConfig     __CTCONFIGRespSym(0x0023)
#define RespCONFIG_TELSUN_StartDialConfig           __CTCONFIGRespSym(0x0013)
#define RespCONFIG_TELSUN_SynchroPriorityListConfig __CTCONFIGRespSym(0x0006)
#define RespCONFIG_TELSUN_SystemConfig              __CTCONFIGRespSym(0x00af)
#define RespCONFIG_TELSUN_TagConfig                 __CTCONFIGRespSym(0x00d0)
#define RespCONFIG_TELSUN_TimeSet                   __CTCONFIGRespSym(0x00b5)
#define RespCONFIG_TELSUN_TransCadencePatternConfig __CTCONFIGRespSym(0x0030)
#define RespCONFIG_TELSUN_TransSignalConfig         __CTCONFIGRespSym(0x0014)
#define RespCONFIG_TELSUN_TransToneConfig           __CTCONFIGRespSym(0x0031)
#define RespCONFIG_TELSUN_VersionRequest            __CTCONFIGRespSym(0x0002)

////////////////////////////////////////////////////////////////////////////////
//CONFIG send out-----------------------------------------------------------
#define __CTCONFIGEvtSym(item) CTsym_BuildEvtID(CT_objCONFIG,\
						CT_vendTELSUN,\
						(item) )

#define EvtCONFIG_TELSUN_ConfigData              __CTCONFIGEvtSym(0x0000)
#define EvtCONFIG_TELSUN_ConfigEnd               __CTCONFIGEvtSym(0x0001)

//**************************************************************************
//MAINT send messages
//**************************************************************************
//(1)Messages' type is COMMAND; MAINT->SWITCH--------------------------------
#define __CTMAINTCmdSym(item) CTsym_BuildCmdID(CT_objMAINT,\
					       CT_vendTELSUN,\
					       (item) )


//#define CmdMAINT_TELSUN_BusyOutFlagConfig         __CTMAINTCmdSys(0x00d3)
#define CmdMAINT_TELSUN_Connect                   __CTMAINTCmdSym(0x0000)
#define CmdMAINT_TELSUN_ReleaseChannel            __CTMAINTCmdSym(0x0008)
#define CmdMAINT_TELSUN_BusyOut                   __CTMAINTCmdSym(0x0018)
#define CmdMAINT_TELSUN_ConnectToConference       __CTMAINTCmdSym(0x004e)
#define CmdMAINT_TELSUN_ConnectOneWayToConferece  __CTMAINTCmdSym(0x004f)
#define CmdMAINT_TELSUN_ConnectOneWayForced       __CTMAINTCmdSym(0x0050)
#define CmdMAINT_TELSUN_RecordAnnounceConnect     __CTMAINTCmdSym(0x0055)
#define CmdMAINT_TELSUN_RecordAnnounceDisconnect  __CTMAINTCmdSym(0x0056)
#define CmdMAINT_TELSUN_DSPServiceCancel          __CTMAINTCmdSym(0x00be)

//(2)Messages' type is COMMAND;MAINT->CONFIG------------------------------
#define CmdMAINT_TELSUN_AnswerSuperModeConfig       __CTMAINTCmdSym(0x00bb)
#define CmdMAINT_TELSUN_AssignEXSHostOrSlave        __CTMAINTCmdSym(0x006e)
#define CmdMAINT_TELSUN_AssignLogicalNodeID         __CTMAINTCmdSym(0x0001)
#define CmdMAINT_TELSUN_AssignLogicalSpanID         __CTMAINTCmdSym(0x00a8)
#define CmdMAINT_TELSUN_BChannelConfig              __CTMAINTCmdSym(0x00c8)
#define CmdMAINT_TELSUN_BusyOutFlagConfig           __CTMAINTCmdSym(0x00d3)
#define CmdMAINT_TELSUN_CallProgressAnalysisClassConfig \
						    __CTMAINTCmdSym(0x00b3)
#define CmdMAINT_TELSUN_CallProgressAnalysisPatternConfig \
						    __CTMAINTCmdSym(0x00b2)
#define CmdMAINT_TELSUN_CCSRedundancyConfig         __CTMAINTCmdSym(0x005b)
#define CmdMAINT_TELSUN_ClearSystemSoftware  	    __CTMAINTCmdSym(0x000c)
#define CmdMAINT_TELSUN_DChannelAssign              __CTMAINTCmdSym(0x00c4)
#define CmdMAINT_TELSUN_DChannelConfig              __CTMAINTCmdSym(0x00c7)
#define CmdMAINT_TELSUN_DChannelDeassign            __CTMAINTCmdSym(0x00c5)
#define CmdMAINT_TELSUN_DChannelFaciliListConfig    __CTMAINTCmdSym(0x00c6)
#define CmdMAINT_TELSUN_DistantReleaseModeConfig    __CTMAINTCmdSym(0x00b8)
#define CmdMAINT_TELSUN_DownloadBeginBRecord        __CTMAINTCmdSym(0x009b)
#define CmdMAINT_TELSUN_DownloadBeginSRecord        __CTMAINTCmdSym(0x00a2)
#define CmdMAINT_TELSUN_DownloadBRecord             __CTMAINTCmdSym(0x009c)
#define CmdMAINT_TELSUN_DownloadComplete            __CTMAINTCmdSym(0x00a4)
#define CmdMAINT_TELSUN_DownloadSRecord             __CTMAINTCmdSym(0x00a3)
#define CmdMAINT_TELSUN_DSPSIMMConfig               __CTMAINTCmdSym(0x00c0)
#define CmdMAINT_TELSUN_E1SpanConfig                __CTMAINTCmdSym(0x00d8)
#define CmdMAINT_TELSUN_EXNETRingConfig             __CTMAINTCmdSym(0x0074)
#define CmdMAINT_TELSUN_EXSNodeConfig               __CTMAINTCmdSym(0x007f)
#define CmdMAINT_TELSUN_Filter_timerConfig          __CTMAINTCmdSym(0x0012)
#define CmdMAINT_TELSUN_FlashTimingConfig           __CTMAINTCmdSym(0x0016)
#define CmdMAINT_TELSUN_InpulsingParametersConfig   __CTMAINTCmdSym(0x0028)
#define CmdMAINT_TELSUN_InseizeInstructListConfig   __CTMAINTCmdSym(0x0029)
#define CmdMAINT_TELSUN_LocalReleaseModeConfig      __CTMAINTCmdSym(0x0021)
#define CmdMAINT_TELSUN_LoopTimingConfig            __CTMAINTCmdSym(0x004a)
#define CmdMAINT_TELSUN_OutseizeInstructListConfig  __CTMAINTCmdSym(0x002a)
#define CmdMAINT_TELSUN_PCMEncodeFormatConfig       __CTMAINTCmdSym(0x00d9)
#define CmdMAINT_TELSUN_PollIntervalConfig          __CTMAINTCmdSym(0x009f)
#define CmdMAINT_TELSUN_PPLAsign                    __CTMAINTCmdSym(0x00d1)
#define CmdMAINT_TELSUN_PPLAuditConfig              __CTMAINTCmdSym(0x00dc)
#define CmdMAINT_TELSUN_PPLConfig                   __CTMAINTCmdSym(0x00d7)
#define CmdMAINT_TELSUN_PPLCreat                    __CTMAINTCmdSym(0x00d4)
#define CmdMAINT_TELSUN_PPLDelete                   __CTMAINTCmdSym(0x00da)
#define CmdMAINT_TELSUN_PPLTableDownload            __CTMAINTCmdSym(0x00d6)
#define CmdMAINT_TELSUN_PPLTableInitiateDownload    __CTMAINTCmdSym(0x00d5)
#define CmdMAINT_TELSUN_PPLTimerConfig              __CTMAINTCmdSym(0x00cf)
#define CmdMAINT_TELSUN_PPLTransmitSignalConfig     __CTMAINTCmdSym(0x00d2)
#define CmdMAINT_TELSUN_ReceiveSignalConfig         __CTMAINTCmdSym(0x0015)
#define CmdMAINT_TELSUN_RecordAnnounceDelete        __CTMAINTCmdSym(0x0054)
#define CmdMAINT_TELSUN_RecordAnnounceDownload      __CTMAINTCmdSym(0x0053)
#define CmdMAINT_TELSUN_RecordAnnounceDownloadInit  __CTMAINTCmdSym(0x0052)
#define CmdMAINT_TELSUN_ResetConfig                 __CTMAINTCmdSym(0x00c4)
#define CmdMAINT_TELSUN_ServiceStateConfig          __CTMAINTCmdSym(0x000a)
#define CmdMAINT_TELSUN_SpanFilterConfig            __CTMAINTCmdSym(0x00cd)
#define CmdMAINT_TELSUN_SS7CICConfig                __CTMAINTCmdSym(0x006a)
#define CmdMAINT_TELSUN_SS7ISUPMessageFormatConfig  __CTMAINTCmdSym(0x006b)
#define CmdMAINT_TELSUN_SS7SignalLinkConfig         __CTMAINTCmdSym(0x005e)
#define CmdMAINT_TELSUN_SS7SignalLinkSetConfig      __CTMAINTCmdSym(0x005d)
#define CmdMAINT_TELSUN_SS7SignalRouteConfig        __CTMAINTCmdSym(0x005f)
#define CmdMAINT_TELSUN_SS7SignalStackConfig        __CTMAINTCmdSym(0x005c)
#define CmdMAINT_TELSUN_StandbyLindCardConfig       __CTMAINTCmdSym(0x0023)
#define CmdMAINT_TELSUN_StartDialConfig             __CTMAINTCmdSym(0x0013)
#define CmdMAINT_TELSUN_SynchroPriorityListConfig   __CTMAINTCmdSym(0x0006)
#define CmdMAINT_TELSUN_SystemConfig                __CTMAINTCmdSym(0x00af)
#define CmdMAINT_TELSUN_TagConfig                   __CTMAINTCmdSym(0x00d0)
#define CmdMAINT_TELSUN_TimeSet                     __CTMAINTCmdSym(0x00b5)
#define CmdMAINT_TELSUN_TransCadencePatternConfig   __CTMAINTCmdSym(0x0030)
#define CmdMAINT_TELSUN_TransSignalConfig           __CTMAINTCmdSym(0x0014)
#define CmdMAINT_TELSUN_TransToneConfig             __CTMAINTCmdSym(0x0031)
#define CmdMAINT_TELSUN_VersionRequest              __CTMAINTCmdSym(0x0002)

////////////////////////////////////////////////////////////////////////
//TS2000 add
#define CmdMAINT_TELSUN_Console						__CTMAINTCmdSym(0x0101)
#define CmdMAINT_TELSUN_QueryResource				__CTMAINTCmdSym(0x0102)
#define CmdMAINT_TELSUN_QueryData					__CTMAINTCmdSym(0x0103)
#define CmdMAINT_TELSUN_QueryInformation			__CTMAINTCmdSym(0x0104)
#define CmdMAINT_TELSUN_QueryConference				__CTMAINTCmdSym(0x0105)
///////////////
#define __CTMAINTRespSym(item) CTsym_BuildRespID(CT_objMAINT,\
					       CT_vendTELSUN,\
					       (item) )
#define RespMAINT_TELSUN_Console					__CTMAINTRespSym(0x0101)
#define RespMAINT_TELSUN_QueryResource				__CTMAINTRespSym(0x0102)
#define RespMAINT_TELSUN_QueryData					__CTMAINTRespSym(0x0102)
#define RespMAINT_TELSUN_QueryInformation			__CTMAINTRespSym(0x0104)
#define RespMAINT_TELSUN_QueryConference			__CTMAINTRespSym(0x0105)
//////////////////





//(3)Messages' type is COMMAND;MAINT->COMMCTRL--------------------------
#define __CTMAINTCmdExcelSym(item)	CTsym_BuildCmdID(CT_objMAINT,\
													 CT_vendEXCEL,\
													 (item))
#define CmdMAINT_EXCEL_BChannelQuery				__CTMAINTCmdExcelSym(0x00ca)
#define CmdMAINT_EXCEL_CallControlInstrucQuery	__CTMAINTCmdExcelSym(0x0087)
#define CmdMAINT_EXCEL_CallProgressAnalysisConfigQuery	\
																__CTMAINTCmdExcelSym(0x008a)
#define CmdMAINT_EXCEL_CardPopulationQuery		__CTMAINTCmdExcelSym(0x0007)
#define CmdMAINT_EXCEL_CardStatusQuery 			__CTMAINTCmdExcelSym(0x0083)
#define CmdMAINT_EXCEL_CCSRedundancyQuery 		__CTMAINTCmdExcelSym(0x006c)
#define CmdMAINT_EXCEL_ChannelConnectStatusQuery \
																__CTMAINTCmdExcelSym(0x0001)
#define CmdMAINT_EXCEL_ChannelParameterQuery 	__CTMAINTCmdExcelSym(0x0080)
#define CmdMAINT_EXCEL_DChannelFacilityListQuery \
																__CTMAINTCmdExcelSym(0x00cb)
#define CmdMAINT_EXCEL_DiagnosticsIndication 	__CTMAINTCmdExcelSym(0x0045)
#define CmdMAINT_EXCEL_E1SpanQuery 					__CTMAINTCmdExcelSym(0x00db)
#define CmdMAINT_EXCEL_EXSNodeConfigQuery 		__CTMAINTCmdExcelSym(0x007e)
#define CmdMAINT_EXCEL_FaultLogQuery 				__CTMAINTCmdExcelSym(0x0086)
#define CmdMAINT_EXCEL_GenericReport 				__CTMAINTCmdExcelSym(0x0046)
#define CmdMAINT_EXCEL_InpulseParametersQuery 	__CTMAINTCmdExcelSym(0x0089)
#define CmdMAINT_EXCEL_NodeStatusQuery 			__CTMAINTCmdExcelSym(0x006f)
#define CmdMAINT_EXCEL_PPLAuditQuery 			__CTMAINTCmdExcelSym(0x00dd)
#define CmdMAINT_EXCEL_PPLDataQueryBySlot		__CTMAINTCmdExcelSym(0x00de)
#define CmdMAINT_EXCEL_PPLDataQueryByChannel	__CTMAINTCmdExcelSym(0x0120)
#define CmdMAINT_EXCEL_PPLProtocolQuery 			__CTMAINTCmdExcelSym(0x00df)
#define CmdMAINT_EXCEL_RecordedAnnounceQuery 	__CTMAINTCmdExcelSym(0x0057)
#define CmdMAINT_EXCEL_RingStatusQuery 			__CTMAINTCmdExcelSym(0x71)
#define CmdMAINT_EXCEL_SpanFilterQuery 			__CTMAINTCmdExcelSym(0x00ce)
#define CmdMAINT_EXCEL_SpanStatusQuery 			__CTMAINTCmdExcelSym(0x0084)
#define CmdMAINT_EXCEL_SS7CICQuery 					__CTMAINTCmdExcelSym(0x0067)
#define CmdMAINT_EXCEL_SS7SignalLinkQuery 		__CTMAINTCmdExcelSym(0x0065)
#define CmdMAINT_EXCEL_SS7SignalLinkSetQuery 	__CTMAINTCmdExcelSym(0x0064)
#define CmdMAINT_EXCEL_SS7SignalRouteQuery 		__CTMAINTCmdExcelSym(0x0066)
#define CmdMAINT_EXCEL_SS7SignalStackQuery 		__CTMAINTCmdExcelSym(0x006d)
#define CmdMAINT_EXCEL_SynchroPriorityListQuery __CTMAINTCmdExcelSym(0x0081)
#define CmdMAINT_EXCEL_SystemConfigQuery 			__CTMAINTCmdExcelSym(0x00b4)
#define CmdMAINT_EXCEL_SystemLogQuery 				__CTMAINTCmdExcelSym(0x0082)
#define CmdMAINT_EXCEL_SystemResourceUtilizeQuery \
												__CTMAINTCmdExcelSym(0x008e)
#define CmdMAINT_EXCEL_TransmitCadencePatternQuery \
																__CTMAINTCmdExcelSym(0x0059)
#define CmdMAINT_EXCEL_TransmitToneQuery 			__CTMAINTCmdExcelSym(0x005a)
#define CmdMAINT_EXCEL_VersionRequest 				__CTMAINTCmdExcelSym(0x0002)


//**************************************************************************
//SWITCH send messages
//***************************************************************************
//(1)Messages' type is COMMAND; SWITCH->COMM----------------------------------
#define __CTSWITCHCmdSys(item) CTsym_BuildCmdID(CT_objSWITCH,\
						CT_vendTELSUN,\
						(item))

#define CmdSWITCH_TELSUN_ReleaseIncomingChannel __CTSWITCHCmdSys(0) //SC->SC

#define __CTSWITCHCmdExcelSym(item) CTsym_BuildCmdID( CT_objSWITCH,\
							 CT_vendEXCEL,\
							  (item) )
#define CmdSWITCH_EXCEL_ConnectWait            __CTSWITCHCmdExcelSym(0x17) //SC->PSP
#define CmdSWITCH_EXCEL_Connect                __CTSWITCHCmdExcelSym(0x00) //SC->PSP
#define CmdSWITCH_EXCEL_ReleaseChannelWData    __CTSWITCHCmdExcelSym(0x36) //SC->PSP
#define CmdSWITCH_EXCEL_ReleaseChannel         __CTSWITCHCmdExcelSym(0x08) //SC->PSP
#define CmdSWITCH_EXCEL_ParkChannel            __CTSWITCHCmdExcelSym(0xbf) //SC->PSP
#define CmdSWITCH_EXCEL_OutseizeControl        __CTSWITCHCmdExcelSym(0x2c) //SC->PSP
#define CmdSWITCH_EXCEL_ConnectOneWayForced    __CTSWITCHCmdExcelSym(0x50) //SC->PSP
#define CmdSWITCH_EXCEL_GenerateCallProcessingEvent __CTSWITCHCmdSym(0xba) //SC->COMM
#define CmdSWITCH_EXCEL_PPLEventRequest        __CTSWITCHCmdExcelSym(0x44) //SC->PSP 

////////////////////////////////////////////////////////////////////////////////
//(2)Messages' type is COMMAND; SWITCH->SWITCH-------------------------------
#define __CTSWITCHCmdSym(item) CTsym_BuildCmdID( CT_objSWITCH,\
						 CT_vendTELSUN,\
						 (item) )

////////////////////////////////////////////////////////////////////////////////
//(3)Messages' type is RESPONSE; SWITCH->COMM----------------------------------
#define __CTSWITCHRespExcelSym(item) CTsym_BuildRespID( CT_objSWITCH,\
							CT_vendEXCEL,\
							(item) )

#define RespSWITCH_EXCEL_Connect                __CTSWITCHRespExcelSym(0x00)//SC->PSP
#define RespSWITCH_EXCEL_ChannelReleased        __CTSWITCHRespExcelSym(0x49)//SC->PSP
#define RespSWITCH_EXCEL_ChannelReleaseRequest  __CTSWITCHRespExcelSym(0x37)//SC->PSP

////////////////////////////////////////////////////////////////////////////////
//(4)Messages' type is RESPONSE; SWITCH->SCR-----------------------------------
#define __CTSWITCHRespSym(item) CTsym_BuildRespID( CT_objSWITCH,\
						   CT_vendTELSUN,\
						   (item) )

#define RespSWITCH_TELSUN_Incoming	             __CTSWITCHRespSym(0x001)  //SCR->SC
#define RespSWITCH_TELSUN_OutseizeControlTimeOut     __CTSWITCHRespSym(0x100)//SC->SC
#define RespSWITCH_TELSUN_ReleaseChannelTimeOut      __CTSWITCHRespSym(0x101)//SC->SC
#define RespSWITCH_TELSUN_ParkChannelTimeOut         __CTSWITCHRespSym(0x102)//SC->SC
#define RespSWITCH_TELSUN_ConnectTimeOut             __CTSWITCHRespSym(0x103)//SC->SC
#define RespSWITCH_TELSUN_ConnectOneWayForcedTimeOut __CTSWITCHRespSym(0x104)//SC->SC
#define RespSWITCH_TELSUN_ReleaseChannelWDataTimeOut __CTSWITCHRespSym(0x105)//SC->SC

//(5)Messages' type is RESPONSE;SWITCH->MAINT------------------------------
#define RespSWITCH_TELSUN_BusyOut                   __CTSWITCHRespSym(0x0018)
#define RespSWITCH_TELSUN_BusyOutFlagConfig         __CTSWITCHRespSym(0x00d3)
#define RespSWITCH_TELSUN_Connect                   __CTSWITCHRespSym(0x0000)
#define RespSWITCH_TELSUN_ConnectOneWayForced       __CTSWITCHRespSym(0x0050)
#define RespSWITCH_TELSUN_ConnectOneWayToConferece  __CTSWITCHRespSym(0x004f)
#define RespSWITCH_TELSUN_ConnectToConference       __CTSWITCHRespSym(0x004e)
#define RespSWITCH_TELSUN_DSPServiceCancel          __CTSWITCHRespSym(0x00be)
#define RespSWITCH_TELSUN_RecordAnnounceConnect     __CTSWITCHRespSym(0x0055)
#define RespSWITCH_TELSUN_RecordAnnounceDisconnect  __CTSWITCHRespSym(0x0056)
#define RespSWITCH_TELSUN_ReleaseChannel            __CTSWITCHRespSym(0x0008)

//(6)Messages' type is RESPONSE; SWITCH->CONFIG----------------------------

#define RespSWITCH_TELSUN_ConfigConform        __CTSWITCHRespSym(0x0010)
#define RespSWITCH_TELSUN_ConfigData           __CTSWITCHRespSym(0x0011)
#define RespSWITCH_TELSUN_ConfigEnd            __CTSWITCHRespSym(0x0012)

//(7)Messages' type is RESPONSE; SWITCH->SWITCH----------------------------
#define RespSWITCH_TELSUN_ConfigTimeOut        __CTSWITCHRespSym(0x0013)//SC->SC


//*************************************************************************
//COMM send messages
//*************************************************************************

//(1)Messages' type is EVENT; COMM->SWITCH----------------------------------
#define __CTCOMMCmdSym(item) CTsym_BuildCmdID( CT_objCOMMCTRL,\
					       CT_vendEXCEL,\
					       (item) )

#define CmdCOMM_EXCEL_ChannelReleaseRequest   __CTCOMMCmdSym(0x37) //COMM->SC
#define CmdCOMM_EXCEL_ReleaseChannelWData	__CTCOMMCmdSym(0x36) //COMM->SC

////////////////////////////////////////////////////////////////////////////////
//(2)Messages' type is RESPONSE; COMM->SWITCH---------------------------------
#define __CTCOMMRespSym(item) CTsym_BuildRespID( CT_objCOMMCTRL,\
						 CT_vendEXCEL,\
						 (item) )

#define RespCOMM_EXCEL_OutseizeControl     __CTCOMMRespSym(0x2c)  //COMM->SC
#define RespCOMM_EXCEL_ReleaseChannel      __CTCOMMRespSym(0x08)  //COMM->SC
#define RespCOMM_EXCEL_Connect             __CTCOMMRespSym(0x00)  //COMM->SC
#define RespCOMM_EXCEL_ParkChannel         __CTCOMMRespSym(0xbf)  //COMM->SC
#define RespCOMM_EXCEL_GenerateCallProcessingEvent  __CTCOMMRespSym(0x0ba)//COMM->SC                                                                   
#define RespCOMM_EXCEL_ConnectOneWayForced  __CTCOMMRespSym(0x050) //COMM->SC
#define RespCOMM_EXCEL_PPLEventRequest      __CTCOMMRespSym(0x44) //COMM->SC   

//(3)Messages' type is RESPONSE; COMM->CONFIG------------------------------
#define RespCOMM_EXCEL_IpAddr                      __CTCOMMRespSym(0x0101)
#define RespCOMM_EXCEL_AnswerSuperModeID           __CTCOMMRespSym(0x00bb)
#define RespCOMM_EXCEL_AssignEXSHost_Slave         __CTCOMMRespSym(0x006e)
#define RespCOMM_EXCEL_AssignLogicNodeID           __CTCOMMRespSym(0x0010)
#define RespCOMM_EXCEL_AssignLogicSpanID           __CTCOMMRespSym(0x00a8)
#define RespCOMM_EXCEL_BusyOutFlagConfig           __CTCOMMRespSym(0x00d3)
#define RespCOMM_EXCEL_CallProgressClassConfig     __CTCOMMRespSym(0x00b3)
#define RespCOMM_EXCEL_CallProgressPatternConfig   __CTCOMMRespSym(0x00b2)
#define RespCOMM_EXCEL_CCSRedundancyConfig         __CTCOMMRespSym(0x005b)
#define RespCOMM_EXCEL_ClearSysSoftware            __CTCOMMRespSym(0x000c)
#define RespCOMM_EXCEL_DistantReleaseModeConfig    __CTCOMMRespSym(0x00b8)
#define RespCOMM_EXCEL_DownloadBeginSRecord        __CTCOMMRespSym(0x00a2)
#define RespCOMM_EXCEL_DownloadBeginBRecord        __CTCOMMRespSym(0x009b)
#define RespCOMM_EXCEL_DownloadBRecord             __CTCOMMRespSym(0x009c)
#define RespCOMM_EXCEL_DownloadComplete            __CTCOMMRespSym(0x00a4)
#define RespCOMM_EXCEL_DownloadSrecord             __CTCOMMRespSym(0x00a3)
#define RespCOMM_EXCEL_DSPSIMMConfig               __CTCOMMRespSym(0x00c0)
#define RespCOMM_EXCEL_E1SpanConfig                __CTCOMMRespSym(0x00d8)
#define RespCOMM_EXCEL_EXNETRingConfig             __CTCOMMRespSym(0x0074)
#define RespCOMM_EXCEL_EXSNodeConfig               __CTCOMMRespSym(0x007f)
#define RespCOMM_EXCEL_Filter_TimerConfig          __CTCOMMRespSym(0x0012)
#define RespCOMM_EXCEL_FlashTimingConfig           __CTCOMMRespSym(0x0016)
#define RespCOMM_EXCEL_InPulsParamterConfig        __CTCOMMRespSym(0x0028)
#define RespCOMM_EXCEL_InseizeListConfig           __CTCOMMRespSym(0x0029)
#define RespCOMM_EXCEL_LocalReleaseModeConfig      __CTCOMMRespSym(0x0021)
#define RespCOMM_EXCEL_LoopTimingConfig            __CTCOMMRespSym(0x004a)
#define RespCOMM_EXCEL_OutseizeListConfig          __CTCOMMRespSym(0x002a)
#define RespCOMM_EXCEL_PCMFormatConfig             __CTCOMMRespSym(0x00d9)
#define RespCOMM_EXCEL_PollIntervalConfig          __CTCOMMRespSym(0x009f)
#define RespCOMM_EXCEL_PPLAsign                    __CTCOMMRespSym(0x00d1)
#define RespCOMM_EXCEL_PPLAuditConfig              __CTCOMMRespSym(0x00dc)
#define RespCOMM_EXCEL_PPLCofig                    __CTCOMMRespSym(0x00d7)
#define RespCOMM_EXCEL_PPLCreate                   __CTCOMMRespSym(0x00d4)
#define RespCOMM_EXCEL_PPLDelete                   __CTCOMMRespSym(0x00da)
#define RespCOMM_EXCEL_PPLTableDownload            __CTCOMMRespSym(0x00d6)
#define RespCOMM_EXCEL_PPLTimerConfig              __CTCOMMRespSym(0x00cf)
#define RespCOMM_EXCEL_PPLTransmitSignalConfig     __CTCOMMRespSym(0x00d2)
#define RespCOMM_EXCEL_ReceiveSignalConfig         __CTCOMMRespSym(0x0015)
#define RespCOMM_EXCEL_RecordAnnounDelete          __CTCOMMRespSym(0x0054)
#define RespCOMM_EXCEL_RecordAnnounDownload        __CTCOMMRespSym(0x0053)
#define RespCOMM_EXCEL_ResetConfig                 __CTCOMMRespSym(0x000b)
#define RespCOMM_EXCEL_ServiceStateConfig          __CTCOMMRespSym(0x000a)
#define RespCOMM_EXCEL_SpanFilterConfig            __CTCOMMRespSym(0x00cd)
#define RespCOMM_EXCEL_SS7CICConfig                __CTCOMMRespSym(0x006a)
#define RespCOMM_EXCEL_SS7ISUPMessFormatConfig     __CTCOMMRespSym(0x006b)
#define RespCOMM_EXCEL_SS7SignalLinkConfig         __CTCOMMRespSym(0x005e)
#define RespCOMM_EXCEL_SS7SignalLinkSetConfig      __CTCOMMRespSym(0x005d)
#define RespCOMM_EXCEL_SS7SignalRouteConfig        __CTCOMMRespSym(0x005f)
#define RespCOMM_EXCEL_SS7SignalStackConfig        __CTCOMMRespSym(0x005c)
#define RespCOMM_EXCEL_StandbyLineCardConfig       __CTCOMMRespSym(0x0023)
#define RespCOMM_EXCEL_StartDialConfig             __CTCOMMRespSym(0x0013)
#define RespCOMM_EXCEL_SynchronPriorListConfig     __CTCOMMRespSym(0x0006)
#define RespCOMM_EXCEL_SystemConfig                __CTCOMMRespSym(0x00af)
#define RespCOMM_EXCEL_T1SpanConfig                __CTCOMMRespSym(0x00a9)
#define RespCOMM_EXCEL_TagConfig                   __CTCOMMRespSym(0x00d0)
#define RespCOMM_EXCEL_TimeSet                     __CTCOMMRespSym(0x00b5)
#define RespCOMM_EXCEL_TransCadencePatternConfig   __CTCOMMRespSym(0x0030)
#define RespCOMM_EXCEL_TrunkTypeConfig             __CTCOMMRespSym(0x0011)
#define RespCOMM_EXCEL_TransSignalConfig           __CTCOMMRespSym(0x0014)
#define RespCOMM_EXCEL_TransToneConfig             __CTCOMMRespSym(0x0031)

//(4) Messages' type is RESPONSE;COMMCTRL->MAINT----------------------------
#define RespCOMM_EXCEL_BChannelQuery				__CTCOMMRespSym(0x00ca)
#define RespCOMM_EXCEL_CallControlInstrucQuery	__CTCOMMRespSym(0x0087)
#define RespCOMM_EXCEL_CallProgressAnalysisConfigQuery	\
																__CTCOMMRespSym(0x008a)
#define RespCOMM_EXCEL_CardPopulationQuery		__CTCOMMRespSym(0x0007)
#define RespCOMM_EXCEL_CardStatusQuery 			__CTCOMMRespSym(0x0083)
#define RespCOMM_EXCEL_CCSRedundancyQuery 		__CTCOMMRespSym(0x006c)
#define RespCOMM_EXCEL_ChannelConnectStatusQuery \
																__CTCOMMRespSym(0x0001)
#define RespCOMM_EXCEL_ChannelParameterQuery 	__CTCOMMRespSym(0x0080)
#define RespCOMM_EXCEL_DChannelFacilityListQuery \
																__CTCOMMRespSym(0x00cb)
#define RespCOMM_EXCEL_DiagnosticsIndication 	__CTCOMMRespSym(0x0045)
#define RespCOMM_EXCEL_E1SpanQuery 					__CTCOMMRespSym(0x00db)
#define RespCOMM_EXCEL_EXSNodeConfigQuery 		__CTCOMMRespSym(0x007e)
#define RespCOMM_EXCEL_FaultLogQuery 				__CTCOMMRespSym(0x0086)
#define RespCOMM_EXCEL_GenericReport 				__CTCOMMRespSym(0x0046)
#define RespCOMM_EXCEL_InpulseParametersQuery 	__CTCOMMRespSym(0x0089)
#define RespCOMM_EXCEL_NodeStatusQuery 			__CTCOMMRespSym(0x006f)
#define RespCOMM_EXCEL_PPLAuditQuery 				__CTCOMMRespSym(0x00dd)
#define RespCOMM_EXCEL_PPLDataQueryBySlot		__CTCOMMRespSym(0x00de)
#define RespCOMM_EXCEL_PPLDataQueryByChannel	__CTCOMMRespSym(0x0120)
#define RespCOMM_EXCEL_PPLProtocolQuery 			__CTCOMMRespSym(0x00df)
#define RespCOMM_EXCEL_RecordedAnnounceQuery 	__CTCOMMRespSym(0x0057)
#define RespCOMM_EXCEL_RingStatusQuery 			__CTCOMMRespSym(0x0071)
#define RespCOMM_EXCEL_SpanFilterQuery 			__CTCOMMRespSym(0x00ce)
#define RespCOMM_EXCEL_SpanStatusQuery 			__CTCOMMRespSym(0x0084)
#define RespCOMM_EXCEL_SS7CICQuery 					__CTCOMMRespSym(0x0067)
#define RespCOMM_EXCEL_SS7SignalLinkQuery 		__CTCOMMRespSym(0x0065)
#define RespCOMM_EXCEL_SS7SignalLinkSetQuery 	__CTCOMMRespSym(0x0064)
#define RespCOMM_EXCEL_SS7SignalRouteQuery 		__CTCOMMRespSym(0x0066)
#define RespCOMM_EXCEL_SS7SignalStackQuery 		__CTCOMMRespSym(0x006d)
#define RespCOMM_EXCEL_SynchroPriorityListQuery __CTCOMMRespSym(0x0081)
#define RespCOMM_EXCEL_SystemConfigQuery 			__CTCOMMRespSym(0x00b4)
#define RespCOMM_EXCEL_SystemLogQuery 				__CTCOMMRespSym(0x0082)
#define RespCOMM_EXCEL_SystemResourceUtilizeQuery \
																__CTCOMMRespSym(0x008e)
#define RespCOMM_EXCEL_TransmitCadencePatternQuery \
																__CTCOMMRespSym(0x0059)
#define RespCOMM_EXCEL_TransmitToneQuery 			__CTCOMMRespSym(0x005a)
#define RespCOMM_EXCEL_VersionRequest 				__CTCOMMRespSym(0x0002)

////////////////////////////////////////////////////////////////////////////////
//(5)Messages' type is EVENT; COMM->SWITCH-------------------------------------
#define __CTCOMMEvtSym(item) CTsym_BuildEvtID( CT_objCOMMCTRL,\
					       CT_vendEXCEL,\
					       (item) )
#define EvtCOMM_EXCEL_PPLEventIndication        __CTCOMMEvtSym(0x43)  //COMM->SC
#define EvtCOMM_EXCEL_RequestForServiceWData 	__CTCOMMEvtSym(0x2d)  //COMM->SC
#define EvtCOMM_EXCEL_ChannelReleased		__CTCOMMEvtSym(0x49)  //COMM->SC
#define EvtCOMM_EXCEL_ChannelReleasedWData	__CTCOMMEvtSym(0x69)  //COMM->SC
#define EvtCOMM_EXCEL_DS0StatusChange           __CTCOMMEvtSym(0x42)  //COMM->SC


#define EvtCOMM_EXCEL_Alarm					__CTCOMMEvtSym(0x00b9)
#define EvtCOMM_EXCEL_Poll			__CTCOMMEvtSym(0x00ab)
#define EvtCOMM_EXCEL_CardStatusReport		__CTCOMMEvtSym(0x00a6)
#define EvtCOMM_EXCEL_CCSRedundancyReport	__CTCOMMEvtSym(0x0073)
#define EvtCOMM_EXCEL_DiagnosticIndication	__CTCOMMEvtSym(0x0045)
#define EvtCOMM_EXCEL_GenericReport			__CTCOMMEvtSym(0x0046)
#define EvtCOMM_EXCEL_NodeStatusReport		__CTCOMMEvtSym(0x0070)
#define EvtCOMM_EXCEL_RecordAnnounceReport	__CTCOMMEvtSym(0x0058)
#define EvtCOMM_EXCEL_RingStatusReport		__CTCOMMEvtSym(0x0072)
#define EvtCOMM_EXCEL_PPLEventIndicateByChannel	__CTCOMMEvtSym(0x0120)
#define EvtCOMM_EXCEL_PPLEventIndicateByLink	__CTCOMMEvtSym(0x0121)
#define EvtCOMM_EXCEL_PPLEventIndicateByStack	__CTCOMMEvtSym(0x0122)

//**************************************************************************
//SCR send messages
//**************************************************************************
//(1)Messages' type is COMMAND; SCR->SWITCH-----------------------------------
#define __CTSCRCmdSym(item) CTsym_BuildCmdID( CT_objSCR,\
					      CT_vendTELSUN,\
					      (item) )

#define CmdSCR_TELSUN_Incoming            __CTSCRCmdSym(0x001)    //SCR->SIA
#define CmdSCR_TELSUN_Connect             __CTSCRCmdSym(0x002)    //SCR->SC
#define CmdSCR_TELSUN_Account             __CTSCRCmdSym(0x003)    //SCR->SC
#define CmdSCR_TELSUN_Outgoing            __CTSCRCmdSym(0x004)    //SCR->SC
#define CmdSCR_TELSUN_ReleaseChannel      __CTSCRCmdSym(0x005)    //SCR->SC
#define CmdSCR_TELSUN_QueryChannel        __CTSCRCmdSym(0x007)    //SCR->SC
#define CmdSCR_TELSUN_Disconnect          __CTSCRCmdSym(0x008)    //SCR->SC
#define CmdSCR_TELSUN_AllocSession	  __CTSCRCmdSym(0x009)

#define CmdSCR_TELSUN_Schedule		  __CTSCRCmdSym(0x011)
#define CmdSCR_TELSUN_Console             __CTSCRCmdSym(0x012)

//(2)Messages' type is RESPONSE; SCR->SWITCH-----------------------------------
#define __CTSCRRespSym(item) CTsym_BuildRespID( CT_objSCR,\
					       CT_vendTELSUN,\
					       (item) )

#define RespSCR_TELSUN_Incoming		  __CTSCRRespSym(0x001)  //SIA->SCR
#define RespSCR_TELSUN_Account            __CTSCRRespSym(0x002)  //SC->SCR
#define RespSCR_TELSUN_Connect            __CTSCRRespSym(0x003)  //SC->SCR
#define RespSCR_TELSUN_Outgoing           __CTSCRRespSym(0x004)  //SC->SCR
#define RespSCR_TELSUN_ReleaseChannel     __CTSCRRespSym(0x005)  //SC->SCR
#define RespSCR_TELSUN_QueryChannel       __CTSCRRespSym(0x007)  //SC->SCR
#define RespSCR_TELSUN_Disconnect         __CTSCRRespSym(0x008)  //SC->SCR
#define RespSCR_TELSUN_AllocSession	  __CTSCRRespSym(0x009)
#define RespSCR_TELSUN_Outgoing1          __CTSCRRespSym(0x00a)

#define RespSCR_TELSUN_Schedule		  __CTSCRRespSym(0x011)
#define RespSCR_TELSUN_Console            __CTSCRRespSym(0x012)


#define __CTSCREvtSym(item) CTsym_BuildEvtID( CT_objSCR,\
					       CT_vendTELSUN,\
					       (item) )

#define EvtSCR_TELSUN_FreeSession        __CTSCREvtSym(0x01)
#define EvtSCR_TELSUN_PrintLog		 __CTSCREvtSym(0x02)
#define EvtSCR_TELSUN_PrintPrefix	 __CTSCREvtSym(0x03)
#define EvtSCR_TELSUN_PrintSession	 __CTSCREvtSym(0x04)
#define EvtSCR_TELSUN_Service		 __CTSCREvtSym(0x05)
#define EvtSCR_TELSUN_Incoming		 __CTSCREvtSym(0x06)
#define EvtSCR_TELSUN_FreePorts		 __CTSCREvtSym(0x07)
#define EvtSCR_TELSUN_OutgoingFail	 __CTSCREvtSym(0x08)
#define EvtSCR_TELSUN_Unblock		 __CTSCREvtSym(0x09)
#define EvtSCR_TELSUN_StartSession	 __CTSCREvtSym(0x0A)

//**************************************************************************
//Messages' type is EVENT   (all event messages are here)
//**************************************************************************
//SWITCH send out
#define __CTSWITCHEvtSym(item) CTsym_BuildEvtID( CT_objSWITCH,\
						 CT_vendTELSUN,\
						 (item) )

#define EvtSWITCH_TELSUN_DS0OutOfService         __CTSWITCHEvtSym(0x002) //SC->SC
#define EvtSWITCH_TELSUN_DS0InService            __CTSWITCHEvtSym(0x003) //SC->SC
#define EvtSWITCH_TELSUN_DS0Blocked              __CTSWITCHEvtSym(0x004) //SC->SC
#define EvtSWITCH_TELSUN_DS0Unblocked            __CTSWITCHEvtSym(0x005) //SC->SC
#define EvtSWITCH_TELSUN_DS0Purge                __CTSWITCHEvtSym(0x006) //SC->SC
#define EvtSWITCH_TELSUN_DS0MaintLoop            __CTSWITCHEvtSym(0x007) //SC->SC
#define EvtSWITCH_TELSUN_OutseizeControlTimeOut  __CTSWITCHEvtSym(0x008) //SC->SC
#define EvtSWITCH_TELSUN_AllReleased             __CTSWITCHEvtSym(0x009) //SC->SC
#define EvtSWITCH_TELSUN_ReleaseChannelTimeOut   __CTSWITCHEvtSym(0x010) //SC->SC
#define EvtSWITCH_TELSUN_ConnectTimeOut          __CTSWITCHEvtSym(0x011) //SC->SC
#define EvtSWITCH_TELSUN_ParkChannelTimeOut      __CTSWITCHEvtSym(0x012) //SC->SC
#define EvtSWITCH_TELSUN_Alarming		 __CTSWITCHEvtSym(0x013) //SC->mAINT
#define EvtSWITCH_TELSUN_ChannelRelease		 __CTSWITCHEvtSym(0x014) //SC->SCR
#define EvtSWITCH_TELSUN_Incoming		 __CTSWITCHEvtSym(0x015) //SC->SCR

/*************************************************************************
* Communication module handle messages
*************************************************************************/

#define __CTCommCmdSym(item)               CTsym_BuildCmdID( CT_objCOMMUNICATION,\
						    CT_vendTELSUN,\
						    (item) )

#define CmdComm_TELSUN_Connect             __CTCommCmdSym(0x001)
#define CmdComm_TELSUN_Disconnect          __CTCommCmdSym(0x002)
#define CmdComm_TELSUN_Echo				   __CTCommCmdSym(0x004)
#define CmdComm_TELSUN_TCPRegister		   __CTCommCmdSym(0x005)
/*
 define for Config Server and CommMain by Yuankai Chen. 1998.11.2
*/
#define CmdComm_TELSUN_CfgPoll		__CTCommCmdSym(0x006)
#define CmdComm_TELSUN_CfgRegister	__CTCommCmdSym(0x007)
#define CmdComm_TELSUN_AdjacentInformation	__CTCommCmdSym(0x008)
/*
 Define for Warn Server and CommMain by Gang Zhao.1998.11.2
+++++++++warn */
#define CmdComm_TELSUN_ClientPoll     __CTCommCmdSym(0x009)        


#define __CTCommRespSym(item)               CTsym_BuildRespID( CT_objCOMMUNICATION,\
								CT_vendTELSUN,\
								(item) )

#define RespComm_TELSUN_Connect             __CTCommRespSym(0x001)
#define RespComm_TELSUN_Disconnect          __CTCommRespSym(0x002)
#define RespComm_TELSUN_Echo		   __CTCommRespSym(0x004)
#define RespComm_TELSUN_TCPRegister         __CTCommRespSym(0x005)
/*
 Define for Config Server and CommMain by Yuankai Chen. 1998.11.2
*/
#define RespComm_TELSUN_CfgPoll		__CTCommRespSym(0x006)
#define RespComm_TELSUN_CfgRegister	__CTCommRespSym(0x007)
#define RespComm_TELSUN_AdjacentInformation	__CTCommRespSym(0x008)
/*
 Define for Warn Server and CommMain by Gang Zhao.1998.11.2
+++++++++warn */
#define RespComm_TELSUN_ClientPoll     __CTCommRespSym(0x009)

////////////////

#define __CTCommEvtSym(item)		  CTsym_BuildEvtID( CT_objCOMMUNICATION,\
						   CT_vendTELSUN,\
						   (item) )

#define EvtComm_TELSUN_Connect		  __CTCommEvtSym(0x001)
#define EvtComm_TELSUN_Disconnect         __CTCommEvtSym(0x002)
#define EvtComm_TELSUN_Stop   		  __CTCommEvtSym(0x003)
#define EvtComm_TELSUN_NewModuleConnected __CTCommEvtSym(0x006)
#define EvtComm_TELSUN_SynStation         __CTCommEvtSym(0x007)
#define EvtComm_TELSUN_ClientDataChanged  __CTCommEvtSym(0x008)
#define EvtComm_TELSUN_Poll               __CTCommEvtSym(0x009)
#define EvtComm_TELSUN_TCPSocketRegisterOK __CTCommEvtSym(0x00A)
/*
 Define for Config Server and CommMain by Yuankai Chen.1998.11.2
*/
#define EvtComm_TELSUN_AdjacentInformation	__CTCommEvtSym(0x00B)
/*
 Define for Warn Server and CommMain by Gang Zhao.1998.11.13
*/
#define EvtComm_TELSUN_UDPWarn           	__CTCommEvtSym(0x00C)
#define EvtComm_TELSUN_CTModuleClearWarn    __CTCommEvtSym(0x00D)

//////
//#define EvtTimer_TELSUN_Timer		  __CTCommEvtSym(0x081)


/*************************************************************************
* TRIGGER module create Event Message
*************************************************************************/
#define __CTTriEvtSym(item)		  CTsym_BuildEvtID( CT_objTRIGGER,\
						   CT_vendTELSUN,\
						   (item) )
#define EvtTri_TELSUN_DlgcEvent	          __CTTriEvtSym(0x001)

/*************************************************************************
* profile module handle messages
*************************************************************************/

#define __CTCfgCmdSym(item)             CTsym_BuildCmdID( CT_objPROFILE, \
                                                          CT_vendTELSUN, \
							  (item))

#define CmdConfig_TELSUN_GetSPCTableInfo           __CTCfgCmdSym(0x002)
#define CmdConfig_TELSUN_GetASIInfo                __CTCfgCmdSym(0x003)

////////////////////////////////////////////////////////////////

#define __CTCfgRespSym(item)             CTsym_BuildRespID( CT_objPROFILE,\
                                                            CT_vendTELSUN,\
							    (item))

#define RespConfig_TELSUN_GetSPCTableInfo          __CTCfgRespSym(0x002)
#define RespConfig_TELSUN_GetASIInfo               __CTCfgRespSym(0x003)

////////////////////////////////////////////////////////////////
#define __CTCfgEvtSym(item)             CTsym_BuildEvtID( CT_objPROFILE,\
                                                          CT_vendTELSUN,\
                                                          (item))

#define EvtConfig_TELSUN_ASIMapLoaded              __CTCfgEvtSym(0x001)
#define EvtConfig_TELSUN_GetASIInfoEnd             __CTCfgEvtSym(0x003)

/*************************************************************************
* SIA module handle messages
*************************************************************************/

#define __CTSIACmdSym(item)             CTsym_BuildCmdID( CT_objSIA, \
							  CT_vendTELSUN, \
							  (item))

#define CmdSIA_TELSUN_RegisterSession     __CTSIACmdSym(0x001)
#define CmdSIA_TELSUN_UnregisterSession   __CTSIACmdSym(0x002)
#define CmdSIA_TELSUN_Account             __CTSIACmdSym(0x003)   //SIA->SCR
#define CmdSIA_TELSUN_Connect		  __CTSIACmdSym(0x004)
#define CmdSIA_TELSUN_Outgoing            __CTSIACmdSym(0x005)   //SIA->SCR  
#define CmdSIA_TELSUN_Disconnect	  __CTSIACmdSym(0x006)
#define CmdSIA_TELSUN_ReleaseChannel	  __CTSIACmdSym(0x007) // Reserved
#define CmdSIA_TELSUN_QueryChannel	  __CTSIACmdSym(0x008) // Reserved
#define CmdSIA_TELSUN_QueryState	  __CTSIACmdSym(0x009)
#define CmdSIA_TELSUN_OpenTrace		  __CTSIACmdSym(0x00a)
#define CmdSIA_TELSUN_CloseTrace	  __CTSIACmdSym(0x00b)
#define CmdSIA_TELSUN_Outgoing200         __CTSIACmdSym(0x00c)

////////////////////////////////////////////////////////////////

#define __CTSIARespSym(item)             CTsym_BuildRespID( CT_objSIA,\
							    CT_vendTELSUN,\
							    (item))

#define RespSIA_TELSUN_RegisterSession    __CTSIARespSym(0x001)//SCR->SIA
#define RespSIA_TELSUN_UnregisterSession  __CTSIARespSym(0x002)//SCR->SIA
#define RespSIA_TELSUN_Account            __CTSIARespSym(0x003)//SCR->SIA change
#define RespSIA_TELSUN_Connect            __CTSIARespSym(0x004)//SCR->SIA change
#define RespSIA_TELSUN_Outgoing           __CTSIARespSym(0x005)//SCR->SIA change
#define RespSIA_TELSUN_Disconnect	  __CTSIARespSym(0x006) //SCR->SIA
#define RespSIA_TELSUN_QueryState	  __CTSIARespSym(0x009) //ANY->SIA
#define RespSIA_TELSUN_OpenTrace	  __CTSIARespSym(0x00a)
#define RespSIA_TELSUN_CloseTrace	  __CTSIARespSym(0x00b)
#define RespSIA_TELSUN_Outgoing200        __CTSIARespSym(0x00c)


////////////////////////////////////////////////////////////////
#define __CTSIAEvtSym(item)             CTsym_BuildEvtID( CT_objSIA,\
							  CT_vendTELSUN,\
							  (item))
#define EvtSIA_TELSUN_FreeSession         __CTSIAEvtSym(0x001)   //SIA->SCR
#define EvtSIA_TELSUN_TraceResult	  __CTSIAEvtSym(0x002)
#define EvtSIA_TELSUN_PrintLog		  __CTSIAEvtSym(0x003)
#define	EvtSIA_TELSUN_Schedule            __CTSIAEvtSym(0x004)


/*************************************************************************
 * Authen module handle messages
 *************************************************************************/

#define __CTAuthenCmdSym(item)             CTsym_BuildCmdID( CT_objAUTHEN, \
							  CT_vendTELSUN, \
							  (item))
#define CmdAuthen_TELSUN_Verify                    __CTAuthenCmdSym(0x001)
#define CmdAuthen_TELSUN_ChangePassword            __CTAuthenCmdSym(0x002)
#define CmdAuthen_TELSUN_SetNotify                 __CTAuthenCmdSym(0x003)
#define CmdAuthen_TELSUN_Account                   __CTAuthenCmdSym(0x004)
#define CmdAuthen_TELSUN_Schedule                  __CTAuthenCmdSym(0x005)
#define CmdAuthen_TELSUN_GetNotify                 __CTAuthenCmdSym(0x006)
#define CmdAuthen_TELSUN_ServerMessageInfo         __CTAuthenCmdSym(0x010)
#define CmdAuthen_TELSUN_ClientMessageInfo         __CTAuthenCmdSym(0x011)

////////////////////////////////////////////////////////////////

#define __CTAuthenRespSym(item)             CTsym_BuildRespID( CT_objAUTHEN,\
							    CT_vendTELSUN,\
							    (item))
#define RespAuthen_TELSUN_Verify                   __CTAuthenRespSym(0x001)
#define RespAuthen_TELSUN_ChangePassword           __CTAuthenRespSym(0x002)
#define RespAuthen_TELSUN_SetNotify                __CTAuthenRespSym(0x003)
#define RespAuthen_TELSUN_Account                  __CTAuthenRespSym(0x004)
#define RespAuthen_TELSUN_Schedule                 __CTAuthenRespSym(0x005)
#define RespAuthen_TELSUN_GetNotify                __CTAuthenRespSym(0x006)
#define RespAuthen_TELSUN_ServerMessageInfo        __CTAuthenRespSym(0x010)
#define RespAuthen_TELSUN_ClientMessageInfo        __CTAuthenRespSym(0x011)

////////////////////////////////////////////////////////////////

#define __CTAuthenEvtSym(item)             CTsym_BuildEvtID( CT_objAUTHEN,\
							  CT_vendTELSUN,\
							  (item))

#define EvtAuthen_TELSUN_Shutdown                  __CTAuthenEvtSym(0x001)
#define EvtAuthen_TELSUN_ClearServerMessageCounter __CTAuthenEvtSym(0x010)
#define EvtAuthen_TELSUN_ClearClientCmdMsgCounter  __CTAuthenEvtSym(0x011)
#define EvtAuthen_TELSUN_ClearClientRespMsgCounter __CTAuthenEvtSym(0x012)
#define EvtAuthen_TELSUN_CommDisconnect            __CTAuthenEvtSym(0x013)
#define EvtAuthen_TELSUN_ReturnNotifyStatus        __CTAuthenEvtSym(0x014)

/*************************************************************************
 * Scheduler module handle messages
 *************************************************************************/

#define __CTSchedulerCmdSym(item)          CTsym_BuildCmdID( CT_objSCHEDULER,\
                                                             CT_vendTELSUN, \
                                                             (item))

#define CmdScheduler_TELSUN_MessageInfo             __CTSchedulerCmdSym(0x001)

////////////////////////////////////////////////////////////////

#define __CTSchedulerRespSym(item)         CTsym_BuildRespID( CT_objSCHEDULER,\
                                                              CT_vendTELSUN,\
                                                              (item))

#define RespScheduler_TELSUN_MessageInfo           __CTSchedulerRespSym(0x001)

////////////////////////////////////////////////////////////////

#define __CTSchedulerEvtSym(item)          CTsym_BuildEvtID( CT_objSCHEDULER,\
                                                             CT_vendTELSUN,\
                                                             (item))

#define EvtScheduler_TELSUN_ClearMessageCounter     __CTSchedulerEvtSym(0x001)

#endif    //__CTMSG_H__

