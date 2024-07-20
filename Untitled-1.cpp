#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "inc/SDL.h"
#include "inc/SDL_image.h"

#undef main
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct ToaDo {
    double x, y;
};

struct Dan {
    ToaDo toaDo;
    bool active;
};

struct Enemy {
    ToaDo toaDo;
    double speed;
    bool active;
    bool movingDown;
    int direction;
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

void veMayBay(SDL_Renderer* renderer, SDL_Texture* mayBayTexture, ToaDo toaDo) {
    SDL_Rect mayBay = { static_cast<int>(toaDo.x), static_cast<int>(toaDo.y), 99, 99 };
    SDL_RenderCopy(renderer, mayBayTexture, NULL, &mayBay);
}

void veDan(SDL_Renderer* renderer, SDL_Texture* danTexture, std::vector<Dan>& danList) {
    for (auto& dan : danList) {
        if (dan.active) {
            SDL_Rect danRect = { static_cast<int>(dan.toaDo.x), static_cast<int>(dan.toaDo.y), 20, 40 };
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
            SDL_Rect danRect = { static_cast<int>(dan.toaDo.x), static_cast<int>(dan.toaDo.y), 20, 40 };
            SDL_RenderCopy(renderer, danTexture, NULL, &danRect);
            dan.toaDo.y += 0.2; // Slower bullet speed
            if (dan.toaDo.y > SCREEN_HEIGHT) {
                dan.active = false;
            }
        }
    }
}

void veEnemy(SDL_Renderer* renderer, SDL_Texture* enemyTexture, std::vector<Enemy>& enemyList, SDL_Texture* danTexture) {
    for (auto& enemy : enemyList) {
        if (enemy.active) {
            SDL_Rect enemyRect = { static_cast<int>(enemy.toaDo.x), static_cast<int>(enemy.toaDo.y), 99, 99 };
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);

            if (enemy.movingDown) {
                enemy.toaDo.y += enemy.speed;
                if (enemy.toaDo.y >= 100) {
                    enemy.movingDown = false;
                    enemy.direction = (std::rand() % 2 == 0) ? -1 : 1;
                }
            } else {
                enemy.toaDo.x += enemy.speed * enemy.direction;
                if (enemy.toaDo.x <= 0 || enemy.toaDo.x >= SCREEN_WIDTH - 99) {
                    enemy.direction *= -1;
                }
                if (std::rand() % 100000 < 5) {
                    enemy.danList.push_back({ {enemy.toaDo.x + 44, enemy.toaDo.y + 99}, true });
                }
            }
            veDanEnemy(renderer, danTexture, enemy.danList);
        }
    }
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Airplane Shooting Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* mayBayTexture = taiAnh(renderer, "img/plane.png");
    SDL_Texture* danTexture = taiAnh(renderer, "img/bullet.png");
    SDL_Texture* enemyTexture = taiAnh(renderer, "img/enemy.png");

    if (!mayBayTexture || !danTexture || !enemyTexture) {
        printf("Failed to load texture image!\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    bool quit = false;
    SDL_Event e;
    ToaDo toaDoMayBay = { SCREEN_WIDTH / 2.0 - 50, SCREEN_HEIGHT - 100 };
    std::vector<Dan> danList;
    std::vector<Enemy> enemyList;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
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
            }
        }

        if (std::rand() % 300000 < 2) {
            double x = static_cast<double>(std::rand() % (SCREEN_WIDTH - 99));
            enemyList.push_back({ {x, 0}, 0.05, true, true, 0 }); // Reduced speed further
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        veMayBay(renderer, mayBayTexture, toaDoMayBay);
        veDan(renderer, danTexture, danList);
        veEnemy(renderer, enemyTexture, enemyList, danTexture);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(mayBayTexture);
    SDL_DestroyTexture(danTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
