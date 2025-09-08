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

#include<iostream>
using namespace std;

struct stAdminThatHavePer{ // struct to assigne user specific permission 
    string name ="";
    int per =0; 
};



