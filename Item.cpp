#include "Item.h"
#include "inc/SDL.h"
#include <vector>
#include "Constants.h"


void veItems(SDL_Renderer* renderer, SDL_Texture* itemInvincibleTexture, SDL_Texture* itemRapidFireTexture, std::vector<Item>& items) {
    for (auto& item : items) {
        if (item.active && item.type == INVINCIBLE_ITEM) {
            SDL_Rect itemRect = { static_cast<int>(item.toaDo.x), static_cast<int>(item.toaDo.y), HEART_WIDTH, HEART_HEIGHT };
            SDL_RenderCopy(renderer, itemInvincibleTexture, NULL, &itemRect);
            item.toaDo.y += item.fallSpeed;
            if (item.toaDo.y > SCREEN_HEIGHT) {
                item.active = false; // Đồ biến mất khi rơi xuống khỏi màn hình
            }
        } else if (item.active && item.type == RAPID_FIRE_ITEM) {
            SDL_Rect itemRect = { static_cast<int>(item.toaDo.x), static_cast<int>(item.toaDo.y), HEART_WIDTH, HEART_HEIGHT };
            SDL_RenderCopy(renderer, itemRapidFireTexture, NULL, &itemRect);
            item.toaDo.y += item.fallSpeed;
            if (item.toaDo.y > SCREEN_HEIGHT) {
                item.active = false; // Đồ biến mất khi rơi xuống khỏi màn hình
            }
        }
    }
}

