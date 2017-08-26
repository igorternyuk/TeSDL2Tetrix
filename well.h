#pragma once

class Well
{
public:
    enum {WIDTH = 10, HEIGHT = 20};
    explicit Well();
    inline auto isCoordsInRange(int x, int y) const noexcept {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT + 1; }
    inline auto isCellSolid(int x, int y) const noexcept {
        return isCoordsInRange(x,y) && data_[y][x]; }
    inline auto setCellState(int x, int y, bool isSolid = true) noexcept {
        if(isCoordsInRange(x,y)) data_[y][x] = isSolid; }
    void clear() noexcept;
private:
    bool data_[HEIGHT + 1][WIDTH];
#ifdef DEBUG
    void print() const; //For debugging only
#endif
};
