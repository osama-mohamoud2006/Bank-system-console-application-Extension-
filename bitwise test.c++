// please if you want to understand read Admin.per in output  and use this number and use win 
// calculator in programmer mode to understand how it works
// for Ex : Admin.per = 6  
//(6&1)==0 (1 is printname in enum ) (does't have printname per) 
//(6&2)==2 (have hiname per) ,
//(6&4)==4 (have fuckname per)
// so the output will be : 
// does't have permission to print his name
// hi name
// fuck name

#include <iostream>
using namespace std;
// you must assign enum according 2 power of 2
// EX : 1,2,4,8,16,32,64,128,256
enum enper {printname = 1,hiname = 2, fuckname = 4}; // this is very imp enum it is used to assigen permission and use it to check also 

struct stAdminThatHavePer
{ // struct to assigne user specific permission
    string name = "";
    int per = 0;
};

// this function is very importan fr it is used to check if you have per or not
//how? it takes the admin.per (int) and use & with the enum enper:: you want to check if the number == the enum enper::  then it return true else false
bool hasPer(stAdminThatHavePer admin, enper per)
{
    return (admin.per & per) == per;
}

string enterName() // to enter the name normally
{
    string name;
    cout << "\nenter name: ";
    cin >> name;
    return name;
}

// functions to test only 
string PrintHisName(string name)
{
    return name;
}

string HelloTheName(string name)
{
    return "hi " + name;
}

string FuckTheName(string name)
{
    return "fuck you " + name;
}

bool YOrN()
{

    char y;
    cin >> y;
    if (y == 'y')
        return true;
    else
        return false;
}

int main()
{
    /*
    | --> used to assign permisson
    & --> to check permission 
    
    */
    stAdminThatHavePer Admin;

    Admin.name = enterName(); // to fill name

    cout << "do you want to print your name? ";
    if (YOrN() == true)
    {
        Admin.per |= printname; // to assign the permisson 
    }
    else
    {
        cout << "ok\n";
    }

    cout << "do you want to hello your name? ";
    if (YOrN() == true)
    {
        Admin.per |= hiname;
    }
    else
    {
        cout << "ok\n";
    }

    cout << "do you want to fuck your name? ";
    if (YOrN() == true)
    {
        Admin.per |= fuckname;
    }
    else
    {
        cout << "ok\n";
    }

    cout<<Admin.per<<endl; // to show the total per 

    if (hasPer(Admin, enper::printname))// if he has the right to print his name
    { 
        cout << PrintHisName(Admin.name) << endl;
    }
    else
    {
        cout << "does't have permission to print his name \n";
    }

    if (hasPer(Admin, enper::hiname))// if he has the right to print his name
    { 
        cout << HelloTheName(Admin.name) << endl;
    }
    else
    {
        cout << "does't have permisson to hello his name \n";
    }

    if (hasPer(Admin, enper::fuckname))// if he has the right to print his name
    { 
        cout << FuckTheName(Admin.name) << endl;
    }
    else
    {
        cout << "does't have permisson to fuck his name \n";
    }
}
