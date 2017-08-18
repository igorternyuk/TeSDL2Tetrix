#ifndef CONTROLLER_H
#define CONTROLLER_H

class Model;

class Controller
{
public:
    explicit Controller(Model *pModel);
    Controller(const Controller &) = delete;
    Controller& operator=(const Controller &) = delete;
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();
    void step();
    void dropDown();
    void newGame();
    void togglePause();
private:
    Model *pModel_;
};

#endif // CONTROLLER_H
