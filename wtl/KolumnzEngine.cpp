//
//  Copyright (C) 2005 - William Bell 
//
//  This file is part of Kolumnz
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//  Coded by William Bell
//  email: william.bell@absamail.co.za
//  Home Page: http://myweb.absamail.co.za/william.bell/
//
//// KolumnzEngine.cpp: implementation of the CKolumnzEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "KolumnzEngine.h"
//#include "stp_Debug.h"

//Debug deb("col.txt");

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C"
{
	long random(void);
	void srandom(unsigned long x);
}

//static UINT speedInt[]={1375, 1375, 1250, 1125, 875, 750, 625, 375, 250, 125 };
//static UINT speedInt[] = {615, 600, 540, 465, 390, 315, 240,  165,    90, 75 };
//static int  levelInt[] = {150, 300, 450, 600, 750, 900, 1050, 1200, 1350, 99999}; 
static int  levelInt[] = {40, 80, 120, 160, 200, 240, 280, 320, 360, 99999}; 

DWORD WINAPI KolumnzThread(LPVOID pData)
{
	DWORD dwMilliseconds = 0;   

	CKolumnzEngine* pT;
	pT = (CKolumnzEngine*) pData;

	WaitForSingleObject(pT->m_hEventRun, INFINITE);
	// loop but check for kill notification
	while (WaitForSingleObject(pT->m_hEventKill, 0) == WAIT_TIMEOUT)
	{
		WaitForSingleObject(pT->m_hEventRun, INFINITE);
		WaitForSingleObject(pT->m_hEventStep, dwMilliseconds);
		ATLTRACE("Step:%d\n", dwMilliseconds);
		pT->Step();
//		dwMilliseconds = pT->m_StepDuration - duration;
		dwMilliseconds = pT->m_StepDuration;
//		Sleep(pT->m_StepDuration);
	}
	SetEvent(pT->m_hEventDead);
	ATLTRACE("Exiting the thread\n");
	return 1;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolumnzEngine::CKolumnzEngine()
{
	m_hEventRun = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventStep = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_Score = 0;
	m_TotalBlocksSunk = 0;
	m_Level = 0;
	m_StepDuration = 0;
	m_GameOver = true;
	m_Preview = 0;
	memset(m_Grid, 0, sizeof(m_Grid));
	DWORD ThreadId;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, KolumnzThread, this, 0, &ThreadId);
}

CKolumnzEngine::~CKolumnzEngine()
{
	SetEvent(m_hEventKill);
	SetEvent(m_hEventRun);
	WaitForSingleObject(m_hEventDead, m_StepDuration+500);
	Sleep(200);			//If this is not here there are memory leaks???
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
	CloseHandle(m_hEventRun);
	CloseHandle(m_hEventStep);
}

void CKolumnzEngine::Create(HWND wnd,
						    UINT msgId,
						    int base,  
							int height,
							int startingLevel,
							int startingRows,
							bool preview,
							int totalColors,
							int pieceLength,
							int lastLevel)
{
//	m_cs.Lock();
	m_MoveCount = 0;
	m_DropCount = 0;
	m_FullRowCount = 0;
	m_FallingRow = -1;
	m_FallingCol = 0;
	m_FallingPiece = 0;
	m_Score = 0;
	m_TotalBlocksSunk = 0;
	m_GameOver = false;
	m_Paused = false;
	
	m_Parent = wnd;
	m_MessageId = msgId;
	m_Base = base;
	m_Height = height;
	m_Level = startingLevel;
	m_StepDuration = 640-55*m_Level;	// speedInt[m_Level-1];
	m_PiecePreview = preview;
	m_TotalColors = totalColors;
	m_PieceLength = pieceLength;
	m_LastLevel = lastLevel;

	memset(m_Grid, 0, sizeof(m_Grid));

	AddRows(startingRows);

	srandom((unsigned)GetTickCount());
	m_Preview = MakePiece();
	m_Center = m_Base/2;
//	m_cs.Unlock();
}

bool CKolumnzEngine::Left()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position

			if (!PlacePiece(m_FallingRow, m_FallingCol-1))	// try to place in new position
			{		
				PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
				m_cs.Unlock();
				return false;
			}
			m_MoveCount++;
			m_FallingCol--;
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 0, 0);
			return true;
		}
	}
	return false;
}

bool CKolumnzEngine::Right()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position

			if (!PlacePiece(m_FallingRow, m_FallingCol+1))	// try to place in new position
			{		
				PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
				m_cs.Unlock();
				return false;
			}
			m_MoveCount++;
			m_FallingCol++;
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 0, 0);
			return true;
		}
	}
	return false;
}

bool CKolumnzEngine::Rotate()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_FallingPiece = m_FallingPiece >> 4 | m_FallingPiece << 4*(m_PieceLength-1);
			m_cs.Lock();
			for (int r=0; r<m_PieceLength; r++)
			{
				m_Grid[r+m_FallingRow][m_FallingCol] = (char)((m_FallingPiece>>(28-4*r))%16);
			}
			m_cs.Unlock();
			m_MoveCount++;
			::PostMessage(m_Parent, m_MessageId, 0, 0);
			return true;
		}
	}
	return false;
}

/*	m_DropCount = 0;
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_cs.Lock();
			while (m_FallingRow >= 0) 
			{
				SetEvent(m_hEventStep);
				if (MovePieceDown()) m_DropCount++;
			}
			ClearFullRows();
			CalculateScore();
			m_cs.Unlock();
			::PostMessage(m_Parent, m_MessageId, 3, 0);
		}
	}
	return m_DropCount;
*/

bool CKolumnzEngine::Drop()
{
	m_DropCount = 0;
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			m_DropCount = 1;
			SetEvent(m_hEventStep);
		}
	}
	return true;
}

bool CKolumnzEngine::ForceStepNow()
{
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		if (m_FallingRow >= 0)
		{
			SetEvent(m_hEventStep);
		}
	}
	return true;
}

int CKolumnzEngine::Step()
{
	static bool cascadeCheck = false;
	int rt = 1;		// 0=game over, 1=0K, 2=new peice, 3=game over
	if (WaitForSingleObject(m_hEventRun, 0) == WAIT_OBJECT_0)
	{
		m_cs.Lock();
		if (cascadeCheck)
		{
			cascadeCheck = ClearCompletePatterns();
			CalculateScore();
		}
		else
		{
			if (m_FallingRow == -1) 
			{
				rt = 2;
				m_FallingCol = m_Center;
				m_FallingPiece = m_Preview;
				m_Preview = MakePiece();
				::PostMessage(m_Parent, m_MessageId, 3, 0);
				m_FallingRow = 0;
				if (!PlacePiece(m_FallingRow, m_FallingCol) || m_Level == m_LastLevel+1)	// try to place
				{
					ResetEvent(m_hEventRun);
					m_GameOver = true;
					rt = 0;		// game over
				}
			}
			else
			{
				if (!MovePieceDown())
				{
					m_StepDuration = 640-55*m_Level;	// speedInt[m_Level-1];
					cascadeCheck = ClearCompletePatterns();
					CalculateScore();
				}
			}
		}
//		Sleep(10000);
		m_cs.Unlock();
		if (rt == 0)	// game over
		{
			::PostMessage(m_Parent, m_MessageId, 1, 0);
		}
		else if (m_DropCount == 0)
		{
			if (rt == 1)
			{
				if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEventStep, 0))
				{
					ResetEvent(m_hEventStep);
				}
				::PostMessage(m_Parent, m_MessageId, 0, 0);
			}
			else // (rt==2)	// new peice
			{
				::PostMessage(m_Parent, m_MessageId, 3, 0);
			}
		}
		else
		{
			m_DropCount++;
		}
	}
	return rt;
}

bool CKolumnzEngine::ClearCompletePatterns()
{
	bool rtc;
	static int r;
	static int c;
	static int m;
	static int count;

	static int cc;
	static int rr;
	static int fullRow;
	rtc = false;
	
	m_cs.Lock();
//	memcpy(m_DebugGrid, m_Grid, sizeof(m_Grid));	// debugging
	// find and mark horizontal patterns
	unsigned char prevChar;
	for (r=0; r<m_Height; r++)
	{
		prevChar = 0xff;
		for (c=0; c<m_Base; c++)
		{
			if (prevChar == m_Grid[r][c])
			{
				count++;
			}
			else
			{
				count = 1;
			}
			if (prevChar)
			{
				if (count == 3)
				{
					for (m=0; m<count; m++)
					{
						m_Grid[r][c-m] = m_Grid[r][c-m] | 0x80;
					}
				}
				else if (count > 3)
				{
					m_Grid[r][c] = m_Grid[r][c] | 0x80;
				}
			}
			prevChar = m_Grid[r][c]&0x7f;
		}
	}
	// find and mark vertical patterns
	for (c=0; c<m_Base; c++)
	{
		prevChar = 0xff;
		for (r=0; r<m_Height; r++)
		{
			if (prevChar == (m_Grid[r][c]&0x7f))
			{
				count++;
			}
			else
			{
				count = 1;
			}
			if (prevChar)
			{
				if (count == 3)
				{
					for (m=0; m<count; m++)
					{
						m_Grid[r-m][c] = m_Grid[r-m][c] | 0x80;
					}
				}
				else if (count > 3)
				{
					m_Grid[r][c] = m_Grid[r][c] | 0x80;
				}
			}
			prevChar = m_Grid[r][c]&0x7f;
		}
	}
	// find and mark right diagonal patterns
	int sc, sr; // start column, start row 
	sr = 0;
	sc = 0;
	while (sr<m_Height && sc<m_Base)
	{
		prevChar = 0xff;
		for (r=sr, c=sc; r>=0 || c<m_Base; r--, c++)
		{
			if (prevChar == (m_Grid[r][c]&0x7f))
			{
				count++;
			}
			else
			{
				count = 1;
			}
			if (prevChar)
			{
				if (count == 3)
				{
					for (m=0; m<count; m++)
					{
						m_Grid[r+m][c-m] = m_Grid[r+m][c-m] | 0x80;
					}
				}
				else if (count > 3)
				{
					m_Grid[r][c] = m_Grid[r][c] | 0x80;
				}
			}
			prevChar = m_Grid[r][c]&0x7f;
		}
		if (sr < m_Height-1)
		{
			sr++;
		}
		else
		{
			sc++;
		}
	}
	// find and mark left diagonal patterns
//	int sc, sr; // start column, start row 
	sr = 0;
	sc = m_Base-1;
	while (sr<m_Height && sc>=0)
	{
		prevChar = 0xff;
		for (r=sr, c=sc; r>=0 || c>=0; r--, c--)
		{
			if (prevChar == (m_Grid[r][c]&0x7f))
			{
				count++;
			}
			else
			{
				count = 1;
			}
			if (prevChar)
			{
				if (count == 3)
				{
					for (m=0; m<count; m++)
					{
						m_Grid[r+m][c+m] = m_Grid[r+m][c+m] | 0x80;
					}
				}
				else if (count > 3)
				{
					m_Grid[r][c] = m_Grid[r][c] | 0x80;
				}
			}
			prevChar = m_Grid[r][c]&0x7f;
		}
		if (sr < m_Height-1)
		{
			sr++;
		}
		else
		{
			sc--;
		}
	}
	// get total number of marked blocks
	int blocks = 0;
	for (c=0; c<m_Base; c++)
	{
		for (r=0; r<m_Height; r++)
		{
			if ((m_Grid[r][c] & 0x80) == 0x80) 
			{
				blocks++;
				rtc = true;
				m_TotalBlocksSunk++;
			}
		}
	}
	if (blocks > 0)
	{
		m_FullRowCount = blocks-2;
	}
	// remove marked blocks
	for (c=0; c<m_Base; c++)
	{
		for (r=0; r<m_Height; r++)
		{
			if ((m_Grid[r][c] & 0x80) == 0x80) 
			{
				for (rr=r; rr>0; rr--)
				{
					m_Grid[rr][c] = m_Grid[rr-1][c];
				}
				m_Grid[0][c] = 0;
			}
		}
	}
/*	if (rtc)	// debugging
	{
		for (r=0; r<m_Height; r++)
		{
			for (c=0; c<m_Base; c++)
			{
				deb << int(m_DebugGrid[r][c]);
			}
			deb << "    ";
			for (c=0; c<m_Base; c++)
			{
				deb << int(m_Grid[r][c]);
			}
			deb << endl;
		}
		deb << endl;
	}
*/	m_cs.Unlock();

	// call this routine again until no marked blocks are found

	// check for new level
	if (m_TotalBlocksSunk > levelInt[m_Level-1])
	{
		m_StepDuration = 640-55*m_Level;	// speedInt[m_Level-1];
		m_Level++;
		::PostMessage(m_Parent, m_MessageId, 2, 0);
	}
	else if (m_FullRowCount > 0)
	{
		::PostMessage(m_Parent, m_MessageId, 4, m_FullRowCount);
	}
	return rtc;
}

void CKolumnzEngine::SetPreview(bool prev)
{
	m_PiecePreview = prev;
}

bool CKolumnzEngine::MovePieceDown()
{
	m_cs.Lock();
	RemovePiece(m_FallingRow, m_FallingCol);	// remove from old position

	if (!PlacePiece(m_FallingRow+1, m_FallingCol))	// try to place in new position
	{		
		PlacePiece(m_FallingRow, m_FallingCol);	// else restore old position
		m_FallingRow = -1;
		m_cs.Unlock();
		return false;
	}
	m_FallingRow = (m_FallingRow+1)%m_Height;
	m_cs.Unlock();
	return true;
}

void CKolumnzEngine::RemovePiece(int row, int col)
{
	static int r;
	static unsigned long mask;

	m_cs.Lock();
	for (r=0; r<m_PieceLength; r++)
	{
		m_Grid[r+row][col] = 0;
	}
	m_cs.Unlock();
}

bool CKolumnzEngine::PlacePiece(int row, int col)
{
	static int r;

	// has the peice hit the bottom?
	if (row+m_PieceLength>m_Height)
	{
		return false;
	}

	// has the piece hit left wall?
	if (col < 0)
	{
		return false;
	}

	// has the peice hit right wall?
	if (col >= m_Base)
	{
		return false;
	}

	// has the peice hit another peice?
	m_cs.Lock();
	for (r=0; r<m_PieceLength; r++)
	{
		if (m_Grid[r+row][col])
		{
			m_cs.Unlock();
			return false;
		}
	}

	// move the peice
	for (r=0; r<m_PieceLength; r++)
	{
		m_Grid[r+row][col] = (char)((m_FallingPiece>>(28-4*r))%16);
	}
	m_cs.Unlock();
	return true;
}

void CKolumnzEngine::CalculateScore()
{
	static long score;

	score = m_DropCount+m_Level*6-(2*m_Level*m_PiecePreview)-m_MoveCount;
	if (score < 6 && !m_FullRowCount)
	{
		if (m_Level != 1)
			score = 7;
		else 
			score = 6;
	}
	if (score < 0 && m_FullRowCount)
		score = 0;

	if (m_FullRowCount>0)
//		score += (unsigned long)(pow(2, (m_FullRowCount-1))*10+.5);
		m_FullRowCount += 2;
		score += (unsigned long)(m_FullRowCount/2.*(44.44444+(m_FullRowCount-1.)*11.111111)+.5);
	m_Score += score;
	m_DropCount = m_MoveCount = m_FullRowCount = 0;


}

// Access Routines
unsigned long CKolumnzEngine::GetPreview()
{
	if (m_PiecePreview)
	{
		return m_Preview;
	}
	return 0;
}

void CKolumnzEngine::GetGrid(char grid[32][32])
{
	m_cs.Lock();
	memcpy(grid, m_Grid, sizeof(m_Grid));
	m_cs.Unlock();
}

bool CKolumnzEngine::Start()
{
	if (!m_GameOver)
	{
		SetEvent(m_hEventRun);
		m_Paused = false;
	}
	::PostMessage(m_Parent, m_MessageId, 3, 0);
	return true;
}

bool CKolumnzEngine::Pause()
{
	ResetEvent(m_hEventRun);
	m_Paused = true;
	return m_Paused;
}

bool CKolumnzEngine::AddRows(int nRows)
{
	m_cs.Lock();
	for (int i=0; i<nRows; i++)
	{
		// is the top row free?
		if (m_FallingRow >= 0)
			RemovePiece(m_FallingRow, m_FallingCol);
		for (int c=0; c<m_Base; c++)
		{
			if (m_Grid[0][c]) 
			{
				m_cs.Unlock();
				return false;
			}
		}
		// move the whole grid one up
		for (int r=0; r<m_Height-1; r++)
		{
			for (c=0; c<m_Base; c++)
			{
				m_Grid[r][c] = m_Grid[r+1][c];
			}
		}
		// add random pieces to the bottom
		for (c=0; c<m_Base; c++)
		{
			m_Grid[m_Height-1][c] = unsigned char(random()%m_TotalColors+1);
		}
		// take care of the falling piece
		if (m_FallingRow > 0)
		{
			m_FallingRow--;
			Step();
		}
		if (m_FallingRow == 0)
		{
			if (!PlacePiece(m_FallingRow, m_FallingCol))	// try to place
			{
				ResetEvent(m_hEventRun);
				m_GameOver = true;
				m_FallingRow = -1;
				m_cs.Unlock();
				::PostMessage(m_Parent, m_MessageId, 1, 0);
				return false;
			}
		}
	}
	m_cs.Unlock();
	return true;
	
}

unsigned long CKolumnzEngine::MakePiece()
{
	static unsigned long piece;
	static int i;
	piece = 0;
	for (i = 0; i<m_PieceLength; i++)
	{
		piece = piece | ((random()%m_TotalColors+1) << (28-4*i));
	}
	return piece;
}

bool CKolumnzEngine::EndGame()
{
	ResetEvent(m_hEventRun);
	m_GameOver = true;
	return m_GameOver;
}

bool CKolumnzEngine::IsGameInProgress()
{
	return !m_GameOver;
}

bool CKolumnzEngine::IsGamePaused()
{
	return m_Paused;
}
