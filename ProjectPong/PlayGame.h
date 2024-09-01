#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <string>

using namespace std; 

class PlayGame
{
public:
	void SinglePlayer(SDL_Renderer * SingleRenderer, SDL_Window * SingleWindow);
	void TwoPlayer(SDL_Renderer * DoubleRenderer, SDL_Window * DoubleWindow);
	bool Collision(SDL_Rect a, SDL_Rect b);
	SDL_Rect rectcreation(int x, int y, int width, int height, int r, int g, int b, SDL_Renderer* render);
	void textcreate(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text, int fontsize);
};
