
#include <iostream>

#include "newSequence.h"

using namespace std;

Sequence::Sequence(int capacity)    // Create an empty sequence (i.e., one whose size() is 0).
{
  if(capacity < 0)
    exit(1);
  m_capacity = capacity;
  m_sequence = new ItemType[m_capacity];
  m_size = 0;
}

Sequence::Sequence(const Sequence &other)
{
  this->m_capacity = other.m_capacity;
  this->m_size     = other.m_size;
  this->m_sequence = new ItemType[m_capacity];
  
  for(int i = 0; i < m_size; i++)
      this->m_sequence[i] = other.m_sequence[i];
}

Sequence::~Sequence()
{
  delete [] m_sequence;
}

bool Sequence::empty()  // Return true if the sequence is empty, otherwise false.
{
  return (m_size == 0);
}

int Sequence::size()    // Return the number of items in the sequence.
{
  return m_size;
}

void Sequence::dump() const
{
    cerr << "Printing: " << endl;
    cerr << "m_capacity: " << m_capacity << endl;
    cerr << "m_size: " << m_size << endl;
    
  for(int i = 0; i < m_size; i++)
  {
    cerr << i << " -> " << 
       m_sequence[i] << endl;
  }

  cerr << endl;
}

bool Sequence::insert(int pos, const ItemType& value)
{
  if(pos > this->size() || m_size+1 > m_capacity)
    return false;
  
  for(int i = pos; i < m_size; i++)
  {
    m_sequence[i + 1] = m_sequence[i];
  }

  m_sequence[pos] = value;

  m_size++;
  return true;
}
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return true if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// (e.g., because it's implemented using a fixed-size array) and is
// full.)  Otherwise, leave the sequence unchanged and return false.
// Notice that if pos is equal to size(), the value is inserted at the
// end.

int Sequence::insert(const ItemType& value)
{
  int i = 0;
  
  while(i < m_size && m_sequence[i] < value)
    i++;

  if(m_capacity > m_size)
    {
  for(int j = i; j < m_size; j++)
  {
    m_sequence[j+1] = m_sequence[j];
  }

  m_sequence[i] = value;
  m_size++;
  return i;
    }
  return -1;
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).
     
bool Sequence::erase(int pos)
{
  if(pos > -1 && pos < m_size)
  {
    for(int i = pos; i < m_size - 1; i++)
      m_sequence[i] = m_sequence[i + 1];
  
    m_size--;
    return true;
  }
  return false;
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed this item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.
     
int Sequence::remove(const ItemType& value)
{
  int count = 0;
  for(int i = 0; i < m_size; i++)
    {
      if(m_sequence[i] == value)
      {
	       erase(i);
         count++;
      }
    }

    return count;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value)
{
  if(pos > -1 && pos < m_size)
    {
      value = m_sequence[pos];
      return true;
    }
  return false;
}
// If 0 <= pos < size(), copy into value the item at position pos
// in the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
  if(pos > -1 && pos < m_size)
    {
      m_sequence[pos] = value;
      return true;
    }
  return false;
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value)
{
  for(int i = 0; i < m_size; i++)
    {
      if(m_sequence[i] == value)
	return i;
    }
  return -1;
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.


void Sequence::swap(Sequence& other)
{
  Sequence temp = *this;
  *this = other;
  other = temp;
}
Sequence& Sequence::operator=(const Sequence& other)
    {
      if(this != &other)
      {
      // delete  this sequence
      delete [] this->m_sequence;

      // reinitialize this sequence to be of size of other sequence
      this->m_sequence = new ItemType[other.m_capacity];

      // set elemets of this sequence equal to those of the other sequence
      for(int i = 0; i < other.m_size; i++)
         this->m_sequence[i] = other.m_sequence[i];

      this->m_capacity = other.m_capacity;
      this->m_size     = other.m_size;
    }
      return *this;
    }
  
// Exchange the contents of this sequence with the other one.