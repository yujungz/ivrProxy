#ifndef __CTkvset_H__
#define __CTkvset_H__

#include <stdlib.h>
#include <assert.h>
#include "cttypes.h"
#include "ctsymbol.h"

#define MAXPAIRLEN			0x1000   //4096
#define MAXKVSETLEN			0x2000	  //8192

const CTbyte KVSET_START = 0xff ;

///////////////////////////////////////////
//
// KVSet :
// KVPair -> KVPair -> KVPair ... -> KVPair
//
// KVPair :
// < Key , Value >
//
///////////////////////////////////////////

class TKVPair
{

public:

	TKVPair( ) : key(0),type(CTkvs_valtypNULL), size(0), value(0), next(0) { }
    TKVPair( CTsymbol toKey, CTuint toType, CTuint toSize, void *toValue, TKVPair * p ) ;
    TKVPair( CTsymbol toKey, CTuint toType, char *szString[], CTuint eiElements, TKVPair * prev ) ;

    ~TKVPair( ) ;

    CTsymbol  key ;
    CTuint	  type ;
    CTuint 	  size ;
    void    * value ;
    TKVPair	* next ;

} ;

///////////////////////////////////////////////////////

class TKVSet
{
public :

	TKVSet() ;
	TKVSet(TKVSet &kvset) ;
	~TKVSet() {	Clear(); }

	//Data Accessor Functions
	//Extracts a Key Value into a supplied area of memory.
	CTerror Get( CTsymbol Key, CTuint *Type, CTuint *Size,  void *Value) ;
	CTerror GetSymbol( CTsymbol Key, CTsymbol *pSymbol ) ;
	CTerror GetError( CTsymbol Key, CTerror *pError ) ;
	CTerror GetStatus( CTsymbol Key, CTstatus *pStatus ) ;
	CTerror GetByte( CTsymbol Key, CTbyte *pByte ) ;
	CTerror GetShort( CTsymbol Key, CTshort *pShort ) ;
	CTerror GetUShort( CTsymbol Key, CTushort *pUShort ) ;
	CTerror GetInt( CTsymbol Key, CTint *pInt ) ;
	CTerror GetUInt( CTsymbol Key, CTuint *pUInt ) ;
	CTerror GetBool( CTsymbol Key, CTbool *pBool ) ;
	CTerror GetFloat( CTsymbol Key, CTfloat *pFloat ) ;
	CTerror GetTime( CTsymbol Key, CTtime *pTime ) ;

	CTerror GetString( CTsymbol Key, char **pString ) ;

	CTerror GetSymbolArray( CTsymbol Key, CTsymbol **pSymbol, CTuint *puiElements ) ;
	CTerror GetErrorArray( CTsymbol Key, CTerror **pError, CTuint *puiElements ) ;
	CTerror GetStatusArray( CTsymbol Key, CTstatus **pStatus, CTuint *puiElements ) ;
	CTerror GetByteArray( CTsymbol Key, CTbyte **pByte, CTuint *puiElements ) ;
	CTerror GetShortArray( CTsymbol Key, CTshort **pShort, CTuint *puiElements ) ;
	CTerror GetUShortArray( CTsymbol Key, CTushort **pUShort, CTuint *puiElements ) ;
	CTerror GetIntArray( CTsymbol Key, CTint **pInt, CTuint *puiElements ) ;
	CTerror GetUIntArray( CTsymbol Key, CTuint **pUInt, CTuint *puiElements ) ;
	CTerror GetBoolArray( CTsymbol Key, CTbool **pBool, CTuint *puiElements ) ;
	CTerror GetFloatArray( CTsymbol Key, CTfloat **pFloat, CTuint *puiElements ) ;

	CTerror GetStringArray( CTsymbol Key, char **aString[], CTuint *puiElements ) ;

	/////////////////////////////////////////////////////////////////////
	// Data Depositor Functions
	//	Inserts a Key Value Pair into a Key Value Set.
	CTerror Put( CTsymbol Key, CTuint Type, CTuint Size, void *Value) ;
	CTerror PutError( CTsymbol key, CTerror errValue ) ;
	CTerror PutStatus( CTsymbol key, CTstatus stsValue ) ;
	CTerror PutSymbol( CTsymbol key, CTsymbol symValue ) ;
	CTerror PutByte( CTsymbol key, CTbyte cValue ) ;
	CTerror PutShort( CTsymbol key, CTshort cValue ) ;
	CTerror PutUShort( CTsymbol key, CTushort cValue ) ;
	CTerror PutInt( CTsymbol key, CTint iValue ) ;
	CTerror PutUInt( CTsymbol key, CTuint uiValue ) ;
	CTerror PutFloat( CTsymbol key, CTfloat fValue ) ;
	CTerror PutBool( CTsymbol key, CTbool bValue ) ;
	CTerror PutTime( CTsymbol key, CTtime tValue ) ;

	CTerror PutString( CTsymbol key, char *szString ) ;
	CTerror PutErrorArray( CTsymbol key, CTerror aError[], CTuint  uiElements ) ;
	CTerror PutStatusArray( CTsymbol key, CTstatus aStatus[], CTuint  uiElements  ) ;
	CTerror PutSymbolArray( CTsymbol key, CTsymbol aSymbol[], CTuint  uiElements  ) ;
	CTerror PutByteArray( CTsymbol key, CTbyte aByte[], CTuint  uiElements  ) ;
	CTerror PutShortArray( CTsymbol key, CTshort aShort[], CTuint  uiElements  ) ;
	CTerror PutUShortArray( CTsymbol key, CTushort aUShort[], CTuint  uiElements  ) ;
	CTerror PutIntArray( CTsymbol key, CTint aInt[], CTuint  uiElements  ) ;
	CTerror PutUIntArray( CTsymbol key, CTuint aUInt[], CTuint  uiElements ) ;
	CTerror PutFloatArray( CTsymbol key, CTfloat aFloat[], CTuint  uiElements  ) ;
	CTerror PutBoolArray( CTsymbol key, CTbool aBool[], CTuint  uiElements  ) ;

	CTerror PutStringArray( CTsymbol key, char *aString[], CTuint uiElement ) ;
	///////////////////////////////////////////////////////////////////////

	CTerror GetFirstKey( CTsymbol *Key, CTuint *Type, CTuint *Size,  void *Value) ;
	//Extracts first Key Value into a supplied area of memory.
	CTerror GetNextKey( CTsymbol *Key, CTuint *type,  CTuint *Size, void *Value) ;
	//Traverse through keys in a Key Value Set.
	CTint   GetNumElements();
	//Gets the number of elements in a Key Value Set.
	CTerror Remove(CTsymbol Key);
	//Removes a Key Value Pair from a Key Value Set.
	CTerror Clear();
	//Removes all Key Value Pair from a Key Value Set.

	virtual CTuint  KVSet2String( char *szString, CTuint maxlen=MAXKVSETLEN ) ;
	virtual CTerror String2KVSet( const char *szString, CTuint maxlen=MAXKVSETLEN ) ;
	TKVPair  *GetHead() { return &head; } ;

	TKVSet & operator = ( TKVSet &kvset ) ;

	void   printLog( void ) ;
	CTbool isEmpty()
	{
		return head.next == &tail;
	}

protected:
	CTbool  isMember( CTsymbol Key ) ;

private :
	TKVPair  head, tail, * pCurrent ;
	void     initList() ;
	TKVPair* findPred( CTsymbol Key ) ;
} ;

#define  constTHISVERSION     0x018E

struct CTtranInfo
{
	CTint                   version ;
	CTobj                   sender ;
	CTobj                   receiver ;
	CTsymbol                messageID ;
	CTuint                  tranID ;
	CTstatus                status ;
	CTuint                  qualifier ;
	CTerror                 errorData ;

private:

	TKVSet                 *messageData ;

public:

	CTtranInfo() : version(constTHISVERSION), sender(0),
		receiver(0), messageID(0), tranID(0),
		status(0), qualifier(0), errorData(CT_errorOK)
	{
		messageData = new TKVSet ;
	}

	CTtranInfo(CTobj snd, CTobj rcv, CTsymbol msgid,
		TKVSet *kvset=NULL, CTerror err=CT_errorOK, CTstatus sts=0,
		CTuint tranid=0, CTuint qlfer=0)
	: version(constTHISVERSION), sender(snd), receiver(rcv),
		messageID(msgid),tranID(tranid),
		status(sts), qualifier(qlfer), errorData(err)
	{
		messageData = new TKVSet ;
		if( kvset )
			* messageData = * kvset ;
	}

	CTtranInfo(const CTtranInfo &tranInfo);
	~CTtranInfo()
	{
		delete messageData;
	}

	CTtranInfo & operator = (const CTtranInfo & tranInfo);

	CTint TranInfo2String(char *str, CTint maxlen);
	CTerror String2TranInfo(char *str, CTint len);

	TKVSet& GetMessageData() { return * messageData; }
} ;

const int constSIZEOFINT_2 = sizeof(CTuint) *2 ;
const int constMINTRANINFOSIZE = sizeof(CTtranInfo)-sizeof(TKVSet*) ;
#endif
