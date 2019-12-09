# include <iostream>
# include <cassert>

#include "Sequence.h"

using namespace std;

// print node values, sanity check
void Sequence::dump() const
{
	// SANITY CHECK
	if(m_head == nullptr)
		return;
	assert(m_head->m_prev == nullptr);
	assert(m_tail->m_next == nullptr);
	int count = 0;
	// cerr << "Dumping:"<< endl;
	for(Node* p = m_head; p != nullptr; p = p->m_next)
	{
		assert(p == m_tail || p->m_next->m_prev == p);	// Check prev pointer
		assert(p == m_head || p->m_prev->m_next == p);	// Check next pointer
		// cerr << count << " : " << p->m_data << endl;
		count++;
	}
}

// Constructor - create empty linked list
Sequence::Sequence()
{
	m_head = nullptr;
	m_tail = nullptr;
	dump();
}

// Destructor
Sequence::~Sequence()
{
	// delete nodes without loosing next
	while(m_head != nullptr)
	{
		Node* n = m_head->m_next;
		delete m_head;
		m_head = n;
	}
}

// Copy constructor
Sequence::Sequence(const Sequence& other)
{
	// if other is empty, set this to empty
	if(other.m_head == nullptr)
	{
		this->m_head = nullptr;
		this->m_tail = nullptr;
		return;
	}

	this->m_head = new Sequence::Node;		// initialize head
	this->m_head->m_prev = nullptr;			// prev of head is nullptr
	this->m_tail = m_head;					// head == tail
	this->m_tail->m_next = nullptr;			// next of tail is nullptr
	Node* newPtr = this->m_head;			// new Sequence pointer

	// iterate and equate appropriately
	for(Node* p = other.m_head; p != nullptr && newPtr != nullptr; p = p->m_next)
	{
		// do not create new Node in next if it should be a nullptr
		if(p->m_next == nullptr)
		{	
			newPtr->m_data = p->m_data;
			newPtr->m_next = nullptr;
			m_tail = newPtr;
		}
		else
		{
			newPtr->m_next = new Sequence::Node;
			newPtr->m_data = p->m_data;
			(newPtr->m_next)->m_prev = newPtr;
		}
		newPtr = newPtr->m_next;
	}
	dump();
}

// Assignment operator
Sequence& Sequence::operator=(const Sequence& other)
{
	Sequence temp = other;	// Copy constructor call!
	this->swap(temp);
	return *this;
}


bool Sequence::empty() const
{
	if(m_head == nullptr) 
		return true;
	return false;
}


int Sequence::size() const
{
	int count = 0;

	// iterate to nullptr
	for(Node* p = this->m_head; p != nullptr; p = p->m_next)
		count++;
	dump();
	return count;
}

// CHECKED for middle, last, greater than size
bool Sequence::insert(int pos, const ItemType& value)
{
	int count = 0;
	Node* p;
	// go to one node at pos
	for(p = m_head; count < pos && p != nullptr; p = p->m_next)
		count++;

	// if pos is valid but strictly less than size of list
	if(p != nullptr && pos >= 0)
	{
		// define new node values
		Node* n = new Sequence::Node;
		n->m_data = value;
		n->m_next = p;
		n->m_prev = p->m_prev;

		// Take care of surroundings
		Node* before = p->m_prev;

		if(before != nullptr)
			before->m_next = n;

		p->m_prev = n;
		if(n->m_prev == nullptr)
			m_head = n;
		dump();
		return true; // everything works
	}
	// if pos is valid and equal to the size of list
	if(count == pos)
	{
		Node* n = new Node;
		n->m_data = value;
		n->m_next = nullptr;
		n->m_prev = m_tail;
		if(m_tail != nullptr)
			m_tail->m_next = n;
		m_tail = n;
		if(m_head == nullptr)		// Watch out for size == 0!
			m_head = n;
		dump();
		return true;
	}
	dump();
	return false;	// pos is not valid
}


int Sequence::insert(const ItemType& value)
{
	int count = 0;
	Node* p;
	// go until you get to pos element of linked list
	for(p = m_head; p != nullptr && value > p->m_data; p = p->m_next)
		count++;

	return insert(count, value)?count:-1;	// return count if element can be 
											// inserted
}

bool Sequence::erase(int pos)
{
	int count = 0;
	Node* p;
	// go until you get to pos element of linked list
	for(p = m_head; p != nullptr && count < pos; p = p->m_next)
		count++;

	// invalid pos
	if(count != pos || p == nullptr)
		return false;

	// pos == 0
	if(p->m_prev == nullptr)
	{
		m_head = m_head->m_next;
		if(m_head != nullptr)
			m_head->m_prev = nullptr;

		if(m_head == nullptr)
			m_tail = nullptr;
		delete p;
		dump();
		return true;
	}

	// pos == size
	if(p->m_next == nullptr)
	{	
		m_tail = m_tail->m_prev;
		m_tail->m_next = nullptr;
		delete p;
		dump();
		return true;
	}

	// pos somewhere in the middle

	Node* before = p->m_prev;
	Node* after = p->m_next;

	before->m_next = after;
	after->m_prev = before;
	delete p;
	dump();
	return true;
}


int Sequence::remove(const ItemType& value)
{
		int count = 0;
		int pos;	
		while((pos = this->find(value)) != -1)
		{
			erase(pos);	// try to find value, try and erase it
			count++;
		}
		return count;
}


bool Sequence::get(int pos, ItemType& value) const
{
	int count = 0;
	Node* p;
	// go until you get to pos element of linked list
	for(p = m_head; p != nullptr && count < pos; p = p->m_next)
		count++;

	// invalid pos
	if(count != pos || p == nullptr)
		return false;
	
	value = p->m_data;
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	int count = 0;
	Node* p;
	// go until you get to pos element of linked list
	for(p = m_head; p != nullptr && count < pos; p = p->m_next)
		count++;

	// invalid pos
	if(count != pos || p == nullptr)
		return false;
	
	p->m_data = value;
	dump();
	return true;
}

int Sequence::find(const ItemType& value) const
{
	int count = 0;
	ItemType val;

	// search through list and return if you find value
	for(count = 0; this->get(count, val); count++)
		if(val == value)
			return count;
	return -1;
}


void Sequence::swap(Sequence& other)
{
	// Swap heads
	Node* temp = this->m_head;
	this->m_head = other.m_head;
	other.m_head = temp;

	// Swap tails
	temp = this->m_tail;
	this->m_tail = other.m_tail;
	other.m_tail = temp;

	dump();
}


int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	int count = 0;
	ItemType p;
	seq1.get(0, p);

	// If second is empty
	if(seq2.size() == 0)
		return -1;

	// go through every element of seq1 and try and see 
	// if k-subsequence starting at that point is equal to seq2
	for(int i = 0; seq1.get(i, p); i++)
	{
		ItemType temp = p;
		ItemType p2;
		seq2.get(0, p2);
		int j;
		// see if all corresponding elements are equal...
		for(j = 0; seq1.get(i+j, temp) && seq2.get(j, p2) && temp == p2; j++)
			;
		// ...if they are
		if(j == seq2.size())
			return i;
	}
	return -1;
}


void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{

	result = seq1;
	ItemType p;

	// Tuck in elements of seq2 between elements of seq1
	for(int count = 0; count < seq1.size(); count++)
	{
		if(seq2.get(count, p))
			result.insert(2*count+1, p);
	}
	// append any extra seq2 elements to the end of result
	for(int count = seq1.size(); count < seq2.size(); count++)
	{
		if(seq2.get(count, p))
			result.insert(count+seq1.size(), p);
	}
	result.dump();
}


