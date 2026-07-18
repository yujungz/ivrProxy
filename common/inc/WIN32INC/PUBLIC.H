#ifndef __PUBLIC__H_
#define __PUBLIC__H_

#include "stdafx.h"
#include "ctgor.h"
#include "tlog.h"

#define MAX_NUMBER_LEN		20

#define INVALID_GOR			0xffffffff
#define NOT_CHARGE          0
#define CHARGE				1

#define CALLERCONTROL       0
#define CALLEDCONTROL       1
#define NOTCONTROL          2

// Macros to device IDs

const CTuint constDEVICEStationOffset = 20;     // 0 ~ F
const CTuint constDEVICEStationMask   = 0xFFF;

const CTuint constDEVICETypeOffset    = 12;     // 0 ~ F
const CTuint constDEVICETypeMask      = 0xFF;

const CTuint constDEVICEIndexMask    = 0xFFF;   // For A Module Id, 0 ~ 255

#define  MAKEDEVICE(stationno, type, index) \
		(CTuint) ( ( ((stationno)&constDEVICEStationMask)<<constDEVICEStationOffset) | \
		  ( ((type)&constDEVICETypeMask)<<constDEVICETypeOffset) | \
		    ((index)&constDEVICEIndexMask) )

#define  GET_DEVICE_STATION(gorno) \
		( ((gorno)>>constDEVICEStationOffset)&constDEVICEStationMask )
#define  GET_DEVICE_TYPE(gorno) \
                ( ((gorno)>>constDEVICETypeOffset)&constDEVICETypeMask )
#define  GET_DEVICE_INDEX(gorno)    ( (gorno)&constDEVICEIndexMask )


#define GET_SPAN(device_id) (CTushort) ((device_id) >> 5)
#define GET_CHANNEL(device_id) (CTbyte) ((device_id) & 0x001F)
#define CHANNEL_TO_DEVICE(span_id,channel_id) (CTushort) \
                                              (((span_id) << 5) | (channel_id))
/*
struct EventLogParam
{
	CTtime	DateTime;
	CTobj	Source;
	CString	Description;
	EventType	Type;
};

// used in MSG_AllocStatusIndicator
typedef enum {
	Status_Idle=0,
	Status_Error=1,
	Status_OK=2,
	Status_Warning=3
} StatusType;

// used in MSG_ShowStatus
struct StatusParam
{
	CTushort	DeviceID;
	CString		Infomation;
	StatusType	Status;
};

struct DeviceParam
{
	CTuint		DeviceID;
	CTushort	DevType;
	CString		Infomation;
	CTshort		Status;
};
*/
#endif