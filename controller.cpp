#ifdef DEBUG
#include <iostream>
#endif
#include "controller.h"
#include "model.h"

Controller::Controller(Model *pModel):
    pModel_(pModel)
{
    pThread_ = SDL_CreateThread(thread_func_wrapper, NULL, this);
}

Controller::~Controller()
{
#ifdef DEBUG
    std::cout << "Controller destructor is working ... " << std::endl;
#endif
    SDL_WaitThread(pThread_, NULL);
}

void Controller::moveLeft()
{
    pModel_->move(Tetramino::Direction::LEFT);
}

void Controller::moveRight()
{
    pModel_->move(Tetramino::Direction::RIGHT);
}

void Controller::rotateLeft()
{
    pModel_->rotate(Tetramino::Direction::LEFT);
}

void Controller::rotateRight()
{
    pModel_->rotate(Tetramino::Direction::RIGHT);
}

void Controller::step()
{
    pModel_->step();
}

void Controller::dropDown()
{
    pModel_->drop();
}

void Controller::newGame()
{
    pModel_->startNewGame();
}

void Controller::togglePause()
{
    pModel_->togglePause();
}

void Controller::stopTimerThread()
{
    isGameRunnig_ = false;
}

int Controller::thread_func_wrapper(void *data)
{
    Controller *self = static_cast<Controller*>(data);
    return self->thread_func();
}

int Controller::thread_func()
{
    while(isGameRunnig_)
    {
#ifdef DEBUG
        std::cout << "Tetramino timer thread is working ... " << std::endl;
#endif
        SDL_Delay(pModel_->calcSpeed() * 1000.f);
        if(pModel_->getGameState() == Model::GameState::PLAY)
            pModel_->step();
    }
    return 0;
}
