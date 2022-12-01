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
#include <iostream>
#include <ctime> // used to work with date and time

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

    // create sign in file
    //  Create and open a text file
    ofstream MyFile("signIn.txt");

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
        int count = 0;
        //**add lines here to compare retrieved user password to
        while (tempPass.password != pass && count <= 2)
        {
            if (count == 2)
            {
                cout << "\nAccount is locked";
                exit(1);
            }
            cout << "Login unsuccesful!!";
            time_t t;      // t passed as argument in function time()
            struct tm *tt; // decalring variable for localtime()
            time(&t);      // passing argument to time()
            tt = localtime(&t);
            count++;
            MyFile << "\n"
                   << name << " had unsuccesful login attempt " << asctime(tt) << endl;
            cout << "\nPassword:";
            cin >> pass;
        }
        cout << "Login succesful";
        time_t t;      // t passed as argument in function time()
        struct tm *tt; // decalring variable for localtime()
        time(&t);      // passing argument to time()
        tt = localtime(&t);
        MyFile << "\n"
               << name << " logged in succesfully " << asctime(tt) << endl;

        //**input password and print "Authentication failure"
        //**or "Authentication successful"
    }

    // Close the file
    MyFile.close();

    cout << endl;
    return 0;
}
