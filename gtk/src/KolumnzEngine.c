/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) William Bell 2001-2009 <william.bell@frog.za.net>
 * 
 * This file is part of the Kolumnz game
 *
 * Kolumnz is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Kolumnz is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "KolumnzEngine.h"

//static UINT speedInt[]={1375, 1375, 1250, 1125, 875, 750, 625, 375, 250, 125 };
//static UINT speedInt[] = {615, 600, 540, 465, 390, 315, 240, 165, 90, 75 };

static int levelInt[] = {  10,   15,   24,   34,  36,  50,  64,  76,  94, 9999}; 
//static int levelInt[] = {  2,   4,   6,   8,  10,  12,  14,  16,  18, 9999}; 

// pentaShapes[][4] = // T, U, V, W, X, Y, Z, F, I, L, P and N
// max shape = 5x5=block + 2=rotation +5=peice

/* DWORD WINAPI StackPackThread(LPVOID pData)
 * {
 * 	DWORD dwMilliseconds = 0;   
 * 
 * 	CStackPackEngine* pT;
 * 	pT = (CStackPackEngine*) pData;
 * 
 * 	WaitForSingleObject(pT->m_hEventRun, INFINITE);
 * 	// loop but check for kill notification
 * 	while (WaitForSingleObject(pT->m_hEventKill, 0) == WAIT_TIMEOUT)
 * 	{
 * 		WaitForSingleObject(pT->m_hEventRun, INFINITE);
 * 		WaitForSingleObject(pT->m_hEventStep, dwMilliseconds);
 * 		//ATLTRACE("Step:%dms\n", dwMilliseconds);
 * 		pT->Step();
 * //		dwMilliseconds = pT->m_StepDuration - duration;
 * 		dwMilliseconds = pT->m_StepDuration;
 * //		Sleep(pT->m_StepDuration);
 * 	}
 * 	SetEvent(pT->m_hEventDead);
 * 	//ATLTRACE("Exiting the thread\n");
 * 	return 1;
 * }
 */

/*----------------------------------------------------------------------------*/
void GameInitialize(struct GameData* pGameData)
{
	pGameData->m_Score = 0;
	pGameData->m_Lines = 0;
	pGameData->m_Level = 1;
	pGameData->m_StepDuration = 0;
	pGameData->m_GameOver = 1;
	pGameData->m_Paused = 0;
	pGameData->m_Preview = 1;
	pGameData->m_CascadeCheck = 0;
	pGameData->m_TotalColors = 7;//totalColors;
	pGameData->m_PieceLength = 3;//pieceLength;
	memset(pGameData->m_Grid, 0, sizeof(pGameData->m_Grid));
}

/*----------------------------------------------------------------------------*/
void GameCreate(struct GameData* pGameData,
							int wnd,
						    int msgId,
						    int base,  
							int height,
							int startingLevel,
							int startingRows,
							int preview,
							int blkType,
							int lastLevel)
{
	pGameData->m_MoveCount = 0;
	pGameData->m_DropCount = 0;
	pGameData->m_FullRowCount = 0;
	pGameData->m_FallingRow = -1;
	pGameData->m_FallingCol = 0;
	pGameData->m_FallingPiece = 0;
	pGameData->m_BlkRot = 0;
	pGameData->m_Score = 0;
	pGameData->m_TotalBlocksSunk = 0;
	pGameData->m_Lines = 0;
	pGameData->m_GameOver = 0;
	pGameData->m_Paused = 0;
	
	pGameData->m_Parent = wnd;
	pGameData->m_MessageId = msgId;
	pGameData->m_Base = base;
	pGameData->m_Height = height;
	pGameData->m_Level = startingLevel;
	pGameData->m_StepDuration = 640-55*pGameData->m_Level;	// speedInt[m_Level-1];
	pGameData->m_PiecePreview = preview;
	pGameData->m_TotalColors = 7;//totalColors;
	pGameData->m_PieceLength = 3;//pieceLength;
	
	pGameData->m_LastLevel = lastLevel;

	memset(&pGameData->m_Grid, 0, sizeof(pGameData->m_Grid));

	AddRows(pGameData, startingRows);

	srand((unsigned)clock());
	pGameData->m_Preview = MakePiece(pGameData);
	pGameData->m_Center = pGameData->m_Base/2;
};

/*----------------------------------------------------------------------------*/
int GameLeft(struct GameData* pGameData)
{
	if (pGameData->m_FallingRow >= 0)
	{
		RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position

		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol-1))	// try to place in new position
		{		
			PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
			return 0;
		}
		pGameData->m_MoveCount++;
		pGameData->m_FallingCol--;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int GameRight(struct GameData* pGameData)
{
	if (pGameData->m_FallingRow >= 0)
	{
		RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position

		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol+1))	// try to place in new position
		{		
			PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
			return 0;
		}
		pGameData->m_MoveCount++;
		pGameData->m_FallingCol++;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
int GameRotate(struct GameData* pGameData)
{
/*
	unsigned long oldPosition;

	if (pGameData->m_FallingRow >= 0)
	{
		RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position
		oldPosition = pGameData->m_FallingPiece;
		pGameData->m_BlkRot = (pGameData->m_BlkRot+1)%4;					// rotate anti-clockwise
		pGameData->m_FallingPiece = pGameData->m_Shapes[pGameData->m_BlkNum][pGameData->m_BlkRot];

		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol))// try to place in new position
		{	
			pGameData->m_BlkRot = (pGameData->m_BlkRot+3)%4;					// rotate anti-clockwise
			pGameData->m_FallingPiece = oldPosition;
			PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
			return 0;
		}
		pGameData->m_MoveCount++;
		return 1;
	}
	return 0;
 */
	int r;
	if (pGameData->m_FallingRow >= 0)
	{
		pGameData->m_FallingPiece = pGameData->m_FallingPiece >> 4 | pGameData->m_FallingPiece << 4*(pGameData->m_PieceLength-1);
		for (r=0; r<pGameData->m_PieceLength; r++)
		{
			pGameData->m_Grid[r+pGameData->m_FallingRow][pGameData->m_FallingCol] = (char)((pGameData->m_FallingPiece>>(28-4*r))%16);
		}
		pGameData->m_MoveCount++;
		return 1;
	}
	return 0;
	
}

/*----------------------------------------------------------------------------*/
int GameDrop(struct GameData* pGameData)
{
	printf("in GameDrop\n");
	int rtc = 0;
	pGameData->m_DropCount = 0;
	if (pGameData->m_FallingRow >= 0)
	{
		while (pGameData->m_FallingRow >= 0) 
		{
			if (MovePieceDown(pGameData)) pGameData->m_DropCount++;
		}
		rtc = ClearCompletePatterns(pGameData);
		CalculateScore(pGameData);
	}
	return rtc;
}

/*----------------------------------------------------------------------------*/
int GameForceStepNow(struct GameData* pGameData)
{
	if (pGameData->m_FallingRow >= 0)
		GameStep(pGameData);
	return 1;
}

/*----------------------------------------------------------------------------*/
int GameStep(struct GameData* pGameData)
{
	int rt = 1;		// 0=game over, 1=0K, 2=new peice, 3=game over
	if (pGameData->m_CascadeCheck)
	{
		pGameData->m_CascadeCheck = ClearCompletePatterns(pGameData);
		CalculateScore(pGameData);
	}
	else
	{
		if (pGameData->m_FallingRow == -1) 
		{
			rt = 2;
			pGameData->m_FallingCol = pGameData->m_Center;
			pGameData->m_FallingPiece = pGameData->m_Preview;
			pGameData->m_Preview = MakePiece(pGameData);
			pGameData->m_FallingRow = 0;
			if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol) || pGameData->m_Level == pGameData->m_LastLevel+1)	// try to place
			{
				//ResetEvent(m_hEventRun);
				pGameData->m_GameOver = 1;
				rt = 0;		// game over
			}
		}
		else
		{
			if (!MovePieceDown(pGameData))
			{
				pGameData->m_StepDuration = 640-55*pGameData->m_Level;	// speedInt[m_Level-1];
				pGameData->m_CascadeCheck = ClearCompletePatterns(pGameData);
				CalculateScore(pGameData);
			}
		}
	}
	if (rt == 0)	// game over
	{
		//::PostMessage(m_Parent, m_MessageId, 1, 0);
	}
	else if (pGameData->m_DropCount == 0)
	{
	//	if (rt == 1)
	//	{
	//		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEventStep, 0))
	//		{
	//			ResetEvent(m_hEventStep);
	//		}
			//::PostMessage(m_Parent, m_MessageId, 0, 0);
	//	}
	//	else // (rt==2)	// new peice
	//	{
	//		//::PostMessage(m_Parent, m_MessageId, 3, 0);
	//	}
	}
	else
	{
		pGameData->m_DropCount++;
	}
	return rt;
/*
	int rt = 1;		// 0=game over, 1=0K, 2=new piece, 3=new level 
	if (pGameData->m_FallingRow == -1) 
	{
		rt = 2;
		pGameData->m_FallingCol = pGameData->m_Center;
		pGameData->m_FallingPiece = pGameData->m_Preview;
		pGameData->m_BlkRot = 0;
		pGameData->m_BlkNum = pGameData->m_BlkPreviewNum;
		pGameData->m_BlkPreviewNum = (int)(rand()/(double)0x7fffffff*pGameData->m_MaxPieces);
		pGameData->m_Preview = pGameData->m_Shapes[pGameData->m_BlkPreviewNum][pGameData->m_BlkRot];
		pGameData->m_FallingRow = 0;
		if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol) || pGameData->m_Level == pGameData->m_LastLevel+1)	// try to place
		{
			pGameData->m_GameOver = 1;
			rt = 0;		// game over
		}
	}
	else
	{
		if (!MovePieceDown(pGameData))
		{
			pGameData->m_StepDuration = 640-55*pGameData->m_Level;	// speedInt[m_Level-1]; 
			rt = GameClearFullRows(pGameData);
			CalculateScore(pGameData);
		}
	}
//		Sleep(10000);
	if (pGameData->m_DropCount > 0)
	{
		pGameData->m_DropCount++;
	}
	return rt;
*/
}

/*----------------------------------------------------------------------------*/
void SetPreview(struct GameData* pGameData, int prev)
{
	pGameData->m_PiecePreview = prev;
}

/*----------------------------------------------------------------------------*/
int MovePieceDown(struct GameData* pGameData)
{
	printf("in MovePieceDown\n");
	RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// remove from old position

	if (!PlacePiece(pGameData, pGameData->m_FallingRow+1, pGameData->m_FallingCol))	// try to place in new position
	{		
		PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);	// else restore old position
		pGameData->m_FallingRow = -1;
		return 0;
	}
	pGameData->m_FallingRow = (pGameData->m_FallingRow+1)%pGameData->m_Height;
	return 1;
}

/*----------------------------------------------------------------------------*/
int ClearCompletePatterns(struct GameData* pGameData)
{
	int rtc;
	static int r;
	static int c;
	static int m;
	static int count;

	static int rr;
	rtc = 0;
	
//	memcpy(m_DebugGrid, m_Grid, sizeof(m_Grid));	// debugging
	// find and mark horizontal patterns
	unsigned char prevChar;
	for (r=0; r<pGameData->m_Height; r++)
	{
		prevChar = 0xff;
		for (c=0; c<pGameData->m_Base; c++)
		{
			if (prevChar == pGameData->m_Grid[r][c])
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
						pGameData->m_Grid[r][c-m] = pGameData->m_Grid[r][c-m] | 0x80;
					}
				}
				else if (count > 3)
				{
					pGameData->m_Grid[r][c] = pGameData->m_Grid[r][c] | 0x80;
				}
			}
			prevChar = pGameData->m_Grid[r][c]&0x7f;
		}
	}
	// find and mark vertical patterns
	for (c=0; c<pGameData->m_Base; c++)
	{
		prevChar = 0xff;
		for (r=0; r<pGameData->m_Height; r++)
		{
			if (prevChar == (pGameData->m_Grid[r][c]&0x7f))
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
						pGameData->m_Grid[r-m][c] = pGameData->m_Grid[r-m][c] | 0x80;
					}
				}
				else if (count > 3)
				{
					pGameData->m_Grid[r][c] = pGameData->m_Grid[r][c] | 0x80;
				}
			}
			prevChar = pGameData->m_Grid[r][c]&0x7f;
		}
	}
	// find and mark right diagonal patterns
	int sc, sr; // start column, start row 
	sr = 0;
	sc = 0;
	while (sr<pGameData->m_Height && sc<pGameData->m_Base)
	{
		prevChar = 0xff;
		for (r=sr, c=sc; r>=0 || c<pGameData->m_Base; r--, c++)
		{
			if (prevChar == (pGameData->m_Grid[r][c]&0x7f))
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
						pGameData->m_Grid[r+m][c-m] = pGameData->m_Grid[r+m][c-m] | 0x80;
					}
				}
				else if (count > 3)
				{
					pGameData->m_Grid[r][c] = pGameData->m_Grid[r][c] | 0x80;
				}
			}
			prevChar = pGameData->m_Grid[r][c]&0x7f;
		}
		if (sr < pGameData->m_Height-1)
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
	sc = pGameData->m_Base-1;
	while (sr<pGameData->m_Height && sc>=0)
	{
		prevChar = 0xff;
		for (r=sr, c=sc; r>=0 || c>=0; r--, c--)
		{
			if (prevChar == (pGameData->m_Grid[r][c]&0x7f))
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
						pGameData->m_Grid[r+m][c+m] = pGameData->m_Grid[r+m][c+m] | 0x80;
					}
				}
				else if (count > 3)
				{
					pGameData->m_Grid[r][c] = pGameData->m_Grid[r][c] | 0x80;
				}
			}
			prevChar = pGameData->m_Grid[r][c]&0x7f;
		}
		if (sr < pGameData->m_Height-1)
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
	for (c=0; c<pGameData->m_Base; c++)
	{
		for (r=0; r<pGameData->m_Height; r++)
		{
			if ((pGameData->m_Grid[r][c] & 0x80) == 0x80) 
			{
				blocks++;
				rtc = 1;
				pGameData->m_TotalBlocksSunk++;
			}
		}
	}
	if (blocks > 0)
	{
		pGameData->m_FullRowCount = blocks-2;
	}
	// remove marked blocks
	for (c=0; c<pGameData->m_Base; c++)
	{
		for (r=0; r<pGameData->m_Height; r++)
		{
			if ((pGameData->m_Grid[r][c] & 0x80) == 0x80) 
			{
				for (rr=r; rr>0; rr--)
				{
					pGameData->m_Grid[rr][c] = pGameData->m_Grid[rr-1][c];
				}
				pGameData->m_Grid[0][c] = 0;
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
*/

	// call this routine again until no marked blocks are found

	// check for new level
	if (pGameData->m_TotalBlocksSunk > levelInt[pGameData->m_Level-1])
	{
		pGameData->m_StepDuration = 640-55*pGameData->m_Level;	// speedInt[m_Level-1];
		pGameData->m_Level++;
		//::PostMessage(pGameData->m_Parent, pGameData->m_MessageId, 2, 0);
	}
	else if (pGameData->m_FullRowCount > 0)
	{
		//::PostMessage(pGameData->m_Parent, pGameData->m_MessageId, 4, pGameData->m_FullRowCount);
	}
	return rtc;
}

/*----------------------------------------------------------------------------*/
void RemovePiece(struct GameData* pGameData, int row, int col)
{
	static int r;

	for (r=0; r<pGameData->m_PieceLength; r++)
	{
		pGameData->m_Grid[r+row][col] = 0;
	}
}

/*----------------------------------------------------------------------------*/
int PlacePiece(struct GameData* pGameData, int row, int col)
{
	static int r;

	// has the peice hit the bottom?
	if (row+pGameData->m_PieceLength > pGameData->m_Height)
	{
		return 0;
	}

	// has the piece hit left wall?
	if (col < 0)
	{
		return 0;
	}

	// has the peice hit right wall?
	if (col >= pGameData->m_Base)
	{
		return 0;
	}

	// has the peice hit another peice?
	for (r=0; r<pGameData->m_PieceLength; r++)
	{
		if (pGameData->m_Grid[r+row][col])
		{
			return 0;
		}
	}

	// move the peice
	for (r=0; r<pGameData->m_PieceLength; r++)
	{
		pGameData->m_Grid[r+row][col] = (char)((pGameData->m_FallingPiece>>(28-4*r))%16);
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
void CalculateScore(struct GameData* pGameData)
{
	long score = pGameData->m_DropCount+pGameData->m_Level*6-(2*pGameData->m_Level*pGameData->m_PiecePreview)-pGameData->m_MoveCount;
	if (score < 6 && !pGameData->m_FullRowCount)
	{
		if (pGameData->m_Level != 1)
			score = 7;
		else 
			score = 6;
	}
	if (score < 0 && pGameData->m_FullRowCount)
		score = 0;

	if (pGameData->m_FullRowCount>0)
//		score += (unsigned long)(pow(2, (m_FullRowCount-1))*10+.5);
		pGameData->m_FullRowCount += 2;
		score += (unsigned long)(pGameData->m_FullRowCount/2.*(44.44444+(pGameData->m_FullRowCount-1.)*11.111111)+.5);
	pGameData->m_Score += score;
	pGameData->m_DropCount = pGameData->m_MoveCount = pGameData->m_FullRowCount = 0;
}

// Access Routines
/*----------------------------------------------------------------------------*/
unsigned long GameGetPreview(struct GameData* pGameData, int* color)
{
	if (pGameData->m_PiecePreview)
	{
		return pGameData->m_Preview;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
void GameGetGrid(struct GameData* pGameData, char grid[32][32])
{
	memcpy(grid, pGameData->m_Grid, sizeof(pGameData->m_Grid));
}

/*----------------------------------------------------------------------------*/
int GameStart(struct GameData* pGameData)
{
	if (!pGameData->m_GameOver)
	{
		pGameData->m_Paused = 0;
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
int GamePause(struct GameData* pGameData)
{
	pGameData->m_Paused = 1;
	return pGameData->m_Paused;
}

/*----------------------------------------------------------------------------*/
int AddRows(struct GameData* pGameData, int nRows)
{
	int i,r,c;
	for (i=0; i<nRows; i++)
	{
		// is the top row free?
		if (pGameData->m_FallingRow >= 0)
			RemovePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol);
		for (c=0; c<pGameData->m_Base; c++)
		{
			if (pGameData->m_Grid[0][c]) 
			{
				return 0;
			}
		}
		// move the whole grid one up
		for (r=0; r<pGameData->m_Height-1; r++)
			for (c=0; c<pGameData->m_Base; c++)
				pGameData->m_Grid[r][c] = pGameData->m_Grid[r+1][c];
		// add random pieces to the bottom
		for (c=0; c<pGameData->m_Base; c++)
			pGameData->m_Grid[pGameData->m_Height-1][c] = rand()%pGameData->m_MaxPieces+1;
		// add about 30% spaces to the rows
		pGameData->m_Grid[pGameData->m_Height-1][rand()%pGameData->m_Base] = 0;
		for (c=0; c<pGameData->m_Base; c++)
			if (rand()%10 < 3)
				pGameData->m_Grid[pGameData->m_Height-1][rand()%pGameData->m_Base] = 0;
		// take care of the falling piece
		if (pGameData->m_FallingRow > 0)
		{
			pGameData->m_FallingRow--;
			GameStep(pGameData);
		}
		if (pGameData->m_FallingRow == 0)
		{
			if (!PlacePiece(pGameData, pGameData->m_FallingRow, pGameData->m_FallingCol))	// try to place
			{
				pGameData->m_GameOver = 1;
				pGameData->m_FallingRow = -1;
				return 0;
			}
		}
	}
	return 1;
}

/*----------------------------------------------------------------------------*/
unsigned long MakePiece(struct GameData* pGameData)
{
	static unsigned long piece;
	static int i;
	piece = 0;
	for (i = 0; i<pGameData->m_PieceLength; i++)
	{
		piece = piece | ((random()%pGameData->m_TotalColors+1) << (28-4*i));
	}
	return piece;
}

/*----------------------------------------------------------------------------*/
int EndGame(struct GameData* pGameData)
{
	pGameData->m_GameOver = 1;
	return pGameData->m_GameOver;
}

/*----------------------------------------------------------------------------*/
int IsGameInProgress(struct GameData* pGameData)
{
	return !pGameData->m_GameOver;
}

/*----------------------------------------------------------------------------*/
int IsGamePaused(struct GameData* pGameData)
{
	return pGameData->m_Paused;
}

/*----------------------------------------------------------------------------*/
