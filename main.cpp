#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "inc/SDL.h"
#include "inc/SDL_image.h"
#include "inc/SDL_ttf.h"
#include "inc/SDL_mixer.h"
#include "Collision.h"
#include "Constants.h"
#include "Dan.h"
#include "Enemy.h"
#include "Item.h"
#include "Graphics.h"
#include "GameState.h"
#include "Player.h"
#include "ToaDo.h"

#undef main


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
    // SDL_Texture* backgroundTexture = taiAnh(renderer, "img/background.png");
    SDL_Texture* itemInvincibleTexture = taiAnh(renderer, "img/khien.png");
    SDL_Texture* itemRapidFireTexture = taiAnh(renderer, "img/star.png");


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

        // SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
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
    // SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font); // Close the font  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit(); // Quit SDL_ttf
    Mix_CloseAudio(); // Close SDL_mixer
    SDL_Quit();

    return 0;
}
