#include "model.h"
#include "modellistener.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

Model::Model()
{
    srand(time(0));
    tetraminoActive_= Tetramino(static_cast<Tetramino::Type>(rand() % Tetramino::NUM_SHAPES));
    tetraminoNext_= Tetramino(static_cast<Tetramino::Type>(rand() % Tetramino::NUM_SHAPES));
}

void Model::move(Tetramino::Direction dir)
{
    Tetramino tmp(tetraminoActive_);
    tmp.move(dir);
    if(!isOverlapping(tmp))
    {
        tetraminoActive_ = tmp;
        notifyAllListeners();
    }
}

void Model::rotate(Tetramino::Direction dir)
{
    Tetramino tmp(tetraminoActive_);
    tmp.rotate(dir);
    if(!isOverlapping(tmp))
    {
        tetraminoActive_ = tmp;
        notifyAllListeners();
    }
}

void Model::step()
{
    if(gameState_ == GameState::PLAY)
    {
        if(isBottomTouch(tetraminoActive_))
        {
            uniteTetraminoAndWell(tetraminoActive_);
            tetraminoActive_ = tetraminoNext_;
            tetraminoNext_ = Tetramino(static_cast<Tetramino::Type>(rand() % Tetramino::NUM_SHAPES));
            if(isBottomTouch(tetraminoNext_))
            {
                uniteTetraminoAndWell(tetraminoNext_);
                gameState_ = GameState::GAME_OVER;
                notifyAllListeners();
                return;
            }
            checkFilling();
        }
        else
        {
            tetraminoActive_.stepDown();
            notifyAllListeners();
        }
    }
}

void Model::drop()
{
    while(!isBottomTouch(tetraminoActive_))
        step();
}

void Model::startNewGame()
{
    score_ = 0;
    numRemovedLines_ = 0;
    level_ = 1;
    well_.clear();
    tetraminoActive_= Tetramino(static_cast<Tetramino::Type>(rand() % Tetramino::NUM_SHAPES));
    tetraminoNext_= Tetramino(static_cast<Tetramino::Type>(rand() % Tetramino::NUM_SHAPES));
    gameState_ = GameState::PLAY;
}

void Model::togglePause()
{
    if(gameState_ == GameState::PLAY)
        gameState_ = GameState::PAUSE;
    else if(gameState_ == GameState::PAUSE)
            gameState_ = GameState::PLAY;
    notifyAllListeners();
}

void Model::addListener(ModelListener * const listener)
{
    listeners_.push_back(listener);
}

void Model::removeListener(ModelListener * const listener)
{
    listeners_.remove(listener);
}

void Model::checkFilling()
{
    int counter = 0;
    for(auto y = 0; y < Well::HEIGHT; ++y)
    {
        bool isLineFilled{true};
        for(auto x = 0; x < getWellWidth(); ++x)
        {
            if(!well_.isCellSolid(x, y))
            {
                isLineFilled = false;
                break;
            }
        }
        if(isLineFilled)
        {
            ++counter;
            for(auto yy = y; yy > 0; --yy)
            {
                for(auto x = 0; x < getWellWidth(); ++x)
                {
                    well_.setCellState(x, yy, well_.isCellSolid(x, yy - 1));
                }
            }
            ++numRemovedLines_;
        }
    }
    int oldScore = score_;
    switch(counter)
    {
        case 1:
            score_ += ONE_ROW;
            break;
        case 2:
            score_ += TWO_ROWS;
            break;
        case 3:
            score_ += THREE_ROWS;
            break;
        case 4:
            score_ += FOUR_ROWS;
            break;
        default:
            break;
    }
    if(score_ - oldScore > 0 && (score_ / NEXT_LEVEL > level_))
    {
        ++level_;
#ifdef DEBUG
        std::cout << "score_ - oldScore = " << score_ - oldScore << std::endl;
        std::cout << "new level_= " << level_ << std::endl;
#endif

    }
#ifdef DEBUG
    std::cout << "score_= " << score_ << std::endl;
    std::cout << "level_= " << level_ << std::endl;
#endif
}

void Model::uniteTetraminoAndWell(const Tetramino &tetramino)
{
    std::array<Tetramino::Block, Tetramino::NUM_BLOCKS> blocks = tetramino.blocks();
    for(auto b: blocks)
        well_.setCellState(b.x, b.y);
}

bool Model::isOverlapping(const Tetramino &tetramino)
{
    std::array<Tetramino::Block, Tetramino::NUM_BLOCKS> blocks = tetramino.blocks();
    for(const auto &b: blocks)
        if(b.x < 0 || b.x >= Well::WIDTH || well_.isCellSolid(b.x, b.y))
            return true;
    return false;
}

bool Model::isBottomTouch(const Tetramino &tetramino)
{
    std::array<Tetramino::Block, Tetramino::NUM_BLOCKS> blocks = tetramino.blocks();
    for(const auto &b: blocks)
        if(well_.isCellSolid(b.x, b.y + 1))
            return true;
    return false;
}

void Model::notifyAllListeners()
{
    for(auto &l: listeners_)
        l->updateView();
}
