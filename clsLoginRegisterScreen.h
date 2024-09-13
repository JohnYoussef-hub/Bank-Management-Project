#pragma once

#include <iostream>
#include "clsScreen.h"
#include <iomanip>
#include <fstream>
#include "clsUser.h"


class clsLoginRegisterScreen :protected clsScreen
{

private:
    
    static void PrintLoginRegisterRecordLine(clsUser::stLogFileInfo LogFileInfo)
    {

        cout << setw(8) << left << "" << "| " << setw(35) << left << LogFileInfo.DateTime;
        cout << "| " << setw(20) << left << LogFileInfo.UserName;
        cout << "| " << setw(20) << left << clsUtil::EncryptText(LogFileInfo.Password);
        cout << "| " << setw(10) << left << LogFileInfo.Permissions;
    }

public:
  
    static void ShowLoginRegisterScreen()
    {

        if (!CheckAccessRights(clsUser::enPermissions::pLogFile))
            return;

        vector<clsUser::stLogFileInfo> vLoginRegisterRecord = clsUser::GetLogFileList();

        string Title = "\tLogin Register List Screen";
        string SubTitle =  "\t\t(" + to_string(vLoginRegisterRecord.size()) + ") Record(s).";

        _DrawScreenHeader(Title, SubTitle);

        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "_________________________________________\n" << endl;

        cout << setw(8) << left << "" << "| " << left << setw(35) << "Date/Time";
        cout << "| " << left << setw(20) << "UserName";
        cout << "| " << left << setw(20) << "Password";
        cout << "| " << left << setw(10) << "Permissions";
        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "_________________________________________\n" << endl;

        if (vLoginRegisterRecord.size() == 0)
            cout << "\t\t\t\tNo Logins Available In the System!";
        else

            for (clsUser::stLogFileInfo Record : vLoginRegisterRecord)
            {

                PrintLoginRegisterRecordLine(Record);
                cout << endl;
            }

        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "_________________________________________\n" << endl;

    }

};
