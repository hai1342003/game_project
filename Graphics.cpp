#include "Graphics.h"
#include "ToaDo.h"
#include "Enemy.h"
#include "Item.h"
#include "Dan.h"
#include "Constants.h"
#include "inc/SDL_image.h"
#include "Collision.h"


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

bool isInsideButton(int x, int y, SDL_Rect buttonRect) {
    return (x > buttonRect.x) && (x < buttonRect.x + buttonRect.w) &&
           (y > buttonRect.y) && (y < buttonRect.y + buttonRect.h);
}