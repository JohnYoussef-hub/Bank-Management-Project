#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include "clsDate.h"
#include "clsUtil.h"
#include "Global.h"
#include <vector>
#include <fstream>

using namespace std;

class clsBankClient : public clsPerson
{
private:
    enum enMode
    {
        EmptyMode = 0,
        UpdateMode = 1,
        AddNewMode = 3
    };
    enMode _Mode;

    string _AccountNumber;
    string _PinCode;
    float _AccountBalance;
    bool _MarkedForDelete = false;

    static clsBankClient _ConvertLineToClientObject(string Line, string Seperator = "#//#")
    {
        vector<string> vClientData;
        vClientData = clsString::Split(Line, Seperator);

        return clsBankClient(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
                             vClientData[3], vClientData[4], clsUtil::DecryptText(vClientData[5]), stod(vClientData[6]));
    }

    static string _ConverClientObjectToLine(clsBankClient Client, string Seperator = "#//#")
    {
        string EncryptedPinCode = clsUtil::EncryptText(Client.GetPinCode(), 2);

        string stClientRecord = "";
        stClientRecord += Client.GetFirstName() + Seperator;
        stClientRecord += Client.GetLastName() + Seperator;
        stClientRecord += Client.GetEmail() + Seperator;
        stClientRecord += Client.GetPhone() + Seperator;
        stClientRecord += Client.AccountNumber() + Seperator;
        stClientRecord += EncryptedPinCode + Seperator;
        stClientRecord += to_string(Client.GetAccountBalance());

        return stClientRecord;
    }

    static clsBankClient _GetEmptyClientObject()
    {
        return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

    static vector<clsBankClient> _LoadClientsDataFromFile()
    {
        vector<clsBankClient> vClients;

        fstream MyFile;
        MyFile.open("ClientsData.txt", ios::in);

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Client = clsBankClient::_ConvertLineToClientObject(Line, "#//#");
                vClients.push_back(Client);
            }
            MyFile.close();
        }
        return vClients;
    }

    static void _AddDataLineToFile(string ClientData)
    {
        fstream MyFile;
        MyFile.open("ClientsData.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {
            MyFile << ClientData << endl;
            MyFile.close();
        }
    }

    static void _SaveCleintsDataToFile(vector<clsBankClient> &vClients)
    {
        fstream MyFile;
        MyFile.open("ClientsData.txt", ios::out);

        string DataLine;
        if (MyFile.is_open())
        {
            for (clsBankClient &C : vClients)
            {
                if (C._MarkedForDelete == false)
                {
                    DataLine = _ConverClientObjectToLine(C);
                    MyFile << DataLine << endl;
                }
            }
            MyFile.close();
        }
    }

    void _Update()
    {
        vector<clsBankClient> _vClients;
        _vClients = _LoadClientsDataFromFile();

        for (clsBankClient &C : _vClients)
        {
            if (C.AccountNumber() == this->AccountNumber())
            {
                C = *this;
            }
            _SaveCleintsDataToFile(_vClients);
        }
    }

    void _AddNew()
    {
        _AddDataLineToFile(_ConverClientObjectToLine(*this));
    }

    static string _ConvertTransferLogObjectToLine(clsBankClient Client1, clsBankClient Client2, double Amount, string Separator = "#//#")
    {
        clsDate Date;

        string DataLine = "";
        DataLine += Date.GetSystemDateTime() + Separator;
        DataLine += Client1.AccountNumber() + Separator;
        DataLine += Client2.AccountNumber() + Separator;
        DataLine += to_string(Amount) + Separator;
        DataLine += to_string(Client1.GetAccountBalance()) + Separator;
        DataLine += to_string(Client2.GetAccountBalance()) + Separator;
        DataLine += CurrentUser.GetUserName();

        return DataLine;
    }

public:
    clsBankClient(enMode Mode, string FirstName, string LastName,
                  string Email, string Phone, string AccountNumber, string PinCode,
                  float AccountBalance) : clsPerson(FirstName, LastName, Email, Phone)
    {
        _Mode = Mode;
        _AccountNumber = AccountNumber;
        _PinCode = PinCode;
        _AccountBalance = AccountBalance;
    }

    struct stTrnsferLogRecord
    {
        string DateTime;
        string SourceAccountNumber;
        string DestinationAccountNumber;
        float Amount;
        float srcBalanceAfter;
        float destBalanceAfter;
        string UserName;
    };

    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }

    string AccountNumber()
    {
        return _AccountNumber;
    }

    void SetPinCode(string PinCode)
    {
        _PinCode = PinCode;
    }

    string GetPinCode()
    {
        return _PinCode;
    }

    void SetAccountBalance(float AccountBalance)
    {
        _AccountBalance = AccountBalance;
    }

    float &GetAccountBalance()
    {
        return _AccountBalance;
    }

    void Print()
    {
        cout << "\nClient Card:";
        cout << "\n--------------------";
        cout << "\nFirstName   : " << GetFirstName();
        cout << "\nLastName    : " << GetLastName();
        cout << "\nFull Name   : " << FullName();
        cout << "\nEmail       : " << GetEmail();
        cout << "\nPhone       : " << GetPhone();
        cout << "\nAcc. Number : " << _AccountNumber;
        cout << "\nPassword    : " << _PinCode;
        cout << "\nBalance     : " << _AccountBalance;
        cout << "\n--------------------\n";
    }

    void PrintSmallCard()
    {
        cout << "\nClient Card:";
        cout << "\n--------------------";
        cout << "\nFull Name   : " << FullName();
        cout << "\nAcc. Number : " << _AccountNumber;
        cout << "\nBalance     : " << _AccountBalance;
        cout << "\n--------------------\n";
    }

    static clsBankClient Find(string AccountNumber)
    {
        fstream MyFile;
        MyFile.open("ClientsData.txt", ios::in);

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Client = _ConvertLineToClientObject(Line, "#//#");
                if (Client.AccountNumber() == AccountNumber)
                {
                    MyFile.close();
                    return Client;
                }
            }
        }
        return _GetEmptyClientObject();
    }

    static clsBankClient Find(string AccountNumber, string PinCode)
    {
        fstream MyFile;
        MyFile.open("ClientsData.txt", ios::in);

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Client = _ConvertLineToClientObject(Line, "#//#");
                if (Client.AccountNumber() == AccountNumber && Client.GetPinCode() == PinCode)
                {
                    MyFile.close();
                    return Client;
                }
            }
        }
        return _GetEmptyClientObject();
    }

    static bool IsClientExist(string AccountNumber)
    {
        clsBankClient Client = clsBankClient::Find(AccountNumber);
        return (!Client.IsEmpty());
    }

    enum enSaveResults
    {
        svFaildEmptyObject = 0,
        svSucceeded = 1,
        svFaildAccountNumberExists = 2
    };

    enSaveResults Save()
    {
        switch (_Mode)
        {
        case enMode::EmptyMode:
        {
            return enSaveResults::svFaildEmptyObject;
            break;
        }
        case enMode::UpdateMode:
        {
            _Update();
            _Mode = enMode::UpdateMode;
            return enSaveResults::svSucceeded;
            break;
        }
        case enMode::AddNewMode:
        {
            if (clsBankClient::IsClientExist(AccountNumber()))
            {
                return enSaveResults::svFaildAccountNumberExists;
            }
            else
            {
                _AddNew();
                _Mode = enMode::AddNewMode;
                return enSaveResults::svSucceeded;
                break;
            }
        }

        default:
            return enSaveResults::svFaildEmptyObject;
        }
    }

    static clsBankClient GetAddNewClientObject(string AccountNumber)
    {
        return clsBankClient(enMode::AddNewMode, "", "",
                             "", "", AccountNumber, "", 0);
    }

    bool Delete(char Answer)
    {
        if (Answer == 'y' || Answer == 'Y')
        {
            vector<clsBankClient> vClients;
            vClients = _LoadClientsDataFromFile();

            for (clsBankClient &C : vClients)
            {
                if (C.AccountNumber() == _AccountNumber)
                {
                    C._MarkedForDelete = true;
                    break;
                }
            }
            _SaveCleintsDataToFile(vClients);
            *this = _GetEmptyClientObject();
            return true;
        }
        else
        {
            return false;
        }
    }

    static vector<clsBankClient> GetClientsList()
    {
        return _LoadClientsDataFromFile();
    }

    static double GetTotalBalances()
    {
        double TotalBalances = 0;
        vector<clsBankClient> vClients = _LoadClientsDataFromFile();

        for (clsBankClient &C : vClients)
        {
            TotalBalances += C.GetAccountBalance();
        }
        return TotalBalances;
    }

    void Deposit(double Amount)
    {
        _AccountBalance += Amount;
        Save();
    }

    bool Withdraw(double Amount)
    {
        if (Amount > _AccountBalance)
        {
            return false;
        }
        else
        {
            _AccountBalance -= Amount;
            Save();
            return true;
        }
    }

    static void AddToTransferLog(clsBankClient Client1, clsBankClient Client2, double Amount, string Separator = "#//#")
    {
        string Datetime = clsDate::GetSystemDateTime();
        fstream MyFile;
        MyFile.open("TransferLog.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {
            string Line = _ConvertTransferLogObjectToLine(Client1, Client2, Amount);
            MyFile << Line << endl;
            MyFile.close();
        }
    }

    static stTrnsferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#")
    {
        stTrnsferLogRecord TrnsferLogRecord;

        vector <string> vTrnsferLogRecordLine = clsString::Split(Line, Seperator);
        TrnsferLogRecord.DateTime = vTrnsferLogRecordLine[0];
        TrnsferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
        TrnsferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
        TrnsferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
        TrnsferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
        TrnsferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
        TrnsferLogRecord.UserName = vTrnsferLogRecordLine[6];

        return TrnsferLogRecord;

    }

    static vector<stTrnsferLogRecord> GetTransfersLogList()
    {
        vector<stTrnsferLogRecord> vTransferLogRecord;

        fstream MyFile;
        MyFile.open("TransferLog.txt", ios::in); // read Mode

        if (MyFile.is_open())
        {

            string Line;

            stTrnsferLogRecord TransferRecord;

            while (getline(MyFile, Line))
            {

                TransferRecord = _ConvertTransferLogLineToRecord(Line);

                vTransferLogRecord.push_back(TransferRecord);
            }

            MyFile.close();
        }

        return vTransferLogRecord;
    }
};