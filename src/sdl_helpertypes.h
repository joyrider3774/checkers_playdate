#ifndef SDL_HELPERTYPES
#define SDL_HELPERTYPES

struct SDL_Point {
	int x;
	int y;
};
typedef struct SDL_Point SDL_Point;

struct SDL_Rect {
	int x;
	int y;
	int w;
	int h;
};
typedef struct SDL_Rect SDL_Rect;

#endif