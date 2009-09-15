
#ifndef DLLIST_H
#define DLLIST_H 1

#include <sys/types.h>

/**
A very simple doubly-linked list class.  You must subclass DLLList::Node in
order to do anything interesting.
*/


class DLList {
public:
	class Node {
	friend class DLList;
	public:
		inline explicit Node() throw ();
		inline explicit Node(Node *prev, Node *next) throw ();
		inline virtual ~Node() throw ();

	private:
		// These two are pretty dangerous.  Don't know if they should even exist.
		//
		inline Node(const Node &n) throw ();
		inline virtual Node &operator=(const Node &) throw ();

	public:
		void unlink() throw ();

		Node *prev;
		Node *next;
	};

	inline DLList() throw ();
	virtual ~DLList() throw ();

	void clear() throw ();
	size_t length() throw ();

	// Remove the node from the linked list.  The node is not deleted.
	//
	virtual void unlink(Node *) throw ();

	// Make the node the last one in the list.  Assumes it is not in any list.
	//
	virtual void append(Node *) throw ();

	// Make the node the first one in the list.  Assumes it is not in any list.
	//
	virtual void prepend(Node *) throw ();

	// Add newNode appear after the node.  Assumes it is not in any list.
	//
	virtual void insertAfter(Node *newNode, Node *) throw ();

	// Add newNode appear before the node.  Assumes it is not in any list.
	//
	virtual void insertBefore(Node *newNode, Node *) throw ();

	Node *head;
	Node *tail;

private:

};


DLList::Node::Node() throw ()
	: prev((Node *) 0), next((Node *) 0)
{
};

DLList::Node::Node(Node *p, Node *n) throw ()
	: prev(p), next(n)
{
};


DLList::Node::Node(const Node &n) throw ()
	: prev(n.prev), next(n.next)
{
};


DLList::Node::~Node() throw ()
{
	unlink();
};


DLList::Node &
DLList::Node::operator=(const Node &n) throw ()
{
	next = n.next;
	prev = n.prev;

	return *this;
};


DLList::DLList() throw ()
	: head((Node *) 0), tail((Node *) 0)
{
};

#endif // DLLIST_H
