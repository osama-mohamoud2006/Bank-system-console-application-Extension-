#include <iostream>
#include <iomanip>
#include <fstream>
#include<cmath>
#include <vector>
#include <cctype>
#include <string>
#include"E:\projects\my library\AllStuff.h"  
using namespace AllStuff;
using namespace std;

///////    esstinals for the project//////////////////////////////////////
struct stdata
{
	string account_number = " "; // this is the main key which is very importat to manage data 
	string pin = " ";
	string name = " ";
	string phone = " ";
	int account_balance = 0;
	bool mark_for_delete = false;

};

struct stadmins {
	string username = "";
	string pin = "";
	int per = 0; // permissions 
	bool MarkForDelete = false;
};

stadmins AdminThatWillAcess;

enum enOption { none = 0, showClientList = 1, addNewClient = 2, deleteClient = 3, updateClient = 4, searchClient = 5, Transactions = 6,  AdminsScreen=7 ,logout = 8 };
enum enAdminPermissions { PListClient = 1, PAddNewClient = 2, PDeleteClient = 4, PUpdateClient = 8, PSearchClient = 16 , PClientTransactions=32 , PManageAdmins=64};

// declarations to access it anywhere
void login();
bool CheckUsernameAndPassword(vector<stadmins>, string, string);
//basics for program to work properly///////////////
const string path = "local db.text"; // clients
const string AdminsPer = "Admins.text"; // admins 
const string delmi = "#//#";
//////////////////////////////////////////////////////

//for admins
bool CheckUsernameInVector(vector<stadmins>& VerctorHaveAdminsList, string username , stadmins &Admin) {
	for (stadmins& admins : VerctorHaveAdminsList)
	{
		if ( admins.username == username) { 
			Admin = admins;
			return true;
		}
	}
	return false;
}

//for admins 
// pass user name and returns it data 
stadmins CheckUserAndReturnItsStruct(vector<stadmins> AllAdminsData,string username , stadmins &AdminData) {

	for (stadmins& admin : AllAdminsData) {
		
		if (admin.username == username) {
			AdminData = admin;
			return AdminData;
		}
	}
	
}

//for admin only ///
bool CheckPermission(stadmins CheckAdmin, enAdminPermissions Permission) {
	return (CheckAdmin.per & Permission) == Permission;
}

// for admin just silly mesaage // 
void YouDonotHavePerTextMessage() {
	cout << "\n\n\tyou don't have permisson to access this section \n\t contact your admin or die better!\a\n";
	screen_color(red);
}


void  back_to_menu(string TextAppearWhenYouBack = "press any key to back to main menu !") {
	screen_color(black);
	cout << "\033[1;31m";
	cout << "\n "<< TextAppearWhenYouBack << endl;
	cout << "\033[0m";
	system("pause>0");

}

void print_menu_option(string option_name) {
	cout << "\n_________________________________________________\n\n\n";
	cout << "\t" << option_name << "\n";
	cout << "\n_________________________________________________\n";
}
//////////////////////////////////////////////////////////////////////////////////

void the_account_isnot_exist(bool menu2 = false) {

	if (menu2 == false) {

     cout << "\a";
	cout << "\nthis account number isn't exist!\n";
	screen_color(red);
	screen_color(black);

	}
	else {

		cout << "\a";
		cout << "\nthis username isn't exist!\n";
		screen_color(red);
		screen_color(black);
	}
}

stdata fill_data(string account_number) {
	stdata data;
	data.account_number = account_number;
	data.pin = read_string("\nenter pin: ");
	data.name = read_full_line("\nenter name: ");
	data.phone = read_string("\nenter your phone number: ");
	data.account_balance = enter_postive_number("\nenter account balance: ");
	return data;


}

//for admins only///
stadmins Fill_Admin_data(string username) {
	stadmins data;
	data.username = username;// pk
	data.pin = read_string("\nenter pin: ");
	return data;


}

// split line(from file)  into raw data (very imp )
vector<string> split_string(string new_client_line, string delmi) {

	vector<string> vdata;
	short pos = 0;
	string sword;

	while ((pos = new_client_line.find(delmi)) != string::npos) {

		sword = new_client_line.substr(0, pos);

		if (sword != "") vdata.push_back(sword);

		new_client_line.erase(0, pos + delmi.length());
	}

	if (new_client_line != "") vdata.push_back(new_client_line);

	return vdata;

}

//convert line of file into data filled in struct (return stdata which filled with data)
stdata convert_line_into_record(string new_client_line) {

	vector<string> dataSplited;
	stdata data;
	dataSplited = split_string(new_client_line, delmi);

	data.account_number = dataSplited[0];
	data.pin = dataSplited[1];
	data.name = dataSplited[2];
	data.phone = dataSplited[3];
	data.account_balance = stoi(dataSplited[4]);

	return data;

}

//for admins///
stadmins convert_line_into_recordAdmins(string new_admin_line) {
	vector<string> dataSplited;
	stadmins data;
	dataSplited = split_string(new_admin_line, delmi);

	data.username = dataSplited[0];
	data.pin = dataSplited[1];
	data.per = stoi(dataSplited[2]); // --> for permissions 


	return data;
}

//read file then push all data into vector of struct 
vector<stdata> Vector_have_all_data(string path) {

	vector<stdata> v_with_all_data;
	fstream write;
	write.open(path, ios::in); //read mode 

	if (write.is_open()) {

		string ld = " "; // line of data 
		stdata data;
		while (getline(write, ld)) {
			if (ld != "")
			{

				data = convert_line_into_record(ld); // fill empty struct with data after convert line into raw data
				v_with_all_data.push_back(data); // push the data in vector 

			}

		}
	}
	else {
		cout << "file couldn't open!" << endl;
		cout << "\a";
		screen_color(red);
	}
	return v_with_all_data;
}

//for admins///
vector<stadmins> Vector_have_all_admins(string path= "Admins.text") {

	vector<stadmins> v_with_all_data;
	fstream write;
	write.open(path, ios::in); //read mode 

	if (write.is_open()) {

		string ld = ""; // line of data 
		stadmins admins;
		while (getline(write, ld)) {
			if (ld != "")
			{

				admins = convert_line_into_recordAdmins(ld); // fill empty struct with data after convert line into raw data
				v_with_all_data.push_back(admins); // push the data in vector 

			}

		}
	}
	else {
		cout << "file couldn't open!" << endl;
		cout << "\a";
		screen_color(red);
	}
	return v_with_all_data;
}


//print header for show client list (FOR "SHOW ALL CLIENT (1) ) 
void print_header(int num = 1, bool menu2 = false , bool menu3=false) {
	if (num == 0) cout << "\nNo One In the System!\a\n";

	if (menu2 == false && menu3==false) {

		cout << "\n\n";
		cout << setw(35) << right << " CLIENTS LIST (" << num << ")\n\n";

		cout << left;
		cout << setw(15) << "Account Num"
			<< setw(10) << "Pin"
			<< setw(22) << "Name"
			<< setw(18) << "Phone"
			<< setw(12) << "Balance"
			<< endl;

		cout << string(77, '-') << endl;

	}
	else if(menu2==true) {
		cout << "\n\n";
		cout << setw(35) << right << " CLIENTS LIST (" << num << ")\n\n";

		cout << left
			<< setw(22) << "Name"
			<< setw(12) << "Balance"
			<< endl;

		cout << string(77, '-') << endl;
	}

	 if (menu3 == true) {
		cout << "\n\n";
		cout << setw(35) << right << " Admins LIST (" << num << ")\n\n";

		cout << left;
		cout<< setw(10) << "Username  "<< setw(15) << "    Pin"   << "Permissions"<<endl;
		cout << string(77, '-') << endl;
	}


}

//print data (FOR "SHOW ALL CLIENT (1) ) 
void printStruct(const stdata& data, bool menu2 = false )
{
	if (menu2 == false) {

		cout << "| " << left
			<< setw(13) << data.account_number << " | "
			<< setw(8) << data.pin << " | "
			<< setw(18) << data.name << " | "
			<< setw(13) << data.phone << " | "
			<< right
			<< setw(10) << data.account_balance << " |"
			<< endl;
		cout << "+---------------+----------+--------------------+---------------+------------+" << endl;

	}
	
	else {

		cout << "| " << left
			<< setw(18) << data.name << " | "
			<< right
			<< setw(10) << data.account_balance << " |"
			<< endl;
		cout << "+---------------+----------+--------------------+---------------+------------+" << endl;

	}

}

//print data (FOR "SHOW list admin (1) ) //for admins///
void printStruct(const stadmins& data)
{
		cout << "| " << left
			<< setw(13) << data.username << " | "
			<< setw(8) << data.pin << " | " << setw(8) << data.per << " | "
			<< endl;
		cout << "+---------------+----------+--------------------+---------------+------------+" << endl;
}


//print data for 1 client only 
void PrintAdminOrClientDetails(stdata Client_data) {
	cout << "\nthe following are the account details :\n";
	cout << "____________________________\n";
	print_header();
	printStruct(Client_data); // to print the client data 
	cout << "\n_________________________________________________\n";
}


//print data for 1 admin only  // for admin only//
void PrintAdminOrClientDetails(stadmins admin_data) {
	cout << "\nthe following are the account details :\n";
	cout << "____________________________\n";
	print_header(  1,  false, true); // to print the proper header for admin 
	printStruct(admin_data); // to print the client data 
	cout << "\n_________________________________________________\n";
}

// option [1] in main menu 
void show_client_list(vector<stdata>& vprint) {


	//vprint = Vector_have_all_data(path); // bring all data from file to print 

	print_header(vprint.size()); // print header
	for (const stdata& client : vprint) {


		printStruct(client); // print clients data 
	}


}

// option [1] in  admins main menu  //for admins only //
void show_admin_list(vector<stadmins>& vprint) {

	print_header(vprint.size(),  false, true); // print header for menu 3(admins list) 
	for (const stadmins& client : vprint) {


		printStruct(client); // print clients data 
	}

}

// check if the account number (entered by user) is exist in file or not 
bool check_the_account_number(vector<stdata>& search_account_number, string account_numberFromUser, stdata& client) {
	stdata data;
	data.account_number = account_numberFromUser;
	//search_account_number = Vector_have_all_data(path);

	for (stdata& vd : search_account_number) {
		if (vd.account_number == data.account_number) {
			client = vd;
			return true;
		}
	}

	return false;
}

//convert stdata into single line to store in file 
string convert_stdata_into_single_line(stdata data) {
	return (data.account_number + delmi + (data.pin) + delmi + data.name + delmi + data.phone + delmi + to_string(data.account_balance));
}

//convert stadmins into single line to store in file  //for admins only // 
string convert_stdata_into_single_line(stadmins data) {
	return (data.username + delmi + data.pin + delmi+ to_string(data.per));
}


//take vector with edited data and write it into file
void edit_file(vector <string>& lines_of_data) {



	fstream write;
	write.open(path, ios::out); //write mode

	if (write.is_open()) {

		for (const string& new_client_line : lines_of_data) {

			if (new_client_line != "") write << new_client_line << endl;

		}
		write.close();
	}

}

//edit admin file 
void edit_Admin_file(vector <string>& lines_of_data ) {

	fstream write;
	write.open(AdminsPer, ios::out); //write mode

	if (write.is_open()) {

		for (const string& new_client_line : lines_of_data) {

			if (new_client_line != "") write << new_client_line << endl;

		}
		write.close();
	}

}


char choice_y_n() {
	char c = ' ';
	cin >> c;
	c = toupper(c);
	return c;
}

//for stdata 
vector <string> New_lines_to_push_in_file_after_delete(vector<stdata>& all_data_from_file_in_vector, stdata client_data)
{

	vector<string> remaining_clients_after_delete; //(empty string) to save the data again without marked for delete 

	for (stdata& client_data : all_data_from_file_in_vector) { // to find the marked for delete and ignore it in new push 

		if (client_data.mark_for_delete == false)
		{
			string line_of_data = convert_stdata_into_single_line(client_data); // take the sttruct and convert it into line 

			remaining_clients_after_delete.push_back(line_of_data);//push All the data(lines) without marked for delete

		}


	}
	return remaining_clients_after_delete;
}

//for stadmins /// for admins only ///
vector <string> New_lines_to_push_in_file_after_delete(vector<stadmins>& all_data_from_file_in_vector, stadmins admins_data)
{

	vector<string> remaining_clients_after_delete; //(empty string) to save the data again without marked for delete 

	for (stadmins& admins_data : all_data_from_file_in_vector) { // to find the marked for delete and ignore it in new push 

		if (admins_data.MarkForDelete == false)
		{
			string line_of_data = convert_stdata_into_single_line(admins_data); // take the struct and convert it into line 

			remaining_clients_after_delete.push_back(line_of_data);//push All the data(lines) without marked for delete

		}


	}
	return remaining_clients_after_delete;
}

//for stdata 
vector<string> update_before_push_into_file(vector<stdata>& all_data_from_file_in_vector) {

	vector<string>New_lines_to_push_in_file_after_update; //push old lines with updated line 

	for (const stdata& Edited_data : all_data_from_file_in_vector) {

		string Nline_of_data = convert_stdata_into_single_line(Edited_data); // convert struct into line
		New_lines_to_push_in_file_after_update.push_back(Nline_of_data);
	}

	return New_lines_to_push_in_file_after_update;

}

//for admin only ///
vector<string> update_before_push_into_file(vector<stadmins>& all_data_from_file_in_vector) {

	vector<string>New_lines_to_push_in_file_after_update; //push old lines with updated line 

	for (const stadmins& Edited_data : all_data_from_file_in_vector) {

		string Nline_of_data = convert_stdata_into_single_line(Edited_data); // convert struct into line
		New_lines_to_push_in_file_after_update.push_back(Nline_of_data);
	}

	return New_lines_to_push_in_file_after_update;

}


//the main logic of update_client function //for stdata // 
vector<stdata> update(vector<stdata>& AlldataFromVector, stdata& FilledDate_Client_to_update, string account_number_to_update) {

	vector<stdata> Edit_the_orignail_data;

	FilledDate_Client_to_update.account_number = account_number_to_update; // new data with account number 

	for (stdata& Origninal_data : AlldataFromVector) {
		if (Origninal_data.account_number == FilledDate_Client_to_update.account_number) {

			Edit_the_orignail_data.push_back(FilledDate_Client_to_update);//push new data only

		}
		else {
			Edit_the_orignail_data.push_back(Origninal_data);
		}
	}
	return Edit_the_orignail_data;
}


//// username is primary key ////////////////////////////////////

//the main logic of update admin function  // for admins only//
vector<stadmins> update(vector<stadmins>& AlldataFromVector, stadmins& FilledData_admin_to_update, string username) {

	vector<stadmins> Edit_the_orignail_data;

	FilledData_admin_to_update.username = username; // new data with account number 

	for (stadmins& Origninal_data : AlldataFromVector) {
		if (Origninal_data.username == FilledData_admin_to_update.username) {

			Edit_the_orignail_data.push_back(FilledData_admin_to_update);//push new data only

		}
		else {
			Edit_the_orignail_data.push_back(Origninal_data);
		}
	}
	return Edit_the_orignail_data;
}


//for clients & admins
void account_is_exist(string account_numberFromUser , bool ForAdmins = false) {

	if (ForAdmins == true) {
		cout << "\nadmin with username \"" << account_numberFromUser << "\" is exist!\n";
		cout << "\a";
	}

	else {
		cout << "\nclient with account number " << account_numberFromUser << " is exist!\n";
		cout << "\a";
	}
	
}


void add_new_client_to_file(vector<stdata>& all_data_from_file_in_vector) {

	vector <string> NewLine;
	for (stdata& d : all_data_from_file_in_vector) {

		string line = convert_stdata_into_single_line(d);
		NewLine.push_back(line);
	}

	edit_file(NewLine);
}

// for admins only//
void add_new_admin_to_file(vector<stadmins>& all_data_from_file_in_vector) {

	vector <string> NewLine;
	for (stadmins& d : all_data_from_file_in_vector) {

		string line = convert_stdata_into_single_line(d);
		NewLine.push_back(line);
	}

	edit_Admin_file(NewLine);
}

//option [2]//////
void add_client(vector<stdata>& all_data_from_file_in_vector) {

	print_menu_option("adding new client"); // print the screen menu 
	string account_numberFromUser = " ";
	stdata Client_data;
	char choice = 'Y';
	bool is_account_exist = false;

	vector<stdata> NewData;
	NewData = all_data_from_file_in_vector; // make copy of struct 

	cout << endl;
	do {
		///////////////////donot touch me ////////////////////////////////////////
		account_numberFromUser = read_string("\nenter account number: "); // enter account number 

		// if the account exists 
		while (check_the_account_number(all_data_from_file_in_vector, account_numberFromUser, Client_data) == true) {

			account_is_exist(account_numberFromUser);
			cout << "\nenter another account number, ";
			account_numberFromUser = read_string("enter account number: "); // enter account number 

		}

		cout << "\n_______________________________________________\n";

		// if the account isn't existing then it will get out from while loop 
		cout << "\nFill user data:";
		cout << "\n__________________________________________\n";
		Client_data = fill_data(account_numberFromUser); // fill data by user(stdata) 

		NewData.push_back(Client_data); // push the new data into struct 

		all_data_from_file_in_vector = NewData; // make the original vector have updated data 

		add_new_client_to_file(all_data_from_file_in_vector);// send the new vector with new lines to add in file 

		cout << "\n________________________________\n";
		cout << "the client addedd successfully!\n";
		cout << "________________________________\n";

		cout << "\ndo you want to add another client [y],[n]: ";
		choice = choice_y_n(); // to input option 

	} while (choice == 'Y');


}

// for admins // ask the admin to give the full access for another admin or not 
bool GiveTheAdminFullAccess(stadmins &admins_data) {

	cout << "\n\nDo you want to give the full access to this admin? ";
	char c = choice_y_n();

	if (c == 'Y') {
      admins_data.per = -1;
	  return true; 
	}
	
	else admins_data.per = 0;

	return false;

}

// for admins // give admin specific roots on system
void CustomAdminPer(stadmins& admins_data) {

	cout << "\nShow client list? ";
	if (choice_y_n() == 'Y') {
		admins_data.per |= enAdminPermissions::PListClient;
	}

	cout << "\nAdd new client ? ";
	if (choice_y_n() == 'Y') {
		admins_data.per |= enAdminPermissions::PAddNewClient;
	}

	cout << "\nDelete client ? ";
	if (choice_y_n() == 'Y') {
		admins_data.per |= enAdminPermissions::PDeleteClient;
	}

	cout << "\nUpdate client ? ";
	if (choice_y_n() == 'Y') {
		admins_data.per |= enAdminPermissions::PUpdateClient;
	}

	cout << "\nFind client ? ";
	if (choice_y_n() == 'Y') {
		admins_data.per |= enAdminPermissions::PSearchClient;
	}

	cout << "\nTransaction permission ? ";
	if (choice_y_n() == 'Y') {
		admins_data.per |= enAdminPermissions::PClientTransactions;
	}


}

//for admins // 
void GiveAccessToTheNewAdmin(stadmins& admins_data) {

	if (GiveTheAdminFullAccess(admins_data))
	{
		cout << "\n\nThe Admin \"" << admins_data.username << "\" have the full access on the system!\n\n";
	}

	else
	{
		CustomAdminPer(admins_data);
	}
}


///for admins only // 
void add_Admin(vector<stadmins>& all_data_from_file_in_vector) {

	print_menu_option("adding new admin"); // print the screen menu 
	stadmins admins_data;// use it to access admin.per 
	string username = "";
	char choice = 'Y';
	bool is_account_exist = false;


	vector<stadmins> NewData;
	NewData = all_data_from_file_in_vector; // make copy of struct 

	cout << endl;
	do {

		username = read_string("\nenter username: "); // enter username   // read string not the  full line

		// if the username exists 
		while (CheckUsernameInVector(all_data_from_file_in_vector, username, admins_data) == true) {

			account_is_exist(username,true);
			cout << "\nenter another username, ";
			username = read_string("enter username: "); // enter account number 

		}

		cout << "\n_______________________________________________\n";

		// if the user isn't existing then it will get out from while loop 
		cout << "\nFill admin data:";
		cout << "\n__________________________________________\n";
		admins_data = Fill_Admin_data(username); // fill data by admin(admin) 
		
		
		GiveAccessToTheNewAdmin(admins_data);// give the full access OR custom access

		NewData.push_back(admins_data); // push the new data into struct 

		all_data_from_file_in_vector = NewData; // make the original vector have updated data 

		add_new_admin_to_file(all_data_from_file_in_vector);// send the new vector with new lines to add in file 

		cout << "\n________________________________\n";
		cout << "the admin addedd successfully!\n";
		cout << "________________________________\n";

		cout << "\ndo you want to add another admin [y],[n]: ";
		choice = choice_y_n(); // to input option 

	} while (choice == 'Y');


}


// mark the record to be deleted 
bool mark_for_delete(vector<stdata>& AlldataFromVector, string Client_to_delete) {
	stdata client;
	client.account_number = Client_to_delete;

	for (stdata& Vdata : AlldataFromVector)
	{
		//  var  names , .account_number (struct) - account number(string)
		if (Vdata.account_number == client.account_number) {
			//cout << client.account_number_by_user << endl; //test only
			Vdata.mark_for_delete = true; // edit the original data 
			return true;
		}
	}
	cout << "\a";
	return false;
}


// mark the record to be deleted 
bool mark_for_delete(vector<stadmins>& AlldataFromVector, string admin_to_delete) {
	/*stadmins admin;
	admin.username = admin_to_delete;*/
	for (stadmins& Vdata : AlldataFromVector)
	{
		if (Vdata.username == admin_to_delete) {
		
			Vdata.MarkForDelete = true; // edit the original data 
			return true;
		}
	}
	cout << "\a";
	return false;
}


// option [3]/////
void delete_client(vector<stdata>& all_data_from_file_in_vector) {


	print_menu_option("delete client"); // print the menu that indicating i delete client 

	string account_numberFromUser = " ";
	stdata Client_data;
	char choice = ' ';
	bool is_account_number_found = false;


	do {

		account_numberFromUser = read_string("\n\nenter account number: "); // read account number from user 

		if ((is_account_number_found = check_the_account_number(all_data_from_file_in_vector, account_numberFromUser, Client_data) == true))
		{

			PrintAdminOrClientDetails(Client_data); // print client details :)

			cout << "\ndo you want to remove this client record [y],[n]: ";
			choice = choice_y_n(); // to input option 
			if (choice == 'Y')
			{

				if (mark_for_delete(all_data_from_file_in_vector, Client_data.account_number) == true) {

					vector<string> remaining_clients_after_delete; //(empty string) to save the data again without marked for delete 

					// the logic of delete 
					remaining_clients_after_delete = New_lines_to_push_in_file_after_delete(all_data_from_file_in_vector, Client_data);
					edit_file(remaining_clients_after_delete);//edited file takes only lines of string 

					cout << "\nclient data deleted successfully!\n";
					break; // exit the loop as i deleted 
				}
			}

			else {
				cout << "\nNO changes on client data.\n";
			}
		}




		else {
			the_account_isnot_exist();
			//cout << "\nthis account number " << "[ " << account_numberFromUser << " ] " << "isn't exist! " << endl;


		}

	} while (is_account_number_found != true);




}

////////for admins.................................................................
void delete_Admin(vector<stadmins>& all_data_from_file_in_vector) {

	print_menu_option("Delete Admin"); // print the menu that indicating i delete client 

	string username = " ";
	stadmins admins_data;
	char choice = ' ';
	bool is_username_exists = false;


	do {

		username = read_string("\n\nenter username: "); // read account number from user 
		if (username == "admin") {
			cout << "\n\n\a Hi Bitch! you cann't delete yourself from the system dump!\n";
			return; 
		}

		if ( is_username_exists = CheckUsernameInVector(all_data_from_file_in_vector, username, admins_data) )
		{

			PrintAdminOrClientDetails(admins_data); // print client details :)

			cout << "\ndo you want to remove this admins [y],[n]: ";
			choice = choice_y_n(); // to input option 
			if (choice == 'Y')
			{

				if (mark_for_delete(all_data_from_file_in_vector, admins_data.username) == true) {

					vector<string> remaining_admins_after_delete; //(empty string) to save the data again without marked for delete 

					// the logic of delete 
					remaining_admins_after_delete = New_lines_to_push_in_file_after_delete(all_data_from_file_in_vector, admins_data);
					edit_Admin_file(remaining_admins_after_delete);//edited file takes only lines of string 

					cout << "\nAdmin data deleted successfully!\n";
					break; // exit the loop as i deleted 
				}
			}

			else {
				cout << "\nNO changes on Admin's data.\n";
			}
		}




		else {
			the_account_isnot_exist(true);
		


		}

	} while (is_username_exists != true);




}

////// option[4] //////
void update_client(vector<stdata>& all_data_from_file_in_vector) {

	print_menu_option(" update client");

	string account_numberFromUser = " ";
	stdata Client_data;
	char choice = ' ';
	bool is_account_number_found = false;
	do {
		screen_color(black);

		account_numberFromUser = read_string("\nenter account number you want to update: ");
		if ((is_account_number_found = check_the_account_number(all_data_from_file_in_vector, account_numberFromUser, Client_data)) == true)
		{

			PrintAdminOrClientDetails(Client_data);

			cout << "do you want to update this client data [y],[n]: ";
			choice = choice_y_n();

			if (choice == 'Y') { // user want to update

				cout << "\n\nenter new data to update the current client data:\n";

				Client_data = fill_data(account_numberFromUser); // fill the new data


				//update the vector of data (the main logic ) 
				all_data_from_file_in_vector = update(all_data_from_file_in_vector, Client_data, account_numberFromUser);

				vector<string>New_lines_to_push_in_file_after_delete; //push old lines with updated line 

				New_lines_to_push_in_file_after_delete = update_before_push_into_file(all_data_from_file_in_vector);

				//update the file 
				edit_file(New_lines_to_push_in_file_after_delete);

				cout << "\nthe client with account number: " << account_numberFromUser << " his data was updated successfully!\n";

			}

		}
		else {
			the_account_isnot_exist();
		}


	} while (is_account_number_found != true);



}


/// for admins only ///////////
void update_admin(vector<stadmins>& all_data_from_file_in_vector) {

	print_menu_option(" update client");

	string username = " "; // --> account_numberFromUser(perv)
	stadmins admin_data; // --> Client_data(perv)
	char choice = ' ';
	bool IsUsernameExist = false;
	do {
		screen_color(black);

		username = read_string("\nenter username to update: ");
		if ((IsUsernameExist = CheckUsernameInVector(all_data_from_file_in_vector, username, admin_data)) == true)
		{

			PrintAdminOrClientDetails(admin_data);

			cout << "do you want to update this admin data [y],[n]: ";
			choice = choice_y_n();

			if (choice == 'Y') { // user want to update

				cout << "\n\nenter new data to update the current admin data:\n";

				admin_data = Fill_Admin_data(username); // fill the new data


				GiveAccessToTheNewAdmin(admin_data);// update permissons 


				//update the vector of data (the main logic ) 
				all_data_from_file_in_vector = update(all_data_from_file_in_vector, admin_data, username);

				vector<string>New_lines_to_push_in_file_after_delete; //push old lines with updated line 

				New_lines_to_push_in_file_after_delete = update_before_push_into_file(all_data_from_file_in_vector);

				//update the file 
				edit_Admin_file(New_lines_to_push_in_file_after_delete);

				cout << "\nthe admin with username \"" << username << "\" his data was updated successfully!\n";

			}

		}
		else {
			the_account_isnot_exist(true);
		}


	} while (IsUsernameExist != true);



}


//option[5]
void find_client(vector<stdata>& all_data_from_file_in_vector) {

	print_menu_option("Find client");

	string account_numberFromUser = " ";
	stdata Client_data;
	bool is_account_number_found = false;
	do {
		screen_color(black);

		account_numberFromUser = read_string("\nenter account number you want to Find: ");
		if ((is_account_number_found = check_the_account_number(all_data_from_file_in_vector, account_numberFromUser, Client_data)) == true)
		{
			PrintAdminOrClientDetails(Client_data);

		}
		else {
			the_account_isnot_exist();
		}


	} while (is_account_number_found != true);



}

// for admins /////////////
void find_admin(vector<stadmins>& all_data_from_file_in_vector) {

	print_menu_option("Find Admin");

	string username = " ";
	stadmins admin_data;
	bool is_username_exist = false;
	do {
		screen_color(black);

		username = read_string("\nenter username you want to Find: ");
		if ((is_username_exist = CheckUsernameInVector(all_data_from_file_in_vector, username, admin_data)) == true)
		{
			PrintAdminOrClientDetails(admin_data);

		}
		else {
			the_account_isnot_exist(true);
		}


	} while (is_username_exist != true);



}
///////////////////////////////////
/*
* Transcations Stuff
*/
///////////////////////////////////////
stdata EditBalance(int amount, stdata& originaData) {
	stdata Client;
	Client.account_number = originaData.account_number;
	Client.name = originaData.name;
	Client.pin = originaData.pin;
	Client.phone = originaData.phone;
	Client.account_balance = originaData.account_balance + amount;
	return Client;
}
stdata start_screen_operation(vector<stdata>& all_data_from_file_in_vector, string& account_numberFromUser) {

	print_menu_option("Deposit Screen");

	stdata Client;

	account_numberFromUser = read_string("enter account number: ");

	// if account number isn't exist 
	while (check_the_account_number(all_data_from_file_in_vector, account_numberFromUser, Client) != true) {

		cout << "\naccount number (" << account_numberFromUser << ") isn't exist! " << endl;
		cout << "\a";
		screen_color(red);
		account_numberFromUser = read_string("enter account number Again: ");

	}
	screen_color(black); // rest color again 
	return Client;
}
void Update_file(vector<stdata>& all_data_from_file_in_vector) {

	//convert the new vector of struct into string to update file 
	vector<string>New_lines_to_push_in_file_after_delete; //push old lines with updated line 

	New_lines_to_push_in_file_after_delete = update_before_push_into_file(all_data_from_file_in_vector);

	//update the file 
	edit_file(New_lines_to_push_in_file_after_delete);
}
void Deposit(vector<stdata>& all_data_from_file_in_vector) {

	string account_numberFromUser = "";
	stdata Client;
	Client = start_screen_operation(all_data_from_file_in_vector, account_numberFromUser);

	PrintAdminOrClientDetails(Client); // print details if account exists 

	int amount = 0;
	amount = enter_postive_number("\nenter the amount you want to deposit to this account number: ");

	cout << "\a\nare you sure? [y],[n]: ";
	if (choice_y_n() == 'Y') {

		screen_color(green);
		Client = EditBalance(amount, Client); // keep the other data and just changed balance 

		// take the whole vector and search about account number if it founded will push new data in the same account number
		all_data_from_file_in_vector = update(all_data_from_file_in_vector, Client, account_numberFromUser);

		Update_file(all_data_from_file_in_vector);
		cout << "\nthe new balance is: " << Client.account_balance << endl;
	}
	else {
		cout << "\nNothing changed!\n";
	}

	screen_color(black);
}
void Withdraw(vector<stdata>& all_data_from_file_in_vector) {

	string account_numberFromUser = "";
	stdata Client;

	Client = start_screen_operation(all_data_from_file_in_vector, account_numberFromUser);
	PrintAdminOrClientDetails(Client); // print details if account exists 

	int amount = 0;
	amount = (enter_postive_number("\nenter the amount you want to withdraw from this account number: ") * -1);

	// if the amount bigger than your actual balance 
	while (abs(amount) > Client.account_balance) {
		screen_color(red);
		cout << "\nthe amount you want to withdraw is bigger than your balance!\a\n";
		cout << "\nYOUR CUURENT BALANCE IS: " << Client.account_balance << endl;
		amount = 0;
		amount = (enter_postive_number("\nenter the amount you want from withdraw to this account number: ") * -1);
	}

	cout << "\a\nare you sure? [y],[n]: ";
	if (choice_y_n() == 'Y') {
		screen_color(green);
		Client = EditBalance(amount, Client); // keep the other data and just changed balance 

		// take the whole vector and search about account number if it founded will push new data in the same account number
		all_data_from_file_in_vector = update(all_data_from_file_in_vector, Client, account_numberFromUser);

		Update_file(all_data_from_file_in_vector);
		cout << "\nthe new balance is: " << Client.account_balance << endl;
	}
	else {
		cout << "\nNothing changed!\n";
	}
	screen_color(black);
}
int total_balances_Sum(vector<stdata>& all_data_from_file_in_vector) {

	int sum = 0;
	stdata data;
	for (stdata client_b : all_data_from_file_in_vector) {
		sum += client_b.account_balance;
	}
	return sum;
}
//option 3 in transcations menu
void Print_total_balances(vector<stdata>& vprint) {

	//true is indicating to menu2
	print_header(vprint.size(), true); // print header
	for (const stdata& client : vprint) {


		printStruct(client, true); // print  total balances 
	}
	cout << "\n\tTHE TOTAL BALANCES IS: " << total_balances_Sum(vprint) << endl;
}
//////////////////////////////Main menu + transcations menu //////////////////////////////////////////
enum entranscations { deposit = 1, withdraw = 2, total_balances = 3, back_to_main_menu = 4 };
enOption select_option() {
	bool is_ok = false;
	int number = 0;
	do {


		cin >> number;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "\a";
			system("color 4F"); // red if number isn't in range 
			cout << "\nplease enter correct number which in range you assigned!\n";

		}

		else {
			is_ok = true;
		}
	} while (is_ok == false);
	system("color 0F"); //rest screen color 
	return (enOption)number;
}
void main_menu() {
	system("cls");
	cout << setw(5) << "\tWelcome to bank system! ";
	cout << "\n_____________________________________________________\n\n";
	cout << setw(5) << "please enter the number of the option you want: \n" << endl;

	cout << setw(4) << "\t[1] print the clients list." << endl;
	cout << setw(4) << "\t[2] add new client." << endl;
	cout << setw(4) << "\t[3] delete client." << endl;
	cout << setw(4) << "\t[4] update clients info." << endl;
	cout << setw(4) << "\t[5] find client." << endl;
	cout << setw(4) << "\t[6] Transactions." << endl;
	cout << setw(4) << "\t[7] Admins screen." << endl;
	cout << setw(4) << "\t[8] logout." << endl;
	cout << "\n_____________________________________________________\n\n";
	cout << "Please enter the option you want: ";


}
void exit_screen() {
	/*cout << "\t________________________________________________________________________" << endl;
	cout << "\t\t\t\t Say Good bye to me , mewo :)" << endl;
	cout << "\t________________________________________________________________________" << endl;
	system("pause");*/

	login();
}
// show the menu of transcations//////////////////////////////
void transcationsScreenMenu() {

	cout << setw(5) << "\tWelcome to Transactions system! ";
	cout << "\n_____________________________________________________\n\n";
	cout << setw(5) << "please enter the number of the option you want: \n" << endl;

	cout << setw(4) << "\t[1] Deposit." << endl;
	cout << setw(4) << "\t[2] Withdraw." << endl;
	cout << setw(4) << "\t[3] Total balances." << endl;
	cout << setw(4) << "\t[4] Main menu." << endl;
	cout << "\n_____________________________________________________\n\n";
	cout << "Please enter the option you want: ";

}
void do_transcations(entranscations operations) {

	system("cls");

	//the vector that have all data as struct 
	vector<stdata> all_data_from_file_in_vector = Vector_have_all_data(path); // call this function once instead  of calling it in every option implementation 
	switch (operations) {

	case entranscations::deposit:
		Deposit(all_data_from_file_in_vector);
		back_to_menu();
		break;

	case entranscations::withdraw:
		Withdraw(all_data_from_file_in_vector);
		back_to_menu();
		break;

	case entranscations::total_balances:
		Print_total_balances(all_data_from_file_in_vector);
		back_to_menu();
		break;

	default:
		entranscations::back_to_main_menu;
		break;
	}

}
void StartTransactions() {

	system("cls");
	screen_color(black);
	entranscations choice; // intial 

	do {

		system("cls");
		transcationsScreenMenu(); // show menu  of transcations
		choice = entranscations(select_option());// select option according to user then run selected operation
		do_transcations(choice);

	} while (choice != entranscations::back_to_main_menu);

}

///////////////////////////////////////////////////////////////////////////////////

enum enadminsStuff {listAdmins = 1, AddNewAdmin = 2, DeleteAdmin = 3, UpdateAdmin = 4, FindAdmin = 5, MainMenu = 6 };

enadminsStuff select_Admins_option() {
	bool is_ok = false;
	int number = 0;
	do {


		cin >> number;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "\a";
			system("color 4F"); // red if number isn't in range 
			cout << "\nplease enter correct number which in range you assigned!\n";

		}

		else {
			is_ok = true;
		}
	} while (is_ok == false);
	system("color 0F"); //rest screen color 
	return (enadminsStuff)number;
}

void main_admins_menu() {
	system("cls");
	cout << "\033[32m"   // green font
		<< setw(5) << "\tWelcome to admins system! "
		<< "\033[0m";  // reset 
	cout << "\n_____________________________________________________\n\n";
	cout << "\033[33m" << setw(5)
		<< "Please enter the number of the option you want: \n\n"
		<< "\033[0m" ;

	cout << setw(4) << "\t[1] list admins." << endl;
	cout << setw(4) << "\t[2] add new admin." << endl;
	cout << setw(4) << "\t[3] delete admin." << endl;
	cout << setw(4) << "\t[4] update admin info." << endl;
	cout << setw(4) << "\t[5] find admin." << endl;
	cout << setw(4) << "\t[6] main menu." << endl;
	cout << "\n_____________________________________________________\n\n";
	cout << "Please enter the option you want: ";


}

void ImplementOptionInAdminMenu(enadminsStuff option) {
	vector<stadmins> admins = Vector_have_all_admins("Admins.text"); // vector have admins list , load admins list into vector

	switch (option) {

	case enadminsStuff::listAdmins:// option [1]
		system("cls");
		show_admin_list(admins);
		back_to_menu("press any key to return to manage users screen"); // to back to main menu again 
		break;

	case enadminsStuff::AddNewAdmin:
		system("cls");
		add_Admin(admins);
		back_to_menu("press any key to return to manage users screen"); // to back to main menu again 
		break;

	case enadminsStuff::DeleteAdmin:
		system("cls");
		delete_Admin(admins);
		back_to_menu("press any key to return to manage users screen"); // to back to main menu again 
		break;

	case enadminsStuff::UpdateAdmin:
		system("cls");
		update_admin(admins);
		back_to_menu("press any key to return to manage users screen"); // to back to main menu again 
		break;

		
	case enadminsStuff::FindAdmin:
		system("cls");
		find_admin(admins);
		back_to_menu("press any key to return to manage users screen"); // to back to main menu again 
		break;
		
	default: //enadminsStuff::MainMenu
	   back_to_menu("press any key to return to manage users screen"); // to back to main menu again 
		break;
	}
}

void StartAdminsMenuScreen() {

	system("cls");
	screen_color(black);
	enadminsStuff choice ; 
	do {

		main_admins_menu(); // show admin menu 

		// select option according to user then run selected job
		choice = select_Admins_option();
		ImplementOptionInAdminMenu(choice);



	} while (choice != enadminsStuff::MainMenu);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void do_job_according_to_number(enOption option) {
	system("cls");
	//the vector that have all data as struct 
	vector<stdata> all_data_from_file_in_vector = Vector_have_all_data(path); // call this function once instead  of calling it in every option implementation 
	switch (option) {

	case enOption::showClientList:

		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PListClient)) {
			show_client_list(all_data_from_file_in_vector); // option[1]

		}
		else {
			YouDonotHavePerTextMessage(); // priint message 
		}

		back_to_menu(); // to back to main menu again 
		break;

	case enOption::addNewClient:

		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PAddNewClient)) {
			add_client(all_data_from_file_in_vector); // option[2]
		}
		else {
			YouDonotHavePerTextMessage(); // priint message 
		}
		back_to_menu(); // to back to main menu again 
		break;

	case enOption::deleteClient:
		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PDeleteClient)) {

		delete_client(all_data_from_file_in_vector); // option [3]

		}
		else {

			YouDonotHavePerTextMessage(); // priint message 
		}

		back_to_menu(); // to back to main menu again 
		break;

	case enOption::updateClient:
		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PUpdateClient)) {
			update_client(all_data_from_file_in_vector); //option [4]
		}
		else {

			YouDonotHavePerTextMessage(); // priint message 
		}

		back_to_menu(); // to back to main menu again 
		break;

	case enOption::searchClient:
		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PSearchClient)) {
			find_client(all_data_from_file_in_vector); // option [5]
		}
		else {

			YouDonotHavePerTextMessage(); // priint message 
		}

		back_to_menu(); // to back to main menu again 
		break;

	case enOption::Transactions: // option [6]
		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PClientTransactions)) {
			StartTransactions();
		}
		else {

			YouDonotHavePerTextMessage(); // priint message 
			back_to_menu();
		}

		//back_to_menu(); // to back to main menu again 
		break;

	case enOption::AdminsScreen:// option[7]
		if (CheckPermission(AdminThatWillAcess, enAdminPermissions::PManageAdmins)) {
			StartAdminsMenuScreen();
		}
		else {

			YouDonotHavePerTextMessage(); // priint message 
		    back_to_menu();
		}

	
		break;


	case enOption::logout: // option[8]
		exit_screen();
		break;

	default:
		enOption::none;

	}

}
void start()
{
	system("cls");
	screen_color(black);
	enOption choice = none; // intial 

	do {

		main_menu(); // show main menu 

		// select option according to user then run selected job
		choice = select_option();
		do_job_according_to_number(choice);



	} while (choice != enOption::logout);


}

// check if username exists or not 

bool CheckUsernameAndPassword(vector<stadmins> VerctorHaveAdminsList, string username,string pin) {
	for (stadmins& admins : VerctorHaveAdminsList)
	{
		if (admins.pin == pin && (admins.username == username)) {
			
			return true;
		}
	}
	return false;
}

void login() {

	system("cls");
	screen_color(black);
	vector<stadmins> admins = Vector_have_all_admins("Admins.text"); // vector have admins list , load admins list into vector

	print_menu_option("Login");
	bool check = false;
	
	do {

		string username = read_string("\nenter username: ");
		string password = read_string("\nenter password: ");

		if ((CheckUsernameAndPassword(admins,  username,password))) // check the user name in  file && and his pass
		{ 
			check = true;

			AdminThatWillAcess = CheckUserAndReturnItsStruct(admins, username, AdminThatWillAcess);// mark the admin that founded to access later
	  
			start(); // start the program normally 
		}

		else {
			screen_color(red);
			cout << "\nIncorrect password or user!\n\a";
		}

	} while (check == false);


}

int main() {

	login();


}