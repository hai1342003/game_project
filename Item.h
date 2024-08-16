#ifndef ITEM_H
#define ITEM_H

#include "ToaDo.h"
#include "inc/SDL.h"
#include <vector>


enum ItemType {
    INVINCIBLE_ITEM,
    RAPID_FIRE_ITEM,
    // Các loại item khác
};


struct Item {
    ToaDo toaDo;
    bool active;
    double fallSpeed;
    ItemType type; // Thêm trường type để phân biệt các loại item
};


void veItems(SDL_Renderer* renderer, SDL_Texture* itemInvincibleTexture, SDL_Texture* itemRapidFireTexture, std::vector<Item>& items);


#endif