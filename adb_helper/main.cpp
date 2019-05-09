#include "puller.h"
#include "backup.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;

#include<stdexcept>
using std::runtime_error;

// displays main menu
void display_menu();

int main()
try
{
	// displays main menu
	display_menu();

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if (choice.length() != 1) choice = INVALID_CHOICE;

		char& user_choice = choice[0];
		cout << newline;

		switch (user_choice) {
		case '1': 
			puller(Type::nosystem);
			break;

		case '2':
			puller(Type::system);
			break;

		case '3':
			puller(Type::all);
			break;

		case '4':
			backup_directory("/sdcard/LuckyPatcher/Backup");
			break;

		case exit_character:
			break;

		default:
			cout << "Please enter a valid choice.\n";
			break;
		}

		if (user_choice == exit_character) break;
		else {
			cout << newline;
			display_menu();
		}
	}

	cout << "Goodbye !\n";

	return 0;
}
catch(runtime_error& e){
	cerr << "Error: " << e.what() << '\n';
	return 1;
}
catch(...){
	cerr << "Error: unknown exception caught.\n";
	return 2;
}

// displays main menu
void display_menu()
// displays main menu
// informs the user about available choices
{
	cout << "[1] Pull third-party apps\n";
	cout << "[2] Pull system apps\n";
	cout << "[3] Pull all apps\n";
	cout << "[4] Back up LP/Directory\n";
	cout << "[x] Exit\n";	
}
