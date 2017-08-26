#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

class Model;

class Controller
{
public:
    explicit Controller(Model *pModel);
    ~Controller();
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller &) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller &&) = delete;
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();
    void step();
    void dropDown();
    void newGame();
    void togglePause();
    void stopTimerThread();
private:
    Model *pModel_;
    SDL_Thread *pThread_;
    bool isGameRunnig_{true};
    static int thread_func_wrapper (void* data);
    int thread_func (void);
};
