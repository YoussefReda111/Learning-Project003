#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

const short SHOW_CLIENTS_LIST = 0b0000001
, ADD_NEW_CLIENT = 0b0000010
, DELETE_CLIENT = 0b0000100
, UPDATE_CLIENT = 0b0001000
, FIND_CLIENT = 0b0010000
, TRANSACTIONS = 0b0100000
, MANAGE_USERS = 0b1000000;

enum en_main_menue_options
{
	en_list_clients = 1,
	en_add_new_client,
	en_delete_client,
	en_update_client,
	en_find_client,
	en_transactions,
	en_manage_users
};

enum en_trans_menue_options
{
	en_deposit = 1,
	en_withdraw,
	en_total_balances
};

enum en_manage_users_options
{
	en_list_users = 1,
	en_add_new_user,
	en_delete_user,
	en_update_user,
	en_find_user
};

struct st_client
{
	string account_number, PIN_code, name, phone;
	float account_balance = 0;
};

struct st_user
{
	string user_name, password;
	short permissions = -2;
};

void load_data_from_file_to_vector(string file_name, vector <string>& vec_file_content)
{
	fstream my_file;
	my_file.open(file_name, ios::in);
	if (my_file.is_open())
	{
		string line;
		while (getline(my_file, line))
			vec_file_content.push_back(line);
	}
	my_file.close();
}

void save_vector_to_file(string file_name, vector <string>& vec_file_content)
{
	fstream my_file;
	my_file.open(file_name, ios::out);
	if (my_file.is_open())
	{
		for (string& line : vec_file_content)
			if (line != "")
				my_file << line << endl;
	}
	my_file.close();
}

void add_line_to_file(string file_name, string line)
{
	fstream my_file;
	my_file.open(file_name, ios::out | ios::app);
	if (my_file.is_open())
	{
		my_file << line << endl;
		my_file.close();
	}
}

void update_record_in_file(string file_name, string record, string update_to)
{
	vector <string> vec_file_content;
	load_data_from_file_to_vector(file_name, vec_file_content);
	for (string& line : vec_file_content)
		if (line == record)
			line = update_to;
	save_vector_to_file(file_name, vec_file_content);
}

void delete_record_from_file(string file_name, string record)
{
	vector <string> vec_file_content;
	load_data_from_file_to_vector(file_name, vec_file_content);
	for (string& line : vec_file_content)
		if (line == record)
			line = "";
	save_vector_to_file(file_name, vec_file_content);
}

st_client convert_line_to_client_record(string client_record_line, string separator)
{
	int position = 0;
	st_client client_info;
	short separator_length = separator.length();
	for (int i = 0; i < client_record_line.length(); i++)
	{
		if (client_record_line.at(i) == separator.at(0))
		{
			if (client_info.account_number.empty())
				client_info.account_number = client_record_line.substr(position, i - position);
			else if (client_info.PIN_code.empty())
				client_info.PIN_code = client_record_line.substr(position, i - position);
			else if (client_info.name.empty())
				client_info.name = client_record_line.substr(position, i - position);
			else if (client_info.phone.empty())
				client_info.phone = client_record_line.substr(position, i - position);
			position = i + separator_length;
			i += separator_length - 1;
		}
	}
	client_info.account_balance = stof(client_record_line.substr(position));
	return client_info;
}

st_user convert_line_to_user_record(string user_record_line, string separator)
{
	int position = 0;
	st_user user_info;
	short separator_length = separator.length();
	for (int i = 0; i < user_record_line.length(); i++)
	{
		if (user_record_line.at(i) == separator.at(0))
		{
			if (user_info.user_name.empty())
				user_info.user_name = user_record_line.substr(position, i - position);
			else if (user_info.password.empty())
				user_info.password = user_record_line.substr(position, i - position);
			position = i + separator_length;
			i += separator_length - 1;
		}
	}
	user_info.permissions = stoi(user_record_line.substr(position));
	return user_info;
}

string convert_client_record_to_line(st_client client_info, string separator)
{
	string line = client_info.account_number;
	line += separator + client_info.PIN_code;
	line += separator + client_info.name;
	line += separator + client_info.phone;
	line += separator + to_string(client_info.account_balance);
	return line;
}

string convert_user_record_to_line(st_user user_info, string separator)
{
	string line = user_info.user_name;
	line += separator + user_info.password;
	line += separator + to_string(user_info.permissions);
	return line;
}

void get_client_info(st_client& client_data)
{
	cout << "\nEnter PIN Code: ";
	cin >> client_data.PIN_code;
	cout << "Enter Name: ";
	getline(cin >> ws, client_data.name);
	cout << "Enter Phone: ";
	cin >> client_data.phone;
	cout << "Enter Account Balance: ";
	cin >> client_data.account_balance;
}

void show_client_card(st_client client_data)
{
	cout << "\n\nThe following are the client details: ";
	cout << "\n------------------------------------------";
	cout << "\nAccount Number: " << client_data.account_number;
	cout << "\nPIN Code: " << client_data.PIN_code;
	cout << "\nName: " << client_data.name;
	cout << "\nPhone: " << client_data.phone;
	cout << "\nAccount Balance: " << client_data.account_balance;
	cout << "\n------------------------------------------\n\n";
}

bool is_client_exist_in_file(string file_name, string account_number)
{
	vector <string> vec_clients_records;
	load_data_from_file_to_vector(file_name, vec_clients_records);
	for (string& client_record : vec_clients_records)
		if (client_record.substr(0, 4) == account_number)
			return true;
	return false;
}

void show_clients_list(string clients_file)
{
	vector <string> vec_clients_records;
	st_client client_data;
	load_data_from_file_to_vector(clients_file, vec_clients_records);
	cout << "\t\t\t\t\tClient List (" << vec_clients_records.size() << ") Client(s).";
	cout << "\n______________________________________________________________________________________________________\n";
	cout << "\n| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "PIN Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(10) << "Phone";
	cout << "| " << left << setw(10) << "Balance";
	cout << "\n______________________________________________________________________________________________________\n";
	for (string& client_record : vec_clients_records)
	{
		client_data = convert_line_to_client_record(client_record, "/#/");
		cout << "\n| " << left << setw(15) << client_data.account_number;
		cout << "| " << left << setw(10) << client_data.PIN_code;
		cout << "| " << left << setw(30) << client_data.name;
		cout << "| " << left << setw(10) << client_data.phone;
		cout << "| " << left << setw(10) << client_data.account_balance;
	}
	cout << "\n\n______________________________________________________________________________________________________\n\n";
}

void show_main_menue()
{
	cout << "==============================================================";
	cout << "\n\t\t\tMain Menue Screen";
	cout << "\n==============================================================";
	cout << "\n\t[1] Show Client List.";
	cout << "\n\t[2] Add New Client.";
	cout << "\n\t[3] Delete Client.";
	cout << "\n\t[4] Update Client Info.";
	cout << "\n\t[5] Find Client.";
	cout << "\n\t[6] Transactions.";
	cout << "\n\t[7] Manage Users.";
	cout << "\n\t[8] Logout.";
	cout << "\n==============================================================";
}

void show_trans_menue()
{
	cout << "==============================================================";
	cout << "\n\t\tTransactions Menue Screen\t\t\t";
	cout << "\n==============================================================";
	cout << "\n\t[1] Deposit.";
	cout << "\n\t[2] Withdraw.";
	cout << "\n\t[3] Total Balances.";
	cout << "\n\t[4] Main Menue.";
	cout << "\n==============================================================";
}

void add_new_client(string clients_file)
{
	st_client added_client;
	char add_or_not;
	cout << "------------------------------------------";
	cout << "\n\tAdd New Clients Screen";
	cout << "\n------------------------------------------";
	cout << "\nAdding New Clients:\n";
	do
	{
		cout << "\nEnter Account Number: ";
		cin >> added_client.account_number;
		while (is_client_exist_in_file(clients_file, added_client.account_number))
		{
			cout << "\nClient with [" << added_client.account_number << "] already exists, enter another account number: ";
			cin >> added_client.account_number;
		}
		get_client_info(added_client);
		string client_record = convert_client_record_to_line(added_client, "/#/");
		add_line_to_file(clients_file, client_record);
		cout << "\nClient added successfully, do you want to add more clients (Y/N)? ";
		cin >> add_or_not;
	} while (tolower(add_or_not) == 'y');
}

void delete_client(string clients_file)
{
	string account_number_input;
	cout << "------------------------------------------";
	cout << "\n\tDelete Client Screen";
	cout << "\n------------------------------------------";
	cout << "\n\nPlease, enter account number: ";
	cin >> account_number_input;
	vector <string> vec_clients_records;
	load_data_from_file_to_vector(clients_file, vec_clients_records);
	for (string& client_record : vec_clients_records)
		if (client_record.substr(0, 4) == account_number_input)
		{
			st_client client_data = convert_line_to_client_record(client_record, "/#/");
			show_client_card(client_data);
			cout << "\nAre you sure you want delete this client (Y/N)? ";
			char delete_or_not;
			cin >> delete_or_not;
			if (toupper(delete_or_not) == 'Y')
			{
				delete_record_from_file(clients_file, client_record);
				cout << "\nClient deleted successfully.";
			}
			else cout << "\nClient hasn't been deleted.";
			return;
		}
	cout << "\nClient with Account Number (" << account_number_input << ") IS NOT FOUND!\n\n";
}

void update_client_info(string clients_file)
{
	string account_number_input;
	cout << "------------------------------------------";
	cout << "\n\tUpdate Client Info Screen";
	cout << "\n------------------------------------------";
	cout << "\n\nPlease, enter account number: ";
	cin >> account_number_input;
	vector <string> vec_clients_records;
	load_data_from_file_to_vector(clients_file, vec_clients_records);
	for (string& client_record : vec_clients_records)
		if (client_record.substr(0, 4) == account_number_input)
		{
			st_client client_data = convert_line_to_client_record(client_record, "/#/");
			show_client_card(client_data);
			cout << "\nAre you sure you want to update this client (Y/N)? ";
			char update_or_not;
			cin >> update_or_not;
			if (toupper(update_or_not) == 'Y')
			{
				get_client_info(client_data);
				string updated_to_record = convert_client_record_to_line(client_data, "/#/");
				update_record_in_file(clients_file, client_record, updated_to_record);
				cout << "\nClient updated successfully.";
			}
			else cout << "\nClient hasn't been updated.";
			return;
		}
	cout << "\nClient with Account Number (" << account_number_input << ") NOT FOUND!\n\n";
}

void find_client(string clients_file)
{
	string account_number_input;
	cout << "------------------------------------------";
	cout << "\n\tFind Client Screen";
	cout << "\n------------------------------------------";
	cout << "\n\nPlease, enter account number: ";
	cin >> account_number_input;
	vector <string> vec_clients_records;
	load_data_from_file_to_vector(clients_file, vec_clients_records);
	for (string& client_record : vec_clients_records)
		if (client_record.substr(0, 4) == account_number_input)
		{
			st_client client_data = convert_line_to_client_record(client_record, "/#/");
			show_client_card(client_data);
			return;
		}
	cout << "\nClient with Account Number (" << account_number_input << ") IS NOT FOUND!\n\n";
}

void deposit(string clients_file)
{
	string account_number_input;
	cout << "------------------------------------------";
	cout << "\n\t\tDeposit Screen";
	cout << "\n------------------------------------------";
	do
	{
		cout << "\n\nPlease, enter account number: ";
		cin >> account_number_input;
		vector <string> vec_clients_records;
		load_data_from_file_to_vector(clients_file, vec_clients_records);
		for (string& client_record : vec_clients_records)
			if (client_record.substr(0, 4) == account_number_input)
			{
				float deposit_amount;
				st_client client_data = convert_line_to_client_record(client_record, "/#/");
				show_client_card(client_data);
				cout << "\nPlease, enter deposit amount: ";
				cin >> deposit_amount;
				cout << "\nAre you sure you want perform this transaction (Y/N)?";
				char perform_or_not;
				cin >> perform_or_not;
				if (tolower(perform_or_not) == 'y')
				{
					client_data.account_balance += deposit_amount;
					string updated_to_record = convert_client_record_to_line(client_data, "/#/");
					update_record_in_file(clients_file, client_record, updated_to_record);
					cout << "\nDone successfully, the new account balance = " << client_data.account_balance;
				}
				else cout << "\nClient [" << client_data.name << "] account balance has not been changed.";
				return;
			}
		cout << "\nClient with [" << account_number_input << "] does not exist.";
	} while (true);
}

void withdraw(string clients_file)
{
	string account_number_input;
	cout << "------------------------------------------";
	cout << "\n\t\tWithdraw Screen";
	cout << "\n------------------------------------------";
	do
	{
		cout << "\n\nPlease, enter account number: ";
		cin >> account_number_input;
		vector <string> vec_clients_records;
		load_data_from_file_to_vector(clients_file, vec_clients_records);
		for (string& client_record : vec_clients_records)
			if (client_record.substr(0, 4) == account_number_input)
			{
				float withdraw_amount;
				st_client client_data = convert_line_to_client_record(client_record, "/#/");
				show_client_card(client_data);
				do
				{
					cout << "\nPlease, enter withdraw amount: ";
					cin >> withdraw_amount;
					if (withdraw_amount < client_data.account_balance)
					{
						cout << "\nAre you sure you want perform this transaction (Y/N)?";
						char perform_or_not;
						cin >> perform_or_not;
						if (tolower(perform_or_not) == 'y')
						{
							client_data.account_balance -= withdraw_amount;
							string updated_to_record = convert_client_record_to_line(client_data, "/#/");
							update_record_in_file(clients_file, client_record, updated_to_record);
							cout << "\nDone successfully, the new account balance = " << client_data.account_balance;
						}
						else cout << "\nClient [" << client_data.name << "] account balance has not been changed.";
						return;
					}
					cout << "\nAmount exceeds the balance, you can withdraw up to: " << client_data.account_balance;
				} while (true);
			}
		cout << "\nClient with [" << account_number_input << "] does not exist.";
	} while (true);
}

void show_total_balances(string clients_file)
{
	vector <string> vec_clients_records;
	st_client client_data;
	float total_balances = 0;
	load_data_from_file_to_vector(clients_file, vec_clients_records);
	cout << "\t\t\t\t\tClient List (" << vec_clients_records.size() << ") Client(s).";
	cout << "\n______________________________________________________________________________________________________\n";
	cout << "\n| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(10) << "Balance";
	cout << "\n______________________________________________________________________________________________________\n";
	for (string& client_record : vec_clients_records)
	{
		client_data = convert_line_to_client_record(client_record, "/#/");
		cout << "\n| " << left << setw(15) << client_data.account_number;
		cout << "| " << left << setw(40) << client_data.name;
		cout << "| " << left << setw(10) << client_data.account_balance;
		total_balances += client_data.account_balance;
	}
	cout << "\n\n______________________________________________________________________________________________________\n\n";
	cout << "\t\t\t\t\tTotal Balances = " << total_balances;
}

void transactions(string clients_file)
{
	short operation_number;
	do
	{
		show_trans_menue();
		cout << "\nChoose what do you want to do? [1 to 4]? ";
		cin >> operation_number;
		system("cls");
		if (operation_number >= 1 && operation_number <= 3)
		{
			switch (operation_number)
			{
			case en_trans_menue_options::en_deposit: deposit(clients_file); break;
			case en_trans_menue_options::en_withdraw: withdraw(clients_file); break;
			case en_trans_menue_options::en_total_balances: show_total_balances(clients_file);
			}
			cout << "\nPress any key to go back to transaction menue...";
			system("pause>0");
			system("cls");
		}
	} while (operation_number != 4);
}

void get_user_info(st_user& user_data)
{
	cout << "\nEnter Username: ";
	cin >> user_data.user_name;
	cout << "Enter Password: ";
	cin >> user_data.password;
}

st_user get_user_permissions_if_exist_in_file(string users_file, st_user user_input, bool for_login = true)
{
	st_user user_in_file;
	vector <string> vec_users_records;
	load_data_from_file_to_vector(users_file, vec_users_records);
	for (string& user_record : vec_users_records)
	{
		user_in_file = convert_line_to_user_record(user_record, "/#/");
		if (user_input.user_name == user_in_file.user_name && (!for_login || (for_login && user_input.password == user_in_file.password)))
		{
			user_input.password = user_in_file.password;
			user_input.permissions = user_in_file.permissions;
			break;
		}
	}
	return user_input;
}

void get_user_permissions(st_user& user_input)
{
	char access_or_not;
	cout << "\nDo you want to give full access (Y/N)? ";
	cin >> access_or_not;
	if (toupper(access_or_not) == 'Y') user_input.permissions = -1;
	else
	{
		user_input.permissions = 0;
		cout << "\nDo you want to give access to (Y/N)? ";
		cout << "\nShow Client list: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= SHOW_CLIENTS_LIST;
		cout << "\nAdd new clients: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= ADD_NEW_CLIENT;
		cout << "\nDelete client: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= DELETE_CLIENT;
		cout << "\nUpdate client: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= UPDATE_CLIENT;
		cout << "\nFind client: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= FIND_CLIENT;
		cout << "\nTransactions: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= TRANSACTIONS;
		cout << "\nManage users: ";
		cin >> access_or_not;
		if (toupper(access_or_not) == 'Y') user_input.permissions |= MANAGE_USERS;
	}
}

void show_manage_users_screen()
{
	cout << "==============================================================";
	cout << "\n\t\t\tManage Users Screen";
	cout << "\n==============================================================";
	cout << "\n\t[1] List Users.";
	cout << "\n\t[2] Add New User.";
	cout << "\n\t[3] Delete User.";
	cout << "\n\t[4] Update User.";
	cout << "\n\t[5] Find User.";
	cout << "\n\t[6] Main Menue.";
	cout << "\n==============================================================";
}

void list_users(string users_file)
{
	vector <string> vec_users_records;
	st_user user_data;
	load_data_from_file_to_vector(users_file, vec_users_records);
	cout << "\t\t\t\t\tusers List (" << vec_users_records.size() << ") user(s).";
	cout << "\n______________________________________________________________________________________________________\n";
	cout << "\n| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(15) << "Permissions";
	cout << "\n______________________________________________________________________________________________________\n";
	for (string& user_record : vec_users_records)
	{
		user_data = convert_line_to_user_record(user_record, "/#/");
		cout << "\n| " << left << setw(15) << user_data.user_name;
		cout << "| " << left << setw(15) << user_data.password;
		cout << "| " << left << setw(15) << user_data.permissions;
	}
	cout << "\n\n______________________________________________________________________________________________________\n\n";
}

void show_user_card(st_user user)
{
	cout << "\nThe following are the client details:";
	cout << "\n------------------------------------------";
	cout << "\nUsername: " << user.user_name;
	cout << "\nPassword: " << user.password;
	cout << "\nPermissions: " << user.permissions;
	cout << "\n------------------------------------------\n";
}

void add_new_user(string users_file)
{
	st_user added_user;
	char add_or_not;
	cout << "------------------------------------------";
	cout << "\n\tAdd New User Screen";
	cout << "\n------------------------------------------";
	cout << "\nAdding New User:\n";
	do
	{
		cout << "\nEnter Username: ";
		cin >> added_user.user_name;
		added_user = get_user_permissions_if_exist_in_file(users_file, added_user, false);
		while (added_user.permissions != -2)
		{
			cout << "\nUser with [" << added_user.user_name << "] already exists, enter another username: ";
			cin >> added_user.user_name;
			added_user.permissions = -2;
			added_user = get_user_permissions_if_exist_in_file(users_file, added_user, false);
		}
		cout << "\nEnter Password: ";
		cin >> added_user.password;
		get_user_permissions(added_user);
		string user_record = convert_user_record_to_line(added_user, "/#/");
		add_line_to_file(users_file, user_record);
		cout << "\nUser added successfully, do you want to add more users (Y/N)? ";
		cin >> add_or_not;
	} while (tolower(add_or_not) == 'y');
}

void delete_users(string users_file)
{
	st_user deleted_user;
	char delete_or_not;
	cout << "------------------------------------------";
	cout << "\n\tDelete Users Screen";
	cout << "\n------------------------------------------";
	cout << "\nEnter Username: ";
	cin >> deleted_user.user_name;
	deleted_user = get_user_permissions_if_exist_in_file(users_file, deleted_user, false);
	if (deleted_user.permissions != -2)
	{
		show_user_card(deleted_user);
		cout << "\nAre you sure you want delete this user (Y/N)? ";
		cin >> delete_or_not;
		if (tolower(delete_or_not) == 'y')
		{
			string user_record = convert_user_record_to_line(deleted_user, "/#/");
			delete_record_from_file(users_file, user_record);
			cout << "\nUser deleted successfully.";
		}
		else cout << "\nClient hasn't been deleted.";
	}
	else cout << "\nUser with username [" << deleted_user.user_name << "] is NOT FOUND!";
}

void update_users(string users_file)
{
	st_user updated_user;
	char update_or_not;
	cout << "------------------------------------------";
	cout << "\n\tUpdate Users Screen";
	cout << "\n------------------------------------------";
	cout << "\nEnter Username: ";
	cin >> updated_user.user_name;
	updated_user = get_user_permissions_if_exist_in_file(users_file, updated_user, false);
	if (updated_user.permissions != -2)
	{
		string old_record = convert_user_record_to_line(updated_user, "/#/");
		show_user_card(updated_user);
		cout << "\nAre you sure you want update this user (Y/N)? ";
		cin >> update_or_not;
		if (tolower(update_or_not) == 'y')
		{
			cout << "\nEnter Password: ";
			cin >> updated_user.password;
			get_user_permissions(updated_user);
			string updated_record = convert_user_record_to_line(updated_user, "/#/");
			update_record_in_file(users_file, old_record, updated_record);
			cout << "\nUser updated successfully.";
		}
		else cout << "\nClient hasn't been updated.";
	}
	else cout << "\nUser with username [" << updated_user.user_name << "] is NOT FOUND!";
}

void find_user(string users_file)
{
	st_user user_data;
	cout << "------------------------------------------";
	cout << "\n\tFind User Screen";
	cout << "\n------------------------------------------";
	cout << "\nEnter Username: ";
	cin >> user_data.user_name;
	user_data = get_user_permissions_if_exist_in_file(users_file, user_data, false);
	if (user_data.permissions != -2) show_user_card(user_data);
	else cout << "\nUser with username [" << user_data.user_name << "] is NOT FOUND!";
}

void manage_users(string users_file)
{
	short operation_number;
	do
	{
		show_manage_users_screen();
		cout << "\nChoose what do you want to do? [1 to 6]? ";
		cin >> operation_number;
		system("cls");
		if (operation_number >= 1 && operation_number <= 5)
		{
			switch (operation_number)
			{
			case en_manage_users_options::en_list_users: list_users(users_file); break;
			case en_manage_users_options::en_add_new_user: add_new_user(users_file); break;
			case en_manage_users_options::en_delete_user: delete_users(users_file); break;
			case en_manage_users_options::en_update_user: update_users(users_file); break;
			case en_manage_users_options::en_find_user: find_user(users_file);
			}
			cout << "\nPress any key to go back to manage users screen...";
			system("pause>0");
			system("cls");
		}
	} while (operation_number != 6);
}

void show_access_denied_message()
{
	cout << "------------------------------------------";
	cout << "\nAccess Denied,";
	cout << "\nYou do not have permission to do this,";
	cout << "\nPlease contact your admin.";
	cout << "\n------------------------------------------\n";
}

void main_menue(string clients_file, string users_file, st_user user)
{
	short operation_number;
	do
	{
		show_main_menue();
		cout << "\nChoose what do you want to do? [1 to 8]? ";
		cin >> operation_number;
		system("cls");
		if (operation_number >= 1 && operation_number <= 5)
		{
			switch (operation_number)
			{
			case en_main_menue_options::en_list_clients:
				if (user.permissions & SHOW_CLIENTS_LIST) show_clients_list(clients_file);
				else show_access_denied_message();
				break;
			case en_main_menue_options::en_add_new_client:
				if (user.permissions & ADD_NEW_CLIENT) add_new_client(clients_file);
				else show_access_denied_message();
				break;
			case en_main_menue_options::en_delete_client:
				if (user.permissions & DELETE_CLIENT) delete_client(clients_file);
				else show_access_denied_message();
				break;
			case en_main_menue_options::en_update_client:
				if (user.permissions & UPDATE_CLIENT) update_client_info(clients_file);
				else show_access_denied_message();
				break;
			case en_main_menue_options::en_find_client:
				if (user.permissions & FIND_CLIENT) find_client(clients_file);
				else show_access_denied_message();
			}
			cout << "\nPress any key to go back to main menue...";
			system("pause>0");
			system("cls");
		}
		else if (operation_number == en_main_menue_options::en_transactions)
		{
			if (user.permissions & TRANSACTIONS) transactions(clients_file);
			else
			{
				show_access_denied_message();
				cout << "\nPress any key to go back to main menue...";
				system("pause>0");
				system("cls");
			}
		}
		else if (operation_number == en_main_menue_options::en_manage_users)
		{
			if (user.permissions & MANAGE_USERS) manage_users(users_file);
			else
			{
				show_access_denied_message();
				cout << "\nPress any key to go back to main menue...";
				system("pause>0");
				system("cls");
			}
		}
	} while (operation_number != 8);
}

void show_login_screen()
{
	cout << "------------------------------------------";
	cout << "\n\t\tLogin Screen";
	cout << "\n------------------------------------------\n";
}

void login(string users_file, st_user& user)
{
	show_login_screen();
	get_user_info(user);
	user = get_user_permissions_if_exist_in_file(users_file, user);
	while (user.permissions == -2)
	{
		system("cls");
		show_login_screen();
		cout << "Invalid Username/Password!";
		get_user_info(user);
		user = get_user_permissions_if_exist_in_file(users_file, user);
	}
}

void bank_program(string clients_file, string users_file)
{
	do
	{
		st_user user;
		login(users_file, user);
		system("cls");
		main_menue(clients_file, users_file, user);
	} while (true);
}

int main()
{
	bank_program("clients.txt", "users.txt");
}