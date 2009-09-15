
#include "DLList.h"


void
DLList::Node::unlink() throw ()
{
	if (prev != (Node *) 0)
	{
		prev->next = next;
	}
	if (next != (Node *) 0)
	{
		next->prev = prev;
	}
	prev = (Node *) 0;
	next = (Node *) 0;
}


DLList::~DLList() throw ()
{
	clear();
}


void
DLList::clear() throw ()
{
	for (Node *p = head; p != (Node *) 0; )
	{
		Node *n = p->next;

		delete p;
		p = n;
	}
	head = (Node *) 0;
	tail = (Node *) 0;
}


void
DLList::unlink(Node *n) throw ()
{
	if (n != (Node *) 0)
	{
		if (head == n)
		{
			head = n->next;
		}
    
		if (tail == n)
		{
			tail = n->prev;
		}
    	n->unlink();
	}
}


void
DLList::prepend(Node *n) throw ()
{
	if (tail == (Node *) 0)
	{
		tail = n;
	}
	if (head == (Node *) 0)
	{
		head = n;
	}
	else
	{
		insertBefore(n, head);
	}
}


void
DLList::append(Node *n) throw ()
{
	if (head == (Node *) 0)
	{
		head = n;
	}
	if (tail == (Node *) 0)
	{
		tail = n;
	}
	else
	{
		insertAfter(n, tail);
	}
}


void
DLList::insertAfter(Node *nn, Node *after) throw ()
{
	if ((after != (Node *) 0) && (nn != (Node *) 0))
	{
		if (after->next != (Node *) 0)
		{
			(after->next)->prev = nn;
		}
		nn->next = after->next;
		nn->prev = after;
		after->next = nn;
	}
	if (after == tail)
	{
		tail = nn;
	}
}


void
DLList::insertBefore(Node *nn, Node *before) throw ()
{
	if ((before != (Node *) 0) && (nn != (Node *) 0))
	{
		if (before->prev != (Node *)0)
		{
			(before->prev)->next = nn;
		}
		nn->next = before;
		nn->prev = before->prev;
		before->prev = nn;
	}
	if (before == head)
	{
		head = nn;
	}
}


size_t
DLList::length() throw ()
{
	size_t retVal = 0;

	for (Node *p = head; p != (Node *) 0; p = p->next)
	{
		retVal++;
	}

	return retVal;
}
