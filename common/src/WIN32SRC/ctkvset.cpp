#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cttypes.h"
#include "ctkvset.h"
#include "ctutil.h"


TKVPair::TKVPair( CTsymbol toKey, CTuint toType, CTuint toSize, void *toValue, TKVPair *p )
{
	if( p && toValue && toSize <= MAXPAIRLEN )
	{
		key = toKey ;
		type = toType ;
		size = toSize ;
		value = 0 ;
		if( size ) 
		{
			value = new char[ size ] ;
			if( !value )
				return ;
			memcpy( value, toValue, size ) ;
		}
		
		next = p->next ;
		p->next = this ;
	}
}

TKVPair::TKVPair(CTsymbol toKey, CTuint toType, char *szString[], CTuint eiElements, TKVPair  *prev )
{
    CTuint  len,i;
    char **pArray = ( char **)szString;
    char *q, *buffer ;
	
    if( !szString || !prev || toType != CTkvs_valtypSTRINGARRAY )
		return ;

    len = sizeof(CTuint) + eiElements * sizeof(char *) ;
    for( i=0; i<eiElements; i++ )
		len += ( strlen( szString[i]) + 1 ) ;
    if( len > MAXPAIRLEN )
		return ;
	
    key = toKey ;
    type = toType ;
    size = len ;
	value = new char[ size ] ;
    if( (buffer = (char *)value) == NULL )
		return ;
    memset( value, 0, size ) ;
    CTuint_byte val( eiElements ) ;
    memcpy( buffer, (void *)(val.str), sizeof( CTuint) ) ;
	
    q = buffer + sizeof(CTuint);
    char **ptrptr = ( char **)q ;
    q += eiElements * sizeof( char *) ;
    for( i=0; i<eiElements; i++ ) {
		*ptrptr = q ;
		//strcpy( q, pArray[i]) ;
		if(pArray[i])
		{
			strncpy( q,pArray[i],strlen(pArray[i])) ;
			q += strlen( pArray[i] ) + 1 ;
			ptrptr ++ ;
		}
    }
	
    next = prev->next;
    prev->next = this;
}

TKVPair::~TKVPair()
{
	if( value )
	{
		delete [] value ;
		value = 0;
	}
	value = 0 ;
}

/////////////////////////////////////
TKVSet::TKVSet()
{
	initList() ;
}

TKVSet::TKVSet( TKVSet &kvset ) 
{
    initList() ;
    *this = kvset ;
}

void TKVSet::initList()
{
    head.next = &tail;
    tail.next = &tail;
    pCurrent  = &tail;
}


CTerror TKVSet::Clear( )
{
    pCurrent = head.next;
    while( pCurrent && pCurrent != &tail )
	{
		TKVPair * temp = pCurrent;
		pCurrent = pCurrent->next;
		delete temp;
		temp=0;
	}
    initList();
	
    return CT_errorOK ;
}


CTint TKVSet::GetNumElements()
{
    CTint   count=0 ;
    TKVPair  *current = head.next;
	
    while( current != &tail ) {
		count ++ ;
		current = current->next;
    }
	
    return count ;
}

CTerror TKVSet::Get( CTsymbol Key, CTuint *Type, CTuint *Size,  void *Value)
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent != &tail ) {
		*Size = pCurrent->size ;
		*Type = pCurrent->type ;
		memcpy( Value, pCurrent->value, pCurrent->size ) ;
		return CT_errorOK ;
	}
	
	return  CT_errorKeyNotFound ;
}


CTerror TKVSet::GetSymbol( CTsymbol Key, CTsymbol *pSymbol )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSYMBOL )
		return CT_errorBadKey ;
	
	*pSymbol = *( (CTsymbol *)(pCurrent->value) ) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetError( CTsymbol Key, CTerror *pError )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypERROR )
		return CT_errorBadKey ;
	
	*pError = *( (CTerror *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetStatus( CTsymbol Key, CTstatus *pStatus )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSTATUS )
		return CT_errorBadKey ;
	
	
	if( pCurrent != &tail ) {
		*pStatus = *( (CTstatus *)(pCurrent->value) ) ;
		return CT_errorOK ;
	}
	
	return  CT_errorKeyNotFound ;
}

CTerror TKVSet::GetByte( CTsymbol Key, CTbyte *pByte )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypBYTE )
		return CT_errorBadKey ;
	
	*pByte = *( (CTbyte *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetShort( CTsymbol Key, CTshort *pShort )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSHORT )
		return CT_errorBadKey ;
	
	*pShort = *( (CTshort *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetUShort( CTsymbol Key, CTushort *pUShort )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypUSHORT )
		return CT_errorBadKey ;
	
	*pUShort = *( (CTushort *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetInt( CTsymbol Key, CTint *pInt )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypINT )
		return CT_errorBadKey ;
	
	*pInt = *( (CTint *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetUInt( CTsymbol Key, CTuint *pUInt )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypUINT )
		return CT_errorBadKey ;
	
	*pUInt = *( (CTuint *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetFloat( CTsymbol Key, CTfloat *pFloat )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypFLOAT )
		return CT_errorBadKey ;
	
	*pFloat = *( (CTfloat *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetBool( CTsymbol Key, CTbool *pBool )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypBOOL )
		return CT_errorBadKey ;
	
	*pBool = *( (CTbool *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetTime( CTsymbol Key, CTtime *pTime )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypTIME )
		return CT_errorBadKey ;
	
	*pTime = *( (CTtime *)(pCurrent->value) ) ;
	return CT_errorOK ;
}

//////////////////////////////////////////////////////////

CTerror TKVSet::GetString( CTsymbol Key, char **pString )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSTRING )
		return CT_errorBadKey ;
	
	*pString =  (char *)(pCurrent->value) ;
	return CT_errorOK ;
}

CTerror TKVSet::GetByteArray( CTsymbol Key, CTbyte **pByte, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypBYTEARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTbyte ) ;
	*pByte = (CTbyte *)(pCurrent->value) ;
	
	return CT_errorOK ;
}


CTerror TKVSet::GetShortArray( CTsymbol Key, CTshort **pShort, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSHORTARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTshort ) ;
	*pShort = (CTshort *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetUShortArray( CTsymbol Key, CTushort **pUShort, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypUSHORTARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTushort ) ;
	*pUShort = (CTushort *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetIntArray( CTsymbol Key, CTint **pInt, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypINTARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTint ) ;
	*pInt = (CTint *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetUIntArray( CTsymbol Key, CTuint **pUInt, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypUINTARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTuint ) ;
	*pUInt = (CTuint *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetFloatArray( CTsymbol Key, CTfloat **pFloat, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypFLOATARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTfloat ) ;
	*pFloat = (CTfloat *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetBoolArray( CTsymbol Key, CTbool **pBool, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypBOOLARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTbool ) ;
	*pBool = (CTbool *)(pCurrent->value) ;
	
	return CT_errorOK ;
}


CTerror TKVSet::GetSymbolArray( CTsymbol Key, CTsymbol **pSymbol, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSYMBOLARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTsymbol ) ;
	*pSymbol = (CTsymbol *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetErrorArray( CTsymbol Key, CTerror **pError, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypERRORARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTerror ) ;
	*pError = (CTerror *)(pCurrent->value) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetStatusArray( CTsymbol Key, CTstatus **pStatus, CTuint *puiElements  )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSTATUSARRAY )
		return CT_errorBadKey ;
	
	*puiElements = pCurrent->size / sizeof( CTstatus ) ;
	*pStatus = (CTstatus *)(pCurrent->value) ;
	return CT_errorOK ;
}


CTerror TKVSet::GetStringArray( CTsymbol Key, char **pString[], CTuint *puiElement )
{
	pCurrent = head.next ;
	char *p ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key ) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent == &tail )
		return CT_errorKeyNotFound ;
	
	if( pCurrent->type != CTkvs_valtypSTRINGARRAY )
		return CT_errorBadKey ;
	
	p = ( char *)pCurrent->value ;
	CTuint_byte u( p ) ;
	*puiElement = u.value ;
	if(u.value==0)
		return CT_errorOK;
	p += sizeof( CTuint ) ; 	
	int str_offset=sizeof(char *)*u.value;
	char **ptrptr;
	ptrptr = ( char **)( p ) ;
	CTuint offset=0,cur_offset=0;
	CTint i;	
	for(i=0;i<(CTint)u.value-1;i++)
	{
		cur_offset=ptrptr[i+1] - ptrptr[i];
		offset=cur_offset + offset;
		ptrptr[i]=(char*) (p+str_offset+offset-cur_offset);
	}
	ptrptr[i]=(char*) (p+str_offset+offset);
	*pString=ptrptr;
	
	return CT_errorOK ;
}

/////////////////////////////////////////////////////////////////////
CTerror TKVSet::Put( CTsymbol Key, CTuint Type, CTuint Size, void *Value)
{

    CTerror ret = Remove( Key ) ;
    new TKVPair( Key,Type, Size, Value, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutError( CTsymbol key, CTerror errValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypERROR, sizeof(CTerror), &errValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror  TKVSet::PutSymbol( CTsymbol key, CTsymbol symValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypSYMBOL, sizeof(CTsymbol), &symValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutStatus( CTsymbol key, CTstatus stsValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypSTATUS, sizeof(CTstatus), &stsValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutByte( CTsymbol key, CTbyte cValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypBYTE, sizeof(CTbyte), &cValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}


CTerror TKVSet::PutShort( CTsymbol key, CTshort cValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypSHORT, sizeof(CTshort), &cValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutUShort( CTsymbol key, CTushort cValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypUSHORT, sizeof(CTushort), &cValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutInt( CTsymbol key, CTint iValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypINT, sizeof(CTint), &iValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutUInt( CTsymbol key, CTuint uiValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypUINT, sizeof(CTuint), &uiValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutFloat( CTsymbol key, CTfloat fValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypFLOAT, sizeof(CTfloat), &fValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutBool( CTsymbol key, CTbool bValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypBOOL, sizeof(CTbool), &bValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutTime( CTsymbol key, CTtime tValue )
{
    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypTIME, sizeof(CTtime), &tValue, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
	
}

CTerror TKVSet::PutString( CTsymbol key, char *szString )
{
	if(!szString)
		return CT_errorKeyExists;

    CTerror ret = Remove( key ) ;
    new TKVPair( key, CTkvs_valtypSTRING, strlen(szString)+1, (void *)szString, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutErrorArray( CTsymbol key, CTerror aError[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTerror ) * uiElements ;
    new TKVPair( key, CTkvs_valtypERRORARRAY, Size, (void *)aError, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutStatusArray( CTsymbol key, CTstatus aStatus[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTstatus ) * uiElements ;
    new TKVPair( key, CTkvs_valtypSTATUSARRAY, Size, (void *)aStatus, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutSymbolArray( CTsymbol key, CTsymbol aSymbol[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTsymbol ) * uiElements ;
    new TKVPair( key, CTkvs_valtypSYMBOLARRAY, Size, (void *)aSymbol, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutByteArray( CTsymbol key, CTbyte aByte[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTbyte ) * uiElements ;
    new TKVPair( key, CTkvs_valtypBYTEARRAY, Size, (void *)aByte, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutShortArray( CTsymbol key, CTshort aShort[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTshort ) * uiElements ;
    new TKVPair( key, CTkvs_valtypSHORTARRAY, Size, (void *)aShort, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutUShortArray( CTsymbol key, CTushort aUShort[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTshort ) * uiElements ;
    new TKVPair( key, CTkvs_valtypUSHORTARRAY, Size, (void *)aUShort, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutIntArray( CTsymbol key, CTint aInt[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTint ) * uiElements ;
    new TKVPair( key, CTkvs_valtypINTARRAY, Size, (void *)aInt, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutUIntArray( CTsymbol key, CTuint aUInt[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTuint ) * uiElements ;
    new TKVPair( key, CTkvs_valtypUINTARRAY, Size, (void *)aUInt, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutFloatArray( CTsymbol key, CTfloat aFloat[], CTuint  uiElements )
{
	
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTfloat ) * uiElements ;
    new TKVPair( key, CTkvs_valtypFLOATARRAY, Size, (void *)aFloat, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutBoolArray( CTsymbol key, CTint aBool[], CTuint  uiElements )
{
    CTerror ret = Remove( key ) ;
    CTuint Size = sizeof( CTbool ) * uiElements ;
    new TKVPair( key, CTkvs_valtypBOOLARRAY, Size, (void *)aBool, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}

CTerror TKVSet::PutStringArray( CTsymbol key, char *szString[], CTuint eiElements )
{
	if(!szString)
		return CT_errorInvalidOp;

    CTerror ret = Remove( key ) ;
	
    new TKVPair( key, CTkvs_valtypSTRINGARRAY, szString, eiElements, &head );
	
    if( ret == CT_errorOK )
		return CT_errorKeyExists ;
	
    return CT_errorOK ;
}


/////////////////////////////////////////////////////////////////////

CTerror TKVSet::GetNextKey( CTsymbol *Key, CTuint *Type,  CTuint *Size, void *Value)
{
	
	if( isEmpty() == CT_boolTRUE )
		return CT_errorKeyNotFound ;
	
	pCurrent = pCurrent->next ;
	if( pCurrent == &tail )
		return CT_errorEndOfKey ;
	
	*Key  = pCurrent->key ;
	*Type = pCurrent->type ;
	*Size = pCurrent->size ;
	memcpy( Value, pCurrent->value, *Size ) ;
	
	return CT_errorOK ;
}

CTerror TKVSet::GetFirstKey( CTsymbol *Key, CTuint *Type, CTuint *Size,  void *Value)
{
	
	if( isEmpty() == CT_boolTRUE )
		return CT_errorKeyNotFound ;
	
	pCurrent = head.next ;
	
	*Key  = pCurrent->key ;
	*Type = pCurrent->type ;
	*Size = pCurrent->size ;
	memcpy( Value, pCurrent->value, *Size ) ;
	
	return CT_errorOK ;
}

TKVPair *TKVSet::findPred( CTsymbol Key )
{
    tail.key = Key;
    TKVPair *cursor = &head;
    while( !(Key == cursor->next->key) )
		cursor = cursor->next;
    return cursor;
}

CTbool TKVSet::isMember( CTsymbol Key )
{
	pCurrent = head.next ;
	
	tail.key = Key ;
	while( !(Key == pCurrent->key) )
		pCurrent = pCurrent->next ;
	
	if( pCurrent != &tail )
		return CT_boolTRUE;
	
	return  CT_boolFALSE;
}


CTerror TKVSet::Remove( CTsymbol Key)
{
    TKVPair  *pred = findPred( Key );
    TKVPair  *item = pred->next;
    if( item != &tail )	{
		pred->next = pred->next->next;
		delete item;
		
		return CT_errorOK ;
    }
	
    return CT_errorNotFound ;
}

TKVSet & TKVSet::operator = ( TKVSet &kvset )
{
	CTuint Key, Type, retval, Size ;
	CTbyte buffer[MAXPAIRLEN] ;
	
	if( CT_boolFALSE == isEmpty() )
		Clear() ;
	
	retval = kvset.GetFirstKey( &Key, &Type, &Size, (void *)buffer ) ;
	while( retval == CT_errorOK )
	{
		if( Type == CTkvs_valtypSTRINGARRAY )
		{
			char *p, **pp;
			p = ( char *)buffer ;
			CTuint_byte u( p ) ;
			p += sizeof( CTuint ) ;
			pp = ( char **)( p ) ;
			new TKVPair( Key,Type, pp, u.value, &head );
		}
		else
			new TKVPair( Key,Type, Size, buffer, &head );
		retval = kvset.GetNextKey( &Key, &Type, &Size, (void *)buffer ) ;
	}
	return *this ;
}


CTuint TKVSet::KVSet2String( char *szString, CTuint maxlen )
{
	char Value[MAXPAIRLEN];
	CTuint Key, Size, Type, retval ;
	CTuint count=0, i,element, array_num;
	CTshort length=0;
	
	length = 4 ;  //left 4 byte to fill marker and length
	retval = GetFirstKey( &Key, &Type, &Size, (void *)Value ) ;
	while( retval == CT_errorOK )
	{
		if( length + 3*sizeof(CTuint) >= MAXKVSETLEN )
		{
			count = 0 ;
			break ;
		}
		
		count ++ ;
		CTuint2CTbyte( Key, szString+length ) ;
		length += sizeof( CTuint ) ;
		CTuint2CTbyte( Type, szString+length ) ;
		length += sizeof( CTuint ) ;
		CTuint2CTbyte( Size, szString+length ) ;
		length += sizeof( CTuint ) ;
		
		if( Size > MAXPAIRLEN || length+Size >= MAXKVSETLEN || length +Size > maxlen)
		{
			count = 0 ;
			break ;
		}
		
		switch( Type )
		{
		case CTkvs_valtypNULL :
		case CTkvs_valtypBYTE :
		case CTkvs_valtypBYTEARRAY  :
		case CTkvs_valtypSTRING	:
			memcpy( szString+length, Value, Size ) ;
			break ;
			
		case CTkvs_valtypSHORT :
		case CTkvs_valtypUSHORT :
			CTshort2CTbyte( *((CTshort *)Value), szString+length ) ; 
			break ;
			
		case CTkvs_valtypFLOAT :
			CTfloat2CTbyte( *((CTfloat *)Value), szString+length ) ;
			break ;
			
		case CTkvs_valtypINT :
		case CTkvs_valtypUINT:
		case CTkvs_valtypBOOL:
		case CTkvs_valtypERROR :
		case CTkvs_valtypSTATUS :
		case CTkvs_valtypSYMBOL :
		case CTkvs_valtypTIME  :   
			CTuint2CTbyte( *((CTuint *)Value), szString+length) ;
			break ;
			
		case CTkvs_valtypSHORTARRAY  :
		case CTkvs_valtypUSHORTARRAY  :
			element = Size / sizeof( CTshort ) ;
			for( i=0; i<element; i++ )
				CTshort2CTbyte( *( (CTshort *)Value + i) , szString+length+i*sizeof(CTshort) ) ;
			break ;
			
		case CTkvs_valtypFLOATARRAY :
			element = Size / sizeof( CTfloat ) ;
			for( i=0; i<element; i++ )
				CTfloat2CTbyte( *( (CTfloat *)Value + i), szString+length+i*sizeof(CTfloat) ) ;	
			break ;
			
		case CTkvs_valtypINTARRAY:
		case CTkvs_valtypUINTARRAY :
		case CTkvs_valtypBOOLARRAY :
		case CTkvs_valtypERRORARRAY :
		case CTkvs_valtypSTATUSARRAY :
		case CTkvs_valtypSYMBOLARRAY :
			element = Size / sizeof( CTuint ) ;
			for( i=0; i<element; i++ )
				CTuint2CTbyte( *( (CTuint *)Value + i) , szString+length+i*sizeof(CTuint) ) ;
			break ;
			
		case CTkvs_valtypSTRINGARRAY:
			array_num = *(CTuint *)Value ;
			CTuint2CTbyte( array_num, Value ) ;
			memcpy( szString+length, Value, Size ) ;
			break ;
			
		default :
			count = 0 ;
			retval = CT_errorBadKey ;
			break ;
		}
		length += (short) Size ;
		
		retval = GetNextKey( &Key, &Type, &Size, (void *)Value ) ;
	}
	
	if( count )
	{  //success
		szString[0] = KVSET_START ;
		CTshort2CTbyte( length, szString+1 ) ;
		szString[3] = (char)count;
	}
	else
		length = 0 ;
	
	return length ;
}


CTerror TKVSet::String2KVSet( const char *szString, CTuint maxlen )
{
	char *buf  ;
	CTuint Key, Size, Type, index ;
	CTuint count=0,element, i, j,num_uint, array_num;
	CTshort length=0, num_short ;
	CTfloat num_float ;
	
	if( (CTbyte)szString[0] != (CTbyte)KVSET_START )
		return CT_errorBadKVSet ;
	
	if( isEmpty() == CT_boolFALSE ) {
		Clear() ;
	}
	
	CTbyte2CTshort( szString+1, &length ) ;
	if( length > (short)maxlen )
		return CT_errorBadKVSet ;
	
	count = (CTbyte)szString[3] ;
	index = 4 ;
	
	CTbool retval = CT_boolTRUE ;
	for( i=0; i<count && retval==CT_boolTRUE; i++ ) {
		if( index + 3*sizeof(CTuint) >= maxlen ) {
			retval = CT_boolFALSE ;
			break ;
		}
		
		CTbyte2CTuint( szString+index, &Key ) ;
		index += sizeof( CTuint ) ;
		CTbyte2CTuint( szString+index, &Type ) ;
		index += sizeof( CTuint ) ;
		CTbyte2CTuint( szString+index, &Size ) ;
		index += sizeof( CTuint ) ;
		
		if( Size > MAXPAIRLEN || index+Size >= MAXKVSETLEN || index+Size > maxlen) {
			retval = CT_boolTRUE ;
			break ;
		}
		
		if( Size > 0 ) {
			switch( Type ) {
			case CTkvs_valtypNULL :
			case CTkvs_valtypBYTE :
			case CTkvs_valtypBYTEARRAY :
			case CTkvs_valtypSTRING :
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
			case CTkvs_valtypSHORT  :
			case CTkvs_valtypUSHORT  :
				CTbyte2CTshort( szString+index, &num_short ) ;
				memcpy( (void *)(szString+index), (void *)&num_short, sizeof(CTshort) ) ;
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
				
			case CTkvs_valtypFLOAT :
				CTbyte2CTfloat( szString+index, &num_float ) ;
				memcpy( (void *)(szString+index), (void *)&num_float, sizeof(CTfloat) ) ;
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
			case CTkvs_valtypINT :
			case CTkvs_valtypUINT:
			case CTkvs_valtypBOOL:
			case CTkvs_valtypERROR :
			case CTkvs_valtypSTATUS :
			case CTkvs_valtypSYMBOL :
			case CTkvs_valtypTIME  :   
				CTbyte2CTuint( szString+index, &num_uint ) ;
				memcpy( (void *)(szString+index), (void *)&num_uint, sizeof(CTuint) ) ;
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
				
			case CTkvs_valtypSHORTARRAY  :
			case CTkvs_valtypUSHORTARRAY  :
				element= Size / sizeof(CTshort)  ;	
				for( j=0; j<element; j++ ) {
					CTbyte2CTshort( szString+index+j*sizeof(CTshort), &num_short ) ;
					memcpy( (void *)(szString+index+j*sizeof(CTshort)), (void *)&num_short, sizeof(CTshort) ) ;
				}	
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
			case CTkvs_valtypFLOATARRAY  :
				element= Size / sizeof(CTfloat)  ;	
				for( j=0; j<element; j++ ) {
					CTbyte2CTfloat( szString+index+j*sizeof(CTfloat), &num_float ) ;
					memcpy( (void *)(szString+index+j*sizeof(CTfloat)), (void *)&num_float, sizeof(CTfloat) ) ;
				}	
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
			case CTkvs_valtypINTARRAY :
			case CTkvs_valtypUINTARRAY:        
			case CTkvs_valtypBOOLARRAY:    
			case CTkvs_valtypERRORARRAY :
			case CTkvs_valtypSTATUSARRAY :
			case CTkvs_valtypSYMBOLARRAY :
				element= Size / sizeof(CTuint)  ;
				for( j=0; j<element; j++ ) {
					CTbyte2CTuint( szString+index+j*sizeof(CTuint), &num_uint ) ;
					memcpy( (void *)(szString+index+j*sizeof(CTuint)), (void *)&num_uint, sizeof(CTuint) ) ;
					
				}
				new TKVPair( Key,Type, Size, (void *)(szString+index), &head );
				break ;
				
			case CTkvs_valtypSTRINGARRAY:
				buf = (char *)szString + index ;
				CTbyte2CTuint( buf, &array_num ) ;
				if( array_num > 0 ) {
					char **astr = new char *[array_num] ;
					char *p = buf + sizeof( CTuint ) + array_num * sizeof( char*) ;
					
					for( CTuint i=0; i<array_num ; i++ ) {
						astr[i] = p ;
						p += strlen(astr[i]) + 1 ;
					}
					
					new TKVPair( Key, Type, astr, array_num, &head ) ;
					delete [] astr ;
				}
				break ;
				
			default :
				retval = CT_boolFALSE ;
				break ;
			}
			index += Size ;
		}
  }
  
  if( retval == CT_boolFALSE || index != (unsigned long)length ) {
	  Clear() ;
	  return CT_errorBadKVSet ;
  }
  
  return CT_errorOK ;
}

void TKVSet::printLog( void )
{
    pCurrent = head.next;
    while( pCurrent != &tail ) {
		TKVPair  *temp = pCurrent;
		char    *p = (char *)(temp->value) ;
		printf( "[%p]::key=%x, type=%x, size=%x next=%p",
			temp, temp->key, temp->type, temp->size, temp->next ) ;
		for( CTuint i=0; i<temp->size; i++ )
			printf( " %x", p[i] ) ;
		printf( "\n" ) ;
		pCurrent = pCurrent->next;
    }
}

////////////////////////////////////////////////////
// CTtranInfo member function
// Add by Qingxiao

CTtranInfo::CTtranInfo( const CTtranInfo &tranInfo)
{
	messageData = new TKVSet ;
	*this = tranInfo;
}

CTtranInfo & CTtranInfo::operator = (const CTtranInfo &tranInfo)
{
	version = tranInfo.version;
	sender = tranInfo.sender;
	receiver = tranInfo.receiver;
	messageID = tranInfo.messageID;
	tranID = tranInfo.tranID;
	status = tranInfo.status;
	qualifier = tranInfo.qualifier;
	errorData = tranInfo.errorData;
	
	if (messageData) {
		messageData->Clear();
		if( tranInfo.messageData ) 
			*messageData = *( tranInfo.messageData ) ;
	}
	return *this;
}

CTint CTtranInfo::TranInfo2String(char *str, CTint maxlen)
{
	//assert(maxlen>=constMINTRANINFOSIZE);
	
	char *p = str;
	CTuint2CTbyte( version, p );
	p += sizeof(CTuint);
	CTuint2CTbyte( sender,  p);
	p += sizeof(CTuint);
	CTuint2CTbyte( receiver, p);
	p += sizeof(CTuint);
	CTuint2CTbyte(messageID, p);
	p += sizeof(CTuint);
	CTuint2CTbyte(tranID, p);
	p += sizeof(CTuint);
	CTuint2CTbyte(status, p);
	p += sizeof(CTuint);
	CTuint2CTbyte(qualifier, p);
	p += sizeof(CTuint);
	CTuint2CTbyte(errorData, p);
	p += sizeof(CTuint);
	if (messageData)
		p += messageData->KVSet2String(p);
	
	return (p-str);
}

CTerror CTtranInfo::String2TranInfo(char *str, CTint len)
{
	//assert(len>=constMINTRANINFOSIZE);
	
	char *p = str;
	CTbyte2CTuint( p, (CTuint*)&version);
	p += sizeof(CTuint);
	CTbyte2CTuint( p, &sender);
	p += sizeof(CTuint);
	CTbyte2CTuint( p, &receiver);
	p += sizeof(CTuint);
	CTbyte2CTuint(p, &messageID);
	p += sizeof(CTuint);
	CTbyte2CTuint(p, &tranID);
	p += sizeof(CTuint);
	CTbyte2CTuint(p, &status);
	p += sizeof(CTuint);
	CTbyte2CTuint(p, &qualifier);
	p += sizeof(CTuint);
	CTbyte2CTuint(p, &errorData);
	p += sizeof(CTuint);
	if (len>constMINTRANINFOSIZE)
	{
		if (!messageData)
			messageData = new TKVSet();
		return messageData->String2KVSet(p);
	}
	else
	{
		if (messageData)
			messageData->Clear() ;
		return CT_errorOK;
	}
}
