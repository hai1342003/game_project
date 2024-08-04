#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "inc/SDL.h"
#include "inc/SDL_image.h"
#include "inc/SDL_ttf.h"

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

enum EnemyState {
    ACTIVE,
    EXPLODING,
    INACTIVE
};

struct Enemy {
    ToaDo toaDo;
    double speed;
    EnemyState state;
    bool movingDown;
    int direction;
    int explosionFrame;
    std::vector<Dan> danList;
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

void veMayBay(SDL_Renderer* renderer, SDL_Texture* mayBayTexture, ToaDo toaDo) {
    SDL_Rect mayBay = { static_cast<int>(toaDo.x), static_cast<int>(toaDo.y), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopy(renderer, mayBayTexture, NULL, &mayBay);
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

void veEnemy(SDL_Renderer* renderer, SDL_Texture* enemyTexture, std::vector<Enemy>& enemyList, SDL_Texture* danTexture, std::vector<Dan>& danList, SDL_Texture* explosionTexture, int& score, std::vector<Dan>& allEnemyBullets) {
    for (auto& enemy : enemyList) {
        if (enemy.state == ACTIVE) {
            SDL_Rect enemyRect = { static_cast<int>(enemy.toaDo.x), static_cast<int>(enemy.toaDo.y), ENEMY_WIDTH, ENEMY_HEIGHT };
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);

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
                }
            }

            // Check collision with player's bullets
            for (auto& dan : danList) {
                if (dan.active && kiemTraVaCham(dan.toaDo, BULLET_WIDTH, BULLET_HEIGHT, enemy.toaDo, ENEMY_WIDTH, ENEMY_HEIGHT)) {
                    enemy.state = EXPLODING;
                    dan.active = false;
                    enemy.explosionFrame = 0;
                    score += 10; // Increase score by 10 when an enemy is destroyed
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

void checkCollisionWithPlayer(const ToaDo& playerPos, std::vector<Enemy>& enemyList, int& playerLives) {
    for (auto& enemy : enemyList) {
        for (auto& dan : enemy.danList) {
            if (dan.active && kiemTraVaCham(dan.toaDo, BULLET_WIDTH, BULLET_HEIGHT, playerPos, PLAYER_WIDTH, PLAYER_HEIGHT)) {
                dan.active = false;
                playerLives--;
                if (playerLives <= 0) {
                    printf("Game Over! Player has lost all lives.\n");
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

bool isInsideButton(int x, int y, SDL_Rect buttonRect) {
    return (x > buttonRect.x) && (x < buttonRect.x + buttonRect.w) &&
           (y > buttonRect.y) && (y < buttonRect.y + buttonRect.h);
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); // Initialize SDL_ttf

    SDL_Window* window = SDL_CreateWindow("Airplane Shooting Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* mayBayTexture = taiAnh(renderer, "img/plane.png");
    SDL_Texture* danTexture = taiAnh(renderer, "img/bullet.png");
    SDL_Texture* enemyTexture = taiAnh(renderer, "img/enemy.png");
    SDL_Texture* explosionTexture = taiAnh(renderer, "img/explosion.png");
    SDL_Texture* heartTexture = taiAnh(renderer, "img/heart.png");
    SDL_Texture* gameOverTexture = taiAnh(renderer, "img/gameover.png");

    TTF_Font* font = TTF_OpenFont("font/VNI-Viettay.ttf", 28); // Load font
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    if (!mayBayTexture || !danTexture || !enemyTexture || !explosionTexture || !heartTexture || !gameOverTexture) {
        printf("Failed to load texture image!\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    bool quit = false;
    bool gameOver = false;
    SDL_Event e;
    ToaDo toaDoMayBay = { SCREEN_WIDTH / 2.0 - 50, SCREEN_HEIGHT - 100 };
    std::vector<Dan> danList;
    std::vector<Enemy> enemyList;


    std::vector<Dan> allEnemyBullets; // Global list for all enemy bullets
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int playerLives = 3;
    int score = 0;
    int explosionFrame = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
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
                        case SDLK_SPACE:
                            danList.push_back({ {toaDoMayBay.x + 44, toaDoMayBay.y - 20}, true });
                            break;
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
                } else if (isInsideButton(x, y, quitRect)) {
                    quit = true;
                }
            }
        }

        if (std::rand() % 30000 < 2) {
            double x = static_cast<double>(std::rand() % (SCREEN_WIDTH - ENEMY_WIDTH));
            enemyList.push_back({ {x, 0}, 0.2, ACTIVE, true, 0, 0 }); // Reduced speed further
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        if (!gameOver) {
            veMayBay(renderer, mayBayTexture, toaDoMayBay);
            veDan(renderer, danTexture, danList);
            veEnemy(renderer, enemyTexture, enemyList, danTexture, danList, explosionTexture, score, allEnemyBullets);
            veHearts(renderer, heartTexture, playerLives); // Render hearts


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

            checkCollisionWithPlayer(toaDoMayBay, enemyList, playerLives);

            if (playerLives <= 0) {
                explosionFrame++;
                if (explosionFrame < 30) {
                    SDL_Rect explosionRect = { static_cast<int>(toaDoMayBay.x), static_cast<int>(toaDoMayBay.y), PLAYER_WIDTH, PLAYER_HEIGHT };
                    SDL_RenderCopy(renderer, explosionTexture, NULL, &explosionRect);
                } else {
                    gameOver = true;
                }
            }
        } else {
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

    SDL_DestroyTexture(mayBayTexture);
    SDL_DestroyTexture(danTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(gameOverTexture);
    TTF_CloseFont(font); // Close the font
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit(); // Quit SDL_ttf
    SDL_Quit();

    return 0;
}
