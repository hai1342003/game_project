#ifndef BULLET_H
#define BULLET_H

#include "ToaDo.h"
#include "inc/SDL.h"
#include <vector>


struct Dan {
    ToaDo toaDo;
    bool active;
};

void veDan(SDL_Renderer* renderer, SDL_Texture* danTexture, std::vector<Dan>& danList);

void veDanEnemy(SDL_Renderer* renderer, SDL_Texture* danTexture, std::vector<Dan>& danList);


#endif
