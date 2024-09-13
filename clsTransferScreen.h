#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsUser.h"
#include "clsBankClient.h"
#include "clsInputValidate.h"

using namespace std;

class clsTransferScreen : protected clsScreen
{
public:
    static void ShowTransferScreen()
    {
        _DrawScreenHeader("\tTransfer Screen");

        string AccountNumber1 = "";

        cout << "\nPlease Enter Account Number to transfer from: ";
        AccountNumber1 = clsInputValidate::ReadString();

        while (!clsBankClient::IsClientExist(AccountNumber1))
        {
            cout << "\nAccount number is not found, choose another one: ";
            AccountNumber1 = clsInputValidate::ReadString();
        }

        clsBankClient Client1 = clsBankClient::Find(AccountNumber1);

        Client1.PrintSmallCard();
        cout<<"\n\n";

        string AccountNumber2 = "";

        cout << "\nPlease Enter Account Number to transfer to: ";
        AccountNumber2 = clsInputValidate::ReadString();

        while (!clsBankClient::IsClientExist(AccountNumber2) || AccountNumber1 == AccountNumber2)
        {
            cout << "\nAccount number is not found, choose another one: ";
            AccountNumber2 = clsInputValidate::ReadString();
        }

        clsBankClient Client2 = clsBankClient::Find(AccountNumber2);

        Client2.PrintSmallCard();
        cout<<"\n\n";

        double Amount = 0;

        cout << "\nPlease Enter Transfer Amount? ";

        Amount = clsInputValidate::ReadDblNumber();
        while (Amount > Client1.GetAccountBalance())
        {
            cout << "\nAmount Exceeds Balance, Enter Another Amount? ";
            Amount = clsInputValidate::ReadDblNumber();
        }

        cout << "\nTransfer " << Amount << " from " << AccountNumber1 << " to " << AccountNumber2 << " ? (y/n) ";
        char Answer = 'n';
        cin >> Answer;
        if (Answer == 'n' || Answer == 'N')
        {
            cout << "\nCanceled.\n";
            return;
        }

        Client1.Withdraw(Amount);
        Client2.Deposit(Amount);

        clsBankClient::AddToTransferLog(Client1, Client2, Amount);

        cout << "\nTransfer Done Successfully.\n";
        cout << "\nNew Balances:\n";

        Client1.PrintSmallCard();
        Client2.PrintSmallCard();
        cout<<"\n\n";
    }
};