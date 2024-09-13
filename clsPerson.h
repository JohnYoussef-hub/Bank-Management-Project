#pragma once
#include <iostream>
#include <string>
using namespace std;

class clsPerson
{

private:
    string _FirstName;
    string _LastName;
    string _Email;
    string _Phone;

public:
    clsPerson(string FirstName, string LastName, string Email, string Phone)
    {

        _FirstName = FirstName;
        _LastName = LastName;
        _Email = Email;
        _Phone = Phone;
    }

    void SetFirstName(string FirstName)
    {
        _FirstName = FirstName;
    }

    string GetFirstName()
    {
        return _FirstName;
    }

    void SetLastName(string LastName)
    {
        _LastName = LastName;
    }

    string GetLastName()
    {
        return _LastName;
    }

    void SetEmail(string Email)
    {
        _Email = Email;
    }

    string GetEmail()
    {
        return _Email;
    }

    void SetPhone(string Phone)
    {
        _Phone = Phone;
    }

    string GetPhone()
    {
        return _Phone;
    }

    string FullName()
    {
        return _FirstName + " " + _LastName;
    }

    void Print()
    {
        cout << "\nInfo:";
        cout << "\n___________________";
        cout << "\nFirstName: " << _FirstName;
        cout << "\nLastName : " << _LastName;
        cout << "\nFull Name: " << FullName();
        cout << "\nEmail    : " << _Email;
        cout << "\nPhone    : " << _Phone;
        cout << "\n___________________\n";
    }
};
