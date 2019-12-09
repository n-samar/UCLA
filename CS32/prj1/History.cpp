#include <iostream>

#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols) : m_nRows(nRows), m_nCols(nCols)
{
	for(int i = 0; i < MAXROWS; i++)
		for(int j = 0; j < MAXCOLS; j++)
			m_hist[i][j] = 0;
}
	
bool History::record(int r, int c)
{
	if(r > m_nRows || r < 1
	|| c > m_nCols || c < 1) return false;

	m_hist[r-1][c-1]++;

	return true;
}

void History::display() const
{
	clearScreen();

	for(int i = 0; i < m_nRows; i++)
	{
		for(int j = 0; j < m_nCols; j++)
		{
			if(m_hist[i][j] > 0)
				cout << (char)(64+m_hist[i][j]);
			else
				cout << ".";
		}
		cout<<endl;
	}

	std::cout<<std::endl;
}