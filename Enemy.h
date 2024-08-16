#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "ToaDo.h"
#include "Dan.h"
#include "Item.h"
#include "inc/SDL_mixer.h"

enum EnemyState {
    ACTIVE,
    EXPLODING,
    INACTIVE
};

enum EnemyType {
    BASIC,
    FAST,
    STRONG
};

struct Enemy {
    ToaDo toaDo;
    double speed;
    EnemyState state;
    bool movingDown;
    int direction;
    int explosionFrame;
    std::vector<Dan> danList;
    EnemyType type;
};

void spawnEnemy(std::vector<Enemy>& enemyList);
void veEnemy(SDL_Renderer* renderer, SDL_Texture* enemyTexture, SDL_Texture* fastEnemyTexture, SDL_Texture* strongEnemyTexture,
             std::vector<Enemy>& enemyList, SDL_Texture* danTexture, std::vector<Dan>& danList, SDL_Texture* explosionTexture, 
             int& score, std::vector<Dan>& allEnemyBullets, Mix_Chunk* explosionSound, Mix_Chunk* bulletSound, std::vector<Item>& items);

#endif
