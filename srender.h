
/*
*** Project:			ConsolePong v0.1
*** File:				srender.h
*** Author:				cocoz1
*** Date:				05, 2019
*** Description:		A pong clone focused on looking
						the same as the original, but only
						using the console window as the
						device to render to
*** srender.h			in-console letter (i.e. number) renderer 
*** Github:				https://www.github.com/cocoz1/
*** ToDo:				it would be safer to read the 
						chars from a custom font file format,
						completely avoiding globals
*/


#ifndef CP_NUMBERS_H
#define CP_NUMBERS_H


static const wchar_t *SR_One[] = {
	L"    ▓",
	L"    ▓",
	L"    ▓",
	L"    ▓",
	L"    ▓"
};

static const wchar_t *SR_Two[] = {
	L"▓▓▓▓▓",
	L"    ▓",
	L"▓▓▓▓▓",
	L"▓    ",
	L"▓▓▓▓▓"
};

static const wchar_t *SR_Three[] = {
	L"▓▓▓▓▓",
	L"    ▓",
	L"▓▓▓▓▓",
	L"    ▓",
	L"▓▓▓▓▓"
};

static const wchar_t *SR_Four[] = {
	L"▓   ▓",
	L"▓   ▓",
	L"▓▓▓▓▓",
	L"    ▓",
	L"    ▓"
};

static const wchar_t *SR_Five[] = {
	L"▓▓▓▓▓",
	L"▓    ",
	L"▓▓▓▓▓",
	L"    ▓",
	L"▓▓▓▓▓"
};

static const wchar_t *SR_Six[] = {
	L"▓▓▓▓▓",
	L"▓    ",
	L"▓▓▓▓▓",
	L"▓   ▓",
	L"▓▓▓▓▓"
};

static const wchar_t *SR_Seven[] = {
	L"▓▓▓▓ ",
	L"   ▓ ",
	L"  ▓▓▓",
	L"   ▓ ",
	L"   ▓ "
};

static const wchar_t *SR_Eight[] = {
	L"▓▓▓▓▓",
	L"▓   ▓",
	L"▓▓▓▓▓",
	L"▓   ▓",
	L"▓▓▓▓▓"
};

static const wchar_t *SR_Nine[] = {
	L"▓▓▓▓▓",
	L"▓   ▓",
	L"▓▓▓▓▓",
	L"    ▓",
	L"▓▓▓▓▓"
};

static const wchar_t *SR_Zero[] = {
	L"▓▓▓▓▓",
	L"▓   ▓",
	L"▓   ▓",
	L"▓   ▓",
	L"▓▓▓▓▓"
};


const static void *SR_Numbers[] = {
	SR_Zero, SR_One, SR_Two, SR_Three,
	SR_Four, SR_Five, SR_Six, SR_Seven,
	SR_Eight, SR_Nine
};

enum SR_ALIGN {
	ALIGN_LEFT = 0,
	ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_NONE
};


static char *
N_Num2Str(int num) {
	char *nstr = (char *)malloc(12);
	assert(nstr != NULL);

	memset(nstr, 0, 12);
	_itoa_s(num, nstr, 12, 10);

	return nstr;
}

static int 
N_DrawNumber(wchar_t *array, size_t x, size_t y, size_t width, size_t height, enum SR_ALIGN align, size_t offset, char *num) {
	assert(array != NULL || num != NULL);
	int printed = 0;

	switch (align) {
		case ALIGN_LEFT:
			if (num[0] == '1')
				offset = 0;
			x += offset;
			break;
		case ALIGN_RIGHT:
			x = width - (5 * strlen(num) 
				+ (strlen(num) - 1) + offset);
			break;
		case ALIGN_CENTER:
			x = (x - ((strlen(num) / 2 + 1) 
				* 5 + (strlen(num) / 2)));
			break;
		case ALIGN_NONE:
			break;
	}

	size_t cur_x = x;

	for (size_t i = 0; i < strlen(num); i++) {
		int g = num[i] - '0';

		for (size_t a = cur_x, m = 0; a < cur_x + 5; a++, m++)
			for (size_t b = y, n = 0; b < y + 5; b++, n++)
				array[a + b * width] = ((wchar_t **)SR_Numbers[g])[n][m];

		++printed;
		cur_x = x + (printed * (5 + 1));
	}

	free(num);
	return 0;
}


#endif
