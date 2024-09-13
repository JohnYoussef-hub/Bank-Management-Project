#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsCurrency.h"
#include "clsInputValidate.h"
#include "clsUtil.h"

using namespace std;

class clsFindCurrencyScreen : protected clsScreen
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

public:
    static void ShowFindCurrencyScreen()
    {

        _DrawScreenHeader("\tFind Currency Screen");

        short Answer;
        cout << "You want to search by [1]Code or [2]Country: ";
        Answer = clsInputValidate::ReadShortNumberBetween(1, 2);

        switch (Answer)
        {
        case 1:
        {
            cout << "\nEnter Currency Code: ";
            string Code = clsInputValidate::ReadString();

            while (!clsCurrency::IsCurrencyExist(Code))
            {
                cout << "This currency code is not exist!";
                Code = clsInputValidate::ReadString();
            }

            clsCurrency Currency = clsCurrency::FindByCode(Code);
            _PrintCurrency(Currency);
            break;
        }

        case 2:
        {
            cout << "\nEnter Country Name: ";
            string Name = clsInputValidate::ReadString();

            clsCurrency Country = clsCurrency::FindByCountry(Name);

            while (Country.IsEmpty())
            {
                cout << "This Country Name is not exist!";
                Country = clsCurrency::FindByCountry(Name);
            }

            clsCurrency Currency = clsCurrency::FindByCountry(Name);
            _PrintCurrency(Currency);
            break;
        }
        }
    }
};