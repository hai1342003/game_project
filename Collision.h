#ifndef COLLISION_H
#define COLLISION_H

#include "ToaDo.h"
#include "Enemy.h"
#include "Item.h"
#include "inc/SDL_mixer.h"


bool kiemTraVaCham(const ToaDo& toaDo1, int width1, int height1, const ToaDo& toaDo2, int width2, int height2);
void checkCollisionWithPlayer(const ToaDo& playerPos, std::vector<Enemy>& enemyList, int& playerLives, Mix_Chunk* explosionSound, bool playerInvincible);
void checkItemCollision(ToaDo playerPos, std::vector<Item>& items, bool& playerInvincible, double& invincibleTime, bool& rapidFire, double& rapidFireTime);

#endif
