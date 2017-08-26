#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "well.h"
#include "tetramino.h"
#include "modellistener.h"

#include <array>
#include <map>
#include <string>

class Model;
class Controller;

using TextAndRect = std::pair<SDL_Texture*, SDL_Rect>;

class View : public ModelListener
{
public:
    explicit View(Model *pModel, Controller *pController);
    ~View();
    View(const View &) = delete;
    View &operator=(const View &) = delete;
    View(View &&) = delete;
    View &operator=(View &&) = delete;
    void run();
    virtual void updateView();
private:
    const char *TITLE_OF_PROGRAM = "TeSDL2Tetrix";
    const char *PATH_TO_FONT_SMALL = "Resources/Fonts/orbitron-light.ttf";
    const char *PATH_TO_FONT_LARGE = "Resources/Fonts/orbitron-medium.ttf";
    enum {
        WINDOW_WIDTH  = 700 / 2 + 6 * 35,
        WINDOW_HEIGHT = 700,
        WINDOW_X = (1366 - WINDOW_WIDTH) / 2,
        WINDOW_Y = (768 - WINDOW_HEIGHT) / 2,
        NEXT_TETRAMINO_PREVIEW_X = 390,
        NEXT_TETRAMINO_PREVIEW_Y = 70,
        BLOCK_SIZE = 35,
        FONT_SIZE_LARGE = 40,
        FONT_SIZE_SMALL = 32,
        NUM_DIGITS = 10
    };
    enum
    {
        TEXTURE_NEXT_PIECE,
        TEXTURE_SCORE,
        TEXTURE_LINES_REMOVED,
        TEXTURE_LEVEL,
        TEXTURE_PAUSE,
        TEXTURE_GAMEOVER
    };
    Model *pModel_;
    Controller *pController_;
    SDL_Window *pWindow_;
    SDL_Renderer *pRenderer_;
    TTF_Font *pFontLarge_, *pFontSmall_;
    std::map<int, TextAndRect> textures_;
    std::array<TextAndRect, NUM_DIGITS> texturesDigits_;
    bool running_{true}, draw_{true};
    float currTime_{0.f}, prevTime_{0.f}, diffTime_{0.f}, frameTime_{0.f};

    //Three phases of the game
    void inputPhase();
    //void updatePhase();
    void drawPhase();
    void loadFonts();
    void loadTextures();
    void drawWell() const;
    void setTetraminoColor(const Tetramino::Type &type) const;
    void drawTetramino(const Tetramino & tetramino) const;
    void drawActiveTetramino() const;
    void drawPreview() const;
    void drawGameInfo() const;
    void drawGameStatus() const;
    void getTextureAndRect(const char *text, TTF_Font *font, SDL_Color color,
                           SDL_Texture *&texture, SDL_Rect &rect);
    void drawNumber(int num, int posX, int posY, int step) const;
};
