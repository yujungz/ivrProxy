/////////////////////////////////////////////////////////////////////
// KernelObject.cpp
// Rao Changzheng, 1998-10-29

#include "KernelObject.h"


CTMultiObject::CTMultiObject( int aSize )
	: Handle( 0 ), Size( aSize )
{
	assert( 0<Size && Size<=MAXIMUM_WAIT_OBJECTS );
	Handle = new HANDLE[Size];
	for( int i=0; i<Size; i++ )
		Handle[i] = 0;
	Counter.Set( 0 );
}

CTMultiObject::~CTMultiObject()
{
	for( int i=0; i<Size; i++ )
	{
		::CloseHandle( Handle );
		Handle[i] = 0;
	}
	delete [] Handle;
	Handle = 0;
	Counter.Set( 0 );
}

CTShareMemory::CTShareMemory( char* aName, int aSize )
	: CTInterObject( aName ), Size( aSize )
{
	// Create or open map file
    #ifdef _WIN64
    Handle = CreateFileMapping((HANDLE)(long)0xFFFFFFFF, 0, PAGE_READWRITE, 0, aSize, aName);
    #else
    Handle = CreateFileMapping((HANDLE)0xFFFFFFFF, 0, PAGE_READWRITE, 0, aSize, aName);
    #endif
	
	if ( !Handle )
	{
		cout << "[TShareMemory()] CreateFileMapping() error! Last error:" << GetLastError() << endl;
		exit( -1 );
	}
	Size = aSize; // Backup size

	// See if it is the first one
	if ( ERROR_ALREADY_EXISTS==GetLastError() )
	{
		#ifdef __SHAREMEMORY__
		cout << "[TShareMemory()] Share Memory Already Exists!" << endl;
		#endif
		AlreadyExist = 1;
	}
	else
	{
		#ifdef __SHAREMEMORY__
		cout << "[TShareMemory()] Share Memory Do Not Exist!" << endl;
		#endif
		AlreadyExist = 0;
	}

	// Map view of entire share memory
	Memory = (char*)MapViewOfFile( Handle, FILE_MAP_WRITE, 0, 0, 0 );
	if ( !Memory )
	{
		cout << "[TShareMemory()] MapViewOfFile() last error:"
			 << GetLastError() << endl;
		exit( -1 );
	}
}
