#include "Dan.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "inc/SDL.h"
#include "inc/SDL_image.h"
#include "inc/SDL_ttf.h"
#include "inc/SDL_mixer.h"
#include "Constants.h"

void veDan(SDL_Renderer* renderer, SDL_Texture* danTexture, std::vector<Dan>& danList) {
    for (auto& dan : danList) {
        if (dan.active) {
            SDL_Rect danRect = { static_cast<int>(dan.toaDo.x), static_cast<int>(dan.toaDo.y), BULLET_WIDTH, BULLET_HEIGHT };
            SDL_RenderCopy(renderer, danTexture, NULL, &danRect);
            dan.toaDo.y -= 0.5;
            if (dan.toaDo.y < 0) {
                dan.active = false;
            }
        }
    }
}


void veDanEnemy(SDL_Renderer* renderer, SDL_Texture* danTexture, std::vector<Dan>& danList) {
    for (auto& dan : danList) {
        if (dan.active) {
            SDL_Rect danRect = { static_cast<int>(dan.toaDo.x), static_cast<int>(dan.toaDo.y), BULLET_WIDTH, BULLET_HEIGHT };
            SDL_RenderCopy(renderer, danTexture, NULL, &danRect);
            dan.toaDo.y += 0.3; // Slower bullet speed
            if (dan.toaDo.y > SCREEN_HEIGHT) {
                dan.active = false;
            }
        }
    }
}