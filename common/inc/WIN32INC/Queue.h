#ifndef __Queue_H__
#define __Queue_H__

#include "KernelObject.h"

#include <iostream.h>
#include <assert.h>
#include <fstream.h>

const int constDEFAULTQUEUESIZE = 2048;

template <class T> class TQueue
{
private:
	T **data;
	int size,count;

	T **head;
	T **tail;
	T **dataend;

	CTSemaphore  pEmptySema;     //Semaphere show that the queue is empty.
	CTSemaphore  pOccupiedSema;  //Semaphere show that the queue is occupied.
	CTCriticalSection m_lock;	 //CTMutex
	
public:
	TQueue(int aSize = constDEFAULTQUEUESIZE, int isMulti = 1);
	~TQueue();

	int  PutHead(T &newData);
	int  Put(T &newData);
	int  Get(T &buf);
	int	 Remove();
	void RemoveAll() { 	while( GetCount() )  Remove() ; }
	
	int  TryPutHead(T &newData,int waittime=100);
	int  TryPut(T &newData,int waittime=100);
	int  TryGet(T &buf,int waittime=100);
	int	 Peek(T &buf);

	int  GetCount(int flag=0);
	int  IsEmpty() { return (GetCount()==0); };
	int  IsOccupied() 
	{ 
		if(size<2)
			return (GetCount(1)==size); 
		else
			return (GetCount(0)>=(size-1)); 
	};

private:
	int  MyPutHead(T &newData);
	int  MyPut(T &newData);
	int  MyGet(T &buf);
};

//-----------------------------------------------------------------------------
//  Queue:   0    1    2    3    4    5    6    7    8   
//         +----+----+----+----+----+----+----+----+----+
//         |T*  |T*  |    |    |    |    |    |    |    |
//         +----+----+----+----+----+----+----+----+----+
//         ^         ^                   ^         ^
//         data = 0  head = 2            tail = 6  dataend = 8
//
//                   <------- count -----> = 4
//         <---------------- size -----------------> = 8 
//         <-------------- buffer size -----------------> = 8 + 1
//
//-----------------------------------------------------------------------------

template <class T>
TQueue< T >::TQueue(int aSize,int isMulti) :
	pEmptySema(aSize,aSize),
	pOccupiedSema(0,aSize)
{
	size = aSize>0?aSize:1;

	typedef T* TP;
	data = new TP [size+1];
	assert(data);
	for(int i=0;i<=size;i++)
		data[i]=0;
	tail = head = data;
	dataend = data + size; 
	count = 0;
}

template <class T>
TQueue< T >::~TQueue()
{
	for(int i=0;i<=size;i++)
	{
		delete data[i];
		data[i] = 0;
	}
	delete [] data;	
	count = 0;
}

template <class T>
int TQueue< T >::TryPutHead(T &newData,int waittime)
{
	if(IsOccupied()) return -1;

	DWORD err = pEmptySema.Wait(waittime);
	if(err != WAIT_OBJECT_0)
		return -1;

	int ret = MyPutHead(newData);
	if(ret == 0)
		pOccupiedSema.Post();
	else
		pEmptySema.Post();
	return ret;
}

template <class T>
int TQueue< T >::PutHead(T &newData)
{
	if(IsOccupied()) return -1;

	pEmptySema.Wait();
	int ret = MyPutHead(newData);
	if(ret == 0)
		pOccupiedSema.Post();
	else
		pEmptySema.Post();
	return ret;
}

template <class T>
int TQueue< T >::MyPutHead(T &newData)
{
	m_lock.Lock();

	T *tmpdata = new T;
	if(!tmpdata) 
	{
		m_lock.Unlock();
		return -1;
	}
	*tmpdata = newData;
	if(head == data)
		head = data+size-1;
	else 
		head--;
	*head = tmpdata;
	count++;
	m_lock.Unlock();
	return 0;
}

template <class T>
int TQueue< T >::Put(T &newData)
{
	if(IsOccupied()) return -1;   

	pEmptySema.Wait();
	int ret = MyPut(newData);
	if(ret == 0)
		pOccupiedSema.Post();
	else
		pEmptySema.Post();
	return ret;
}

template <class T>
int TQueue< T >::TryPut(T &newData,int waittime)
{
	if(IsOccupied()) return -1;

	DWORD err = pEmptySema.Wait(waittime); 
	if(err != WAIT_OBJECT_0)
		return -1;

	int ret = MyPut(newData);
	if(ret == 0)
		pOccupiedSema.Post();
	else
		pEmptySema.Post();
	return ret;
}

template <class T>
int TQueue< T >::MyPut(T &newData)
{
	m_lock.Lock();

	T *tmpdata = new T;
	if(!tmpdata) 
	{
		m_lock.Unlock();
		return -1;
	}
	*tmpdata = newData;
	*tail = tmpdata;
	tail++;
	if(tail >= dataend)
		tail = data;
	count++;
	m_lock.Unlock();
	return 0 ;
}

template <class T>
int TQueue< T >::Get(T &buf)
{
	pOccupiedSema.Wait();
	int ret = MyGet(buf);
	if(ret==0)
		pEmptySema.Post();
	else
	{
		pOccupiedSema.Post();
		ret=-1;
	}
	return ret;
}

template <class T>
int TQueue< T >::TryGet(T &buf,int waittime)
{
	DWORD err = pOccupiedSema.Wait(waittime);
	if(err != WAIT_OBJECT_0)
		return -1;
	int ret = MyGet(buf);
	if(ret==0)
		pEmptySema.Post();
	else
	{
		pOccupiedSema.Post();
		ret = -2;
	}
	return ret;
}

template <class T>
int TQueue< T >::MyGet(T &buf)
{
	m_lock.Lock();
	if(*head == NULL) 
	{
		m_lock.Unlock();
		return -1;
	}
	buf = **head; // operator =
	T* prev = *head;

	delete prev;
	*head = NULL;
	head++;
	if(head >= dataend)
		head = data;
	if(count>0)
		count--;
	m_lock.Unlock();
	return 0;
}

template <class T>
int TQueue< T >::Peek(T &buf)
{
	int ret = -1;
	m_lock.Lock();
	if(*head) 
	{
		buf = **head; 
		ret = 0;
	}
	m_lock.Unlock();
	return ret;
}

template <class T>
int TQueue< T >::Remove()
{
	if(GetCount())  
	{
		T tmp;
		Get(tmp);
	}
	return 0;
}

template <class T>
int TQueue< T >::GetCount(int flag)
{
	int acount = 0;
	m_lock.Lock();
	if(flag == 0) 
	{
		acount = count;
		m_lock.Unlock();
		return acount;
	}
	//后面部分属于另外一种取法flag=1
	if(tail>=head)
		acount = tail - head;
	else
		acount = size-(head-tail); 
	m_lock.Unlock();
	
	return count;
}

#endif
