#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
const string ClientsFileName = "local db.text";
const string UsersFileName = "Users.txt";

void ShowMainMenue();
void ShowTransactionsMenue();
enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3,
    eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6,
    eManageUsers = 7, eExit = 8};
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;};
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

struct stUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};
stUser CurrentUser;
enum enManageUsersMenueOptions {eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,eUpdateUser = 4, eFindUser = 5, eMainMenue = 6};

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void Login();



vector< string> SplitString(string S1, string Delmi = "#//#") {
    vector<string> vstring;
    short pos = 0;
    string sword;

    while ((pos = S1.find(Delmi)) != string::npos) {

        sword = S1.substr(0, pos);
        if (sword != "") vstring.push_back(sword);

        S1.erase(0, pos + Delmi.size());
    }

    if (S1 != "") vstring.push_back(S1);

    return vstring;
}

sClient ConvertLinetoRecord(string line, string delmi = "#//#") {

    sClient data;

    vector<string> data_of_lines = SplitString(line);

    data.AccountNumber = data_of_lines[0];
    data.PinCode = data_of_lines[1];
    data.Name = data_of_lines[2];
    data.Phone = data_of_lines[3];
    data.AccountBalance = stod(data_of_lines[4]);

    return data;

}

stUser ConvertUserLinetoRecord(string line, string delmi = "#//#") {
    stUser User;
    vector<string> vUserData;

    vUserData = SplitString(line,delmi);

    User.UserName = vUserData.at(0);
    User.Password= vUserData.at(1);
    User.Permissions= stoi(vUserData.at(2));

    return User; 
}


string ConvertRecordToLine(sClient Client, string delmi = "#//#") {
    string ClientLine = "";

    ClientLine += Client.AccountNumber + delmi;
    ClientLine += Client.PinCode + delmi;
    ClientLine += Client.Name + delmi;
    ClientLine += Client.Phone + delmi;
    ClientLine += to_string(Client.AccountBalance);

    return ClientLine;
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#") {
    string stClientRecord = "";

    stClientRecord += User.UserName + Seperator;
    stClientRecord += User.Password + Seperator;
    stClientRecord += to_string(User.Permissions);

    return stClientRecord;


}


stUser ConvertUserLinetoRecord2(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData;

    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;

}

bool CheckAccessPermission(enMainMenuePermissions Permission) {
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll) return true;

    if ((CurrentUser.Permissions & Permission) == Permission) return true;
    else return false; 
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {

    sClient data;
    vector<sClient> Vclients;

    fstream read;
    read.open(FileName, ios::in); // read mode 
    if (read.is_open()) {

        string Line;

        while (getline(read, Line)) {

            data = ConvertLinetoRecord(Line);

            if (data.AccountNumber == AccountNumber) {
                read.close();
                return true;
            }
            Vclients.push_back(data);
        }
        read.close();
    }
    return false;
}


bool UserExistsByUsername(string Username, string FileName) {
    //read file and convert each line into record 
    stUser user;
    fstream read;
    read.open(FileName, ios::in); // read 
    if (read.is_open()) {

        string line;
        while (getline(read, line)) {

            user = ConvertUserLinetoRecord(line);
            if (user.UserName == Username) {
                read.close();
                return true;
            }
        }

        read.close();
    }
    return false;
}


// for add new client 
sClient ReadNewClient() {

    sClient data;

    cout << "Enter account number: ";
    getline(cin >> ws, data.AccountNumber);

    while (ClientExistsByAccountNumber(data.AccountNumber, "local db.text")) {

        cout << "\nClient with [" << data.AccountNumber << "] already exists, Enter another Account Number? ";

        getline(cin >> ws, data.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, data.PinCode);

    cout << "Enter Name? ";
    getline(cin, data.Name);

    cout << "Enter Phone? ";
    getline(cin, data.Phone);

    cout << "Enter AccountBalance? ";
    cin >> data.AccountBalance;

    return data;

}

int ReadPermissionsToSet() {

    char Answer = 'n';
    int per = 0;

    //dry 
    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pListClients;
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pDeleteClient;
    }


    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pUpdateClients;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pFindClient;
    }


    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        per += enMainMenuePermissions::pManageUsers;
    }

    return per;
}

stUser ReadNewUser()
{

    stUser user;
    cout << "Enter Username? ";
    getline(cin >> ws, user.UserName);
    while (UserExistsByUsername(user.UserName, UsersFileName))
    {
        cout << "\nUser with [" << user.UserName << "] already exists, Enter another Username? ";
        getline(cin >> ws, user.UserName);
    }

    cout << "Enter Password? ";
    getline(cin, user.Password);

    user.Permissions = ReadPermissionsToSet();

    return user; 

}


//vector of structure that have all data from file 
vector<sClient> LoadCleintsDataFromFile(string FileName) {

    vector< sClient> VectorOFStructure;
    fstream read;
    read.open(FileName, ios::in); // read mode 

    if (read.is_open()) {
        sClient data;
        string line = " ";

        while (getline(read, line)) {
            data = ConvertLinetoRecord(line);
            VectorOFStructure.push_back(data);
        }
        read.close();
    }

    return VectorOFStructure;
}


vector <stUser> LoadUsersDataFromFile(string FileName) {
    vector <stUser> res;
    fstream read;
        read.open(FileName, ios::in);
        if (read.is_open()) {
            stUser user;
            string line;

            while (getline(read, line)) {
                user = ConvertUserLinetoRecord(line);
                res.push_back(user);
            }

            read.close();
        }

        return res;
}

// will use it in show all client menu 
void PrintClientRecordLine(sClient client) {
    // print Client  data 
    cout << "|" << setw(15) << client.AccountNumber;
    cout << "|" << setw(10) << left << client.PinCode;
    cout << "|" << setw(40) << left << client.Name;
    cout << "|" << setw(12) << left << client.Phone;
    cout << "|" << setw(12) << left << client.AccountBalance;
}

//option 1 
void ShowAllClientsScreen() {
    if (!CheckAccessPermission(pListClients)) {
        ShowAccessDeniedMessage();
        return;
    }

    vector<sClient> Vclient = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << Vclient.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;


    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;

    if (Vclient.size() == 0)  cout << "\t\t\t\tNo Clients Available In the System!";

    else {

        for (const sClient& clientData : Vclient) {

            PrintClientRecordLine(clientData);
            cout << endl;
        }

        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n"
            << endl;
    }

}

void ShowAllUsersScreen() {
    vector<stUser>vusers = LoadUsersDataFromFile(UsersFileName);
    cout << "\n\t\t\t\t\tUsers List (" << vusers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vusers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (stUser User : vusers)
        {

            PrintUserRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";

}
void PrintClientCard(sClient client) {
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << client.AccountNumber;
    cout << "\nPin Code     : " << client.PinCode;
    cout << "\nName         : " << client.Name;
    cout << "\nPhone        : " << client.Phone;
    cout << "\nAccount Balance: " << client.AccountBalance;
    cout << "\n-----------------------------------\n";
}
void PrintUserRecordLine(stUser User)
{

    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}
void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}
void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}
bool FindClientByAccountNumber(string AccountNumber, vector<sClient> Vclients, sClient& client) {

    for (sClient& foundClient : Vclients) {

        if (foundClient.AccountNumber == AccountNumber) {

            client = foundClient;
            return true;
        }
    }

    return false;
}

bool FindUserByUsername(string Username, vector <stUser> vUsers, stUser& User)
{
    for (stUser U : vUsers)
    {

        if (U.UserName == Username)
        {
            User = U;
            return true;
        }

    }
    return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, stUser& User)
{

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUser U : vUsers)
    {

        if (U.UserName == Username && U.Password == Password)
        {
            User = U;
            return true;
        }

    }
    return false;

}

// fill to update data 
sClient ChangeClientRecord(string AccountNumber) {

    sClient client;
    client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, client.PinCode);

    cout << "Enter Name? ";
    getline(cin, client.Name);

    cout << "Enter Phone? ";
    getline(cin, client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> client.AccountBalance;

    return client;

}

stUser  ChangeUserRecord(string Username) {

    stUser user;
    user.UserName = Username;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, user.Password);
    
    user.Permissions = ReadPermissionsToSet();
    return user;
}



// the main logic of delete client
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& Vclient) {

    for (sClient& c : Vclient) {
        if (c.AccountNumber == AccountNumber) {
            c.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool MarkUserForDeleteByUsername(string Username, vector <stUser>& vUsers) {

    for (stUser& U : vUsers)
    {
        if (U.UserName == Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;

}

vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (stUser U : vUsers)
        {

            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;

}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> Vclients) {

    fstream write;
    write.open(FileName, ios::out); // write mode 

    string DataLine = " ";
    if (write.is_open()) {

        for (sClient client : Vclients) {

            if (client.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(client);
                write << DataLine << endl; // push to file 
            }
        }
        write.close();
    }
    return Vclients;
}

// add the new line of data to file directly 
void AddDataLineToFile(string FileName, string NeLineOfData) {

    fstream write;
    write.open(FileName, ios::out | ios::app);
    if (write.is_open()) {
        write << NeLineOfData << endl;
        write.close();
    }
}

void AddNewClient() {
    sClient client;
    client = ReadNewClient(); // fill struct 
    // AddDataLineToFile takes the new line, so i convert the struct to line to push it directly to file 
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(client));
}

void AddNewUser() {
    stUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}


void AddNewClients() {

    char AddMore = 'Y';

    do {

        cout << "Adding New Client:\n\n";
        AddNewClient();

        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void AddNewUsers() {
    char AddMore = 'Y';
    do {
        cout << "Adding New User:\n\n";
        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";

        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& Vclients) {

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, Vclients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {

            MarkClientForDeleteByAccountNumber(AccountNumber, Vclients);
            SaveCleintsDataToFile(ClientsFileName, Vclients);
            Vclients = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DeleteUserByUsername(string Username, vector <stUser>& vUsers) {
    if (Username == "admin") {
        cout << "\n\nYou cannot Delete This User.";
        return false;
    }

    stUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User)) {
        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            vUsers = LoadUsersDataFromFile(UsersFileName);
            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
        else
        {
            cout << "\nUser with Username (" << Username << ") is Not Found!";
            return false;
        }
    }
}


bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {

            for (sClient& client : vClients) {
                if (client.AccountNumber == AccountNumber) {
                    client = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}


bool UpdateUserByUsername(string Username, vector <stUser>& vUsers) {
    stUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User)) {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {

            for (stUser& U : vUsers) {

                if (U.UserName == Username) 
                {
                    U = ChangeUserRecord(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Account Number (" << Username << ") is Not Found!";
        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

string ReadUserName()
{
    string Username = "";

    cout << "\nPlease enter Username? ";
    cin >> Username;
    return Username;

}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> Vclients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, Vclients);

}

void ShowDeleteUserScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen";
    cout << "\n-----------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUserName();

    DeleteUserByUsername(Username, vUsers);

}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string Username = ReadUserName();

    UpdateUserByUsername(Username, vUsers);
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}
void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();


}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, client)) {
        PrintClientCard(client);
    }
    else {
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
    }
}
void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUser User;
    string Username = ReadUserName();
    if (FindUserByUsername(Username, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with Username [" << Username << "] is not found!";

}



void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :)";
    cout << "\n-----------------------------------\n";
}




void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}
void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}
short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowManageUsersMenue();

}
bool  DepositBalanceToClientByAccountNumber(string AccountNumebr, double amount, vector <sClient>& vClients) {

    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& client : vClients) {
            if (client.AccountNumber == AccountNumebr) {
                client.AccountBalance += amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << client.AccountBalance;
                return true;
            }
        }
    }
    return false;
}


void ShowDepositScreen() {


    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    sClient client;
    vector<sClient> vclients = LoadCleintsDataFromFile(ClientsFileName);
    string accouhntnumber = ReadClientAccountNumber();

    // if account number isn't exist 
    while (!FindClientByAccountNumber(accouhntnumber, vclients, client)) {
        cout << "\nClient with [" << accouhntnumber << "] does not exist.\n";
        accouhntnumber = ReadClientAccountNumber();
    }

    PrintClientCard(client); // print the client that its account number is exist

    double amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> amount;

    DepositBalanceToClientByAccountNumber(accouhntnumber, amount, vclients);

}

void PrintClientRecordBalanceLine(sClient Client) {

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowListUsersScreen()
{

    ShowAllUsersScreen();

}
void ShowWithDrawScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    vector < sClient> vclients = LoadCleintsDataFromFile(ClientsFileName);
    sClient client;
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vclients, client)) {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(client);

    double amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> amount;

    while (amount > client.AccountBalance) {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, (amount * -1), vclients);

}

void ShowTotalBalances() {
    vector<sClient> vclinets = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vclinets.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double total_balances = 0;

    if (vclinets.size() == 0)   cout << "\t\t\t\tNo Clients Available In the System!";
    else {

        for (sClient client : vclinets) {
            PrintClientRecordBalanceLine(client);
            total_balances += client.AccountBalance;
        }
        cout << endl;


        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        cout << "\t\t\t\t\t   Total Balances = " << total_balances;
    }

}


void ShowTotalBalancesScreen() {
    ShowTotalBalances();
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();
}

short ReadTransactionsMenueOption() {
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}


void PerfromTranactionsMenueOption(enTransactionsMenueOptions option)
{
    switch (option) {
    case eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;

    case eWithdraw:
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;

    case enTransactionsMenueOptions::eShowTotalBalance:
        system("cls");
        ShowTotalBalances();
        GoBackToTransactionsMenue();
        break;

    default:
        GoBackToMainMenue();

    }
}

void PerfromManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption) {
    switch (ManageUsersMenueOption)
    {
    case enManageUsersMenueOptions::eListUsers:
    {
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eAddNewUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eFindUser:
    {
        system("cls");

        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eMainMenue:
    {
        ShowMainMenue();
    }
    }
}

void ShowTransactionsMenue() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";

    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}


short ReadManageUsersMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowManageUsersMenue()
{

    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";


    PerfromManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        //cout << "under development \n";
        ShowTransactionsMenue();
        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenue();
        break;

    case enMainMenueOptions::eExit:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";


    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool  LoadUserInfo(string Username, string Password) {
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser)) return true;
    else return false; 
}

void Login()
{
    bool LoginFaild = false;
    string Username, Password;

    do {

        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild) {
            cout << "Invlaid Username/Password!\n";
        }


        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;


        LoginFaild = !LoadUserInfo(Username, Password);
    } while (LoginFaild);

    ShowMainMenue();
}

int main()

{
    Login();
    system("pause>0");
    return 0;
}
