#pragma once

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "clsScreen.h"
#include "clsUser.h"
#include "clsMainScreen.h"
#include "Global.h"

class clsLoginScreen : protected clsScreen
{
private:
    static void _Login()
    {
        bool LoginFailed = false;

        string Username, Password;

        for (int i = 2; i >= 0; i--)
        {
            cout << "Enter Username: ";
            cin >> Username;

            cout << "Enter Password: ";
            cin >> Password;

            CurrentUser = clsUser::Find(Username, Password);

            LoginFailed = CurrentUser.IsEmpty();

            if (!LoginFailed)
            {
                clsUser::AddToUserLogFile(CurrentUser);
                clsMainScreen::ShowMainMenue();
                break;
            }
            else
            {
                if (i > 0)
                {
                    cout << "\nInvalid Username/Password!\n";
                    cout << "You have " << i << " trial(s) to login!\n\n";
                }
                else
                {
                    cout << "\n\nYou are locked after 3 failed trials!\n\n\n";
                    exit(0);
                }
            }
        }
    }

public:
    static void ShowLoginScreen()
    {
        system("cls");
        _DrawScreenHeader("\t     Login Screen");
        _Login();
    }
};

