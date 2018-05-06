#include "tetramino.hpp"

Tetramino::Tetramino(Type type):
    type_(type),angle_{0},
    x_{type == Type::I || type == Type::L ? 3 : 4},
    y_{type == Type::J || type == Type::L ? -3 : -2}
{
    updateBlocks();
}

Tetramino::Shapes Tetramino::shapes_;

void Tetramino::move(Tetramino::Direction dir)
{
    switch(dir)
    {
        case Direction::LEFT:
            --x_;
            break;
        case Direction::RIGHT:
            ++x_;
            break;
        default:
            break;
    }
    updateBlocks();
}

void Tetramino::rotate(Tetramino::Direction dir)
{
    switch(dir)
    {
        case Direction::LEFT:
            --angle_;
            if(angle_ < 0)
                angle_ = 3;
            break;
        case Direction::RIGHT:
            ++angle_;
            break;
        default:
            break;
    }
    angle_ %= 4;
    updateBlocks();
}

void Tetramino::stepDown()
{
    ++y_;
    updateBlocks();
}

const std::vector<std::__cxx11::string> &Tetramino::shape() const noexcept
{
    auto pos = type_ == Type::O ? 0 : angle_;
    return shapes_.data.at(static_cast<int>(type_)).at(pos);
}

void Tetramino::updateBlocks()
{
    int index{0};
    for(unsigned short int y = 0; y < shape().size(); ++y)
    {
        for(unsigned short int x = 0; x < shape()[y].size(); ++x)
        {
            if(shape()[y][x] == 'X')
            {
                blocks_[index].x = x_ + x;
                blocks_[index].y = y_ + y;
                ++index;
            }
        }
    }
}
