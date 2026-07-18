// This file should be obsoleted because they aren't elegent lines
// But...as we all know, it's hard to meet every one! Rao Changzheng.
#ifndef __CTSTATUS_H_
#define __CTSTATUS_H_

#define ANYKEY              "?"
#define E_Success			0
#define E_SUCCESS           0
#define E_Fail              -1

/*#ifdef  E_FAIL
#undef  E_FAIL
#endif//E_FAIL 
#define E_FAIL              -1
*/
#define E_VerifyOK			0
#define E_InvalidUserID			-2
#define E_UserLock			-3
#define E_UserHalt			-4
#define E_NoUser			-5
#define E_CheckPwdOK			0
#define E_NoPassword			-6
#define E_InvalidPwd			-7
#define E_SetPwdOK			0

#define E_IntData			-8
#define E_FloatData			-9
#define E_StringData			-10
#define E_InvalidNum			-11
#define E_UserOptOK			0
#define E_InvalidType			-12
#define E_InvalidData			-13
#define E_NoLimit			-14

#define E_MailboxOptOK			0
#define E_InvalidMailbox		-15
#define E_NoOpenedMailbox		-16
#define E_MailboxOpened			-17
#define E_MailboxUsed			-18
#define E_MailboxLocked			-19
#define E_MailboxNoLocked		-20
#define E_InvalidFileName		-21
#define E_InvalidMailNum		-22

#define E_UserOptOK			0
#define E_UserExist			-23
#define E_UserOptFail			-24

#define E_DialTerminate			1
#define E_StopPlayBack			2
#define E_Disconnect			3

#define E_StopRecord			5
#define E_StopDigits			6
#define E_DialBreak			7
#define E_Null				8
#define E_DialOK			0

#define E_TimeOut			-25
#define E_NoVoxRes			-30
#define E_NoResGroup			-31
#define E_VoidCharExist			-32

#define E_RecordOK			0
#define E_VoidFileName			-33

#define E_InputOK			0

#define E_NoVoxGroup			-34
#define E_TooManyDigits  		-35

#define E_SetOK				0

#define E_ResGrpOptOK			0

#define E_PlayVoxOK			0
#define E_NoOPenMailbox			-36
#define E_NoVoxSource			-37

#define E_AttachResOK			0
#define E_DetachResOK			0
#define E_NoResource			-38
#define E_ResUsed			-39
#define E_OtherResExist	  		-40

#define E_CalloutOK			0
#define E_InvalidCall			-50
#define E_InvalidResGrpID		-51
#define E_TooLongTime			-52
#define E_ConnectOK			0
#define E_NoAnswer			5
#define E_UserBusy			2
#define E_LineBusy			6
#define E_Blocked			3
#define E_EmptyNum			1
#define E_SystemError                   4

#define E_SetSchedOK			0
#define E_InvalidSerType		-60
#define E_InvalidFunc			-61
#define E_InvalidTime			-63
#define E_SetSchedFail			-64
#define E_SetCallOK			0
#define E_InvalidCalled			-65
#define E_ActivateOK			0
#define E_ActivateFail			-66
#define E_Used				-67
#define E_BadMailIndex			-68

#define	E_PEERNOTFOUND                  -69

#define	E_TCBPAUSE                      -70
#define	E_QUEUEFULL                     -71
#define E_NETWORK                       -72
#define E_NOTFOUND                      -73
#define E_MAXTCBNUM	                    -74
#define E_MEMORY	                    -75
#define E_QUEUEEMPTY					-76
#define E_OUTOFRANGE	                -1000

#endif
