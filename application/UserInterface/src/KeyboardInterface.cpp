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
#ifdef __linux__
        initscr();
        noecho();
        timeout(-1);
#endif
        thread=std::thread(&KeyboardInterface::update, this);
    }

    KeyboardInterface::~KeyboardInterface()
    {
        stop();
        thread.join();
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