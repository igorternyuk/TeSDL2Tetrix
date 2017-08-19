#ifndef WELL_H
#define WELL_H
#include <SDL2/SDL.h>

class Well
{
public:
    enum {WIDTH = 10, HEIGHT = 20};
    explicit Well();
    inline bool isCoordsInRange(int x, int y) const noexcept { return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT + 1; }
    inline bool isCellSolid(int x, int y) const noexcept { return isCoordsInRange(x,y) && data_[y][x]; }
    inline void setCellState(int x, int y, bool isSolid = true) noexcept { if(isCoordsInRange(x,y)) data_[y][x] =isSolid; }
    void clear() noexcept;
private:
    bool data_[HEIGHT + 1][WIDTH];
    void print() const; //For debugging only
};

#endif // WELL_H
