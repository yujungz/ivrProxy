// *******************************************************************
// Delaration of a list template
// Template CTList
// CTList.h
// *******************************************************************

#ifndef __TLIST_H
#define __TLIST_H


#include <assert.h>

struct CTNode
{
	CTNode * next;	
	CTNode * prev;
	CTNode() : next(NULL),prev(NULL) { }
	virtual ~CTNode() { next=prev=NULL; }
};

class CTLink
{
public:
	CTNode * head;
	CTNode * tail;

public :
	CTLink() : head(NULL),tail(NULL) { }
	virtual ~CTLink() { Clear(); }

	int Length()
	{
		CTNode * p = head;
		int i = 0;
		for (;p;i++,p=p->next);
		return i;
	}

	void Add(CTNode * p) // Insert to the head of list
	{		
		if( p ) 
		{
			p->next = head;
			p->prev = NULL ;
			if (head)
				head->prev = p;
			if (!tail)
				tail = p; // First node of the list
			head = p;
		}
	}

	void Append(CTNode * p) // Insert to the tail of list
	{		
		if( p ) 
		{
			p->next = NULL;
			p->prev = tail;
			if (!head)
				head = p; // First node of the list
			if (tail)
				tail->next = p;
			tail = p;
		}
	}

	void InsertBefore(CTNode * newOne, CTNode * oldOne) // Insert before the node of the list
	{
		if (newOne && oldOne)
		{
			CTNode * & beforeOldPtrRef = oldOne->prev;
			newOne->next = oldOne;
			newOne->prev = beforeOldPtrRef;
			if (head==oldOne)
			{// Old one is the first node of the list
				head = newOne;
			}
			else
			{
				assert( beforeOldPtrRef );
				beforeOldPtrRef->next = newOne;
			}
			beforeOldPtrRef = newOne;
		}
	}

	void InsertAfter(CTNode * newOne, CTNode * oldOne) // Insert after the node of the list
	{
		if (newOne && oldOne)
		{
			CTNode * & afterOldPtrRef = oldOne->next;
			newOne->next = afterOldPtrRef ;
			newOne->prev = oldOne ;
			if (tail==oldOne)
			{// Old one is the last node of the list
				tail = newOne ;
			}
			else
			{
				assert( afterOldPtrRef ) ;
				afterOldPtrRef->prev = newOne ;
			}
			afterOldPtrRef = newOne ;
		}
	}

	void Dislink(CTNode * p)
	{
		CTNode * q = NULL;
		int isHead = 0;
		int isTail = 0;

		if( !p )
			return ;

		if (p==head) // Head node
		{
			head = p->next;
			if (head)
			{
				head->prev = NULL;
			}
			isHead = 1;
		}

		if (p==tail) // Tail node
		{
			tail = p->prev;
			if (tail)
			{
				tail->next = 0;
			}
			isTail = 1;
		}

		if (!isHead && !isTail) // If the node is not the head or the tail
		{
			q = p->next;
			if( q )
			//if(q == NULL)
				//return ;		
				q->prev = p->prev;

			q = p->prev;
//			if( q == NULL)
//				return ;		
			if( q )
				q->next = p->next;
		}
	}

	void Del(CTNode * p)
	{
		Dislink(p);
		delete p;
	}

	void Clear()
	{
		CTNode *q=NULL, * p = head;
		while (p)
		{
			q = p->next;
//			if( q == NULL)
//				break;
			delete p;//Del(p);
			p = q;

		}
		head = NULL;
		tail = NULL;
	}

};

// class T must derived from class TNode
template <class T>
class CTList : public CTLink
{

public:

	~CTList() { Clear(); }

	T * Tail() { return (T *)tail; }
	T * Head() { return (T *)head; }
	T * Next(T * p) { return (T *)p->next; }
	T * Prev(T * p)	{ return (T *)p->prev; }

	// 0 - false ; 1 - true
	int isEmpty()
	{
		return (NULL==head);
	}
	
	int Length() { return CTLink::Length(); }

	void Dislink(T * p) { CTLink::Dislink( (CTNode *)p) ; }

	void Del(T * p)
	{
		Dislink( p );
		delete p;
	}

	void Add(T * p) { CTLink::Add( (CTNode *)p ); }
	void Append(T * p) { CTLink::Append( (CTNode *)p ); }
	void InsertBefore(T * newOne, T * oldOne)
	{ CTLink::InsertBefore( (CTNode *)newOne, (CTNode *)oldOne ) ; }
	void InsertAfter(CTNode * newOne, CTNode * oldOne)
	{ CTLink::InsertAfter( (CTNode *)newOne, (CTNode *)oldOne ) ; }

	void Clear()
	{
		T * q;
		T * p = (T *)head;

		while (p)
		{
			q = Next(p);
			delete (T *)p;//Del(p);
			p = q;
		}
		head = NULL;
		tail = NULL;
	}
};

#endif
