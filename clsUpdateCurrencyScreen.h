#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsCurrency.h"
#include "clsInputValidate.h"
#include "clsUtil.h"

using namespace std;

class clsUpdateCurrencyScreen : protected clsScreen
{
private:
    static void _PrintCurrency(clsCurrency Currency)
    {
        cout << "\nCurrency Card:\n";
        cout << "_____________________________\n";
        cout << "\nCountry    : " << Currency.Country();
        cout << "\nCode       : " << Currency.CurrencyCode();
        cout << "\nName       : " << Currency.CurrencyName();
        cout << "\nRate(1$) = : " << Currency.Rate();
        cout << "\n_____________________________\n";
    }

    static void _UpdateCurrency(clsCurrency &Currency, double Rate)
    {
        Currency.UpdateRate(Rate);
    }

public:
    static void ShowUpdateCurrencyScreen()
    {

        _DrawScreenHeader("\tUpdate Currency Screen");

        cout << "\nEnter Currency Code: ";
        string Code = clsInputValidate::ReadString();

        while (!clsCurrency::IsCurrencyExist(Code))
        {
            cout << "This currency code is not exist!";
            Code = clsInputValidate::ReadString();
        }

        clsCurrency Currency = clsCurrency::FindByCode(Code);
        _PrintCurrency(Currency);

        char Answer;
        do
        {
            cout << "\nAre you sure you want to update this currency? (y/n)? ";
            cin >> Answer;
            if(Answer != 'y' && Answer != 'n')
            {
                cout << "Invalid Answer!";
                cout << "\nAre you sure you want to update this currency? (y/n)? ";
                cin >> Answer;
            }
        }while(Answer != 'y' && Answer != 'n');

        if(Answer == 'y')
        {
            cout << "\nUpdate Currency Rate:";
            cout << "\n======================";
            cout << "\n\nEnter New Currency Rate: ";
            double Rate = clsInputValidate::ReadDblNumber();

            _UpdateCurrency(Currency, Rate);

            _PrintCurrency(Currency);

            cout << "\nCurrency Rate Updated Successfully.";
        }

        else if(Answer == 'n')
        {
            cout << "\nCurrency Rate Not Updated!";
        }
    }
};