#ifndef __CTGOR_H__
#define __CTGOR_H__

#include "cttypes.h"

/////////////////////////////////////////////////////////////////////////////
// Station related stuff

#define CT_ALL_STATION            0

/////////////////////////////////////////////////////////////////////////////
// Module identification

#define GOR_CommManager           0x00 // CommMain
//#define GOR_TCPServer             0x01 // CommMain
//#define GOR_UDPServer             0x01 // CommMain
#define GOR_MediaServer           0x02 // IVR
#define GOR_CSSControler          0x02 // MCU(IVR)	
#define GOR_EventTrigger          0x03 
#define GOR_SIA                   0x04 // IVR
#define GOR_MCUService            0x04 // MCU(IVR)
#define GOR_Profile               0x05 // Not used
#define GOR_NMAgent				  0x05  

#define GOR_Authen                0x07 // Reserve
#define GOR_CHATSERVER            0x08 // Dennis
#define GOR_PUSHSERVER            0x09 // Dennis
#define GOR_CTSERVER              0x0a // Dennis
#define GOR_PREDIALSERVER         0x0b // Dennis
#define GOR_IROUTERSERVER         0x0c // Dennis
#define GOR_MAINTAINSERVER        0x0d // Dennis

#define GOR_Console               0x0f // Console of CommMain
#define GOR_SCR                   0x10
#define GOR_Configuration         0x11 // Configuration 
#define GOR_SwitchControl         0x12 // SwitchControl
#define GOR_ACD                   0x12 // ACD is SwitchControl
#define GOR_CommControl           0x13 // Communication
#define GOR_Maint                 0x14 // Maintenance 
#define GOR_DataCom               0x15 // DataManager
#define GOR_ConfigServer          0x15 // Config Server
#define GOR_Terminal			  0x16
#define GOR_TerminalServer        0x17
#define GOR_AAASERVER             0x18 // Chen RuiFeng
#define GOR_DBACCESSSERVER        0x19 // Chen RuiFeng
#define GOR_GATEWAYSERVER         0x1a // Chen RuiFeng
#define GOR_ACDAccount            0x1b // Li ChangFeng 98.12.31


#define GOR_AgentTest1            0x1d
#define GOR_AgentTest             0x1e
#define GOR_Test                  0x1f

#define GOR_RMonitor			  0x20 // Chen SongHeng for host redundant


#define GOR_NetDevice			  0x21
#define GOR_VoxDevice			  0x22
#define GOR_MsiDevice			  0x23
#define GOR_FaxDevice			  0x24
#define GOR_IptDevice			  0x25
#define GOR_LsiDevice			  0x26
#define GOR_SS7Service            0x27
#define GOR_ConfDevice			  0x28
#define GOR_ConfManager           0x29

#ifdef  GOR_CTSERVER
#define GOR_TeleManager			  GOR_CTSERVER
#else
#define GOR_TeleManager			  0x30
#endif
#define GOR_CallManager			  0x31
#define GOR_SoftPhone			  0x32
#define GOR_StaticsManager		  0x33
#ifdef  GOR_PREDIALSERVER
#define GOR_PredialServer		  GOR_PREDIALSERVER
#else
#define GOR_PredialServer		  0x34
#endif

#define GOR_DispatchData		  0x35
#define GOR_GateKeeperRAS		  0x36
#define GOR_GateKeeperCM		  0x37
#define GOR_PciServiceStation	  0x38
#define GOR_DispatchFile		  0x39
#define GOR_OAM					  0x3a

#define GOR_RECORDER			  0x3f
#define GOR_TTS                   0x40
#define GOR_RAS                   0x41

#define GOR_VipDevice			  0x43
#define GOR_VdtDevice			  0x44
#define GOR_SHARECOMM			  0x45
#define GOR_DBFGW				  0x46    //add 2003/01/01 by wjf
#define	GOR_CONFIGPROXY			  0x47    //add 2003/12/21 by wjf
// +---------+------+-----------+
// |31     20|19  16|15        0|             A Key
// +---------+------+-----------+
//  StationNo  Type  Alloc Value

// +---------+------+-------+------------+
// |31     20|19  16|15    8|7          0|    A Module Id
// +---------+------+-------+------------+
//  StationNo  Type  Reserve Module Index

const CTuint constGORStationOffset = 20;     // 0 ~ 4095
const CTuint constGORStationMask   = 0xFFF;

const CTuint constGORTypeOffset    = 16;     // 0 ~ F
const CTuint constGORTypeMask      = 0xF;

const CTuint constGORModuleMask    = 0x7F;   // For A Module Id, 0 ~ 255
const CTuint constGORIndexMask     = 0xFFFF; // For A Key,       0 ~ 65535

const CTuint constGORModuleType = 0;
const CTuint constGORModuleRange = 0xFFFF;

const CTuint constGORResourceType = 1;
const CTuint constGORResourceRange = 0xFFFF;

const CTuint constGORSpcType = 2;
const CTuint constGORSpcRange = 0xFFFF;

const CTuint constGORSessionType = 3;
const CTuint constGORSessionRange = 0xFFFF;

const CTuint constGORContainerType = 4;
const CTuint constGORContainerRange = 0xFFFF;

const CTuint constGORGroupType = 5;
const CTuint constGORGroupRange = 0xFFFF;

const CTuint constGORPromptType = 6;
const CTuint constGORPromptRange = 0xFFFF;

const CTuint constGORGatewayType = 7;
const CTuint constGORGatewayRange = 0xFFFF;

const CTuint constGORConferenceType = 8;		// MCU MediaServer	1999-4-6 wsh
const CTuint constGORConferenceRange = 0xFFFF;

const CTuint constGORConferenceControlerType = 9;	// MCU Service	1999-4-6 llz
const CTuint constGORConferenceControlerRange = 0xFFFF;

// Macros to process IDs

#define  MAKEGOR(stationno, type, index) \
		( ( ((stationno)&constGORStationMask)<<constGORStationOffset) | \
		  ( ((type)&constGORTypeMask)<<constGORTypeOffset) | \
		    ((index)&constGORIndexMask) )

#define  MAKEGORRANGE(stationno, type, range) MAKEGOR(stationno, type, range)

// Form an id of a certain kind of module.
#define  MAKEMODULEGOR(stationno, moduleno)     \
		MAKEGOR(stationno, constGORModuleType, (moduleno)&constGORModuleMask)

// Get the station No. from an id.
#define  GETGORSTATION(gorno) \
		( ((gorno)>>constGORStationOffset)&constGORStationMask )

#define  GETGORTYPE(gorno) \
                ( ((gorno)>>constGORTypeOffset)&constGORTypeMask )

// Get the kind of module from an id.
#define  GETGORINDEX(gorno)    ( (gorno)&constGORIndexMask )
#define  GETSTATION(moduleno) GETGORSTATION(moduleno)
// just don't delete it !!!
#define  GETMODULE(moduleno)   ( (moduleno&constGORModuleMask) )

/////////////////////////////////////////////////////////////////////////////
// UDP performence related stuff

#define UDP_VSVMS_PORT 3333

const int MAX_TCBSUM           = 1024;
const int constMAXSTATIONSUM   = MAX_TCBSUM;
const int constMINSTATIONNUM   = 1;
const int constMAXSTATIONNUM   = constMAXSTATIONSUM;
const int constMAXMODULENUM    = 128;
const int constMINMODULEGOR    = 0;
const int constMINCLIENTMODULEGOR = GOR_CommManager+1;
const int constMAXMODULEGOR    = constMAXMODULENUM - 1;
const int constSTATIONDATASIZE = constMAXMODULENUM / 8;
								//(constMAXMODULENUM+7) / 8;

/////////////////////////////////////////////////////////////////////////////
// Alarm Entity

const int constAlarmEntityAdjacent           = 1;
const int constAlarmEntityFlowControl        = 2;
const int constAlarmEntityAbnormal           = 3;
const int constAlarmEntityUDP                = 4;

// Alarm Level
const int constAlarmLevelVenial              = 1;
const int constAlarmLevelMedium              = 2;
const int constAlarmLevelSevere              = 3;

// Alarm No.
const int constAlarmNoActivate               = 1;
const int constAlarmNoDeactivate             = 2;
const int constAlarmNoModuleActivate         = 3;
const int constAlarmNoModuleDeactivate       = 4;
const int constAlarmNoStationConnect         = 5;
const int constAlarmNoStationDisconnect      = 6;

const int constAlarmNoValveSet               = 1;
const int constAlarmNoValveClear             = 2;
const int constAlarmNoUnknownModule          = 3;
const int constAlarmNoUnknownStation         = 4;

const int constAlarmNoModuleAbort            = 1;
const int constAlarmNoStationAbort           = 2;

const int constAlarmNoQueueFull              = 1;
const int constAlarmNoTCBPause               = 2;
const int constAlarmNoDeliverFailure         = 3; // Not used

#endif
