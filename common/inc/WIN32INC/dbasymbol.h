/////////////////////////////////////////////////////////////////////////////
// DBAsymbol.h

#ifndef __DBASYMBOL_H__
#define __DBASYMBOL_H__

#include "ctsymbol.h"

#define CT_objDBA				(0x00BF)
#define __CTivsp_DBA(item)  	CTsym_Build( CT_objDBA,CT_vendTELSUN,(item) )

/////////////////////////////////////////////////////////////////////////////

//KVSet in account messages sent by IVR / IVF
#define DBA_TELSUN_Index 		__CTivsp_DBA(0x0000)
#define DBA_TELSUN_Argument 	__CTivsp_DBA(0x0001)
#define DBA_TELSUN_Value  		__CTivsp_DBA(0x0002)
#define DBA_TELSUN_ExecuteSql 	__CTivsp_DBA(0x0003)

//KVSet in account messages sent by DBA server
#define DBA_TELSUN_FieldCount 	__CTivsp_DBA(0x0010)
#define DBA_TELSUN_FieldContent __CTivsp_DBA(0x0011)
#define DBA_TELSUN_FieldOn 		__CTivsp_DBA(0x0012)
#define DBA_TELSUN_FieldData 	__CTivsp_DBA(0x0013)

#define DBA_TELSUN_Param  DBA_TELSUN_FieldContent

//procedure's return code, CTint
#define DBA_TELSUN_Status  		__CTivsp_DBA(0x0016)
#define DBA_TELSUN_Compute  	__CTivsp_DBA(0x0017)
#define DBA_TELSUN_DataBaseType __CTivsp_DBA(0x0018)

//used for row resultset in procedure.
#define DBA_TELSUN_ProcRowResult __CTivsp_DBA(0x0019)
#define DBA_TELSUN_Field1	 	 __CTivsp_DBA(0x001a)
#define DBA_TELSUN_Field2	 	 __CTivsp_DBA(0x001b)
#define DBA_TELSUN_Field3	 	 __CTivsp_DBA(0x001c)
#define DBA_TELSUN_Field4	 	 __CTivsp_DBA(0x001d)
#define DBA_TELSUN_Field5	 	 __CTivsp_DBA(0x001e)
#define DBA_TELSUN_Field6	 	 __CTivsp_DBA(0x001f)
#define DBA_TELSUN_Field7	 	 __CTivsp_DBA(0x0020)
#define DBA_TELSUN_Field8	 	 __CTivsp_DBA(0x0021)
#define DBA_TELSUN_Field9	 	 __CTivsp_DBA(0x0022)
#define DBA_TELSUN_Field10	 	 __CTivsp_DBA(0x0023)
#define DBA_TELSUN_Field11	 	 __CTivsp_DBA(0x0024)

#endif
