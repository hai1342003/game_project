#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "inc/SDL.h"
#include "inc/SDL_ttf.h"
#include "ToaDo.h"
#include "Dan.h"
#include "Enemy.h"
#include "inc/SDL_mixer.h"
#include <string>
#include <vector>

SDL_Texture* taiAnh(SDL_Renderer* renderer, const char* file);
SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color);
void veMenuChinh(SDL_Renderer* renderer, TTF_Font* font);
void veHearts(SDL_Renderer* renderer, SDL_Texture* heartTexture, int playerLives);
bool isInsideButton(int x, int y, SDL_Rect buttonRect);

#endif
