#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsCurrency.h"
#include "clsInputValidate.h"

using namespace std;

class clsCurrencyCalculatorScreen : protected clsScreen
{
private:
    static void _PrintCurrency(clsCurrency Currency)
    {
        cout << "_____________________________\n";
        cout << "\nCountry    : " << Currency.Country();
        cout << "\nCode       : " << Currency.CurrencyCode();
        cout << "\nName       : " << Currency.CurrencyName();
        cout << "\nRate(1$) = : " << Currency.Rate();
        cout << "\n_____________________________\n";
    }

    static double _ConvertToOrFromUSD(clsCurrency Currency1, clsCurrency Currency2, double Amount)
    {
        if (Currency2.CurrencyCode() == "USD")
        {
            return (Amount / Currency1.Rate());
        }

        else if (Currency1.CurrencyCode() == "USD")
        {
            return (Amount * Currency2.Rate());
        }
        return 0;
    }

    static double _ConvertToUSD(clsCurrency Currency1, double Amount)
    {
        return Amount / Currency1.Rate();
    }

    static double _ConvertFromUSD(clsCurrency Currency1, double Amount)
    {
        return Amount * Currency1.Rate();
    }

public:
    static void ShowCurrencyCalculatorScreen()
    {
        _DrawScreenHeader("\tCurrency Calculator Screen");

        cout << "Enter Currency1 Code (From): ";
        string Code = clsInputValidate::ReadString();

        while (!clsCurrency::IsCurrencyExist(Code))
        {
            cout << "This currency code is not exist!";
            Code = clsInputValidate::ReadString();
        }

        clsCurrency Currency1 = clsCurrency::FindByCode(Code);
        cout << "\n";

        cout << "Enter Currency2 Code (To)  : ";
        Code = clsInputValidate::ReadString();

        while (!clsCurrency::IsCurrencyExist(Code))
        {
            cout << "This currency code is not exist!";
            Code = clsInputValidate::ReadString();
        }

        clsCurrency Currency2 = clsCurrency::FindByCode(Code);
        cout << "\n";

        cout << "Enter Amount To Exchange: ";
        double Amonut = clsInputValidate::ReadDblNumber();

        if (Currency1.CurrencyCode() == "USD" || Currency2.CurrencyCode() == "USD")
        {
            double AfterCalc = _ConvertToOrFromUSD(Currency1, Currency2, Amonut);

            if (Currency1.CurrencyCode() == "USD")
            {
                cout << "\n\nConvert To:\n";
                _PrintCurrency(Currency2);
            }
            else if (Currency2.CurrencyCode() == "USD")
            {
                cout << "\n\nConvert From:\n";
                _PrintCurrency(Currency1);
            }
            cout << "\n" << Amonut << " " << Currency1.CurrencyCode() << " = " << AfterCalc << " " << Currency2.CurrencyCode();
            cout << "\n";
        }
        else
        {
            cout << "\n\nConvert From:\n";
            _PrintCurrency(Currency1);
            double ToUSD = _ConvertToUSD(Currency1, Amonut);
            cout << "\n" << Amonut << " " << Currency1.CurrencyCode() << " = " << ToUSD << " USD\n";

            cout << "\nConvert To:\n";
            _PrintCurrency(Currency2);
            double FromUSD = _ConvertFromUSD(Currency2, ToUSD);
            cout << "\n"
                 << Amonut << " " << Currency1.CurrencyCode() << " = " << FromUSD << " " << Currency2.CurrencyCode();
        }

    }
};