#ifndef MainmsgH
#define MainmsgH
#include "mainsymbol.h"
//仿造IVSP的消息格式，使用32位的MessageID
//
#define CT_msgCOMMAND 	      0x00    //message type is Command
#define CT_msgRESPOND 	      0x01    //message type is response
#define CT_msgEVENT   	      0x02    //message type is event

//定义消息的位置和在32bit中的偏移量
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

//**************************************************************************************
//     定义公用消息  Public message
//**************************************************************************************
#define __CTTS2000OAMCmdTELSUNSym(item)	   CTsym_BuildCmdID( CT_objTS2000OAM,\
							     CT_vendTELSUN,\
                                     			     (item) )
#define CmdTS2000OAM_TELSUN_PollInterval	   __CTTS2000OAMCmdTELSUNSym(0x00)

//NetDevice 使用
#define CmdTS2000OAM_TELSUN_Block      			__CTTS2000OAMCmdTELSUNSym(0x01)
#define CmdTS2000OAM_TELSUN_Unblock      	    __CTTS2000OAMCmdTELSUNSym(0x02)
#define CmdTS2000OAM_TELSUN_Query       	    __CTTS2000OAMCmdTELSUNSym(0x03)
#define CmdTS2000OAM_TELSUN_ReloadProfile       __CTTS2000OAMCmdTELSUNSym(0x04)
#define CmdTS2000OAM_TELSUN_SnapShot		    __CTTS2000OAMCmdTELSUNSym(0x05)
#define CmdTS2000OAM_TELSUN_Trace               __CTTS2000OAMCmdTELSUNSym(0x06)
#define CmdTS2000OAM_TELSUN_GroupBlock      	__CTTS2000OAMCmdTELSUNSym(0x07)
#define CmdTS2000OAM_TELSUN_GroupUnblock      	__CTTS2000OAMCmdTELSUNSym(0x08)

//IVR流程使用消息
#define CmdTS2000OAM_TELSUN_QuerySession        __CTTS2000OAMCmdTELSUNSym(0x20)
#define CmdTS2000OAM_TELSUN_TraceSession        __CTTS2000OAMCmdTELSUNSym(0x21)
#define CmdTS2000OAM_TELSUN_StopSession         __CTTS2000OAMCmdTELSUNSym(0x22)
#define CmdTS2000OAM_TELSUN_QueryTopCall        __CTTS2000OAMCmdTELSUNSym(0x23)          

//定义公用消息 -----应答消息  Respond
#define __CTTS2000OAMRespTELSUNSym(item)	   CTsym_BuildRespID( CT_objTS2000OAM,\
							      CT_vendTELSUN,\
							      (item) )
#define RespTS2000OAM_TELSUN_PollInterval	   __CTTS2000OAMRespTELSUNSym(0x00)

//NetDevice 使用
#define RespTS2000OAM_TELSUN_Block     			 __CTTS2000OAMRespTELSUNSym(0x01)
#define RespTS2000OAM_TELSUN_Unblock	         __CTTS2000OAMRespTELSUNSym(0x02)
#define RespTS2000OAM_TELSUN_Query     			 __CTTS2000OAMRespTELSUNSym(0x03)
#define RespTS2000OAM_TELSUN_ReloadProfile		 __CTTS2000OAMRespTELSUNSym(0x04)
#define RespTS2000OAM_TELSUN_SnapShot  			 __CTTS2000OAMRespTELSUNSym(0x05)
#define RespTS2000OAM_TELSUN_Trace     			 __CTTS2000OAMRespTELSUNSym(0x06)
#define RespTS2000OAM_TELSUN_GroupBlock     	 __CTTS2000OAMRespTELSUNSym(0x07)
#define RespTS2000OAM_TELSUN_GroupUnblock	     __CTTS2000OAMRespTELSUNSym(0x08)

//IVR流程使用
#define RespTS2000OAM_TELSUN_QuerySession   	 __CTTS2000OAMRespTELSUNSym(0x20)
#define RespTS2000OAM_TELSUN_TraceSession        __CTTS2000OAMRespTELSUNSym(0x21)
#define RespTS2000OAM_TELSUN_StopSession         __CTTS2000OAMRespTELSUNSym(0x22)
#define RespTS2000OAM_TELSUN_QueryTopCall        __CTTS2000OAMRespTELSUNSym(0x23)          

//定义公用消息 -----事件消息 Event
#define __CTTS2000OAMEvtTELSUNSym(item)	   CTsym_BuildEvtID( CT_objTS2000OAM,\
							      CT_vendTELSUN,\
								 (item) )
//NetDevice 使用
#define EvtTS2000OAM_TELSUN_Alarm              __CTTS2000OAMEvtTELSUNSym(0x00)
#define EvtTS2000OAM_TELSUN_Trace              __CTTS2000OAMEvtTELSUNSym(0x01)

//IVR流程使用
#define EvtTS2000OAM_TELSUN_TraceSession       __CTTS2000OAMEvtTELSUNSym(0x20)
//************************************************************************************


//NMAGENT使用
#define CmdTS2000NM_TELSUN_REQUEST         __CTTS2000OAMCmdTELSUNSym(0x30)
#define RespTS2000NM_TELSUN_REQUEST        __CTTS2000OAMRespTELSUNSym(0x30)
#define EvtTS2000NM_TELSUN_TRAP            __CTTS2000OAMEvtTELSUNSym(0x30)

//************************************************************************************


#endif
