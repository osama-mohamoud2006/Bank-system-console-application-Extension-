// #include <iostream>
// using namespace std;

// enum enPermissions
// {
//     pAddClient = 1 << 0,    // 0001
//     pDeleteClient = 1 << 1, // 0010
//     pUpdateClient = 1 << 2, // 0100
//     pListClients = 1 << 3,  // 1000
//     // ممكن تزود اللي انت عايزه
// };

// struct Admin
// {
//     string name;
//     int permissions = 0; // مبدئياً مفيش صلاحيات
// };

// bool hasPermission(Admin admin, enPermissions perm)
// {
//     return (admin.permissions & perm) == perm;
// }

// int main()
// {

//     Admin a1;
//     a1.name = "Osama";
//     // نديله صلاحيات إضافة وحذف
//     a1.permissions = pDeleteClient | pUpdateClient | pListClients; // 0001 | 0010 = 0011

//     if (hasPermission(a1, pListClients))
//     {
//         cout << "You can\n";
//     }
//     else
//     {
//         cout << "Access Denied\n";
//     }
// }

#include <iostream>
using namespace std;

enum enper
{
    printname = 1,
    hiname = 2,
    fuckname = 3
};
struct stAdminThatHavePer
{ // struct to assigne user specific permission
    string name = "";
    int per = 0;
};

bool hasPer(stAdminThatHavePer admin, enper per)
{
    return (admin.per & per) == per;
}
string enterName()
{ // to enter the name normally
    string name;
    cout << "\nenter name: ";
    cin >> name;
    return name;
}

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
    stAdminThatHavePer Admin;

    Admin.name = enterName(); // to fill name

    cout << "do you want to print your name? ";
    if (YOrN() == true)
    {
        Admin.per |= printname;
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

    

    if (hasPer(Admin, printname))
    { // if he has the right to print his name
        cout << PrintHisName(Admin.name) << endl;
    }
    else
    {
        cout << "does't have permission to print his name \n";
    }

    if (hasPer(Admin, hiname))
    { // if he has the right to print his name
        cout << HelloTheName(Admin.name) << endl;
    }
    else
    {
        cout << "does't have permisson to hello his name \n";
    }

    if (hasPer(Admin, fuckname))
    { // if he has the right to print his name
        cout << FuckTheName(Admin.name) << endl;
    }
    else
    {
        cout << "does't have permisson to fuck his name \n";
    }
}
