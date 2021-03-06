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

#include <gtk/gtk.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "gameData.h"
/*----------------------------------------------------------------------------*/
void Initialize(struct GameDataStr* data)
{
	int i;
	time_t now;
	time(&now);
	struct tm* nowtime = localtime(&now);
	unsigned long nowDate = (nowtime->tm_year+1900)*10000 + (nowtime->tm_mon+1)*100 + nowtime->tm_mday;

	data->m_Base = 6;
	data->m_Height = 18;
	data->m_BlkSize = 3;
	data->m_Colors = 6;
	data->m_StartingLevel = 1;
	data->m_StartingRows = 0;
	data->m_LastLevel = 10;	 // Maximum level of this game
	data->m_EndLevel = 10;		 // current last level of this game m_EndLevel <= m_LastLevel
	data->m_Spare1 = 0;
	data->m_PiecePreview = 1;
	data->m_Penalize = 1;
	strcpy(data->m_Comment, "Standard Kolumnz");
	for (i=0; i<10; i++)
	{
		data->m_HighScore[i].level = 1;
		data->m_HighScore[i].name[0] = 0;
		data->m_HighScore[i].quote[0] = 0;
		data->m_HighScore[i].date = nowDate;
		data->m_HighScore[i].score = (10-i)*500;
		data->m_HighScore[i].rows = 0;
	}
}

/*----------------------------------------------------------------------------*/
void SaveGameData(struct GameDataStr* data, const gchar *filename)
{
	printf("in SaveGameData\n");
	int i;
	FILE* fd = fopen(filename, "w");
	if (fd == NULL) /* file not found */
	{
		printf("File %s not found\n", filename);
		fd = fopen(filename, "w");
		if (fd != NULL)
		{
			chmod(filename, S_IREAD | S_IWRITE);
		}
	}
	if (fd != NULL)
	{
		fprintf(fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
				data->m_Base, 
				data->m_Height,
				data->m_BlkType,
				data->m_StartingLevel,
				data->m_StartingRows,
				data->m_LastLevel,
				data->m_EndLevel,
				data->m_Spare1,
				data->m_Penalize,
				data->m_Comment);
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
				data->m_Base, 
				data->m_Height,
				data->m_BlkType,
				data->m_StartingLevel,
				data->m_StartingRows,
				data->m_LastLevel,
				data->m_EndLevel,
				data->m_Spare1,
				data->m_Penalize,
				data->m_Comment);
		for (i=0; i<10; i++)
		{
			printf("%d,%d,%d,%ld,%s,%s\n",
			data->m_HighScore[i].score,
			data->m_HighScore[i].rows,
			data->m_HighScore[i].level,
			data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
			fprintf(fd, "%d,%d,%d,%ld,%s,%s\n",
			data->m_HighScore[i].score,
			data->m_HighScore[i].rows,
			data->m_HighScore[i].level,
			data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
		}
		
		fclose(fd);
	}
}

/*----------------------------------------------------------------------------*/
void ReadGameData(struct GameDataStr* data, const gchar *filename)
{
	//todo check for buffer overflows
	printf("in ReadGameData\n");
	int i;
	FILE* fd = fopen(filename, "r");
	if (fd != NULL)
	{
		fscanf(fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^\n]\n",
				&data->m_Base, 
				&data->m_Height,
				&data->m_BlkType,
				&data->m_StartingLevel,
				&data->m_StartingRows,
				&data->m_LastLevel,
				&data->m_EndLevel,
				&data->m_Spare1,
				&data->m_Penalize,
				data->m_Comment);
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
				data->m_Base, 
				data->m_Height,
				data->m_BlkType,
				data->m_StartingLevel,
				data->m_StartingRows,
				data->m_LastLevel,
				data->m_EndLevel,
				data->m_Spare1,
				data->m_Penalize,
				data->m_Comment);
		for (i=0; i<10; i++)
		{
			fscanf(fd, "%d,%d,%d,%ld,%[^,\n],%[^,\n]\n",
			&data->m_HighScore[i].score,
			&data->m_HighScore[i].rows,
			&data->m_HighScore[i].level,
			&data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
			printf("%d,%d,%d,%ld,%s,%s\n",
			data->m_HighScore[i].score,
			data->m_HighScore[i].rows,
			data->m_HighScore[i].level,
			data->m_HighScore[i].date,
			data->m_HighScore[i].name,
			data->m_HighScore[i].quote);
		}
		fclose(fd);
	}
	/* do validation */
	if (data->m_Base < 5)
		data->m_Base = 5;
	if (data->m_Base > 32)
		data->m_Base = 32;
	if (data->m_Height < 5)
		data->m_Height = 5;
	if (data->m_Height > 32)
		data->m_Height = 32;
	if (data->m_Penalize)
		data->m_Penalize = 1;
	if (data->m_StartingRows < 0)
		data->m_StartingRows = 0;
	if (data->m_StartingRows > data->m_Height - 5)
		data->m_StartingRows = data->m_Height - 5;
	if (data->m_LastLevel > 10)
		data->m_LastLevel = 10;
	if (data->m_LastLevel < 1)
		data->m_LastLevel = 1;
	if (data->m_StartingLevel < 0)
		data->m_StartingLevel = 1;
	if (data->m_StartingLevel > data->m_LastLevel)
		data->m_StartingLevel = data->m_LastLevel;
	if (data->m_EndLevel <= data->m_StartingLevel)
		data->m_EndLevel = data->m_StartingLevel+1;
	if (data->m_EndLevel > data->m_LastLevel)
		data->m_EndLevel = data->m_LastLevel;
	data->m_Comment[31] = 0;
	for (i=0; i<10; i++)
	{
		if (data->m_HighScore[i].score < (10-i)*50*data->m_LastLevel)
			data->m_HighScore[i].score = (10-i)*50*data->m_LastLevel;
		if (data->m_HighScore[i].rows < 1)
			data->m_HighScore[i].rows = 1;
		if (data->m_HighScore[i].level < 0)
			data->m_HighScore[i].level = 1;
		if (data->m_HighScore[i].level > data->m_LastLevel)
			data->m_HighScore[i].level = data->m_LastLevel;
		if (data->m_HighScore[i].date < 20000101)
			data->m_HighScore[i].date = 20000101;
		data->m_HighScore[i].name[31] = 0;
		data->m_HighScore[i].quote[31] = 0;
	}

}

/*----------------------------------------------------------------------------*/
void InitializeGameData(struct GameDataStr* data, const char* filename)
{
	printf("%s", filename);
	Initialize(data);
	
	ReadGameData(data, filename);
	if (!strcmp(filename, "junior.gam"))
	{
		data->m_Base = 6;
		data->m_BlkSize = 2;
		data->m_Colors = 5;
		strcpy(data->m_Comment, "Easy Kolumnz");
	}
	else if (!strcmp(filename, "classic.gam"))
	{
		strcpy(data->m_Comment, "Classic Kolumnz");
	}
	else if (!strcmp(filename, "cool.gam"))
	{
		strcpy(data->m_Comment, "Cool Kolumnz");
		data->m_Base = 10;
	}
	else if (!strcmp(filename, "mania.gam"))
	{
		strcpy(data->m_Comment, "KolumnzMania says it all!");
		data->m_BlkSize = 4;
		data->m_Colors = 10;
		data->m_Base = 14;
	}

}

/*----------------------------------------------------------------------------*/
int IsHighScore(struct GameDataStr* data, int score)
{
	int i, rtc = 0;
	for (i=0; i<10; i++)
	{
		if (data->m_HighScore[i].score < score)
		{
			rtc = 1;
			break;
		}
	}
	return rtc;
}

/*----------------------------------------------------------------------------*/
int AddHighScore(struct GameDataStr* data, const char* name, const char* quote, int score, int rows, int level)
{
	int i, rtc = 0;
	for (i=0; i<10; i++)
	{
		if (data->m_HighScore[i].score < score)
		{
			rtc = 1;
			if (i < 9 )
			{
				memmove(&data->m_HighScore[i+1], 
					&data->m_HighScore[i], 
					sizeof(struct HighScoreRecord)*(9-i));
			}
			data->m_HighScore[i].score = score;
			data->m_HighScore[i].rows = rows;
			data->m_HighScore[i].level = level;
			time_t now;
			time(&now);
			struct tm* nowtime = localtime(&now);
			unsigned long nowDate = (nowtime->tm_year+1900)*10000 + (nowtime->tm_mon+1)*100 + nowtime->tm_mday;
			data->m_HighScore[i].date = nowDate;
			if (strlen(name) > 0)
			{
				strcpy(data->m_HighScore[i].name, name);
			}
			else
			{
				strcpy(data->m_HighScore[i].name, " ");
			}
			if (strlen(quote) > 0)
			{
				strcpy(data->m_HighScore[i].quote, quote);
			}
			else
			{
				strcpy(data->m_HighScore[i].quote, " ");
			}
			break;
		}
	}
	return rtc;
}
