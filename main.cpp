#include "model.hpp"
#include "controller.hpp"
#include "view.hpp"

#include <iostream>
#include <stdexcept>
#include <ctime>

int main()
{
    try
    {
        srand(time(0));
        Model model;
        Controller controller{&model};
        View view{&model, &controller};
        model.addListener(&view);
        view.run();
    }
    catch(std::runtime_error &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}
