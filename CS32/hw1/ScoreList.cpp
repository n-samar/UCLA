#include "ScoreList.h"
#include <iostream>


// Create an empty score list.
ScoreList::ScoreList()      
{
}

bool ScoreList::add(unsigned long score)
{
  if(score > -1 && score < 101)
    return m_sequence.insert(score);
  return false;
}
// If the score is valid (a value from 0 to 100) and the score list
// has room for it, add it to the score list and return true.
// Otherwise, leave the score list unchanged and return false.

bool ScoreList::remove(unsigned long score)
{
  return (m_sequence.remove(score) != -1);
}
          // Remove one instance of the specified score from the score list.
          // Return true if a score was removed; otherwise false.

int ScoreList::size() const
{
    return m_sequence.size();
}  // Return the number of scores in the list.

unsigned long ScoreList::minimum() const
{
    if(m_sequence.empty()) {
      std::cerr<< "Here";
      return NO_SCORE;
    }
  m_sequence.dump();
  unsigned long min;
  unsigned long test;
  m_sequence.get(0, min);
  for(int i = 1; i < m_sequence.size(); i++)
  {
    m_sequence.get(i, test);
    if(test < min)
        min = test;
  }
  return min;
}
          // Return the lowest score in the score list.  If the list is
          // empty, return NO_SCORE.

unsigned long ScoreList::maximum() const
{
    if(m_sequence.empty()) {
      std::cerr<< "Here";
      return NO_SCORE;
    }
    m_sequence.dump();
  unsigned long max;
  unsigned long test;
  m_sequence.get(0, max);
  for(int i = 1; i < m_sequence.size(); i++)
  {
    m_sequence.get(i, test);
    if(test > max)
        max = test;
  }
  return max;
}
          // Return the highest score in the score list.  If the list is
          // empty, return NO_SCORE.