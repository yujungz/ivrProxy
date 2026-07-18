#ifndef __service_symbol__H__
#define __service_symbol__H__

#define __CTSIAKeySym(item)             CTsym_Build( CT_objSIA,\
												  CT_vendTELSUN,\
                                                  (item) )

#define SIA_TELSUN_Module						__CTSIAKeySym(0x001)
#define SIA_TELSUN_Device						__CTSIAKeySym(0x002)
#define SIA_TELSUN_FlowType						__CTSIAKeySym(0x003)
#define SIA_TELSUN_EventID						__CTSIAKeySym(0x004)
#define SIA_TELSUN_EventValue					__CTSIAKeySym(0x005)
#define SIA_TELSUN_ActionID						__CTSIAKeySym(0x006)
#define SIA_TELSUN_ArgValue						__CTSIAKeySym(0x007)
#define SIA_TELSUN_ArgType						__CTSIAKeySym(0x008)
#define SIA_TELSUN_TraceType					__CTSIAKeySym(0x009)
#define SIA_TELSUN_RunningState					__CTSIAKeySym(0x00a)
#define SIA_TELSUN_State						__CTSIAKeySym(0x00b)
#define SIA_TELSUN_GroupID						__CTSIAKeySym(0x00c)
#define SIA_TELSUN_PartID						__CTSIAKeySym(0x00d)
#define SIA_TELSUN_Attribute					__CTSIAKeySym(0x00e)
#define SIA_TELSUN_PrivateData					__CTSIAKeySym(0x00f)
#define SIA_TELSUN_Status						__CTSIAKeySym(0x010)
#define SIA_TELSUN_Position						__CTSIAKeySym(0x011)
#define SIA_TELSUN_Password						__CTSIAKeySym(0x012)


#endif
