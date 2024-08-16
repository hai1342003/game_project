#include "Enemy.h"
#include "Constants.h"
#include <cstdlib>
#include "inc/SDL_mixer.h"
#include "Item.h"
#include "Collision.h"


void spawnEnemy(std::vector<Enemy>& enemyList) {
    double x = static_cast<double>(std::rand() % (SCREEN_WIDTH - ENEMY_WIDTH));
    EnemyType type;

    int randomValue = std::rand() % 100;
    if (randomValue < 70) { // 70% tạo kẻ địch loại BASIC
        type = BASIC;
    } else if (randomValue < 90) { // 20% tạo kẻ địch loại FAST
        type = FAST;
    } else { // 10% tạo kẻ địch loại STRONG
        type = STRONG;
    }

    double speed;
    if (type == BASIC) {
        speed = 0.2;
    } else if (type == FAST) {
        speed = 0.4;
    } else { // STRONG
        speed = 0.5;
    }

    enemyList.push_back({ {x, 0}, speed, ACTIVE, true, 0, 0, {}, type});
}




void veEnemy(SDL_Renderer* renderer, SDL_Texture* enemyTexture, SDL_Texture* fastEnemyTexture, SDL_Texture* strongEnemyTexture,
             std::vector<Enemy>& enemyList, SDL_Texture* danTexture, std::vector<Dan>& danList, SDL_Texture* explosionTexture, 
             int& score, std::vector<Dan>& allEnemyBullets, Mix_Chunk* explosionSound, Mix_Chunk* bulletSound, std::vector<Item>& items) {
    for (auto& enemy : enemyList) {

        SDL_Texture* currentTexture;
        if (enemy.type == BASIC) {
            currentTexture = enemyTexture;
        } else if (enemy.type == FAST) {
            currentTexture = fastEnemyTexture;
        } else {
            currentTexture = strongEnemyTexture;
        }

        if (enemy.state == ACTIVE) {
            SDL_Rect enemyRect = { static_cast<int>(enemy.toaDo.x), static_cast<int>(enemy.toaDo.y), ENEMY_WIDTH, ENEMY_HEIGHT };
            SDL_RenderCopy(renderer, currentTexture, NULL, &enemyRect);

            if (enemy.movingDown) {
                enemy.toaDo.y += enemy.speed;
                if (enemy.toaDo.y >= 100) {
                    enemy.movingDown = false;
                    enemy.direction = (std::rand() % 2 == 0) ? -1 : 1;
                }
            } else {
                enemy.toaDo.x += enemy.speed * enemy.direction;
                if (enemy.toaDo.x <= 0 || enemy.toaDo.x >= SCREEN_WIDTH - ENEMY_WIDTH) {
                    enemy.direction *= -1;
                }
                if (std::rand() % 10000 < 5) {
                    enemy.danList.push_back({ {enemy.toaDo.x + 44, enemy.toaDo.y + ENEMY_HEIGHT}, true });
                    Mix_PlayChannel(-1, bulletSound, 0);
                }
            }

            // Check collision with player's bullets
            for (auto& dan : danList) {
                if (dan.active && kiemTraVaCham(dan.toaDo, BULLET_WIDTH, BULLET_HEIGHT, enemy.toaDo, ENEMY_WIDTH, ENEMY_HEIGHT)) {
                    enemy.state = EXPLODING;
                    dan.active = false;
                    enemy.explosionFrame = 0;
                    score += 10; // Increase score by 10 when an enemy is destroyed
                    Mix_PlayChannel(-1, explosionSound, 0); // Play explosion sound

                    if (enemy.type == STRONG) {
                        // Tạo ra đồ rơi khi kẻ địch STRONG bị tiêu diệt
                        Item item;
                        item.toaDo = enemy.toaDo;
                        item.active = true;
                        item.fallSpeed = 0.2;
                        item.type = INVINCIBLE_ITEM;
                        items.push_back(item);
                    }

                    if (enemy.type == FAST) {
                        // Tạo ra đồ rơi khi kẻ địch STRONG bị tiêu diệt
                        Item item;
                        item.toaDo = enemy.toaDo;
                        item.active = true;
                        item.fallSpeed = 0.3;
                        item.type = RAPID_FIRE_ITEM;
                        items.push_back(item);
                    }

                    

                    break; // Exit the loop since this enemy is now exploding
                }
            }

            veDanEnemy(renderer, danTexture, enemy.danList);
        } else if (enemy.state == EXPLODING) {
            SDL_Rect explosionRect = { static_cast<int>(enemy.toaDo.x), static_cast<int>(enemy.toaDo.y), ENEMY_WIDTH, ENEMY_HEIGHT };
            SDL_RenderCopy(renderer, explosionTexture, NULL, &explosionRect);

            allEnemyBullets.insert(allEnemyBullets.end(), enemy.danList.begin(), enemy.danList.end());
            enemy.danList.clear();

            enemy.explosionFrame++;
            if (enemy.explosionFrame > 30) { // Assume 30 frames for explosion animation
                enemy.state = INACTIVE;
            }
        }
    }
}