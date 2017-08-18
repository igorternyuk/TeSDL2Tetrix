#ifndef MODEL_H
#define MODEL_H

#include <list>
#include "well.h"
#include "tetramino.h"

class ModelListener;

class Model
{
public:
    enum class GameState{PLAY, PAUSE, GAME_OVER};
    enum Scores{ONE_ROW = 100, TWO_ROWS = 300, THREE_ROWS = 500, FOUR_ROWS = 1000};
    explicit Model();
    Model(const Model&) = delete;
    Model(Model&&) = delete;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;
    inline int getScore() const { return score_; }
    inline int getNumRemovedLines() const { return numRemovedLines_; }
    inline int getLevel() const { return level_; }
    inline float calcSpeed() const { return 3.0f / (2.0f + level_); }
    inline GameState getGameState() const { return gameState_; }
    inline int getWellWidth() const { return Well::WIDTH; }
    inline int getWellHeight() const { return Well::HEIGHT; }
    inline bool isWellCellSolid(int x, int y) const { return well_.isCellSolid(x, y); }
    inline const Tetramino& getActiveTetromino() const { return tetraminoActive_; }
    inline const Tetramino& getNextTetromino() const { return tetraminoNext_; }
    void move(Tetramino::Direction dir);
    void rotate(Tetramino::Direction dir);
    void step();
    void drop();
    void startNewGame();
    void togglePause();
    void addListener(ModelListener * const listener);
    void removeListener(ModelListener * const listener);
private:
    enum {NEXT_LEVEL = 500};
    Well well_;
    Tetramino tetraminoActive_, tetraminoNext_;
    GameState gameState_{GameState::PLAY};
    int score_{0};
    int numRemovedLines_{0};
    int level_{1};
    std::list<ModelListener*> listeners_;
    void checkFilling();
    void uniteTetraminoAndWell(const Tetramino &tetramino);
    bool isOverlapping(const Tetramino &tetramino);
    bool isBottomTouch(const Tetramino &tetramino);
    void notifyAllListeners();
};

#endif // MODEL_H
