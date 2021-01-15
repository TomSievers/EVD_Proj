#include "../include/KeyboardInterface.hpp"

#ifndef __linux__
#include <conio.h>
#endif
#if defined(__linux__) && defined(HAVE_CURSES) 
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
        noecho();
        nodelay(stdscr,1);
#endif
        thread=std::thread(&KeyboardInterface::update, this);
    }

    KeyboardInterface::~KeyboardInterface()
    {
        stop();
        thread.join();
#if defined(__linux__) && defined(HAVE_CURSES)
        endwin();
#endif
    }

void KeyboardInterface::update()
    {
        int ch = -1;
        while(active)
        {  
            ch = -1;
            ch = getch();
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
                    case 110:
                    {
                        //n
                        callback(NEXT_CALIBRATION);
                    }
                    case 27:
                    {
                        //ESC
                        stop();
                    }
                    break;
                }
            }
            curKey = ch;
        }
    }

    void KeyboardInterface::stop()
    {
        active = false;
    }

    int KeyboardInterface::getCurKey()
    {
        return curKey;
    }


}