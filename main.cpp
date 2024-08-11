#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "inc/SDL.h"
#include "inc/SDL_image.h"
#include "inc/SDL_ttf.h"
#include "inc/SDL_mixer.h"

#undef main
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_WIDTH = 99;
const int PLAYER_HEIGHT = 99;
const int BULLET_WIDTH = 20;
const int BULLET_HEIGHT = 40;
const int ENEMY_WIDTH = 99;
const int ENEMY_HEIGHT = 99;
const int HEART_WIDTH = 30;
const int HEART_HEIGHT = 30;
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 50;

struct ToaDo {
    double x, y;
};

struct Dan {
    ToaDo toaDo;
    bool active;
};

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


enum EnemyState {
    ACTIVE,
    EXPLODING,
    INACTIVE
};

// man hinh chinh
enum GameState {
    MAIN_MENU,
    PLAYING,
    GAME_OVER
};

// tính năng
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

SDL_Texture* taiAnh(SDL_Renderer* renderer, const char* file) {
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", file, IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void veMayBay(SDL_Renderer* renderer, SDL_Texture* mayBayTexture, ToaDo toaDo, SDL_Texture* itemInvincibleTexture, SDL_Texture* itemRapidFireTexture, bool playerInvincible, bool rapidFire) {
    SDL_Rect mayBay = { static_cast<int>(toaDo.x), static_cast<int>(toaDo.y), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopy(renderer, mayBayTexture, NULL, &mayBay);
    if (playerInvincible) {
        SDL_RenderCopy(renderer, itemInvincibleTexture, NULL, &mayBay); // Vẽ hình khiên đè lên máy bay

    }
    if (rapidFire) {
        SDL_RenderCopy(renderer, itemRapidFireTexture, NULL, &mayBay); // Vẽ hình khiên đè lên máy bay
    }
}

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

bool kiemTraVaCham(const ToaDo& toaDo1, int width1, int height1, const ToaDo& toaDo2, int width2, int height2) {
    return !(toaDo1.x > toaDo2.x + width2 ||
             toaDo1.x + width1 < toaDo2.x ||
             toaDo1.y > toaDo2.y + height2 ||
             toaDo1.y + height1 < toaDo2.y);
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

// tính năng
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
        speed = 0.3;
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

void checkCollisionWithPlayer(const ToaDo& playerPos, std::vector<Enemy>& enemyList, int& playerLives, Mix_Chunk* explosionSound, bool playerInvincible) {
    for (auto& enemy : enemyList) {
        for (auto& dan : enemy.danList) {
            if (dan.active && kiemTraVaCham(dan.toaDo, BULLET_WIDTH, BULLET_HEIGHT, playerPos, PLAYER_WIDTH, PLAYER_HEIGHT)) {
                
                dan.active = false;
                if (!playerInvincible) {
                    playerLives--;
                    Mix_PlayChannel(-1, explosionSound, 0);

                    if (playerLives <= 0) {
                        printf("Game Over! Player has lost all lives.\n");
                    }
                }
                break;
            }
        }
    }
}

void veHearts(SDL_Renderer* renderer, SDL_Texture* heartTexture, int playerLives) {
    for (int i = 0; i < playerLives; ++i) {
        SDL_Rect heartRect = { 10 + i * (HEART_WIDTH + 10), 10, HEART_WIDTH, HEART_HEIGHT };
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }
}

// man hinh chinh
void veMenuChinh(SDL_Renderer* renderer, TTF_Font* font) {
    // Tăng kích thước font và vẽ tên game
    TTF_Font* titleFont = TTF_OpenFont("font/VNI-Viettay.ttf", 72); // Tăng kích thước font lên 72

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Texture* titleTexture = createTextTexture(renderer, titleFont, "Airplane Shooting Game", textColor);

    int textW = 0, textH = 0;
    SDL_QueryTexture(titleTexture, NULL, NULL, &textW, &textH);

    SDL_Color shadowColor = { 0, 0, 0, 255 }; // Màu đen cho đổ bóng
    SDL_Texture* shadowTexture = createTextTexture(renderer, titleFont, "Airplane Shooting Game", shadowColor);
    SDL_Rect shadowRect = { SCREEN_WIDTH / 2 - textW / 2 + 5, SCREEN_HEIGHT / 4 + 5, textW, textH };
    SDL_RenderCopy(renderer, shadowTexture, NULL, &shadowRect);
    SDL_DestroyTexture(shadowTexture);

    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - textW / 2, SCREEN_HEIGHT / 4, textW, textH };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_DestroyTexture(titleTexture);

    TTF_CloseFont(titleFont); // Đóng phông chữ sau khi sử dụng

    // Vẽ nút Play
    SDL_Rect playRect = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_Color buttonColor = { 255, 0, 0, 255 };
    SDL_Texture* playTexture = createTextTexture(renderer, font, "Play", buttonColor);
    SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
    SDL_DestroyTexture(playTexture);
}

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
            SDL_RenderCopy(renderer, itemInvincibleTexture, NULL, &itemRect);
            item.toaDo.y += item.fallSpeed;
            if (item.toaDo.y > SCREEN_HEIGHT) {
                item.active = false; // Đồ biến mất khi rơi xuống khỏi màn hình
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




bool isInsideButton(int x, int y, SDL_Rect buttonRect) {
    return (x > buttonRect.x) && (x < buttonRect.x + buttonRect.w) &&
           (y > buttonRect.y) && (y < buttonRect.y + buttonRect.h);
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); // Initialize SDL_ttf
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // Initialize SDL_mixer

    SDL_Window* window = SDL_CreateWindow("Airplane Shooting Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* mayBayTexture = taiAnh(renderer, "img/plane.png");
    SDL_Texture* danTexture = taiAnh(renderer, "img/bullet.png");
    SDL_Texture* enemyTexture = taiAnh(renderer, "img/enemy.png");
    SDL_Texture* fastEnemyTexture = taiAnh(renderer, "img/fast_enemy.png");
    SDL_Texture* strongEnemyTexture = taiAnh(renderer, "img/strong_enemy.png");
    SDL_Texture* explosionTexture = taiAnh(renderer, "img/explosion.png");
    SDL_Texture* heartTexture = taiAnh(renderer, "img/heart.png");
    SDL_Texture* gameOverTexture = taiAnh(renderer, "img/gameover.png");
    SDL_Texture* backgroundTexture = taiAnh(renderer, "img/background.png");
    SDL_Texture* itemInvincibleTexture = taiAnh(renderer, "img/khien.png");
    SDL_Texture* itemRapidFireTexture = taiAnh(renderer, "img/bullet.png");


    TTF_Font* font = TTF_OpenFont("font/VNI-Viettay.ttf", 28); // Load font
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    if (!mayBayTexture || !danTexture || !enemyTexture ||!fastEnemyTexture || !strongEnemyTexture || !explosionTexture || !heartTexture || !gameOverTexture || !itemInvincibleTexture || !itemRapidFireTexture) {
        printf("Failed to load texture image!\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }




    Mix_Chunk* bulletSound = Mix_LoadWAV("sound/bullet.wav");
    Mix_Chunk* explosionSound = Mix_LoadWAV("sound/explosion.wav");

    bool quit = false;
    bool gameOver = false;
    SDL_Event e;
    ToaDo toaDoMayBay = { SCREEN_WIDTH / 2.0 - 50, SCREEN_HEIGHT - 100 };
    std::vector<Dan> danList;
    std::vector<Enemy> enemyList;

    std::vector<Enemy> fastEnemyList;
    std::vector<Enemy> strongEnemyList;


    std::vector<Dan> allEnemyBullets; // Global list for all enemy bullets
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int playerLives = 3;
    int score = 0;
    int explosionFrame = 0;
    GameState gameState = MAIN_MENU; // man hinh chinh

    Uint32 startTime = SDL_GetTicks(); // Lấy thời gian bắt đầu chơi
    Uint32 lastShotTime = 0;
    
    std::vector<Item> items;
    bool playerInvincible = false;
    double invincibleTime = 0;



    bool rapidFire = false;
    double rapidFireTime = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && gameState == PLAYING) {
                if (!gameOver) {
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:
                            toaDoMayBay.x -= 10.0;
                            break;
                        case SDLK_RIGHT:
                            toaDoMayBay.x += 10.0;
                            break;
                        case SDLK_UP:
                            toaDoMayBay.y -= 10.0;
                            break;
                        case SDLK_DOWN:
                            toaDoMayBay.y += 10.0;
                            break;
                        case SDLK_SPACE: {
                            Uint32 currentTime = SDL_GetTicks();
                            if (rapidFire || currentTime - lastShotTime >= 1000) {
                                danList.push_back({ {toaDoMayBay.x + 44, toaDoMayBay.y - 20}, true });
                                Mix_PlayChannel(-1, bulletSound, 0); // Play bullet sound
                                lastShotTime = currentTime;
                            }
                            break;
                        }
                    }
                } else {
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        // Play Again: Reset game
                        gameOver = false;
                        playerLives = 3;
                        score = 0;
                        toaDoMayBay = { SCREEN_WIDTH / 2.0 - 50, SCREEN_HEIGHT - 100 };
                        danList.clear();
                        enemyList.clear();
                        explosionFrame = 0;
                    } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (gameState == MAIN_MENU) {
                SDL_Rect playRect = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
                if (isInsideButton(x, y, playRect)) {
                    gameState = PLAYING;
                }
            } else if (gameState == GAME_OVER) {
                SDL_Rect playAgainRect = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT };
                SDL_Rect quitRect = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT };
                if (isInsideButton(x, y, playAgainRect)) {
                    // Play Again: Reset game
                    gameOver = false;
                    playerLives = 3;
                    score = 0;
                    toaDoMayBay = { SCREEN_WIDTH / 2.0 - 50, SCREEN_HEIGHT - 100 };
                    danList.clear();
                    enemyList.clear();
                    explosionFrame = 0;
                    gameState = PLAYING;
                } else if (isInsideButton(x, y, quitRect)) {
                    quit = true;
                }
            }
            }
        }


        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        // Tần suất kể địch xuất hiện tăng dần đều
        int spawnRate = 30000 - (elapsedTime / 100) * 50;
        if (spawnRate < 5000) spawnRate = 5000;

        if (std::rand() % spawnRate < 2) {
            spawnEnemy(enemyList);
        } 


        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        if (gameState == MAIN_MENU) {
            veMenuChinh(renderer, font);
        } else if (gameState == PLAYING) {
            veMayBay(renderer, mayBayTexture, toaDoMayBay, itemInvincibleTexture, itemRapidFireTexture, playerInvincible, rapidFire);
            veDan(renderer, danTexture, danList);
            veEnemy(renderer, enemyTexture, fastEnemyTexture, strongEnemyTexture, enemyList, danTexture, danList, explosionTexture, score, allEnemyBullets, explosionSound, bulletSound, items);
            veHearts(renderer, heartTexture, playerLives); // Render hearts
            veItems(renderer, itemInvincibleTexture, itemRapidFireTexture, items);
            checkItemCollision(toaDoMayBay, items, playerInvincible, invincibleTime, rapidFire, rapidFireTime);
            veDanEnemy(renderer, danTexture, allEnemyBullets); // Render all enemy bullets

            // Render score
            SDL_Color textColor = { 255, 255, 255, 255 };
            std::string scoreText = "Score: " + std::to_string(score);
            SDL_Texture* scoreTexture = createTextTexture(renderer, font, scoreText, textColor);
            int textW = 0, textH = 0;
            SDL_QueryTexture(scoreTexture, NULL, NULL, &textW, &textH);
            SDL_Rect scoreRect = { SCREEN_WIDTH - textW - 20, 20, textW, textH };
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            SDL_DestroyTexture(scoreTexture);

            if (playerInvincible) {
                invincibleTime -= 0.017; // Giảm thời gian bất tử
                if (invincibleTime <= 0) {
                    playerInvincible = false; // Hết thời gian, người chơi không còn bất tử
                }
            }

            if (rapidFire) {
                rapidFireTime -= 0.017; // Giảm thời gian bắn liên tục
                if (rapidFireTime <= 0) {
                    rapidFire = false; // Hết thời gian, người chơi không còn bắn được liên tục
                }
            }

            checkCollisionWithPlayer(toaDoMayBay, enemyList, playerLives, explosionSound, playerInvincible);
            if (playerLives <= 0) {
                explosionFrame++;
                if (explosionFrame < 30) {
                    SDL_Rect explosionRect = { static_cast<int>(toaDoMayBay.x), static_cast<int>(toaDoMayBay.y), PLAYER_WIDTH, PLAYER_HEIGHT };
                    SDL_RenderCopy(renderer, explosionTexture, NULL, &explosionRect);
                } else {
                    gameState = GAME_OVER;
                }
            }
        } else if (gameState == GAME_OVER) {
            SDL_Rect gameOverRect = { SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, 300, 100 };
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

            // Render final score
            SDL_Color textColor = { 255, 255, 255, 255 };
            std::string finalScoreText = "Score: " + std::to_string(score);
            SDL_Texture* finalScoreTexture = createTextTexture(renderer, font, finalScoreText, textColor);
            int textW = 0, textH = 0;
            SDL_QueryTexture(finalScoreTexture, NULL, NULL, &textW, &textH);
            SDL_Rect finalScoreRect = { SCREEN_WIDTH / 2 - textW / 2, SCREEN_HEIGHT / 2 + 50, textW, textH };
            SDL_RenderCopy(renderer, finalScoreTexture, NULL, &finalScoreRect);
            SDL_DestroyTexture(finalScoreTexture);

            // Render buttons
            SDL_Rect playAgainRect = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT };
            SDL_Rect quitRect = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT };

            SDL_Color buttonColor = { 255, 0, 0, 255 };
            SDL_Texture* playAgainTexture = createTextTexture(renderer, font, "Play Again", buttonColor);
            SDL_Texture* quitTexture = createTextTexture(renderer, font, "Quit", buttonColor);

            SDL_RenderCopy(renderer, playAgainTexture, NULL, &playAgainRect);
            SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

            SDL_DestroyTexture(playAgainTexture);
            SDL_DestroyTexture(quitTexture);
        }

        SDL_RenderPresent(renderer);
    }

    // Clean up sound effects
    Mix_FreeChunk(bulletSound);
    Mix_FreeChunk(explosionSound);

    SDL_DestroyTexture(mayBayTexture);
    SDL_DestroyTexture(danTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(fastEnemyTexture);
    SDL_DestroyTexture(strongEnemyTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(itemInvincibleTexture);
    SDL_DestroyTexture(itemRapidFireTexture);
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font); // Close the font  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit(); // Quit SDL_ttf
    Mix_CloseAudio(); // Close SDL_mixer
    SDL_Quit();

    return 0;
}
