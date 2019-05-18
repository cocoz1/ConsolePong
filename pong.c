
/*
*** Project:			ConsolePong v0.1
*** File:				pong.c
*** Author:				cocoz1
*** Date:				05, 2019
*** Description:		A pong clone focused on looking
						the same as the original, but only
						using the console window as the
						device to render to
*** pong.c				implementation of the game stuff defined in pong.h
*** Github:				https://www.github.com/cocoz1/
*** ToDo:				-
*/


#include "pong.h"


void CP_ResizeConsoleWindow(HANDLE console, CP_Uint width, CP_Uint height) {
	assert(console != NULL 
		|| width != 0 || height != 0);

	SMALL_RECT rect;
	COORD coord;

	coord.X = (SHORT)width;
	coord.Y = (SHORT)height;

	SetConsoleScreenBufferSize(console, coord);

	rect.Left = 0;
	rect.Top = 0;
	rect.Right = coord.X - 1;
	rect.Bottom = coord.Y - 1;

	SetConsoleWindowInfo(console, 0, &rect);
}


wchar_t *CP_CreateScreenBuffer(size_t width, size_t height) {
	wchar_t *buffer = (wchar_t *)
		malloc(sizeof(*buffer) * (width * height));
	assert(buffer != NULL);

	memset(buffer, 0, sizeof(*buffer) * (width * height));

	return buffer;
}


HANDLE CP_ActivateScreenBuffer(void) {
	DWORD written = 0;
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	SetConsoleTitleW(L"ConsolePong v0.1 - by セバスチャン");

	return console;
}


CP_Game *CP_CreateGameInstance(CP_Uint width, CP_Uint height) {
	assert(width != 0 || height != 0);

	CP_Game *game = (CP_Game *)malloc(sizeof(*game));
	assert(game != NULL);

	game->buffer = CP_CreateScreenBuffer(width, height);
	game->console = CP_ActivateScreenBuffer();
	assert(game->buffer != NULL || game->console != NULL);
	CP_ResizeConsoleWindow(game->console, width, height);

	game->written = 0;
	game->width = width;
	game->height = height;

	game->left = CP_CreatePaddle(2.f, (CP_Float)height / 2.f);
	game->right = CP_CreatePaddle((CP_Float)width - 3.f, (CP_Float)height / 2.f);
	game->ball = CP_CreateBall((CP_Float)width / 2.f, (CP_Float)height / 2.f);
	assert(game->left != NULL 
		|| game->right != NULL || game->ball != NULL);

	return game;
}

CP_Paddle *CP_CreatePaddle(CP_Float x, CP_Float y) {
	CP_Paddle *paddle = (CP_Paddle *)malloc(sizeof(*paddle));
	assert(paddle != NULL);

	paddle->x = x;
	paddle->y = y;
	paddle->score = 0;

	return paddle;
}

CP_Ball *CP_CreateBall(CP_Float x, CP_Float y) {
	CP_Ball *ball = (CP_Ball *)malloc(sizeof(*ball));
	assert(ball != NULL);

	ball->x = x;
	ball->y = y;
	ball->angle = 0;

	return ball;
}

void CP_DestroyGameInstance(CP_Game *game) {
	if (game != NULL) {
		free(game->buffer);
		CloseHandle(game->console);

		free(game->ball);
		free(game->left);
		free(game->right);
	}

	free(game);
}


int __cdecl CP_MainLoop(void) {
	srand((unsigned int)time(NULL));

	clock_t cl = 0;
	CP_Float frametime = 0;
	CP_Event event = CP_DEFEVENT;
	CP_Game *game = CP_CreateGameInstance(200, 60);

	while (1) {
		cl = clock();
		CP_GetKeyEvent(game, &event, frametime);
		CP_ExecuteKeyEvent(game, event);

		CP_RemoveBall(game);
		CP_CheckBallBoundaries(game);

		CP_MoveBall(game->ball, CP_BALLSPEED, 0, frametime);
		CP_ResetBall(game);

		CP_DrawGame(game);
		CP_CheckCollision(game, game->left);
		CP_CheckCollision(game, game->right);

		frametime = (CP_Float)(clock() - cl) / CLOCKS_PER_SEC;
		/* CP_DrawFPS(game, frametime); */
	}

	CP_DestroyGameInstance(game);
	return 0;
}


CP_Int CP_DrawCenterLine(CP_Game *game) {
	assert(game != NULL);

	for (CP_Uint i = 0; i < game->height; i++)
		game->buffer[game->width / 2 + i * game->width] 
			= (i && i % 2 == 0 ? L'▓' : 0);

	return 0;
}

CP_Int CP_DrawBall(CP_Game *game) {
	assert(game != NULL);

	game->buffer[(CP_Uint)game->ball->y 
		* game->width + (CP_Uint)game->ball->x] = L'■';

	return 0; 
}

CP_Int CP_DrawPaddle(CP_Game *game, CP_Paddle *paddle) {
	assert(game != NULL || paddle != NULL);

	for (CP_Uint i = (CP_Uint)paddle->y - (CP_PADDLEHEIGHT / 2); i < paddle->y + (CP_PADDLEWIDTH / 2); i++)
		game->buffer[i * game->width + (CP_Uint)paddle->x] = L'█';

	return 0;
}

CP_Int CP_RemovePaddle(CP_Game *game, CP_Paddle *paddle) {
	assert(game != NULL || paddle != NULL);

	for (CP_Uint i = (CP_Uint)paddle->y - (CP_PADDLEHEIGHT / 2); i < paddle->y + (CP_PADDLEWIDTH / 2); i++)
		game->buffer[i * game->width + (CP_Uint)paddle->x] = 0;

	return 0;
}


CP_Int CP_ResetBall(CP_Game *game) {
	assert(game != NULL);

	if (game->ball->x >= game->width || game->ball->x <= 0) {
		game->ball->angle = (game->ball->x > game->width / 2 ? 0.001f : -0.001f);

		if (game->ball->x >= game->width)
			++game->left->score;
		if (game->ball->x <= 0)
			++game->right->score;

		game->ball->x = game->width / 2.f;
		game->ball->y = game->height / 2.f;
	}

	return 0;
}

CP_Int CP_RemoveBall(CP_Game *game) {
	assert(game != NULL);

	game->buffer[(CP_Uint)game->ball->y 
		* game->width + (CP_Uint)game->ball->x] = 0;

	return 0;
}

CP_Int CP_MoveBall(CP_Ball *ball, CP_Float nx, CP_Float ny, CP_Float frametime) {
	assert(ball != NULL);

	ball->x += cosf(ball->angle) * frametime * CP_BALLSPEED;
	ball->y += sinf(ball->angle) * frametime * CP_BALLSPEED;

	return 0;
}


CP_Int CP_CheckBallBoundaries(CP_Game *game) {
	assert(game != NULL);

	if (game->ball->y <= 0.f) /* upper boundary */ {
		game->ball->angle = -game->ball->angle;
		game->ball->y = 1.f;
	}

	if (game->ball->y >= game->height) /* lower boundary */ {
		game->ball->angle = -game->ball->angle;
		game->ball->y = game->height - 1.f;
	}

	return 0;
}


CP_Int CP_CheckPaddleBoundaries(CP_Game *game, CP_Paddle *paddle) {
	assert(game != NULL || paddle != NULL);

	if (paddle->y - CP_PADDLEHEIGHT / 2 <= 0)
		return 1;
	if (paddle->y >= game->height)
		return 2;

	return 0;
}

CP_Int CP_CheckCollision(CP_Game *game, CP_Paddle *paddle) {
	assert(game != NULL || paddle != NULL);

	if (game->buffer[(CP_Uint)game->ball->x + game->width * (CP_Uint)game->ball->y] == L'█') {
		if (game->ball->y > paddle->y)
			game->ball->angle = CP_PI + game->ball->angle + (rand() % 15) /* add some noise */ * CP_PI / 180;
		else 
			game->ball->angle = CP_PI - game->ball->angle + (rand() % 15) /* add some noise */ * CP_PI / 180;
		
		game->ball->x = (game->ball->x > game->width / 2 ? game->width - paddle->x - 2 : paddle->x + 2);
	}

	return 0;
}


CP_Int CP_GetKeyEvent(CP_Game *game, CP_Event *event, float frametime) {
	assert(game != NULL);

	/* check for close event */
	if (GetAsyncKeyState((unsigned char)'X') & 0x8000)
		return *event = CP_EVCLOSE;

	/* Move left paddle */
	CP_RemovePaddle(game, game->left);
	if (GetAsyncKeyState((unsigned char)'W') & 0x8000) {
			game->left->y -= (CP_PADDLESPEED * frametime);

		if (CP_CheckPaddleBoundaries(game, game->left) == 1)
			game->left->y += (CP_PADDLESPEED * frametime);
	}
	if (GetAsyncKeyState((unsigned char)'S') & 0x8000) {
			game->left->y += (CP_PADDLESPEED * frametime);

		if (CP_CheckPaddleBoundaries(game, game->left) == 2)
			game->left->y -= (CP_PADDLESPEED * frametime);
	}

	/* move right paddle */
	CP_RemovePaddle(game, game->right);
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		game->right->y -= (CP_PADDLESPEED * frametime);

		if (CP_CheckPaddleBoundaries(game, game->right) == 1)
			game->right->y += (CP_PADDLESPEED * frametime);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		game->right->y += (CP_PADDLESPEED * frametime);

		if (CP_CheckPaddleBoundaries(game, game->right) == 2)
			game->right->y -= (CP_PADDLESPEED * frametime);
	}

	return *event = CP_DEFEVENT;
}

CP_Int CP_ExecuteKeyEvent(CP_Game *game, CP_Event event) {
	assert(game != NULL);

	switch (event) {
		case CP_EVCLOSE:
			CP_DestroyGameInstance(game);
			exit(EXIT_SUCCESS);
			break;
	}

	return 0;
}


CP_Int CP_DrawScores(CP_Game *game) {
	assert(game != NULL);
	
	N_DrawNumber(game->buffer, game->width / 4, 2, game->width, 
		game->height, ALIGN_CENTER, 4, N_Num2Str(game->left->score));
	N_DrawNumber(game->buffer, game->width / 4 * 3, 2, game->width,
		game->height, ALIGN_CENTER, 4, N_Num2Str(game->right->score));

	return 0;
}

CP_Int CP_DrawFPS(CP_Game *game, CP_Float frametime) {
	assert(game != NULL);
	wchar_t buffer[20];
	memset(buffer, 0, sizeof(*buffer) * 20);

	swprintf(game->buffer + (game->width - 10) 
		+ (game->height - 1) * game->width, 20, L"FPS: %.3g", 1 / frametime);

	return 0;
}

CP_Int CP_DrawGame(CP_Game *game) {
	assert(game != NULL);
	COORD coord = { 0,0 };

	CP_DrawBall(game);
	CP_DrawCenterLine(game);
	CP_DrawPaddle(game, game->right);
	CP_DrawPaddle(game, game->left);
	CP_DrawScores(game);

	WriteConsoleOutputCharacterW(game->console, game->buffer, 
		game->width * game->height, coord, &game->written);

	return 0;
}

CP_Int CP_ClearScreen(CP_Game *game) {
	assert(game != NULL);

	memset(game->buffer, 0, sizeof(*game->buffer) * game->width * game->height);

	return 0;
}



