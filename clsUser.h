#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include "clsDate.h"
#include "clsUtil.h"
#include <vector>
#include <fstream>

using namespace std;
class clsUser : public clsPerson
{
private:
    enum enMode
    {
        EmptyMode = 0,
        UpdateMode = 1,
        AddNewMode = 2
    };
    enMode _Mode;
    string _UserName;
    string _Password;
    int _Permissions;

    bool _MarkedForDelete = false;

    static clsUser _ConvertLinetoUserObject(string Line, string Seperator = "#//#")
    {
        vector<string> vUserData;
        vUserData = clsString::Split(Line, Seperator);

        return clsUser(enMode::UpdateMode, vUserData[0], vUserData[1], vUserData[2],
                       vUserData[3], vUserData[4], clsUtil::DecryptText(vUserData[5]), stoi(vUserData[6]));
    }

    static string _ConverUserObjectToLine(clsUser User, string Seperator = "#//#")
    {
        string EncryptedPassword = clsUtil::EncryptText(User.GetPassword());

        string UserRecord = "";
        UserRecord += User.GetFirstName() + Seperator;
        UserRecord += User.GetLastName() + Seperator;
        UserRecord += User.GetEmail() + Seperator;
        UserRecord += User.GetPhone() + Seperator;
        UserRecord += User.GetUserName() + Seperator;
        UserRecord += EncryptedPassword + Seperator;
        UserRecord += to_string(User.GetPermissions());

        return UserRecord;
    }

    static string _ConvertObjectToLogFileLine(clsUser User, string Seperator = "#//#")
    {
        clsDate Date;

        string EncryptedPassword = clsUtil::EncryptText(User.GetPassword());

        string LogLine = "";
        LogLine += Date.GetSystemDateTime();
        LogLine += Seperator + User.GetUserName() + Seperator + EncryptedPassword;
        LogLine += Seperator + to_string(User.GetPermissions());

        return LogLine;
    }

    static vector<clsUser> _LoadUsersDataFromFile()
    {

        vector<clsUser> vUsers;

        fstream MyFile;
        MyFile.open("Users.txt", ios::in); // read Mode

        if (MyFile.is_open())
        {

            string Line;

            while (getline(MyFile, Line))
            {

                clsUser User = _ConvertLinetoUserObject(Line);

                vUsers.push_back(User);
            }

            MyFile.close();
        }

        return vUsers;
    }

    

    static void _SaveUsersDataToFile(vector<clsUser> vUsers)
    {

        fstream MyFile;
        MyFile.open("Users.txt", ios::out); // overwrite

        string DataLine;

        if (MyFile.is_open())
        {

            for (clsUser U : vUsers)
            {
                if (U.MarkedForDeleted() == false)
                {
                    // we only write records that are not marked for delete.
                    DataLine = _ConverUserObjectToLine(U);
                    MyFile << DataLine << endl;
                }
            }

            MyFile.close();
        }
    }

    void _Update()
    {
        vector<clsUser> _vUsers;
        _vUsers = _LoadUsersDataFromFile();

        for (clsUser &U : _vUsers)
        {
            if (U.GetUserName() == _UserName)
            {
                U = *this;
                break;
            }
        }

        _SaveUsersDataToFile(_vUsers);
    }

    void _AddNew()
    {

        _AddDataLineToFile(_ConverUserObjectToLine(*this));
    }

    void _AddDataLineToFile(string stDataLine)
    {
        fstream MyFile;
        MyFile.open("Users.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }
    }

    static clsUser _GetEmptyUserObject()
    {
        return clsUser(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }

public:
    enum enPermissions
    {
        eAll = -1,
        pListClients = 1,
        pAddNewClient = 2,
        pDeleteClient = 4,
        pUpdateClients = 8,
        pFindClient = 16,
        pTranactions = 32,
        pManageUsers = 64,
        pLogFile = 128
    };

    struct stLogFileInfo
    {
        string DateTime;
        string UserName;
        string Password;
        int Permissions;
    };

    clsUser(enMode Mode, string FirstName, string LastName,
            string Email, string Phone, string UserName, string Password,
            int Permissions) : clsPerson(FirstName, LastName, Email, Phone)

    {
        _Mode = Mode;
        _UserName = UserName;
        _Password = Password;
        _Permissions = Permissions;
    }

    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }

    bool MarkedForDeleted()
    {
        return _MarkedForDelete;
    }

    string GetUserName()
    {
        return _UserName;
    }

    void SetUserName(string UserName)
    {
        _UserName = UserName;
    }

    void SetPassword(string Password)
    {
        _Password = Password;
    }

    string GetPassword()
    {
        return _Password;
    }

    void SetPermissions(int Permissions)
    {
        _Permissions = Permissions;
    }

    int GetPermissions()
    {
        return _Permissions;
    }

    static clsUser Find(string UserName)
    {
        fstream MyFile;
        MyFile.open("Users.txt", ios::in); // read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsUser User = _ConvertLinetoUserObject(Line);
                if (User.GetUserName() == UserName)
                {
                    MyFile.close();
                    return User;
                }
            }

            MyFile.close();
        }

        return _GetEmptyUserObject();
    }

    static clsUser Find(string UserName, string Password)
    {
        
        fstream MyFile;
        MyFile.open("Users.txt", ios::in); // read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                //string EncryptedPassword = clsUtil::EncryptText(Password);

                clsUser User = _ConvertLinetoUserObject(Line);
                if (User.GetUserName() == UserName && User.GetPassword() == Password)
                {
                    MyFile.close();
                    return User;
                }
            }

            MyFile.close();
        }
        return _GetEmptyUserObject();
    }

    enum enSaveResults
    {
        svFaildEmptyObject = 0,
        svSucceeded = 1,
        svFaildUserExists = 2
    };

    enSaveResults Save()
    {

        switch (_Mode)
        {
        case enMode::EmptyMode:
        {
            if (IsEmpty())
            {
                return enSaveResults::svFaildEmptyObject;
            }
        }

        case enMode::UpdateMode:
        {
            _Update();
            return enSaveResults::svSucceeded;

            break;
        }

        case enMode::AddNewMode:
        {
            // This will add new record to file or database
            if (clsUser::IsUserExist(_UserName))
            {
                return enSaveResults::svFaildUserExists;
            }
            else
            {
                _AddNew();
                // We need to set the mode to update after add new
                _Mode = enMode::UpdateMode;
                return enSaveResults::svSucceeded;
            }

            break;
        }
        default:
            return enSaveResults::svFaildEmptyObject;
        }
    }

    static bool IsUserExist(string UserName)
    {

        clsUser User = clsUser::Find(UserName);
        return (!User.IsEmpty());
    }

    bool Delete()
    {
        vector<clsUser> _vUsers;
        _vUsers = _LoadUsersDataFromFile();

        for (clsUser &U : _vUsers)
        {
            if (U.GetUserName() == _UserName)
            {
                U._MarkedForDelete = true;
                break;
            }
        }

        _SaveUsersDataToFile(_vUsers);

        *this = _GetEmptyUserObject();

        return true;
    }

    static clsUser GetAddNewUserObject(string UserName)
    {
        return clsUser(enMode::AddNewMode, "", "", "", "", UserName, "", 0);
    }

    static vector<clsUser> GetUsersList()
    {
        return _LoadUsersDataFromFile();
    }

    bool CheckAccessPermission(short Permissions)
    {
        enPermissions Permission = enPermissions(Permissions);
        if (this->GetPermissions() == enPermissions::eAll)
            return true;

        else if ((Permission & this->GetPermissions()) == Permission)
            return true;
        else
            return false;
    }

    void AddToUserLogFile()
    {
        fstream MyFile;
        MyFile.open("UserLogFile.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {
            string Line;
            Line = _ConvertObjectToLogFileLine(*this);
            MyFile << Line << endl;
            MyFile.close();
        }
    }

    static stLogFileInfo _ConvertLogInfoToObject(string Line, string Seperator = "#//#")
    {
        stLogFileInfo Info;

        vector <string> LoginRegisterDataLine = clsString::Split(Line, Seperator);
        Info.DateTime = LoginRegisterDataLine[0];
        Info.UserName = LoginRegisterDataLine[1];
        clsUtil::DecryptText(Info.Password) = LoginRegisterDataLine[2];
        Info.Permissions = stoi(LoginRegisterDataLine[3]);

        return Info;
    }

    static vector<stLogFileInfo> _LoadLogInfoFromFile()
    {
        vector<stLogFileInfo> LogFileInfo;
        fstream MyFile;
        MyFile.open("UserLogFile.txt", ios::in);

        if(MyFile.is_open())
        {
            string Line;
            stLogFileInfo Info;
            while(getline(MyFile, Line))
            {
                Info = _ConvertLogInfoToObject(Line);
                LogFileInfo.push_back(Info);
            }
            MyFile.close();
        }
        return LogFileInfo;
    }

    static void AddToUserLogFile(clsUser User)
    {
        fstream MyFile;
        MyFile.open("UserLogFile.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {
            string Line;
            Line = _ConvertObjectToLogFileLine(User);
            MyFile << Line << endl;
            MyFile.close();
        }
    }

    static vector<stLogFileInfo> GetLogFileList()
    {
        vector<stLogFileInfo> LogInfo;
        LogInfo = _LoadLogInfoFromFile();
        return LogInfo;
    }
};
