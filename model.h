#pragma once

#include "well.h"
#include "tetramino.h"

#include <list>

class ModelListener;

class Model
{
public:
    enum class GameState{PLAY, PAUSE, GAME_OVER};
    enum Scores{ONE_ROW = 100, TWO_ROWS = 500, THREE_ROWS = 700, FOUR_ROWS = 1500};
    explicit Model();
    Model(const Model&) = delete;
    Model(Model&&) = delete;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;
    inline auto getScore() const noexcept { return score_; }
    inline auto getNumRemovedLines() const noexcept { return numRemovedLines_; }
    inline auto getLevel() const noexcept { return level_; }
    inline auto calcSpeed() const noexcept { return 3.0f / (2.0f + level_); }
    inline auto getGameState() const noexcept { return gameState_; }
    inline auto getWellWidth() const noexcept { return Well::WIDTH; }
    inline auto getWellHeight() const noexcept { return Well::HEIGHT; }
    inline auto isWellCellSolid(int x, int y) const noexcept { return well_.isCellSolid(x, y); }
    inline const Tetramino& getActiveTetromino() const noexcept { return tetraminoActive_; }
    inline const Tetramino& getNextTetromino() const noexcept { return tetraminoNext_; }
    void move(Tetramino::Direction dir);
    void rotate(Tetramino::Direction dir);
    void step();
    void drop();
    void startNewGame();
    void togglePause();
    void addListener(ModelListener* const listener);
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
