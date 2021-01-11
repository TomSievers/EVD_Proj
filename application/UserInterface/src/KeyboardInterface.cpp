#include "../include/KeyboardInterface.hpp"


#if defined(__linux__) && defined(HAVE_CURSES) 
#include <curses.h>
#include <stdio.h>
#endif
#include <iostream>

namespace UserInterface
{
#if defined(_WIN32)
    KeyboardInterface::KeyboardInterface(std::function<void(const Event&)> callback) : IUserinterface(callback), curKey(-1), conIn(GetStdHandle(STD_INPUT_HANDLE))
#else
    KeyboardInterface::KeyboardInterface(std::function<void(const Event&)> callback) : IUserinterface(callback), curKey(-1)
#endif
    {
        active = true;
#if defined(__linux__) && defined(HAVE_CURSES) 
        initscr();
        nodelay(stdscr, true);
        noecho();
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
#if defined(_WIN32)
        DWORD nEvents;
        INPUT_RECORD input;
        KEY_EVENT_RECORD keyEvent;
#endif
        int ch;
        while(active)
        {  
            ch = -1;
#if defined(__linux__) && defined(HAVE_CURSES) 
            ch = getch();
#elif defined(_WIN32)
            ReadConsoleInput(conIn, &input, 1, &nEvents);
            if(input.EventType == KEY_EVENT)
            {
                keyEvent = (KEY_EVENT_RECORD&)input.Event;
                if(keyEvent.bKeyDown == TRUE)
                {
                    ch = keyEvent.uChar.AsciiChar;
                }
            }
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
                        //ESC
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