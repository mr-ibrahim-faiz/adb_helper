#include "backup.h"
#include "install.h"
#include "helper.h"
#include "shared.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;

#include<stdexcept>
using std::runtime_error;

int main()
try
{
	// starts adb server
	start_server();

	// displays main menu
	display_main_menu();

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if (choice.length() != valid_choice_length) choice = INVALID_CHOICE;

		const char& user_choice = choice[0];
		cout << newline;

		switch (user_choice) {
		case '1': 
			pull();
			break;

		case '2':
			backup_files();
			break;

		case '3':
			packages_installer(backup_root_directory+"Backup/");
			break;

		case exit_character:
			kill_server();
			break;

		default:
			cout << "Please enter a valid choice.\n";
			break;
		}

		if (user_choice == exit_character) break;
		else {
			cout << newline;
			display_main_menu();
		}
	}

	cout << "Goodbye !\n";

	return 0;
}
catch(runtime_error& e){
	const string error_message = "Error: " + string(e.what()) + "\n";
	write_to_file(log_filename, error_message);
	return 1;
}
catch(...){
	cerr << "Error: something went wrong.\n";
	write_to_file(log_filename, "Error: unknown exception caught.\n");
	return 2;
}
