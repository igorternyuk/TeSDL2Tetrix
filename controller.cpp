#include "controller.h"
#include "model.h"

Controller::Controller(Model *pModel):
    pModel_(pModel)
{}

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
