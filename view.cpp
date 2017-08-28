#include "view.h"

#include "model.h"
#include "controller.h"

#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>

View::View(Model *pModel, Controller *pController):
    pModel_(pModel), pController_(pController)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), NULL);
        const char *msg = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error(msg);
    }
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &pWindow_, &pRenderer_);
    SDL_SetWindowTitle(pWindow_, TITLE_OF_PROGRAM);
    SDL_SetWindowPosition(pWindow_, WINDOW_X, WINDOW_Y);
    TTF_Init();
    loadFonts();
    loadTextures();
}

View::~View()
{
    TTF_CloseFont(pFontLarge_);
    TTF_CloseFont(pFontSmall_);
    for(auto it = textures_.begin(); it != textures_.end(); ++it)
        SDL_DestroyTexture(it->second.first);
    SDL_DestroyRenderer(pRenderer_);
    SDL_DestroyWindow(pWindow_);
    TTF_Quit();
    SDL_Quit();
}

void View::run()
{
    //Main loop of the game
    while(running_)
    {
        inputPhase();
        //updatePhase(); - I use additional thread instead
        drawPhase();
    }
}

void View::updateView()
{
    draw_ = true;
}

void View::inputPhase()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                pController_->stopTimerThread();
                running_ = false;
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        pController_->rotateLeft();
                        break;
                    case SDLK_DOWN:
                        pController_->rotateRight();
                        break;
                    case SDLK_LEFT:
                        pController_->moveLeft();
                        break;
                    case SDLK_RIGHT:
                        pController_->moveRight();
                        break;
                    case SDLK_SPACE:
                        pController_->dropDown();
                        break;
                    case SDLK_n:
                        pController_->newGame();
                        break;
                    case SDLK_p:
                        pController_->togglePause();
                        break;
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        pController_->stopTimerThread();
                        running_ = false;
                        break;
                }
                break;
        }
    }
}

/*void View::updatePhase()
{
    prevTime_ = currTime_;
    currTime_ = SDL_GetTicks();
    diffTime_= (currTime_ - prevTime_) / 1000.0f;
    frameTime_ += diffTime_;
    if(frameTime_ >= pModel_->calcSpeed())
    {
#ifdef DEBUG
        std::cout << "Speed = " << pModel_->calcSpeed() << std::endl;
#endif
        pController_->step();
        frameTime_ = 0.f;
    }
}*/

void View::drawPhase()
{
    if(draw_)
    {
        SDL_SetRenderDrawColor(pRenderer_, 0,0,0,255);
        SDL_RenderClear(pRenderer_);
        //Drawing stuff
        drawWell();
        if(pModel_->getGameState() != Model::GameState::GAME_OVER)
            drawActiveTetramino();
        drawPreview();
        drawGameInfo();
        drawGameStatus();
        SDL_RenderPresent(pRenderer_);
        //Score int the window title
        std::stringstream ss;
        ss << "Tetrix score: " << pModel_->getScore();
#ifdef DEBUG
        std::cout << ss.str().c_str() << std::endl;
#endif
        SDL_SetWindowTitle(pWindow_, ss.str().c_str());
        draw_ = false;
    }
}

void View::loadFonts()
{
    pFontLarge_ = TTF_OpenFont(PATH_TO_FONT_LARGE, FONT_SIZE_LARGE);
    pFontSmall_ = TTF_OpenFont(PATH_TO_FONT_SMALL, FONT_SIZE_SMALL);
    if(pFontSmall_ == NULL || pFontLarge_ == NULL)
        throw std::runtime_error("Font file not found");
}

void View::loadTextures()
{
    getTextureAndRect("Next piece", pFontSmall_, {0, 180, 0, 255},
                               textures_[TEXTURE_NEXT_PIECE].first,
                               textures_[TEXTURE_NEXT_PIECE].second);
    textures_[TEXTURE_NEXT_PIECE].second.x = BLOCK_SIZE * pModel_->getWellWidth() + 10;
    textures_[TEXTURE_NEXT_PIECE].second.y = 15;
    getTextureAndRect("Score", pFontSmall_, {0, 127, 234, 255},
                               textures_[TEXTURE_SCORE].first,
                               textures_[TEXTURE_SCORE].second);
    textures_[TEXTURE_SCORE].second.x = BLOCK_SIZE * pModel_->getWellWidth() + 40;
    textures_[TEXTURE_SCORE].second.y = 210;
    getTextureAndRect("Level", pFontSmall_, {0, 255, 255, 255},
                               textures_[TEXTURE_LINES_REMOVED].first,
                               textures_[TEXTURE_LINES_REMOVED].second);
    textures_[TEXTURE_LINES_REMOVED].second.x = BLOCK_SIZE * pModel_->getWellWidth() + 40;
    textures_[TEXTURE_LINES_REMOVED].second.y = 310;
    getTextureAndRect("Lines", pFontSmall_, {255, 255, 255, 255},
                               textures_[TEXTURE_LEVEL].first,
                               textures_[TEXTURE_LEVEL].second);
    textures_[TEXTURE_LEVEL].second.x = BLOCK_SIZE * pModel_->getWellWidth() + 40;
    textures_[TEXTURE_LEVEL].second.y = 410;
    //Pause end game over
    getTextureAndRect("GAME OVER", pFontLarge_, {200, 0, 0, 255},
                               textures_[TEXTURE_GAMEOVER].first,
                               textures_[TEXTURE_GAMEOVER].second);
    textures_[TEXTURE_GAMEOVER].second.x = 35;
    textures_[TEXTURE_GAMEOVER].second.y = 350;
    getTextureAndRect("GAME PAUSED", pFontLarge_, {255, 255, 0, 255},
                               textures_[TEXTURE_PAUSE].first,
                               textures_[TEXTURE_PAUSE].second);
    textures_[TEXTURE_PAUSE].second.x = 5;
    textures_[TEXTURE_PAUSE].second.y = 350;
    //Numbers
    for(unsigned short int i = 0; i < texturesDigits_.size(); ++i)
    {
        getTextureAndRect(std::to_string(i).c_str(), pFontSmall_, {0, 255, 0, 255},
                          texturesDigits_[i].first, texturesDigits_[i].second);
        texturesDigits_[i].second.x = 0;
        texturesDigits_[i].second.y = 0;
    }
}

void View::drawWell() const
{
    SDL_SetRenderDrawColor(pRenderer_, 215, 230, 209, 0);
    SDL_Rect rect{0, 0, BLOCK_SIZE * pModel_->getWellWidth(), 700};
    SDL_RenderDrawRect(pRenderer_, &rect);
    for(auto y = 0; y < pModel_->getWellHeight(); ++y)
    {
        for(auto x = 0; x < pModel_->getWellWidth(); ++x)
        {
            if(pModel_->isWellCellSolid(x, y))
            {
                SDL_SetRenderDrawColor(pRenderer_, 215, 230, 209, 0);
                SDL_Rect rect{x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1,
                              BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                SDL_RenderFillRect(pRenderer_, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(pRenderer_, 204, 226, 125, 0);
                SDL_RenderDrawPoint(pRenderer_, x * BLOCK_SIZE, y * BLOCK_SIZE);
            }
        }
    }
}

void View::setTetraminoColor(const Tetramino::Type &type) const
{
    switch(type)
    {
        case Tetramino::Type::I:
            SDL_SetRenderDrawColor(pRenderer_, 255, 0, 0, 255);
            break;
        case Tetramino::Type:: J:
            SDL_SetRenderDrawColor(pRenderer_, 127, 127, 127, 255);
            break;
        case Tetramino::Type::L:
            SDL_SetRenderDrawColor(pRenderer_, 255, 0, 255, 255);
            break;
        case Tetramino::Type::O:
            SDL_SetRenderDrawColor(pRenderer_, 0, 0, 127, 255);
            break;
        case Tetramino::Type::S:
            SDL_SetRenderDrawColor(pRenderer_, 0, 127, 0, 255);
            break;
        case Tetramino::Type::T:
            SDL_SetRenderDrawColor(pRenderer_, 127, 64, 255, 255);
            break;
        case Tetramino::Type::Z:
            SDL_SetRenderDrawColor(pRenderer_, 0, 127, 127, 255);
            break;
    }
}

void View::drawTetramino(const Tetramino &tetramino) const
{
    setTetraminoColor(tetramino.type());
    std::array<Tetramino::Block, Tetramino::NUM_BLOCKS> blocks = tetramino.blocks();
    for(auto b: blocks)
    {
        SDL_Rect rect{b.x * BLOCK_SIZE + 1, b.y * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
        SDL_RenderFillRect(pRenderer_, &rect);
    }
}

void View::drawActiveTetramino() const
{
    Tetramino tmp = pModel_->getActiveTetromino();
    drawTetramino(tmp);
}

void View::drawPreview() const
{
    Tetramino tmp = pModel_->getNextTetromino();
    setTetraminoColor(tmp.type());
    for(unsigned short int  y = 0; y < tmp.shape().size(); ++y)
    {
        for(unsigned short int x = 0; x < tmp.shape()[y].size(); ++x)
        {
            if(tmp.shape()[y][x] == 'X')
            {
                SDL_Rect rect{NEXT_TETRAMINO_PREVIEW_X + x * BLOCK_SIZE + 1,
                              NEXT_TETRAMINO_PREVIEW_Y + y * BLOCK_SIZE + 1,
                              BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                SDL_RenderFillRect(pRenderer_, &rect);
            }
        }
    }
}

void View::drawGameInfo() const
{
    SDL_RenderCopy(pRenderer_, textures_.at(TEXTURE_NEXT_PIECE).first, NULL,
                   &textures_.at(TEXTURE_NEXT_PIECE).second);
    SDL_RenderCopy(pRenderer_, textures_.at(TEXTURE_SCORE).first, NULL,
                   &textures_.at(TEXTURE_SCORE).second);
    drawNumber(pModel_->getScore(), 390, 260, 25);
    SDL_RenderCopy(pRenderer_, textures_.at(TEXTURE_LEVEL).first, NULL,
                   &textures_.at(TEXTURE_LEVEL).second);
    drawNumber(pModel_->getLevel(), 390, 360, 25);
    SDL_RenderCopy(pRenderer_, textures_.at(TEXTURE_LINES_REMOVED).first, NULL,
                   &textures_.at(TEXTURE_LINES_REMOVED).second);
    drawNumber(pModel_->getNumRemovedLines(), 390, 460, 25);

}

void View::drawGameStatus() const
{
    switch(pModel_->getGameState())
    {
        case Model::GameState::PAUSE:
            SDL_RenderCopy(pRenderer_, textures_.at(TEXTURE_PAUSE).first, NULL,
                           &textures_.at(TEXTURE_PAUSE).second);
            break;
        case Model::GameState::GAME_OVER:
            SDL_RenderCopy(pRenderer_, textures_.at(TEXTURE_GAMEOVER).first, NULL,
                           &textures_.at(TEXTURE_GAMEOVER).second);
            break;
        default:
            break;
    }
}

void View::getTextureAndRect(const char *text, TTF_Font *font, SDL_Color color,
                             SDL_Texture *&texture, SDL_Rect &rect)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(pRenderer_, surface);
    rect.w = surface->w;
    rect.h = surface->h;
    SDL_FreeSurface(surface);
}

void View::drawNumber(int num, int posX, int posY, int step) const
{
    std::string numberAsString = std::to_string(num);
    for(unsigned short int i = 0; i < numberAsString.size(); ++i)
    {
        int index = std::atoi(numberAsString.substr(i, 1).c_str());
        SDL_Rect rect = texturesDigits_.at(index).second;
        rect.x = posX + i * step;
        rect.y = posY;
        SDL_RenderCopy(pRenderer_, texturesDigits_.at(index).first, NULL,
                       &rect);
    }
}
