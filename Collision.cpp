#include "Collision.h"
#include "Dan.h"
#include "Constants.h"
#include "inc/SDL_mixer.h"

bool kiemTraVaCham(const ToaDo& toaDo1, int width1, int height1, const ToaDo& toaDo2, int width2, int height2) {
    return !(toaDo1.x > toaDo2.x + width2 ||
             toaDo1.x + width1 < toaDo2.x ||
             toaDo1.y > toaDo2.y + height2 ||
             toaDo1.y + height1 < toaDo2.y);
}

void checkCollisionWithPlayer(const ToaDo& playerPos, std::vector<Enemy>& enemyList, int& playerLives, Mix_Chunk* explosionSound, bool playerInvincible) {
    for (auto& enemy : enemyList) {
        for (auto& dan : enemy.danList) {
            if (dan.active && kiemTraVaCham(dan.toaDo, BULLET_WIDTH, BULLET_HEIGHT, playerPos, PLAYER_WIDTH, PLAYER_HEIGHT)) {
                
                dan.active = false;
                if (!playerInvincible) {
                    playerLives--;
                    Mix_PlayChannel(-1, explosionSound, 0);


                }
                break;
            }
        }
    }
}

void checkItemCollision(ToaDo playerPos, std::vector<Item>& items, bool& playerInvincible, double& invincibleTime, bool& rapidFire, double& rapidFireTime) {
    for (auto& item : items) {
        if (item.active && kiemTraVaCham(playerPos, PLAYER_WIDTH, PLAYER_HEIGHT, item.toaDo, HEART_WIDTH, HEART_HEIGHT)) {
            item.active = false;
            if (item.type == INVINCIBLE_ITEM) { // Kiểm tra nếu là item bất tử
                playerInvincible = true;
                invincibleTime = 35; // Bất tử trong 3 giây
            } else if (item.type == RAPID_FIRE_ITEM) { // Kiểm tra nếu là item bắn đạn liên tục
                rapidFire = true;
                rapidFireTime = 35; // Bắn liên tục trong 3 giây
            }
        }
    }
}
