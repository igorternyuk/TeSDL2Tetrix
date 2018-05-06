#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <string>

class Tetramino
{
public:
    enum class Type {I, J, L, O, S, T, Z};
    enum class Direction{LEFT, RIGHT};
    enum {NUM_BLOCKS = 4, NUM_SHAPES = 7};
    struct Block { int x, y; };
    explicit Tetramino(Type type = Type::I);
    void move(Direction dir);
    void rotate(Direction dir);
    void stepDown();
    inline auto x() const noexcept { return x_; }
    inline auto y() const noexcept { return y_; }
    inline auto type() const noexcept { return type_; }
    inline const std::array<Block, NUM_BLOCKS>& blocks() const noexcept {
        return blocks_; }
    const std::vector<std::string>& shape() const noexcept;
private:
    static struct Shapes
    {
        const std::vector<std::vector<std::vector<std::string>>> data
        {
            //I-shape
            {
                {
                  "    ",
                  "XXXX",
                  "    ",
                  "    "
                },
                {
                  " X   ",
                  " X   ",
                  " X   ",
                  " X   "
                },
                {
                  "    ",
                  "XXXX",
                  "    ",
                  "    "
                },
                {
                  " X   ",
                  " X   ",
                  " X   ",
                  " X   "
                }
            },
            //J-shape
            {
                {
                  " X ",
                  " X ",
                  "XX "
                },
                {
                  "X  ",
                  "XXX",
                  "   "
                },
                {
                  " XX",
                  " X ",
                  " X "
                },
                {
                  "   ",
                  "XXX",
                  "  X"
                }
            },
            //L-shape
            {
                {
                  " X ",
                  " X ",
                  " XX"
                },
                {
                  "   ",
                  "XXX",
                  "X  "
                },
                {
                  "XX ",
                  " X ",
                  " X "
                },
                {
                  "  X",
                  "XXX",
                  "   "
                }
            },
            //O-shape
            {
              {
                "XX",
                "XX"
              }
            },
            //S-shape
            {
                {
                  " XX",
                  "XX ",
                  "   "
                },
                {
                  "X  ",
                  "XX ",
                  " X "
                },
                {
                  " XX",
                  "XX ",
                  "   "
                },
                {
                  " X ",
                  " XX",
                  "  X"
                }
            },
            //T-shape
            {
                {
                  "XXX",
                  " X ",
                  "   "
                },
                {
                  "  X",
                  " XX",
                  "  X"
                },
                {
                  "   ",
                  " X ",
                  "XXX"
                },
                {
                  "X  ",
                  "XX ",
                  "X  "
                }
            },
            //Z-shape
            {
                {
                  "XX ",
                  " XX",
                  "   "
                },
                {
                  "  X",
                  " XX",
                  " X "
                },
                {
                  "   ",
                  "XX ",
                  " XX"
                },
                {
                  " X ",
                  "XX ",
                  "X  "
                }
            }
        };
    } shapes_;
    Type type_;
    int angle_; // 0 - 0deg, 1- 90deg, 2 - 180deg, 3 - 270deg
    int x_, y_;
    std::array<Block, NUM_BLOCKS> blocks_;
    void updateBlocks();
};
