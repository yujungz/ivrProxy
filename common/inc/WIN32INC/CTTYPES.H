#ifndef __CTtypes_H__
#define __CTtypes_H__

#include <time.h>

typedef unsigned char            uchar;
typedef unsigned short           ushort;
typedef unsigned long            ulong;

typedef unsigned char            CTbyte;
typedef short  int               CTshort;
typedef unsigned short int       CTushort;// Walker, 1998-06-02
typedef signed long              CTint;
typedef unsigned long            CTuint;
typedef float                    CTfloat;
typedef signed long              CTbool;
typedef unsigned long            CTstatus;
typedef unsigned long            CTerror;
typedef CTuint                   CTsymbol;
typedef time_t                   CTtime;
typedef CTuint                   CTobj;

#define CT_boolTRUE              1
#define CT_boolFALSE             0

// KV Set value type definitions
#define VALTYP_SCALAR_BASE        10
#define VALTYP_NONSCALAR_BASE     40
#define VALTYP_ARRAY_BASE         60

#define CTkvs_valtypNULL         VALTYP_SCALAR_BASE + 0
#define CTkvs_valtypBYTE         VALTYP_SCALAR_BASE + 1
#define CTkvs_valtypSHORT        VALTYP_SCALAR_BASE + 2
#define CTkvs_valtypINT          VALTYP_SCALAR_BASE + 3
#define CTkvs_valtypUINT         VALTYP_SCALAR_BASE + 4
#define CTkvs_valtypBOOL         VALTYP_SCALAR_BASE + 5
#define CTkvs_valtypFLOAT        VALTYP_SCALAR_BASE + 6
#define CTkvs_valtypERROR        VALTYP_SCALAR_BASE + 7
#define CTkvs_valtypSTATUS       VALTYP_SCALAR_BASE + 8
#define CTkvs_valtypSYMBOL       VALTYP_SCALAR_BASE + 9
#define CTkvs_valtypTIME         VALTYP_SCALAR_BASE + 10
#define CTkvs_valtypUSHORT       VALTYP_SCALAR_BASE + 11 // Walker,1998-06-02

#define CTkvs_valtypIRANGE       VALTYP_NONSCALAR_BASE + 0
#define CTkvs_valtypFRANGE       VALTYP_NONSCALAR_BASE + 1
#define CTkvs_valtypSTRING       VALTYP_NONSCALAR_BASE + 2

#define CTkvs_valtypNULLARRAY       VALTYP_ARRAY_BASE + 0
#define CTkvs_valtypBYTEARRAY       VALTYP_ARRAY_BASE + 1
#define CTkvs_valtypSHORTARRAY      VALTYP_ARRAY_BASE + 2
#define CTkvs_valtypINTARRAY        VALTYP_ARRAY_BASE + 3
#define CTkvs_valtypUINTARRAY       VALTYP_ARRAY_BASE + 4
#define CTkvs_valtypBOOLARRAY       VALTYP_ARRAY_BASE + 5
#define CTkvs_valtypSYMBOLARRAY     VALTYP_ARRAY_BASE + 6
#define CTkvs_valtypFLOATARRAY      VALTYP_ARRAY_BASE + 7
#define CTkvs_valtypSTRINGARRAY     VALTYP_ARRAY_BASE + 8
#define CTkvs_valtypARRAYARRAY      VALTYP_ARRAY_BASE + 9
#define CTkvs_valtypERRORARRAY      VALTYP_ARRAY_BASE + 10
#define CTkvs_valtypSTATUSARRAY     VALTYP_ARRAY_BASE + 11
#define CTkvs_valtypFRANGEARRAY     VALTYP_ARRAY_BASE + 12
#define CTkvs_valtypIRANGEARRAY     VALTYP_ARRAY_BASE + 13
#define CTkvs_valtypUSHORTARRAY     VALTYP_ARRAY_BASE + 14 // Walker,1998-06-02


typedef enum { CT_modeSYNC, CT_modeASYNC } CTmode;

typedef enum {
	RSC_NULL=0,
	RSC_PLAYER=0x1,
	RSC_RECORDER=0x2,
	RSC_SD=0x4,
	RSC_SG=0x8,
	RSC_CCR=0x10,
	RSC_SENDFAX=0x20,
	RSC_RECFAX=0x40,
	RSC_MSI=0x80
} CTresourceType ;

typedef enum {
	GRP_IDLE=0,
	GRP_PLAY_PROMPT,
	GRP_PLAY_CONT,
	GRP_REC,
	GRP_SD,
	GRP_SG,
	GRP_SENDFAX,
	GRP_RECVFAX
} CTgrpStatus ;

typedef enum {
	RSC_PRIMARY,
	RSC_SECONDARY
} CTresourceStatus ;

typedef enum 
{
	SPC_Net,
	SPC_Voice
} TSPCType  ;

#define connectModeSingle     0
#define connectModeDouble     1
#define disconnectModeSingle  0
#define disconnectModeDouble  1

// Integer Range
typedef struct {
   CTint         lower;
   CTint         upper;
} CTirange ;

// Floating point range
typedef struct {
   CTfloat        lower;         
   CTfloat        upper;
} CTfrange ;

typedef struct {
   CTuint         lower;
   CTuint         upper;
} CTurange ;

typedef struct {
   CTuint mailboxKey ;
   CTint voxOffset ;
   CTint voxLength ;
   CTint voxIndex ;
} CTMailBoxInfo ;

typedef struct {
   char   caller[18];
   char   called[21];
   char   notifyType;
   int    notifyCount;
   long   notifyInterval;
} CTScheduleInfo ;

#define _delete(p) delete p; p=0

#ifndef WIN32
typedef enum {
	false,
	true
} bool ; // For compatity of Smart. Walker, 1998-06-03
#endif//WIN32

#endif

