// DBAmsg.h

#ifndef _DBAMSG_H_
#define _DBAMSG_H_

#include "ctmsg.h"
#include "dbasymbol.h"
#define DBAMSG_BASE		0x0

#define __CTDBACmdSym(item) CTsym_BuildCmdID( CT_objDBA,\
                                                      CT_vendTELSUN,\
                                                      (item) )
#define __CTDBARespSym(item) CTsym_BuildRespID( CT_objDBA,\
                                                      CT_vendTELSUN,\
                                                      (item) )
#define __CTDBAEvtSym(item) CTsym_BuildEvtID( CT_objDBA,\
                                                      CT_vendTELSUN,\
													  (item) )

// command message definition
#define CmdDBA_TELSUN_Select		__CTDBACmdSym(DBAMSG_BASE+0x001)
#define CmdDBA_TELSUN_Insert		__CTDBACmdSym(DBAMSG_BASE+0x002)
#define CmdDBA_TELSUN_Update		__CTDBACmdSym(DBAMSG_BASE+0x003)
#define CmdDBA_TELSUN_Delete		__CTDBACmdSym(DBAMSG_BASE+0x004)
#define CmdDBA_TELSUN_ExecuteSql	__CTDBACmdSym(DBAMSG_BASE+0x005)
#define CmdDBA_TELSUN_Procedure		__CTDBACmdSym(DBAMSG_BASE+0x006)
#define CmdDBA_TELSUN_DataBaseType	__CTDBACmdSym(DBAMSG_BASE+0x007)
#define CmdDBA_TELSUN_QuitThread	__CTDBACmdSym(DBAMSG_BASE+0x008)
#define CmdDBA_TELSUN_ShowPacket1	__CTDBACmdSym(DBAMSG_BASE+0x009)


#define RespDBA_TELSUN_Select		__CTDBARespSym(DBAMSG_BASE+0x001)
#define RespDBA_TELSUN_Insert		__CTDBARespSym(DBAMSG_BASE+0x002)
#define RespDBA_TELSUN_Update		__CTDBARespSym(DBAMSG_BASE+0x003)
#define RespDBA_TELSUN_Delete		__CTDBARespSym(DBAMSG_BASE+0x004)
#define RespDBA_TELSUN_ExecuteSql	__CTDBARespSym(DBAMSG_BASE+0x005)
#define RespDBA_TELSUN_Procedure	__CTDBARespSym(DBAMSG_BASE+0x006)
#define RespDBA_TELSUN_DataBaseType __CTDBARespSym(DBAMSG_BASE+0x007)
#define RespDBA_TELSUN_QuitThread   __CTDBARespSym(DBAMSG_BASE+0x008)

#define DBAMSG_MaxCmd				__CTDBACmdSym(DBAMSG_BASE+0x020)

#endif