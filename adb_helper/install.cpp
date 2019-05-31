#include "install.h"
#include "shared.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;

#include<stdexcept>
using std::runtime_error;

// file names
const string temp_filename { "/tmp/adb.helper.install.txt" }; 
const string package_extension { ".apk" };

// installs a single package
void install_package(const string& package_name){
	const string command = "adb install -r " + package_name + " >> " + log_filename + " 2>&1";
	try {
		run_command(command);
	}
	catch(runtime_error& e){
		const string error_message = "Error: " + string(e.what()) + "\n"; 
		write_to_file(log_filename, error_message);
		clear_line(150);
		cerr << "Error: unable to install " << package_name << ".\n"; 
	}
}

// packages installer
void packages_installer(const string& path)
// installs packages from a file
{
	list_files(path, temp_filename, package_extension);
	vector<string> packages = get_lines(temp_filename);
	remove_file(temp_filename);

	if(!packages.empty()){
		cout << "Installing packages...\n";
		
		const size_t size = packages.size();
		for(size_t i = 0; i < size; ++i){
			const string& name = packages[i];
			clear_line(150);
			display_progression(i, size);
			cout << " Installing " << name << " " << std::flush;
			install_package(path + process_name_if(name));
		}
		clear_line(150);
		display_progression(size-1, size);
		cout << " Done.\n";

		cout << "\nDo you want to delete the directory? [Y/n]\n";
	
		// gets user's choice
		for(string choice; getline(cin, choice);){
			if (choice.length() != valid_choice_length) choice = INVALID_CHOICE;
	
			char& user_choice = choice[0];
			switch(user_choice){
			case yes:
				cout << "\nRemoving " << path << "... ";
				remove_directory(path);
				cout << "Done.\n";
				return;

			case no:
				return;

			default:
				cout << "\nPlease enter a valid choice.\n";
				break;
			}
		}
	}
	else cout << "There is no app to install.\n";
}
