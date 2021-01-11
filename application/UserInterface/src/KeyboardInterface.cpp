#include "../include/KeyboardInterface.hpp"
#if defined(_WIN32)
#include <conio.h>
#elif defined(__linux__) && defined(HAVE_CURSES) 
#include <curses.h>
#include <stdio.h>
#endif
#include <iostream>

namespace UserInterface
{
    KeyboardInterface::KeyboardInterface(std::function<void(const Event&)> callback) : IUserinterface(callback), curKey(-1)
    {
        active = true;
#if defined(__linux__) && defined(HAVE_CURSES) 
        initscr();
        nodelay(stdscr, true);
        noecho();
#elif defined(_WIN32)

#endif
        thread = std::thread(&KeyboardInterface::update, this);
    }

    KeyboardInterface::~KeyboardInterface()
    {
#if defined(__linux__) && defined(HAVE_CURSES) 
        endwin();
#elif defined(_WIN32)

#endif
        stop();
        thread.join();
    }

    void KeyboardInterface::stop()
    {
        active = false;
    }

    void KeyboardInterface::update()
    {
        while(active)
        {  
            int ch = -1;
#if defined(__linux__) && defined(HAVE_CURSES) 
            ch = getch();
#elif defined(_WIN32)
            ch = _getch_nolock();
#endif
            if(callback)
            {
                switch(ch)
                {
                    case 97:
                    {
                        //A
                        callback(START);
                    }
                    break;
                    case 99:
                    {
                        //C
                        callback(CALIBRATE);
                    }
                    break;
                    case 115:
                    {
                        //S
                        callback(STOP);
                    }
                    break;
                    case 27:
                    {
                        stop();
                    }
                    break;
                }
            }
            curKey = ch;
        }
        
    }

    int KeyboardInterface::getCurKey()
    {
        return curKey;
    }


}