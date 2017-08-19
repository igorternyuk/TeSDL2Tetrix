#include "well.h"
#include <iostream>

Well::Well(): data_{false}
{
    //Bottom and side walls of the well
    clear();
#ifdef DEBUG
    print();
#endif
}

void Well::clear() noexcept
{
    for(auto y = 0; y <= HEIGHT; ++y)
    {
        for(auto x = 0; x < WIDTH; ++x)
            data_[y][x] = y == HEIGHT;
    }
}

void Well::print() const
{
    for(auto y = 0; y <= HEIGHT; ++y)
    {
        for(auto x = 0; x < WIDTH; ++x)
        {
            char ch = data_[y][x] ? 'X' : ' ';
            std::cout << ch;
        }
        std::cout << std::endl;
    }
}
