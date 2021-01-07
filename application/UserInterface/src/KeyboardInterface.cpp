#include "../include/KeyboardInterface.hpp"
#ifndef __linux__
#include <conio.h>
#endif
#ifdef __linux__
#include <curses.h>
#endif

namespace UserInterface
{
    KeyboardInterface::KeyboardInterface()
    {

    }

    KeyboardInterface::~KeyboardInterface()
    {

    }

    void KeyboardInterface::stop()
    {
        active = false;
    }

    void KeyboardInterface::update()
    {
        while(active)
        {  

            char ch = getch();
            printf("received input: %c\n", ch);
        }
    }


}