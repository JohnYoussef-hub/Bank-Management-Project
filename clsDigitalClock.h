/*#pragma once
#include <iostream>
#include <Windows.h>
#include "clsDate.h"

using namespace std;


class clsDigitalClock
{
private:
    static short _Hours;
    static short _Minutes;
    static short _Seconds;

public:
    static void ShowSystemClock()
    {

        _Hours = clsDate::GetSystemHours();
        _Minutes = clsDate::GetSystemMinutes();
        _Seconds = clsDate::GetSystemSeconds();

        while(true)
        {
            system("cls");
            printf("%02d : %02d : %02d ", _Hours, _Minutes, _Seconds);
            _Seconds++;

            if(_Seconds > 59)
            {
                _Seconds = 0;
                _Minutes++;
            }
            if(_Minutes > 59)
            {
                _Minutes = 0;
                _Hours++;
            }
            if(_Hours > 23)
            {
                _Hours = 0;
            }
            Sleep(1000);
        }
    }
};

short clsDigitalClock::_Hours = 0;
short clsDigitalClock::_Minutes = 0;
short clsDigitalClock::_Seconds = 0;*/

#pragma once
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "clsDate.h"

class clsDigitalClock
{
public:
    static void ShowSystemClock(int X, int Y)
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD position;
        position.X = X;
        position.Y = Y;

        while (true)
        {
            SetConsoleCursorPosition(console, position);

            SYSTEMTIME st;
            GetLocalTime(&st);

            std::cout << std::setfill('0')
                      << std::setw(2) << st.wHour << ":"
                      << std::setw(2) << st.wMinute << ":"
                      << std::setw(2) << st.wSecond;

            Sleep(1000);
        }
    }
};