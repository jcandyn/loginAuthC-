//--------------------------------------------------------------------
//
//  Laboratory                                      login.cpp
//
//  In-lab 1 program that reads in username/login pairs and then
//  performs authentication of usernames until EOF is encountered.
//
//--------------------------------------------------------------------

#include <string>
#include <iostream>
#include <fstream>
#include "hashtbl.cpp"

using namespace std;

// This will be the data stored in the HashTbl (class DT)
struct Password
{
    void setKey(string newKey) { username = newKey; }
    string getKey() const { return username; }

    // this hash converts a string to an integer
    int hash(const string str) const
    {
        int val = 0;

        for (unsigned int i = 0; i < str.length(); i++)
            val += str[i];
        return val;
    }
    string username,
        password;
};

int main()
{
    HashTbl<Password, string> passwords(10);
    Password tempPass;
    string name,    // user-supplied name
        pass;       // user-supplied password
    bool userFound; // is user in table?

    //*********************************************************
    // Step 1: Replace "," with " " in file
    //*********************************************************

    std::fstream fs("LoginsAndPasswords.txt", std::fstream::in | std::fstream::out);
    if (fs.is_open())
    {
        while (!fs.eof())
        {
            if (fs.get() == ',')
            {
                fs.seekp((fs.tellp() - static_cast<std::streampos>(1)));
                fs.put(' ');
                fs.seekp(fs.tellp());
            }
        }
        fs.close();
    }
    else
    {
        std::cout << "Faild to open" << '\n';
    }

    //*********************************************************
    // Step 2: Read in file
    //*********************************************************
    ifstream passFile("LoginsAndPasswords.txt");

    if (!passFile)
    {
        cout << "Unable to open 'LoginsAndPasswords.txt'!" << endl;
        return 1;
    }

    while (passFile >> tempPass.username >> tempPass.password && !passwords.isFull())
    {
        //**add line here to insert passwords into the HashTbl
        passwords.insert(tempPass);
    }

    //**add line here to show (print) the HashTbl
    passwords.showStructure();
    //*********************************************************
    // Step 3: Prompt for a Login and Password and check if valid
    //*********************************************************
    cout << "Login: ";
    while (cin >> name) // to quit, type CTRL Z in Visual C++
    {
        //**add line here to retrieve user from HashTbl
        while (!passwords.retrieve(name, tempPass))
        {
            cout << "User is NOT in table";
            cout << "\nLogin: ";
            cin >> name;
        }

        cout << "User is in table";
        cout << "\nPassword: ";
        cin >> pass;

        //**add lines here to compare retrieved user password to
        if (tempPass.password == pass)
        {
            cout << "Login succesful";
        }
        else
        {
            cout << "Login unsuccesful!!";
            cout << "\nLogin:";
        }
        //**input password and print "Authentication failure"
        //**or "Authentication successful"
    }

    cout << endl;
    return 0;
}
