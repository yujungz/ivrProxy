//
#ifndef __PSPSYMBOL_H__
#define __PSPSYMBOL_H__

#include "ctsymbol.h"


#define CT_objPSP				 (0x007F)

#define CT_objTERMINAL			 (0x007E)


//*************defined for SWITCH CONTROL*************************

#define CT_errorInvalidCalledNumber			__CTErrorSym(0x070)
#define CT_errorNeedCallingNumber			__CTErrorSym(0x071)
#define CT_errorInvalidChannel				__CTErrorSym(0x072)
#define CT_errorInvalidICB 					__CTErrorSym(0x073)
#define CT_errorNoStoredInfo 				__CTErrorSym(0x074)
#define CT_errorFailInSendMessage 			__CTErrorSym(0x075)
#define CT_errorWaitingMsgNotFound 			__CTErrorSym(0x076)
#define CT_errorNeedMoreDigits	 			__CTErrorSym(0x077)
#define CT_errorInvalidContextID			__CTErrorSym(0x078)
#define CT_errorContextVariableNotFound		__CTErrorSym(0x079)
#define CT_errorInvalidICBCount            	__CTErrorSym(0x07a) 
#define CT_errorInvalidICBType             	__CTErrorSym(0x07b) 
#define CT_errorInvalidICBSubtype          	__CTErrorSym(0x07c) 
#define CT_errorInvalidICBLength           	__CTErrorSym(0x07d) 
#define CT_errorInvalidICBMessageType      	__CTErrorSym(0x07e) 
#define CT_errorInvalidICBFieldDataLength  	__CTErrorSym(0x07f)
#define CT_errorInvalidMCT			       	__CTErrorSym(0x080)
#define CT_errorInvalidCallingNumber		__CTErrorSym(0x081)
#define CT_errorOverFlow					__CTErrorSym(0x082)
#define CT_errorCanNotStop					__CTErrorSym(0x083)
#define CT_errorPrivilageViolationOnSpecifiedDevice	__CTErrorSym(0x084)


////////////////////////////////////////////////////////////////////////
#define __CTEXCELErrorSym(item)             CTsym_Build( CT_objERROR,\
														CT_vendEXCEL,\
														  (item))

#define CT_errorIpAddress  	   		__CTEXCELErrorSym(0x001)
#define CT_errorNoSuchSocket		__CTEXCELErrorSym(0x002)
#define CT_errorNoActiveSocket		__CTEXCELErrorSym(0x003)
#define CT_errorGetTComSocket  		__CTEXCELErrorSym(0x004)
#define CT_errorGetMatrixStatus		__CTEXCELErrorSym(0x005)
#define CT_errorSocketNotRunning    __CTEXCELErrorSym(0x006)
#define CT_errorSocketExist			__CTEXCELErrorSym(0x007)
#define CT_errorSocketConnected		__CTEXCELErrorSym(0x008)
#define CT_errorSocketKilled   		__CTEXCELErrorSym(0x009)
#define CT_errorSocketCreateBusy	__CTEXCELErrorSym(0x00a)
#define CT_errorOpenLogFile         __CTEXCELErrorSym(0x00b)
#define CT_errorAllocMemery         __CTEXCELErrorSym(0x00c)
#define CT_errorNoExistLogFile      __CTEXCELErrorSym(0x00d)

#define CT_errorSequence			__CTEXCELErrorSym(0x00e)
#define CT_errorAllocSequence		__CTEXCELErrorSym(0x00f)
#define CT_errorMessageVector		__CTEXCELErrorSym(0x010)





//*************************************************************************
//                            PSP keys
//*************************************************************************
//base value for PSP Control defined keys
#define __CTkvs_PSP(item)  	CTsym_Build( CT_objPSP,\
                                                CT_vendTELSUN,\
                                                (item) )

//KVSet in messages sent by PSP Control
#define PSP_TELSUN_MessageType                __CTkvs_PSP(0x0000)  
#define PSP_TELSUN_NodeID                     __CTkvs_PSP(0x0001)
#define PSP_TELSUN_SpanID					  __CTkvs_PSP(0x0002)
#define PSP_TELSUN_Channel                    __CTkvs_PSP(0x0003)
#define PSP_TELSUN_OrigSpanID 				  __CTkvs_PSP(0x0004)
#define PSP_TELSUN_TermSpanID                 __CTkvs_PSP(0x0005)
#define PSP_TELSUN_OrigChannel                __CTkvs_PSP(0x0006)
#define PSP_TELSUN_TermChannel                __CTkvs_PSP(0x0007)
#define PSP_TELSUN_SpanIDA                    __CTkvs_PSP(0x0008)
#define PSP_TELSUN_ChannelA                   __CTkvs_PSP(0x0009)
#define PSP_TELSUN_SpanIDB                    __CTkvs_PSP(0x000a)
#define PSP_TELSUN_ChannelB                   __CTkvs_PSP(0x000b)
#define PSP_TELSUN_Status                     __CTkvs_PSP(0x000c)
#define PSP_TELSUN_Data                       __CTkvs_PSP(0x000d)
#define PSP_TELSUN_BusyOutAction              __CTkvs_PSP(0x000e)
#define PSP_TELSUN_BusyOutFlag                __CTkvs_PSP(0x000f)

#define PSP_TELSUN_Const					  __CTkvs_PSP(0x0010)
#define PSP_TELSUN_NoLengthData				  __CTkvs_PSP(0x0011)
#define PSP_TELSUN_ICBData					  __CTkvs_PSP(0x0012)
#define PSP_SUNTKE_DataWithLength			  __CTkvs_PSP(0x0013)
#define PSP_TELSUN_Length					  __CTkvs_PSP(0x0014)
#define PSP_TELSUN_InternalFlag				  __CTkvs_PSP(0x0015)
#define PSP_TELSUN_CallMode				 	  __CTkvs_PSP(0x0016)
#define PSP_TELSUN_ConnectMode				  __CTkvs_PSP(0x0017)
#define PSP_TELSUN_BillFlag					  __CTkvs_PSP(0x0018)
#define PSP_TELSUN_Caller					  __CTkvs_PSP(0x0019)
#define PSP_TELSUN_Called					  __CTkvs_PSP(0x001a)
#define PSP_TELSUN_StackID					  __CTkvs_PSP(0x001c)
#define PSP_TELSUN_DataFlag					  __CTkvs_PSP(0x001d)
#define PSP_TELSUN_SpanArray				  __CTkvs_PSP(0x001e)
#define PSP_TELSUN_ChannelsPerSpanArray       __CTkvs_PSP(0x001f)
#define PSP_TELSUN_ChannelArray				  __CTkvs_PSP(0x0020)
#define PSP_TELSUN_QueryType				  __CTkvs_PSP(0x0021)
#define PSP_TELSUN_PSPandNumber				  __CTkvs_PSP(0x0022)
#define PSP_TELSUN_Reason					  __CTkvs_PSP(0x0023)
#define PSP_TELSUN_ReportType				  __CTkvs_PSP(0x0024)
#define PSP_TELSUN_MsgStatus				  __CTkvs_PSP(0x0025)
#define PSP_TELSUN_IRouteFlag				  __CTkvs_PSP(0x0026)
#define PSP_TELSUN_IRouteServer				  __CTkvs_PSP(0x0027)
#define PSP_TELSUN_TelNumber				  __CTkvs_PSP(0x0028)
#define PSP_TELSUN_Count				  	  __CTkvs_PSP(0x0029)
#define PSP_TELSUN_CallingDeviceType	  	  __CTkvs_PSP(0x002a)
#define PSP_TELSUN_SelectPortParams	  	  	  __CTkvs_PSP(0x002b)
#define PSP_TELSUN_TimeSlot	  	  	 		  __CTkvs_PSP(0x002c)
 
//	Added by sxr for message out sc.
#define PSP_TELSUN_AgentIDQueried  	 		  __CTkvs_PSP(0x002d)
#define PSP_TELSUN_AgentGroupIDQueried 		  __CTkvs_PSP(0x002e)
#define PSP_TELSUN_ResetGrade		 		  __CTkvs_PSP(0x002f)
#define PSP_TELSUN_WaitTime			 		  __CTkvs_PSP(0x0030)
#define PSP_TELSUN_AgentIDArray		 		  __CTkvs_PSP(0x0031)
#define PSP_TELSUN_AgentBeRecorded	 		  __CTkvs_PSP(0x0032)
#define PSP_TELSUN_AgentIDLogoff	 		  __CTkvs_PSP(0x0033)
#define PSP_TELSUN_ConferenceDevice	 		  __CTkvs_PSP(0x0034)
#define PSP_TELSUN_LastJoinDevice	 		  __CTkvs_PSP(0x0035)
#define PSP_TELSUN_Location			 		  __CTkvs_PSP(0x0036)
#define PSP_TELSUN_ServerNumber		 		  __CTkvs_PSP(0x0037)
#define PSP_TELSUN_ServiceNumber	 		  __CTkvs_PSP(0x0038)
#define PSP_TELSUN_VoiceNumber		 		  __CTkvs_PSP(0x0039)
#define PSP_TELSUN_DigitNumber		 		  __CTkvs_PSP(0x003a)
#define PSP_TELSUN_CallLayerModule	 		  __CTkvs_PSP(0x003b)
#define PSP_TELSUN_CrossReference	 		  __CTkvs_PSP(0x003c)
#define PSP_TELSUN_BillingRefID	 			  __CTkvs_PSP(0x003d)
#define PSP_TELSUN_JoinType	 			  	  __CTkvs_PSP(0x003e)
#define PSP_TELSUN_ConferenceParties	  	  __CTkvs_PSP(0x003f)
#define PSP_TELSUN_SubjectDevice		  	  __CTkvs_PSP(0x0040)

// here 0x40 - 0x42 is free

#define PSP_TELSUN_ConferenceID				        __CTkvs_PSP(0x0043)
#define PSP_TELSUN_ConferenceSize                   __CTkvs_PSP(0x0044)
#define PSP_TELSUN_ConferenceType			        __CTkvs_PSP(0x0045)
#define PSP_TELSUN_CallProcessingEvent		        __CTkvs_PSP(0x0046)
#define PSP_TELSUN_ReleaseDataType			        __CTkvs_PSP(0x0047)
#define PSP_TELSUN_Mode								__CTkvs_PSP(0x0048)
#define PSP_TELSUN_MaxDigits						__CTkvs_PSP(0x0049)
#define PSP_TELSUN_NumberOfTerminatingCharacters	__CTkvs_PSP(0x004a)
#define PSP_TELSUN_ConfigurationBits				__CTkvs_PSP(0x004b)
#define PSP_TELSUN_TerminatingCharacters			__CTkvs_PSP(0x004c)
#define PSP_TELSUN_InterdigitTimer					__CTkvs_PSP(0x004d)
#define PSP_TELSUN_FirstdigitTimer					__CTkvs_PSP(0x004e)
#define PSP_TELSUN_CompletionTimer					__CTkvs_PSP(0x004f)
#define PSP_TELSUN_MinimumReceiveDigitDurationTimer __CTkvs_PSP(0x0050)
#define PSP_TELSUN_AddressSignalingType				__CTkvs_PSP(0x0051)
#define PSP_TELSUN_NumberOfDigitStrings				__CTkvs_PSP(0x0052)
#define PSP_TELSUN_ResumeDigitCollectionTimer		__CTkvs_PSP(0x0053)

#define PSP_TELSUN_Digit							__CTkvs_PSP(0x0054)
#define PSP_TELSUN_R2Signal							__CTkvs_PSP(0x0055)
// Call Processing Analysis --- Cpa
#define PSP_TELSUN_CpaResult						__CTkvs_PSP(0x0057)
#define PSP_TELSUN_ChannelStatus					__CTkvs_PSP(0x0058)
#define PSP_TELSUN_PurgeReason						__CTkvs_PSP(0x0059)
#define PSP_TELSUN_LinkID							__CTkvs_PSP(0x005a)
#define PSP_TELSUN_PPLComponentID					__CTkvs_PSP(0x005b)
#define PSP_TELSUN_ResendFlag						__CTkvs_PSP(0x005c)
#define PSP_TELSUN_AddrDataType						__CTkvs_PSP(0x005d)
#define PSP_TELSUN_BroadcastEnable					__CTkvs_PSP(0x005f)
#define PSP_TELSUN_TransmitCallProgressPatternID	__CTkvs_PSP(0x0060)
#define PSP_TELSUN_NumCyclesToTransmit				__CTkvs_PSP(0x0061)
#define PSP_TELSUN_GenerateEventFlag				__CTkvs_PSP(0x0062)
#define PSP_TELSUN_ConnectDataType					__CTkvs_PSP(0x0063)
#define PSP_TELSUN_ChannelAPadValue					__CTkvs_PSP(0x0064)
#define PSP_TELSUN_ChannelBPadValue					__CTkvs_PSP(0x0065)
#define PSP_TELSUN_CpcDetectionAction				__CTkvs_PSP(0x0066)
#define PSP_TELSUN_ChannelAEncodingFormat			__CTkvs_PSP(0x0067)
#define PSP_TELSUN_ChannelBEncodingFormat			__CTkvs_PSP(0x0068)
#define PSP_TELSUN_ServiceType						__CTkvs_PSP(0x0069)
#define PSP_TELSUN_StringCount						__CTkvs_PSP(0x006a)
#define PSP_TELSUN_StringMode						__CTkvs_PSP(0x006b)
#define PSP_TELSUN_FirstDigitDuration				__CTkvs_PSP(0x006c)
#define PSP_TELSUN_DigitDuration					__CTkvs_PSP(0x006d)
#define PSP_TELSUN_InterdigitDuration				__CTkvs_PSP(0x006e)
#define PSP_TELSUN_DelayDuration					__CTkvs_PSP(0x006f)
#define PSP_TELSUN_String1DigitCount				__CTkvs_PSP(0x0070)
#define PSP_TELSUN_String1Digits					__CTkvs_PSP(0x0071)
#define PSP_TELSUN_String2DigitCount				__CTkvs_PSP(0x0072)
#define PSP_TELSUN_String2Digits					__CTkvs_PSP(0x0073)
#define PSP_TELSUN_PPLEvent							__CTkvs_PSP(0x0075)
#define PSP_TELSUN_ConfigurationFlag				__CTkvs_PSP(0x0076)
#define PSP_TELSUN_EventFlag						__CTkvs_PSP(0x0077)
#define PSP_TELSUN_AnnouncementCount				__CTkvs_PSP(0x0078)
#define PSP_TELSUN_AnnouncementID					__CTkvs_PSP(0x0079)

#define PSP_TELSUN_EventID					  __CTkvs_PSP(0x007a)
#define PSP_TELSUN_TupMsgTemplate			  __CTkvs_PSP(0x007b)
#define PSP_TELSUN_TupMsgFieldFlag		 	  __CTkvs_PSP(0x007c)

#define PSP_TELSUN_RangeAndStatusField 		  __CTkvs_PSP(0x0080)
#define PSP_TELSUN_RangeField             	  __CTkvs_PSP(0x0081)

#define PSP_TELSUN_CodeFile                   __CTkvs_PSP(0x0082)
#define PSP_TELSUN_TransitType                __CTkvs_PSP(0x0083)
#define PSP_TELSUN_PortType                   __CTkvs_PSP(0x0084)
#define PSP_TELSUN_ModuleID                   __CTkvs_PSP(0x0085)
#define PSP_TELSUN_Layer			 		  __CTkvs_PSP(0x0086)
#define PSP_TELSUN_OPC						  __CTkvs_PSP(0x0087)
#define PSP_TELSUN_DPC                        __CTkvs_PSP(0x0088)
#define PSP_TELSUN_CIC                        __CTkvs_PSP(0x0089)
#define PSP_TELSUN_RouteID                    __CTkvs_PSP(0x008a)
#define PSP_TELSUN_IpType                     __CTkvs_PSP(0x008b)
#define PSP_TELSUN_PrefixString	              __CTkvs_PSP(0x008c)
#define PSP_TELSUN_NeedCallingFlag            __CTkvs_PSP(0x008d)
#define PSP_TELSUN_NeedCallingPartyCategory   __CTkvs_PSP(0x008e)
#define PSP_TELSUN_NeedOrigCalledFlag		  __CTkvs_PSP(0x008f)
#define PSP_TELSUN_DigitsIgnored              __CTkvs_PSP(0x0090)
#define PSP_TELSUN_ControlMode                __CTkvs_PSP(0x0091)
#define PSP_TELSUN_QueueLength                __CTkvs_PSP(0x0092)
#define PSP_TELSUN_MessageIndex               __CTkvs_PSP(0x0093)
#define PSP_TELSUN_TUPMessageIndex			  __CTkvs_PSP(0x0094)
#define PSP_TELSUN_Action					  __CTkvs_PSP(0x0095)
#define PSP_TELSUN_ForcedFlag				  __CTkvs_PSP(0x0096)
 
#define PSP_TELSUN_AllSS7PathIndicator		  __CTkvs_PSP(0x0097)
#define PSP_TELSUN_ConnectFlag				  __CTkvs_PSP(0x0098)
#define PSP_TELSUN_CallingPartyCategory		  __CTkvs_PSP(0x0099)
#define PSP_TELSUN_NatureOfCircuit			  __CTkvs_PSP(0x009a)
#define PSP_TELSUN_ContinuityCheckIndicator	  __CTkvs_PSP(0x009b)
#define PSP_TELSUN_AllDigitalPathIndicator	  __CTkvs_PSP(0x009c)
#define PSP_TELSUN_RedirectedCallIndicator	  __CTkvs_PSP(0x009d)
#define PSP_TELSUN_KA						  __CTkvs_PSP(0x009e)
#define PSP_TELSUN_KB						  __CTkvs_PSP(0x009f)
#define PSP_TELSUN_KC						  __CTkvs_PSP(0x00a0)
#define PSP_TELSUN_KC15						  __CTkvs_PSP(0x00a1)
#define PSP_TELSUN_KD						  __CTkvs_PSP(0x00a2)
#define PSP_TELSUN_RouteType                  __CTkvs_PSP(0x00a3)
#define PSP_TELSUN_ForwardingNumber           __CTkvs_PSP(0x00a5)
#define PSP_TELSUN_ReleaseReason   		      __CTkvs_PSP(0x00a6)
#define PSP_TELSUN_OrigDPC  		      	__CTkvs_PSP(0x00a9)
#define PSP_TELSUN_TermDPC  		      	__CTkvs_PSP(0x00aa)
#define PSP_TELSUN_OrigCIC  		      	__CTkvs_PSP(0x00ab)
#define PSP_TELSUN_TermCIC  		      	__CTkvs_PSP(0x00ac)
#define PSP_TELSUN_FileName  		      	__CTkvs_PSP(0x00ad)
#define PSP_TELSUN_OrigCaller  		      	__CTkvs_PSP(0x00ae)
#define PSP_TELSUN_PrivateData 		      	__CTkvs_PSP(0x00af)
#define PSP_TELSUN_ControllingDevice       	__CTkvs_PSP(0x00b0)

// here 0xb1 - 0xff is free

#define PSP_TELSUN_CallSetupMessageIndicators			__CTkvs_PSP(0x100)
#define PSP_TELSUN_IAINationalUseField     				__CTkvs_PSP(0x101)
#define PSP_TELSUN_ClosedUserGroupInformation			__CTkvs_PSP(0x102)
#define PSP_TELSUN_AdditionalCallingPartyInformation	__CTkvs_PSP(0x103)
#define PSP_TELSUN_AdditionalRoutingInformation			__CTkvs_PSP(0x104)
#define PSP_TELSUN_CallingLineID           				__CTkvs_PSP(0x105)
#define PSP_TELSUN_ChargingInformation     				__CTkvs_PSP(0x106)
#define PSP_TELSUN_CalledPartyAddressDigit 				__CTkvs_PSP(0x107)
#define PSP_TELSUN_ACMMessageIndicator     				__CTkvs_PSP(0x108)
#define PSP_TELSUN_RequestTypeIndicator    				__CTkvs_PSP(0x109)
#define PSP_TELSUN_IncomingTrunkAndTransmitIdentity		__CTkvs_PSP(0x110)
#define PSP_TELSUN_EchoSuppressorIndicator 				__CTkvs_PSP(0x111)
#define PSP_TELSUN_MaliciousCallIdentication 			__CTkvs_PSP(0x112)
#define PSP_TELSUN_HoldIndicator           				__CTkvs_PSP(0x113)
#define PSP_TELSUN_ACCMessageIndicator     				__CTkvs_PSP(0x114)
#define PSP_TELSUN_EUMOctetIndicator       				__CTkvs_PSP(0x115)
#define PSP_TELSUN_SignalingPointCode      				__CTkvs_PSP(0x116)
#define PSP_TELSUN_SignalingType						__CTkvs_PSP(0x119)
#define PSP_TELSUN_OrigCalledAddr						__CTkvs_PSP(0x11c)
#define PSP_TELSUN_OrigCalled						PSP_TELSUN_OrigCalledAddr
#define PSP_TELSUN_TUPMessageType						__CTkvs_PSP(0x11d)
#define PSP_TELSUN_CallingAddrIndicator					__CTkvs_PSP(0x11f)
#define PSP_TELSUN_CallingOrigIndicator					__CTkvs_PSP(0x120)
#define PSP_TELSUN_OrigCalledAddrIndicator				__CTkvs_PSP(0x121)
#define PSP_TELSUN_EUMMessageIndicator					__CTkvs_PSP(0x122)
#define PSP_TELSUN_ResponseTypeIndicator				__CTkvs_PSP(0x123)

#define PSP_TELSUN_TransitExchangeIdentity  __CTkvs_PSP(0x124)
#define PSP_TELSUN_IncomingTrunkIdentity   	__CTkvs_PSP(0x125)
#define PSP_TELSUN_IdentityTypeIndicator   	__CTkvs_PSP(0x126)
#define PSP_TELSUN_CallingLineIDIndicator  	__CTkvs_PSP(0x127)
#define PSP_TELSUN_ChargingPulse	  		__CTkvs_PSP(0x128)

#define PSP_TELSUN_AnsweredFlag				__CTkvs_PSP(0x129)

#define PSP_TELSUN_IdleChannels				__CTkvs_PSP(0x120)
#define PSP_TELSUN_InServiceChannels		__CTkvs_PSP(0x121)
#define PSP_TELSUN_OutOfServiceChannels		__CTkvs_PSP(0x122)
#define PSP_TELSUN_UsedContextCount			__CTkvs_PSP(0x123)
#define PSP_TELSUN_WaitingMessageCount		__CTkvs_PSP(0x124)
#define PSP_TELSUN_BlockedChannels			__CTkvs_PSP(0x125)

#define PSP_TELSUN_AlarmType				__CTkvs_PSP(0x126)
#define PSP_TELSUN_BlockType				__CTkvs_PSP(0x127)
#define PSP_TELSUN_CICcode					__CTkvs_PSP(0x128)
#define PSP_TELSUN_TUPOutseizeMessageType	__CTkvs_PSP(0x129)

#define PSP_TELSUN_AgentID					__CTkvs_PSP(0x12a)
#define PSP_TELSUN_AgentGrade				__CTkvs_PSP(0x12b)
#define PSP_TELSUN_AgentGroupID				__CTkvs_PSP(0x12c)
#define PSP_TELSUN_AgentStation				__CTkvs_PSP(0x12d)
#define PSP_TELSUN_AgentType				__CTkvs_PSP(0x12e)
#define PSP_TELSUN_AgentDevice				__CTkvs_PSP(0x12f)
#define PSP_TELSUN_Connection				__CTkvs_PSP(0x130)
#define PSP_TELSUN_AlertingDevice			__CTkvs_PSP(0x131)
#define PSP_TELSUN_CallingDevice			__CTkvs_PSP(0x132)
#define PSP_TELSUN_CalledDevice				__CTkvs_PSP(0x133)
#define PSP_TELSUN_CallToBeDiverted			__CTkvs_PSP(0x134)
#define PSP_TELSUN_NewDestination			__CTkvs_PSP(0x135)
#define PSP_TELSUN_ActiveCall				__CTkvs_PSP(0x136)
#define PSP_TELSUN_DeviceToJoin				__CTkvs_PSP(0x137)
#define PSP_TELSUN_DeviceToTransferTo		__CTkvs_PSP(0x138)
#define PSP_TELSUN_PrimaryOldCall			__CTkvs_PSP(0x139)
#define PSP_TELSUN_SecondaryOldCall			__CTkvs_PSP(0x13a)
#define PSP_TELSUN_TransferringDevice		__CTkvs_PSP(0x13b)
#define PSP_TELSUN_TransferredDevice		__CTkvs_PSP(0x13c)
#define PSP_TELSUN_DroppedConnection		__CTkvs_PSP(0x13d)
#define PSP_TELSUN_ReleasingDevice			__CTkvs_PSP(0x13e)
#define PSP_TELSUN_DestinationType			__CTkvs_PSP(0x13f)
#define PSP_TELSUN_QueueList				__CTkvs_PSP(0x140)
#define PSP_TELSUN_AgentGroupClass			__CTkvs_PSP(0x141)
#define PSP_TELSUN_GroupRelate				__CTkvs_PSP(0x143)
#define PSP_TELSUN_AllLogoffSelection		__CTkvs_PSP(0x144)
#define PSP_TELSUN_AllLogoffTransfer		__CTkvs_PSP(0x145)
#define PSP_TELSUN_NoAnswerTime				__CTkvs_PSP(0x146)
#define PSP_TELSUN_AllBusyIndication		__CTkvs_PSP(0x147)
#define PSP_TELSUN_Maxlength				__CTkvs_PSP(0x148)
#define PSP_TELSUN_MaxWaitTime				__CTkvs_PSP(0x149)
#define PSP_TELSUN_WaitIndication1Time		__CTkvs_PSP(0x14a)
#define PSP_TELSUN_WaitIndication2Time		__CTkvs_PSP(0x14b)
#define PSP_TELSUN_CallToBeCleared			__CTkvs_PSP(0x14d)
#define PSP_TELSUN_ConnectionID				__CTkvs_PSP(0x14e)
#define PSP_TELSUN_TimerType				__CTkvs_PSP(0x14f)
#define PSP_TELSUN_QueueID					__CTkvs_PSP(0x150)
#define PSP_TELSUN_AgentGroupType			__CTkvs_PSP(0x151)
#define PSP_TELSUN_QueueType				__CTkvs_PSP(0x152)
#define PSP_TELSUN_MaxLength				__CTkvs_PSP(0x153)
#define PSP_TELSUN_Cause					__CTkvs_PSP(0x154)
#define PSP_TELSUN_FailedConnection			__CTkvs_PSP(0x155)
#define PSP_TELSUN_EstablishedConnection	__CTkvs_PSP(0x156)
#define PSP_TELSUN_HeldConnection			__CTkvs_PSP(0x157)
#define PSP_TELSUN_RetrievedConnection		__CTkvs_PSP(0x158)
#define PSP_TELSUN_FailingDevice			__CTkvs_PSP(0x159)
#define PSP_TELSUN_AnsweringDevice			__CTkvs_PSP(0x15a)
#define PSP_TELSUN_HoldingDevice			__CTkvs_PSP(0x15b)
#define PSP_TELSUN_RetrievingDevice			__CTkvs_PSP(0x15c)
#define PSP_TELSUN_ConfController			__CTkvs_PSP(0x15d)
#define PSP_TELSUN_AddedParty				__CTkvs_PSP(0x15e)
#define PSP_TELSUN_PartyOne					__CTkvs_PSP(0x15f)
#define PSP_TELSUN_AgentGroupNum			__CTkvs_PSP(0x160)
#define PSP_TELSUN_AlertingConnection		__CTkvs_PSP(0x161)
#define PSP_TELSUN_ActiveConnection			__CTkvs_PSP(0x162)

#define PSP_TELSUN_DivertionType			__CTkvs_PSP(0x169)
#define PSP_TELSUN_AgentState				__CTkvs_PSP(0x16a)
#define PSP_TELSUN_DeviceID 				__CTkvs_PSP(0x16b)
// here 0x16c - 0x16f is free

#define PSP_TELSUN_PartyTwo					__CTkvs_PSP(0x170)
#define PSP_TELSUN_PartyThree				__CTkvs_PSP(0x171)
#define PSP_TELSUN_SupervisedDevice			__CTkvs_PSP(0x172)
#define PSP_TELSUN_CalledDeviceFlag			__CTkvs_PSP(0x173)
#define PSP_TELSUN_CallToBeAnswered			__CTkvs_PSP(0x174)	//CTuint
#define PSP_TELSUN_ConferenceCall			__CTkvs_PSP(0x175)	//CTuint
#define PSP_TELSUN_ConsultedDevice			__CTkvs_PSP(0x176)	//CTushort
#define PSP_TELSUN_CrossRefID				__CTkvs_PSP(0x177)	//CTuint
#define PSP_TELSUN_CurrentRoute				__CTkvs_PSP(0x178)	//CTushort
#define PSP_TELSUN_DivertingDevice			__CTkvs_PSP(0x179)	//CTushort
#define PSP_TELSUN_ExistingCall				__CTkvs_PSP(0x17a)	//CTuint
#define PSP_TELSUN_Feature					__CTkvs_PSP(0x17b)	//CTbyte
#define PSP_TELSUN_InitiatedCall			__CTkvs_PSP(0x17c)	//CTuint
#define PSP_TELSUN_InitiatedConnection		__CTkvs_PSP(0x17d)	//CTuint
#define PSP_TELSUN_LastRedirectionDevice	__CTkvs_PSP(0x17e)	//CTushort
#define PSP_TELSUN_OperationErrors			__CTkvs_PSP(0x17f)	//CTbyte
#define PSP_TELSUN_OrignatedConnetion		__CTkvs_PSP(0x180)	//CTuint
#define PSP_TELSUN_OrignatingDevice			__CTkvs_PSP(0x181)	//CTushort
#define	PSP_TELSUN_OutboundConnection		__CTkvs_PSP(0x182)	//CTuint
#define PSP_TELSUN_PerformanceErrors		__CTkvs_PSP(0x183)	//CTbyte
#define PSP_TELSUN_Queue					__CTkvs_PSP(0x184)	//CTushort
#define PSP_TELSUN_QueuedConnection			__CTkvs_PSP(0x185)	//CTuint
#define PSP_TELSUN_RouteSelected			__CTkvs_PSP(0x186)	//CTuint
#define PSP_TELSUN_RouteUsed				__CTkvs_PSP(0x187)	//CTuint
#define PSP_TELSUN_SecurityErrors			__CTkvs_PSP(0x188)	//CTbyte
#define PSP_TELSUN_SystemResourceErrors		__CTkvs_PSP(0x189)	//CTbyte
#define PSP_TELSUN_StateErrors				__CTkvs_PSP(0x18a)	//CTbyte
#define PSP_TELSUN_SubscribedResourceErrors	__CTkvs_PSP(0x18b)  //CTbyte
#define PSP_TELSUN_TrunkUsed				__CTkvs_PSP(0x18c)	//CTushort
#define PSP_TELSUN_CallID					__CTkvs_PSP(0x18d)  //CTushort
#define PSP_TELSUN_HeldCall					__CTkvs_PSP(0x18e)
#define PSP_TELSUN_ConnectionToBeCleared	__CTkvs_PSP(0x18f)
#define PSP_TELSUN_AgentPasswd				__CTkvs_PSP(0x190)
#define PSP_TELSUN_PasswdLength				__CTkvs_PSP(0x191)
#define PSP_TELSUN_AgentUserName			__CTkvs_PSP(0x192)
#define PSP_TELSUN_AgentAddr				__CTkvs_PSP(0x193)
#define PSP_TELSUN_AgentPort				__CTkvs_PSP(0x194)
#define PSP_TELSUN_WaitingMessageID			__CTkvs_PSP(0x195)
#define PSP_TELSUN_CurrentMessageID			__CTkvs_PSP(0x196)
#define PSP_TELSUN_QueueNumber				__CTkvs_PSP(0x197)
#define PSP_TELSUN_GlobalObjectREF			__CTkvs_PSP(0x198)
#define PSP_TELSUN_TelephoneNumber			__CTkvs_PSP(0x199)
#define PSP_TELSUN_TransactionID			__CTkvs_PSP(0x19a)
#define PSP_TELSUN_ItemID					__CTkvs_PSP(0x19b)
#define PSP_TELSUN_TraceRangeType			__CTkvs_PSP(0x19c)
#define PSP_TELSUN_State					__CTkvs_PSP(0x19d)
#define PSP_TELSUN_SamplePoint				__CTkvs_PSP(0x19e)

//used only for cfg_proc.TEST temporary use!!!
#define PSP_TELSUN_PhysicalNode				__CTkvs_PSP(0x19f)
#define PSP_TELSUN_IP_Matrix				__CTkvs_PSP(0x1a0)
#define PSP_TELSUN_IPNumber					__CTkvs_PSP(0x1a1)
#define PSP_TELSUN_Type						__CTkvs_PSP(0x1a2)
#define PSP_TELSUN_FlowType					__CTkvs_PSP(0x1a3)
#define PSP_TELSUN_RecordMode				__CTkvs_PSP(0x1a5)
#define PSP_TELSUN_RecordingDevice			__CTkvs_PSP(0x1a6)
#define PSP_TELSUN_RecordedDevice			__CTkvs_PSP(0x1a7)
#define PSP_TELSUN_AddedPartyOne			__CTkvs_PSP(0x1a8)
#define PSP_TELSUN_AddedPartyTwo			__CTkvs_PSP(0x1a9)
#define PSP_TELSUN_AddedPartyOneNumber		__CTkvs_PSP(0x1aa)
#define PSP_TELSUN_AddedPartyTwoNumber		__CTkvs_PSP(0x1ab)		
#define PSP_TELSUN_AimCode					__CTkvs_PSP(0x1ac)	
#define PSP_TELSUN_H323ID                   __CTkvs_PSP(0x1ad)
#define PSP_TELSUN_E164                     __CTkvs_PSP(0x1ae)


#define CSTA_DigitsStringNumber			    PSP_TELSUN_DigitsStringNumber
#define CSTA_CalledDirectoryNumber			PSP_TELSUN_Called
#define CSTA_Connection						PSP_TELSUN_Connection
#define CSTA_AlertingDevice					PSP_TELSUN_AlertingDevice
#define CSTA_AlertingConnection				PSP_TELSUN_AlertingConnection
#define CSTA_AnsweringDevice				PSP_TELSUN_AnsweringDevice
#define CSTA_CallingDevice					PSP_TELSUN_CallingDevice
#define CSTA_CalledDevice					PSP_TELSUN_CalledDevice
#define CSTA_CallToBeCleared				PSP_TELSUN_CallToBeCleared
#define CSTA_ConnectionToBeCleared			PSP_TELSUN_ConnectionToBeCleared
#define CSTA_ConnectionID					PSP_TELSUN_ConnectionID
#define CSTA_ActiveCall						PSP_TELSUN_ActiveCall
#define CSTA_HeldCall						PSP_TELSUN_HeldCall
#define CSTA_PrimaryOldCall					PSP_TELSUN_PrimaryOldCall
#define CSTA_SecondaryOldCall				PSP_TELSUN_SecondaryOldCall
#define CSTA_Cause							PSP_TELSUN_Cause
#define CSTA_FailedConnection				PSP_TELSUN_FailedConnection
#define CSTA_FailingDevice					PSP_TELSUN_FailingDevice	
#define CSTA_HoldingDevice					PSP_TELSUN_HoldingDevice	
#define CSTA_RetrievingDevice				PSP_TELSUN_RetrievingDevice	
#define CSTA_ReleasingDevice				PSP_TELSUN_ReleasingDevice
#define CSTA_EstablishedConnection			PSP_TELSUN_EstablishedConnection
#define CSTA_RetrievedConnection			PSP_TELSUN_RetrievedConnection
#define CSTA_HeldConnection					PSP_TELSUN_HeldConnection
#define CSTA_DroppedConnection				PSP_TELSUN_DroppedConnection
#define CSTA_ConfController					PSP_TELSUN_ConfController
#define CSTA_AddedParty						PSP_TELSUN_AddedParty
#define CSTA_PartyOne						PSP_TELSUN_PartyOne			
#define CSTA_PartyTwo						PSP_TELSUN_PartyTwo
#define CSTA_PartyThree						PSP_TELSUN_PartyThree
#define CSTA_SupervisedDevice				PSP_TELSUN_SupervisedDevice	
#define CSTA_CalledDeviceFlag				PSP_TELSUN_CalledDeviceFlag
#define CSTA_CallToBeDiverted				PSP_TELSUN_CallToBeDiverted
#define CSTA_DivertionType					PSP_TELSUN_DivertionType
#define CSTA_CallToBeAnswered				PSP_TELSUN_CallToBeAnswered
#define CSTA_ConferenceCall					PSP_TELSUN_ConferenceCall 
#define CSTA_ConsultedDevice 				PSP_TELSUN_ConsultedDevice 
#define CSTA_CrossRefID						PSP_TELSUN_CrossRefID 
#define CSTA_CurrentRoute					PSP_TELSUN_CurrentRoute 
#define CSTA_DeviceToJoin					PSP_TELSUN_DeviceToJoin
#define CSTA_DeviceToTransferTo				PSP_TELSUN_DeviceToTransferTo
#define CSTA_DivertingDevice				PSP_TELSUN_DivertingDevice
#define CSTA_ExistingCall					PSP_TELSUN_ExistingCall
#define CSTA_Feature						PSP_TELSUN_Feature	
#define CSTA_InitiatedCall					PSP_TELSUN_InitiatedCall
#define CSTA_InitiatedConnection			PSP_TELSUN_InitiatedConnection
#define CSTA_LastRedirectionDevice			PSP_TELSUN_LastRedirectionDevice
#define CSTA_NewDestination					PSP_TELSUN_NewDestination
#define CSTA_DestinationType				PSP_TELSUN_DestinationType
#define CSTA_OperationalErrors				PSP_TELSUN_OperationErrors
#define CSTA_OriginatedConnection			PSP_TELSUN_OrignatedConnetion
#define CSTA_OriginatingDevice				PSP_TELSUN_OrignatingDevice	
#define CSTA_OutboundConnection				PSP_TELSUN_OutboundConnection
#define CSTA_PerformanceErrors				PSP_TELSUN_PerformanceErrors
#define CSTA_Queue							PSP_TELSUN_Queue	
#define CSTA_QueuedConnection				PSP_TELSUN_QueuedConnection
#define CSTA_RouteSelected					PSP_TELSUN_RouteSelected
#define CSTA_RouteUsed						PSP_TELSUN_RouteUsed
#define CSTA_SecurityErrors					PSP_TELSUN_SecurityErrors
#define CSTA_SystemResourceErrors			PSP_TELSUN_SystemResourceErrors
#define CSTA_StateErrors					PSP_TELSUN_StateErrors
#define CSTA_SubscribedResourceErrors		PSP_TELSUN_SubscribedResourceErrors
#define CSTA_TransferringDevice				PSP_TELSUN_TransferringDevice 
#define CSTA_TransferredDevice				PSP_TELSUN_TransferredDevice 
#define CSTA_TrunkUsed						PSP_TELSUN_TrunkUsed		
#define CSTA_CallID							PSP_TELSUN_CallID	 
#define CSTA_EventID						PSP_TELSUN_EventID	
#define CSTA_TUPOutseizeMessageType			PSP_TELSUN_TUPOutseizeMessageType
#define CSTA_Status							PSP_TELSUN_Status
#define CSTA_ServiceType					PSP_TELSUN_ServiceType
#define CSTA_ActiveConnection				PSP_TELSUN_ActiveConnection
#define CSTA_ACMMessageIndicator			PSP_TELSUN_ACMMessageIndicator
#define CSTA_ChargeFlag						PSP_TELSUN_BillFlag
#define CSTA_RequestTypeIndicator			PSP_TELSUN_RequestTypeIndicator
#define CSTA_ResponseTypeIndicator			PSP_TELSUN_ResponseTypeIndicator
#define CSTA_FlowType						PSP_TELSUN_FlowType
// here 0x19f is free

// account key
#define PSP_TELSUN_RecordType					__CTkvs_PSP(0x0200)
#define PSP_TELSUN_PartialRecordIndicator		__CTkvs_PSP(0x0201)
#define PSP_TELSUN_SequenceNumber				__CTkvs_PSP(0x0202)
#define PSP_TELSUN_CalledAddrIndicator			__CTkvs_PSP(0x0203)
#define PSP_TELSUN_AnswerTime					__CTkvs_PSP(0x0204)
#define PSP_TELSUN_CallType						__CTkvs_PSP(0x0205)
#define PSP_TELSUN_CallEndTime					__CTkvs_PSP(0x0206)
#define PSP_TELSUN_ConversationTime				__CTkvs_PSP(0x0207)
#define PSP_TELSUN_RecordValid					__CTkvs_PSP(0x0208)
#define PSP_TELSUN_ClockChangedFlag				__CTkvs_PSP(0x0209)
#define PSP_TELSUN_TryCallFlag					__CTkvs_PSP(0x020a)
#define PSP_TELSUN_InTrunkID					__CTkvs_PSP(0x020b)
#define PSP_TELSUN_OutTrunkID					__CTkvs_PSP(0x020c)
#define PSP_TELSUN_SupplementService			__CTkvs_PSP(0x020d)
#define PSP_TELSUN_PayType						__CTkvs_PSP(0x020e)
#define PSP_TELSUN_ConnectionNumberIndicator	__CTkvs_PSP(0x020f)
#define PSP_TELSUN_ConnectionNumber				__CTkvs_PSP(0x0210)
#define PSP_TELSUN_AddressChanged               __CTkvs_PSP(0x0211)

//*****************end of SWITCH CONTROL definition

// here 0x211 - 0x27f is free

//*********************************************************************
//Private keys of CONFIG
//**********************************************************************
#define PSP_TELSUN_ConfigData             __CTkvs_PSP(0x0280)
#define PSP_TELSUN_Path                   __CTkvs_PSP(0x0281)
#define PSP_TELSUN_TableName              __CTkvs_PSP(0x0282)
#define PSP_TELSUN_AlarmEntity            __CTkvs_PSP(0x0283)
#define PSP_TELSUN_PhysicalNodeId         __CTkvs_PSP(0x0284)
#define PSP_TELSUN_RingId                 __CTkvs_PSP(0x0285)
#define PSP_TELSUN_Slot                   __CTkvs_PSP(0x0286)
#define PSP_TELSUN_CpuID                  __CTkvs_PSP(0x0287)
#define PSP_TELSUN_IpAddress              __CTkvs_PSP(0x0288)
#define PSP_TELSUN_Report                 __CTkvs_PSP(0x0289)
#define PSP_TELSUN_EXCPUState             __CTkvs_PSP(0x028a)
#define PSP_TELSUN_MatrixStatus           __CTkvs_PSP(0x028b)
#define PSP_TELSUN_SocketSequence         __CTkvs_PSP(0x028c)
#define PSP_TELSUN_AdjacentEXCPUState     __CTkvs_PSP(0x028d)
#define PSP_TELSUN_PhysicalSpanId         __CTkvs_PSP(0x028e)
#define PSP_TELSUN_Format                 __CTkvs_PSP(0x028f)
#define PSP_TELSUN_LineLength             __CTkvs_PSP(0x0290)
#define PSP_TELSUN_SARegister             __CTkvs_PSP(0x0291)
#define PSP_TELSUN_TS16Register           __CTkvs_PSP(0x0292)
#define PSP_TELSUN_GroupId                __CTkvs_PSP(0x0293)
#define PSP_TELSUN_Signaling              __CTkvs_PSP(0x0294)
#define PSP_TELSUN_CicTotal               __CTkvs_PSP(0x0295)
#define PSP_TELSUN_UserPart               __CTkvs_PSP(0x0296)
#define PSP_TELSUN_PplProtocolId          __CTkvs_PSP(0x0297)
#define PSP_TELSUN_TimerId                __CTkvs_PSP(0x0298)
#define PSP_TELSUN_TimerValue             __CTkvs_PSP(0x0299)
#define PSP_TELSUN_PplEntity              __CTkvs_PSP(0x029a)
#define PSP_TELSUN_DataLocations          __CTkvs_PSP(0x029b)
#define PSP_TELSUN_ProtocolSignalingState __CTkvs_PSP(0x029c)
#define PSP_TELSUN_SignalingBits          __CTkvs_PSP(0x029d)
#define PSP_TELSUN_InstructionNumber      __CTkvs_PSP(0x029e)
#define PSP_TELSUN_InstructionType        __CTkvs_PSP(0x029f)
#define PSP_TELSUN_Data0                  __CTkvs_PSP(0x0230)
#define PSP_TELSUN_Data1                  __CTkvs_PSP(0x0231)
#define PSP_TELSUN_StageNumber            __CTkvs_PSP(0x0232)
#define PSP_TELSUN_DigitsStringNumber     __CTkvs_PSP(0x0233)
#define PSP_TELSUN_StageCompleteTimeOut   __CTkvs_PSP(0x0234)
#define PSP_TELSUN_LinkSetId              __CTkvs_PSP(0x0235)
#define PSP_TELSUN_CollectionMethod1      __CTkvs_PSP(0x0236)
#define PSP_TELSUN_CollectionData1        __CTkvs_PSP(0x0237)
#define PSP_TELSUN_CollectionMethod2      __CTkvs_PSP(0x0238)
#define PSP_TELSUN_CollectionData2        __CTkvs_PSP(0x0239)
#define PSP_TELSUN_ForceFlag              __CTkvs_PSP(0x023a)
#define PSP_TELSUN_LoopTimerType          __CTkvs_PSP(0x023b)
#define PSP_TELSUN_Entity                 __CTkvs_PSP(0x023c)
#define PSP_TELSUN_EntityID               __CTkvs_PSP(0x023d)
#define PSP_TELSUN_PrimarySlot            __CTkvs_PSP(0x023e)
#define PSP_TELSUN_RedundancySlot         __CTkvs_PSP(0x023f)
#define PSP_TELSUN_Option                 __CTkvs_PSP(0x0240)
#define PSP_TELSUN_Timestamp              __CTkvs_PSP(0x0241)
#define PSP_TELSUN_ActiveSlot             __CTkvs_PSP(0x0242)
#define PSP_TELSUN_APC                    __CTkvs_PSP(0x0243)
#define PSP_TELSUN_DestinationId          __CTkvs_PSP(0x0244)
#define PSP_TELSUN_Priority               __CTkvs_PSP(0x0245)
#define PSP_TELSUN_Severity	   	  __CTkvs_PSP(0x0246)
#define PSP_TELSUN_AlarmNumber            __CTkvs_PSP(0x0247)
#define PSP_TELSUN_AlarmData              __CTkvs_PSP(0x0248)
#define PSP_TELSUN_LinkId                 __CTkvs_PSP(0x0249)
#define PSP_TELSUN_SLC                    __CTkvs_PSP(0x024a)
#define PSP_TELSUN_DataRate               __CTkvs_PSP(0x024b)
#define PSP_TELSUN_AnswerSupervisionMode  __CTkvs_PSP(0x024c)
#define PSP_TELSUN_LocalReleaseMode       __CTkvs_PSP(0x024d)
#define PSP_TELSUN_DistantReleaseMode     __CTkvs_PSP(0x024e)
#define PSP_TELSUN_Modules				  __CTkvs_PSP(0x024f)
#define PSP_TELSUN_StatusBits			  __CTkvs_PSP(0x0250)
#define PSP_TELSUN_SystemType			  __CTkvs_PSP(0x0251)
#define PSP_TELSUN_Tag					  __CTkvs_PSP(0x0252)
#define PSP_TELSUN_ByteNum				__CTkvs_PSP(0x0253)
#define PSP_TELSUN_LogicalNodeId                        __CTkvs_PSP(0x0254)
#define PSP_TELSUN_NodeStatus                           __CTkvs_PSP(0x0255)
#define PSP_TELSUN_HostNodeID                           __CTkvs_PSP(0x0256)
#define PSP_TELSUN_RedundancyStatus			__CTkvs_PSP(0x0257)
#define PSP_TELSUN_RedundancyType			__CTkvs_PSP(0x0258)
#define PSP_TELSUN_SIMM					__CTkvs_PSP(0x0259)
#define PSP_TELSUN_RingStatus				__CTkvs_PSP(0x025a)
#define PSP_TELSUN_MoreStatus				__CTkvs_PSP(0x025b)
#define PSP_TELSUN_EXNETPortAStatus			__CTkvs_PSP(0x025c)
#define PSP_TELSUN_EXNETPortBStatus 			__CTkvs_PSP(0x025d)
#define PSP_TELSUN_MasterStatus				__CTkvs_PSP(0x025e)
#define PSP_TELSUN_TransmitMode				__CTkvs_PSP(0x025f)
#define PSP_TELSUN_SourcePacketNumber 			__CTkvs_PSP(0x0260)
#define PSP_TELSUN_LinkSets					__CTkvs_PSP(0x0261)
#define PSP_TELSUN_DSP0						__CTkvs_PSP(0x0262)
#define PSP_TELSUN_DSP1						__CTkvs_PSP(0x0263)
#define PSP_TELSUN_DSP2						__CTkvs_PSP(0x0264)
#define PSP_TELSUN_DSP3						__CTkvs_PSP(0x0265)
#define PSP_TELSUN_UpdateFlag				__CTkvs_PSP(0x0266)
#define PSP_TELSUN_ToneType					__CTkvs_PSP(0x0267)
#define PSP_TELSUN_StartTime                __CTkvs_PSP(0x0268)
#define PSP_TELSUN_EndTime                  __CTkvs_PSP(0x0269)
#define PSP_TELSUN_CallTotal                __CTkvs_PSP(0x026a)
#define PSP_TELSUN_CallSuccess              __CTkvs_PSP(0x026b)
#define PSP_TELSUN_CallFail1                __CTkvs_PSP(0x026c)
#define PSP_TELSUN_CallFail2                __CTkvs_PSP(0x026d)
#define PSP_TELSUN_CallFail3                __CTkvs_PSP(0x026e)
#define PSP_TELSUN_CallFail4                __CTkvs_PSP(0x026f)
#define PSP_TELSUN_CallFail5                __CTkvs_PSP(0x0270)
#define PSP_TELSUN_CallFail6                __CTkvs_PSP(0x0271)

// Add by zja
#define PSP_TELSUN_SocketStatus			  __CTkvs_PSP(0x0450)
#define PSP_TELSUN_SocketNumber			  __CTkvs_PSP(0x0451)

// keys used by Maint and Terminal
#define PSP_TELSUN_TermID				__CTkvs_PSP(0x0300)
#define PSP_TELSUN_OperatorName			__CTkvs_PSP(0x0301)
#define PSP_TELSUN_OperatorID			__CTkvs_PSP(0x0302)
#define PSP_TELSUN_OperatorPurview		__CTkvs_PSP(0x0303)
#define PSP_TELSUN_OperatorPassword		__CTkvs_PSP(0x0304)
#define PSP_TELSUN_MsgID				__CTkvs_PSP(0x0305)
#define PSP_TELSUN_PortNO				__CTkvs_PSP(0x0306) //HHJ

#define PSP_TELSUN_StatType				__CTkvs_PSP(0x0307)
#define PSP_TELSUN_StatPeriod			__CTkvs_PSP(0x0308)


// Add by Terminal  
#define PSP_TELSUN_SlaveNodeID			__CTkvs_PSP(0x0320)
#define PSP_TELSUN_Value			    __CTkvs_PSP(0x0321)
#define PSP_TELSUN_Facility 			__CTkvs_PSP(0x0322)
#define PSP_TELSUN_FlashMode			__CTkvs_PSP(0x0327)
#define PSP_TELSUN_ConfigurationType    __CTkvs_PSP(0x0328)
#define PSP_TELSUN_PollInterval 		__CTkvs_PSP(0x0329)
#define PSP_TELSUN_AuditType		    __CTkvs_PSP(0x032a)
#define PSP_TELSUN_SignalingValue 		__CTkvs_PSP(0x032b)
#define PSP_TELSUN_BitMask      		__CTkvs_PSP(0x032c)
#define PSP_TELSUN_AnnouncementSize 	__CTkvs_PSP(0x032d)
#define PSP_TELSUN_AnnouncementChecksum	__CTkvs_PSP(0x032e)
#define PSP_TELSUN_AnnouncementFormat	__CTkvs_PSP(0x032f)
#define PSP_TELSUN_EncodingFormat   	__CTkvs_PSP(0x0330)
#define PSP_TELSUN_CardType				__CTkvs_PSP(0x0331)
#define PSP_TELSUN_StartDialType       	__CTkvs_PSP(0x0332)
#define PSP_TELSUN_StartDialValue   	__CTkvs_PSP(0x0333)
#define PSP_TELSUN_SynchronizationMode1	__CTkvs_PSP(0x0334)
#define PSP_TELSUN_SynchronizationMode2 __CTkvs_PSP(0x0335)
#define PSP_TELSUN_SynchronizationMode3 __CTkvs_PSP(0x0336)
#define PSP_TELSUN_SynchronizationMode4 __CTkvs_PSP(0x0337)
#define PSP_TELSUN_SynchronizationMode5 __CTkvs_PSP(0x0338)
#define PSP_TELSUN_TrunkType           	__CTkvs_PSP(0x0339)
#define PSP_TELSUN_CPUSpeed            	__CTkvs_PSP(0x033a)
#define PSP_TELSUN_CommandCode			__CTkvs_PSP(0x033b)
#define PSP_TELSUN_Listener             __CTkvs_PSP(0x033c)
#define PSP_TELSUN_Broadcaster          __CTkvs_PSP(0x033d)
#define PSP_TELSUN_PartDevice           __CTkvs_PSP(0x033e)
#define PSP_TELSUN_WatchOutTime         __CTkvs_PSP(0x033F)

#define PSP_TELSUN_BeginTime			__CTkvs_PSP(0x0340)
#define PSP_TELSUN_SystemTime			__CTkvs_PSP(0x0341)
#define PSP_TELSUN_TranToCTS			__CTkvs_PSP(0x0342)


// keys used by TComModule
#define PSP_TELSUN_EventMultiAIB		__CTkvs_PSP(0x0400)
#define PSP_TELSUN_RespMultiAIB			__CTkvs_PSP(0x0401)
// key used by ss7 gateway
#define PSP_TELSUN_CalledAddrLength			__CTkvs_PSP(0x0500)

#define AIBOfSlot        				0x101
#define AIBOfChannel     				0x10d
#define AIBOfSS7Link     				0x109
#define AIBOfSS7Stack        			0x108
#define AIBOfVCIC						0x131

//******************** end ********************************

/* CSTA error code and cause code */
#define CSTA_OK					0x0000
#define CSTA_FAIL				0x0001

#define CSTA_OP_GenericError			0x0101
#define CSTA_OP_RequestInvalid			0x0102
#define CSTA_OP_ValueOutOfRange			0x0103
#define CSTA_OP_ObjectUnKnown			0x0104
#define CSTA_OP_InvalidCallingDevice		0x0105
#define CSTA_OP_InvalidCalledDevice		0x0106
#define CSTA_OP_PVOnSpecifiedDevice		0x0107
#define CSTA_OP_InvalidForwadingDest		0x0108
#define CSTA_OP_PVOnCalledDevice			0x0109
#define CSTA_OP_PVOnCallingDevice			0x010a
#define CSTA_OP_InvalidCallID			0x010b
#define CSTA_OP_InvalidDeviceID			0x010c
#define CSTA_OP_InvalidConnectionID		0x010d
#define CSTA_OP_InvalidDestination			0x010e
#define CSTA_OP_InvalidFeature			0x010f
#define CSTA_OP_InvalidAllocationState		0x0110
#define CSTA_OP_InvalidCrossRefID			0x0111
#define CSTA_OP_InvalidObjectType			0x0112
#define CSTA_OP_SecurityViolation			0x0113
#define CSTA_OP_InvalidCorrelatorData		0x0114
#define CSTA_OP_InvalidAccountCode			0x0115
#define CSTA_OP_InvalidAuthorizationCode		0x0116
#define CSTA_OP_RIWithCalllingDevice		0x0117
#define CSTA_OP_RIWithCalledDevice			0x0118
#define CSTA_OP_InvalidMessageID			0x0119
#define CSTA_OP_MessageIDRequired			0x011a
#define CSTA_OP_MediaIncompatible			0x011b
#define CSTA_OP_FileNotFound			0x011c
#define CSTA_OP_NeedCallingNumber			0x011d
#define CSTA_OP_NeedOrigCalledNumber		0x011e
#define CSTA_OP_LineOutOfService			0x011f
#define CSTA_OP_AddressNotCompleted			0x0120
#define CSTA_OP_InvalidAgentGroup			0x0121
#define CSTA_OP_DeviceHasAgent				0x0122
#define CSTA_OP_ExistSameAgent				0x0123

// state incompatibility errors
#define CSTA_ST_GenericError			0x0301
#define CSTA_ST_IncorrectObjectState		0x0302
#define CSTA_ST_InvalidConnectionID		0x0303
#define CSTA_ST_NoActiveCall			0x0304
#define CSTA_ST_NoHeldCall				0x0305
#define CSTA_ST_NoCallToClear			0x0306
#define CSTA_ST_NoConnectionToClear		0x0307
#define CSTA_ST_NoCallToAnswer			0x0308
#define CSTA_ST_NoCallToComplete			0x0309
#define CSTA_ST_NotAbleToPlay			0x030a
#define CSTA_ST_NotAbleToResume			0x030b
#define CSTA_ST_EndOfMessage			0x030c
#define CSTA_ST_BeginingOfMessage			0x030d
#define CSTA_ST_MessageSuspended			0x030e

// system resource avaliability errors
#define CSTA_SR_GenericError			0x0401
#define CSTA_SR_ServiceBusy				0x0402
#define CSTA_SR_ResourceBusy			0x0403
#define CSTA_SR_ResourceOutOfService		0x0404
#define CSTA_SR_NetworkBusy				0x0405
#define CSTA_SR_NetworkOutOfService		0x0406
#define CSTA_SR_OverallMonitorLimitExceeded	0x0407
#define CSTA_SR_ConferenceMemberLimitExceeded	0x0408

// cause code define

#define CSTA_Cause_Busy				0x0002
#define CSTA_Cause_CallCancelled			0x0003
#define CSTA_Cause_CallForward			0x0004
#define CSTA_Cause_CallFd_Immediate		0x0005
#define CSTA_Cause_CallFd_Busy			0x0006
#define CSTA_Cause_CallFd_NoAnswer			0x0007
#define CSTA_Cause_CallNotAnswered			0x0008
#define CSTA_Cause_CampOn				0x0009
#define CSTA_Cause_DestNotObtainable		0x000a
#define CSTA_Cause_DoNotDisturb			0x000b
#define CSTA_Cause_IncompatibleDest		0x000c
#define CSTA_Cause_InvalidAccountCode		0x000d
#define CSTA_Cause_KeyOperation			0x000e
#define CSTA_Cause_Lockout				0x000f
#define CSTA_Cause_Maintenance			0x0010
#define CSTA_Cause_NetworkCongestion		0x0011
#define CSTA_Cause_NetworkNotObtainable		0x0012
#define CSTA_Cause_NoAvalableAgent			0x0013
#define CSTA_Cause_Overflow				0x0014
#define CSTA_Cause_Override				0x0015
#define CSTA_Cause_Recall				0x0016
#define CSTA_Cause_Redirected			0x0017
#define CSTA_Cause_RecorderTone			0x0018
#define CSTA_Cause_ResourceNotAvaliable		0x0019
#define CSTA_Cause_Transfer				0x001a
#define CSTA_Cause_TrunkBusy			0x001b
#define CSTA_Cause_Consultation			0x001c
#define CSTA_Cause_Conference			0x001d
#define CSTA_Cause_ListenCall			0x001e
#define CSTA_Cause_UserBusy				0x001f
#define CSTA_Cause_NoAnswer				0x0020
#define CSTA_Cause_CallFail				0x0021
#define CSTA_Cause_CallRelease          0x0022

#endif


