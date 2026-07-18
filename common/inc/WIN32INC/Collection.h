//Collection.h
//Change some names of types to avoid violations of some class library.
//Base on zzz TCollect.h and tcollect.h
//Rao Chang Zheng, 1998-08-12
#ifndef __Collection_H__
#define __Collection_H__

#include <assert.h>
#include <string.h>
#include "cttypes.h"

#define MAXCollectionSize  (0x7fffffff-1)

typedef int  (*CTtestFunc)(void *, void *);
typedef void (*CTappFunc) (void *, void *);

class TCTObject
{
public:
    virtual ~TCTObject(){};
    static void destroy( TCTObject* o );
	virtual void shutDown(){};
};

inline void TCTObject::destroy( TCTObject* o )
{
	if( o != 0 )
	{
		o->shutDown();
        delete o;
	}
}

template <class T> class TNSCollection : public TCTObject
{
public:

	TNSCollection( CTint aLimit, CTint aDelta,CTbool aShouldDelete=CT_boolFALSE );
	~TNSCollection();

	virtual void shutDown(); //when shutDown , free all the item

	T*    at( CTint index ); //return NULL when fails , else return the item at index .
	CTint indexOf( T* item ); //return -1 when no this item , else return the index of the item .

	int	  atFree( CTint index ); //return -1 when fail , else return 0 .
	int   atRemove( CTint index ); //return 0 when success ,else fail !
	int   remove( T* item ); //return 0 when success ,else fail !
	void  removeAll();
	int   free( T* item ); //return 0 when success ,else fail !
	void  freeAll();

	int   atInsert( CTint index, T* item ); //return -1 when fail ,else return 0 .
	int   atPut( CTint index, T* item ); //return -1 when fail ,else return 0 .
	CTint insert( T* item ); //return -1 when fail , else return the index of the item just inserted .

	static void error( CTint code, CTint info );

	T*    firstThat( CTtestFunc Test, void* arg );
	T*    lastThat( CTtestFunc Test, void* arg );
	void  forEach( CTappFunc action, void* arg );

	void  pack();
	virtual int setLimit( CTint aLimit );

	CTint getCount() { return count; }

protected:

	TNSCollection();

	T**    items;
	CTint  count;
	CTint  limit;
	CTint  delta;
	CTbool shouldDelete;

private:

	virtual void freeItem( T* item );

};

template <class T> TNSCollection<T>::TNSCollection( CTint  aLimit,
													CTint  aDelta,
													CTbool aShouldDelete )
	: count( 0 ), items( 0 ), limit( 0 ), delta( aDelta ), shouldDelete( aShouldDelete )
{
    setLimit( aLimit );
}

template <class T> TNSCollection<T>::TNSCollection()
	: count( 0 ), items( 0 ), limit( 0 ), delta( 0 ), shouldDelete( CT_boolTRUE )
{
	items = 0;
}

template <class T> TNSCollection<T>::~TNSCollection()
{
	if( items )
	{
		delete[] items;
		items=NULL;
	}
}

template <class T> void TNSCollection<T>::shutDown()
{
	if( shouldDelete == CT_boolTRUE )
		freeAll();
	setLimit( 0 );
	TCTObject::shutDown();
}

template <class T> T* TNSCollection<T>::at( CTint index )
{
	if( index < 0 || index >= count )
	{
		error( 1, index );
		return NULL;
	}
	return items[index];
}

template <class T> int TNSCollection<T>::atRemove( CTint index )
{
	if( index < 0 || index >= count )
	{
		error( 2, index );
		return -1;
	}
	count--;  
	memmove( &items[index], &items[index+1], (count-index)*sizeof(T*) );
	return 0;
}

template <class T> int TNSCollection<T>::atFree( CTint index )
{
	T* item = at( index );
	if(item)
	{
		atRemove( index );
		freeItem( item );
		return 0;
	}
	else
		return -1;
}

template <class T> int TNSCollection<T>::atInsert( CTint index, T* item )
{
	if( !item )
	{
		error( 3, index );
		return -1;
	}

	if( index < 0 )
	{
		error( 4, index );
		return -1;
	}

	if( count == limit )
		if( !delta )
			return -1;//collection full
		else
		{
			if( setLimit(count + delta)==-1 )//setLimit() failed
			return -1;
		}

	memmove( &items[index+1], &items[index], (count-index)*sizeof(T*) );
	count++;
	items[index] = item;
	return 0;
}

template <class T> int TNSCollection<T>::atPut( CTint index, T* item )
{
	if( index < 0 || index >= count )
	{
		error( 5, index );
		return -1;
	}

	items[index] = item;
	return 0;
}

template <class T> int TNSCollection<T>::remove( T* item )
{
	CTint index = indexOf( item ) ;
	if( index < 0 )
		return -1;
	atRemove( index );
	return 0;
}

template <class T> void TNSCollection<T>::removeAll()
{
	count = 0;
}

template <class T> void TNSCollection<T>::error( CTint errCode, CTint index )
{
	printf( "[TNSCollection] errCode=%d, Index=%d\n", errCode, index );
}

template <class T> T* TNSCollection<T>::firstThat( CTtestFunc Test, void* arg )
{
	for( CTint i=0; i<count; i++ )
	{
		if( Test( items[i], arg ) )
			return items[i];
	}
	return 0;
}

template <class T> T* TNSCollection<T>::lastThat( CTtestFunc Test, void* arg )
{
	for( CTint i=count-1; i>=0; i-- )
	{
		if( Test( items[i], arg ) )
			return items[i];
	}
	return 0;
}

template <class T> void TNSCollection<T>::forEach( CTappFunc action, void* arg )
{
	for( CTint i=0; i<count; i++ )
		action( items[i], arg );
}

template <class T> int TNSCollection<T>::free( T* item )
{
	if(remove( item )) //remove failed
		return -1;
	freeItem( item );
	return 0;
}

template <class T> void TNSCollection<T>::freeAll()
{
	//it is a better way than that 'for(CTint i=0;i<count;i++)'.
	//the reason is that if 'freeItem()' is overloaded, and 'count' is changed,
	//the later way will fail -- some items will not be free!
	if( count )
		for( CTint i=count-1; i>=0; i-- )
		{
			T* item = at( i );
			if( item )
				freeItem( item );
		}
	count = 0;
}

template <class T> void TNSCollection<T>::freeItem( T* item )
{
	/*assert( item );
	if( shouldDelete == CT_boolTRUE )
	{
		delete item;
		item=0;
	}
	*/
	if(item)
		if( shouldDelete == CT_boolTRUE )
		{
			delete item;
			item=0;
		}
}

template <class T> CTint TNSCollection<T>::indexOf( T* item )
{
	for( CTint i = 0; i < count; i++ )
		if( item == items[i] )
			return i;
	return -1;
}

template <class T> CTint TNSCollection<T>::insert( T* item )
{
    CTint loc = count;
    if( atInsert( count, item ) )
		return -1;
    return loc;
}

template <class T> void TNSCollection<T>::pack( )
{
	setLimit( count );
}

template <class T> int TNSCollection<T>::setLimit( CTint aLimit )
{
    if( aLimit < count ) aLimit = count;

    if( aLimit >= MAXCollectionSize )
	{
		aLimit = MAXCollectionSize;
		count = MAXCollectionSize;
		error( 6, aLimit );
        return -1;
    }

    if( aLimit != limit )
	{
        T** aItems=0;
        if ( !aLimit )
		{
			aItems = 0;
		}
		else
		{
			try{
				aItems = new T*[aLimit];
			}
			catch(...)
			{}

			if( !aItems )
			{
				error( 7, aLimit );
				return -1;
			}
			if( count )
			{
				//memcpy( aItems, items, count*sizeof(T*) );//comment by 2006.06.13
				memmove( aItems, items, count*sizeof(T*) ); //add 2006.06.13
			}
			if(items)   //add 2006.06.13
			{
				delete[] items;//add 2006.06.13
				items = 0;
			}
			items = aItems;//add 2006.06.13
		}

		/*if(items)   //add 2006.06.13
		{
			delete[] items;//add 2006.06.13
			items = 0;
		}
		items = aItems;//add 2006.06.13
		*/

		//delete items;  // comment 2006.06.13
		//items = aItems; //comment 2006.06.13
		limit = aLimit;
    }
	return 0;
}

#endif
