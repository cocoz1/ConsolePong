
/*
*** Project:			ConsolePong v0.1
*** File:				pong.h
*** Author:				cocoz1
*** Date:				05, 2019
*** Description:		A pong clone focused on looking
						the same as the original, but only
						using the console window as the
						device to render to
*** pong.h				main header file for game stuff
*** Github:				https://www.github.com/cocoz1/
*** ToDo:				-
*/


#ifndef CONSOLE_PONG_H
#define CONSOLE_PONG_H

#include <stdio.h>		/* debug stuff */
#include <stdlib.h>		/* exit(), malloc(), free() */
#include <assert.h>		/* assert() */
#include <Windows.h>	/* Windows-specific functions */
#include <time.h>		/* time measuring */
#include <math.h>		/* sin(), cos() */

#include "srender.h"	/* score renderer */


#define CP_DEFEVENT (0)
	#define CP_EVCLOSE (-1)
	#define CP_EVPAUSE (1)
	#define CP_EVRESUME (2)

#define CP_PADDLELEFT (1)
#define CP_PADDLERIGHT (2)

#define CP_PADDLEWIDTH (1)
#define CP_PADDLEHEIGHT (11)
#define CP_PADDLESPEED (80)

#define CP_BALLSPEED (100)

#define CP_PI (3.14159265f)


typedef int CP_Int, CP_Event;
typedef size_t CP_Uint;
typedef float CP_Float;

typedef struct CP_Paddle {
	CP_Float x, y;
	CP_Uint score;
} CP_Paddle;

typedef struct CP_Ball {
	CP_Float x, y,
		angle;
} CP_Ball;

typedef struct CP_Game {
	CP_Uint width, height;
	HANDLE console;
	DWORD written;
	wchar_t *buffer;
	
	CP_Ball *ball;
	CP_Paddle *left, *right;
} CP_Game;


extern void CP_ResizeConsoleWindow(HANDLE, CP_Uint, CP_Uint);

extern wchar_t *CP_CreateScreenBuffer(CP_Uint, CP_Uint);
extern HANDLE CP_ActivateScreenBuffer(void);

extern CP_Game *CP_CreateGameInstance(CP_Uint, CP_Uint);
	extern CP_Paddle *CP_CreatePaddle(CP_Float, CP_Float);
	extern CP_Ball *CP_CreateBall(CP_Float, CP_Float);
extern void CP_DestroyGameInstance(CP_Game *);

extern int __cdecl CP_MainLoop(void);

extern CP_Int CP_DrawCenterLine(CP_Game *);
extern CP_Int CP_DrawBall(CP_Game *);
extern CP_Int CP_DrawPaddle(CP_Game *, CP_Paddle *);
extern CP_Int CP_RemovePaddle(CP_Game *, CP_Paddle *);

extern CP_Int CP_ResetBall(CP_Game *);
extern CP_Int CP_RemoveBall(CP_Game *);
extern CP_Int CP_MoveBall(CP_Ball *, CP_Float, CP_Float, CP_Float);

extern CP_Int CP_CheckBallBoundaries(CP_Game *);
extern CP_Int CP_CheckPaddleBoundaries(CP_Game *, CP_Paddle *);
extern CP_Int CP_CheckCollision(CP_Game *, CP_Paddle *);

extern CP_Int CP_GetKeyEvent(CP_Game *, CP_Event *, CP_Float);
extern CP_Int CP_ExecuteKeyEvent(CP_Game *, CP_Event);

extern CP_Int CP_DrawScores(CP_Game *);
extern CP_Int CP_DrawFPS(CP_Game *, CP_Float);
extern CP_Int CP_DrawScores(CP_Game *);
extern CP_Int CP_DrawGame(CP_Game *);
extern CP_Int CP_ClearScreen(CP_Game *);


#endif
