#include "manager.h"
#include "shared.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include<stdexcept>
using std::runtime_error;

// file names
const string temp_install_filename { "/tmp/adb.helper.install.txt" }; 
const string temp_uninstall_filename { "/tmp/adb.helper.uninstall.txt" }; 

// symbolic names
const string package_extension { ".apk" };

// installs a single package
void install_package(const string& package_name){
	const string command = "adb install -r " + package_name + " >> " + log_filename + " 2>&1";
	try {
		run_command(command);
	}
	catch(runtime_error& e){
		const string log_message = "Error: " + string(e.what()) + "\n";
		write_to_file(log_filename, log_message);
		clear_line();
		const string error_message = "Error: unable to install " + package_name + ".";
		cerr << trim_line(error_message) << endl;
	}
}

// uninstalls a single package
void uninstall_package(const string& package_name, const bool& with_root_privileges){
	const string command = string("adb shell") + ((with_root_privileges)? " su -c " : " ") + "pm uninstall --user 0 " + package_name + " >> " + log_filename + " 2>&1";
	try {
		run_command(command);
	}
	catch(runtime_error& e){
		const string log_message = "Error: " + string(e.what()) + "\n";
		write_to_file(log_filename, log_message);
		clear_line();
		const string error_message = "Error: unable to uninstall " + package_name + ".";
		cerr << trim_line(error_message) << endl;
	}
}

// deletes data associated to a package
void clear_package(const string& package_name, const bool& with_root_privileges){
	const string command = string("adb shell") + ((with_root_privileges)? " su -c " : " ") + "pm clear --user 0 " + package_name + " >> " + log_filename + " 2>&1";
	try {
		run_command(command);
	}
	catch(runtime_error& e){
		const string log_message = "Error: " + string(e.what()) + "\n";
		write_to_file(log_filename, log_message);
		clear_line();
		const string error_message = "Error: unable to clear " + package_name + ".";
		cerr << trim_line(error_message) << endl;
	}
}

// disables package
void disable_package(const string& package_name, const bool& with_root_privileges){
	const string command = string("adb shell") + ((with_root_privileges)? " su -c " : " ") + "pm disable --user 0 " + package_name + " >> " + log_filename + " 2>&1";
	try {
		run_command(command);
	}
	catch(runtime_error& e){
		const string log_message = "Error: " + string(e.what()) + "\n";
		write_to_file(log_filename, log_message);
		clear_line();
		const string error_message = "Error: unable to disable " + package_name + ".";
		cerr << trim_line(error_message) << endl;
	}
}

// packages installer
void packages_installer(const string& path)
// installs packages from a directory
{
	list_files(path, temp_install_filename, package_extension);
	vector<string> packages = get_lines(temp_install_filename);
	remove_file(temp_install_filename);

	if(!packages.empty()){
		cout << "Installing packages...\n";
		
		const size_t size = packages.size();
		for(size_t i = 0; i < size; ++i){
			const string& name = packages[i];
			clear_line();
			display_progression(i, size);
			cout << empty_space;

			const string message = "Installing " + name + " ";
			const size_t number_of_ignored_characters { progression_output_length };
			cout << trim_line(message, number_of_ignored_characters) << std::flush;
			install_package(path + process_name_if(name));
		}
		clear_line();
		display_progression(size-1, size);
		cout << " Done.\n";

		cout << "\nDo you want to delete the directory? [Y/n]\n";
	
		// gets user's choice
		for(string choice; getline(cin, choice);){
			if (choice.length() != valid_choice_length) choice = INVALID_CHOICE;
	
			const char& user_choice = choice[0];
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

// packages uninstaller
void packages_uninstaller()
// uninstall packages from a file
{
	create_file(temp_uninstall_filename);
	run_command("vim " + temp_uninstall_filename);
	vector<string> packages = get_lines(temp_uninstall_filename);
	const size_t package_size = packages.size();
	vector<string> paths;

	cout << "Getting packages information... \n";
	list_packages();
	vector<Package> packages_information = get_packages_information();

	for(size_t i = 0; i < package_size; ++i) {
		const string& name = packages[i];
		clear_line();
		display_progression(i, package_size);
		const size_t number_of_ignored_characters { progression_output_length };
		cout << trim_line(name, number_of_ignored_characters) << std::flush;
		string path = get_package_path(name, packages_information);
		if(!path.empty()) paths.push_back(path);
	}
	clear_line();
	if(package_size == 0) display_progression(1, 2);
	else display_progression(package_size -1, package_size);
	cout << " Done.\n\n";

	if(!packages.empty()){
		cout << "Processing packages...\n";
		
		for(size_t i = 0; i < package_size; ++i){
			const string& name = packages[i];
			const size_t number_of_ignored_characters { progression_output_length };

			clear_line();
			display_progression(i, package_size);
			cout << empty_space;

			const string message_clearing = "Clearing " + name + "... ";
			cout << trim_line(message_clearing, number_of_ignored_characters) << std::flush;
			clear_package(name, true);

			clear_line();
			display_progression(i, package_size);
			cout << empty_space;

			const string message_disabling = "Disabling " + name + "... ";
			cout << trim_line(message_disabling, number_of_ignored_characters) << std::flush;
			disable_package(name, true);

			clear_line();
			display_progression(i, package_size);
			cout << empty_space;

			const string message_uninstalling = "Uninstalling " + name + "... ";
			cout << trim_line(message_uninstalling, number_of_ignored_characters) << std::flush;
			uninstall_package(name, true);
		}
		clear_line();
		display_progression(package_size-1, package_size);
		cout << " Done.\n\n";
		
		if(!paths.empty()){
			cout << "Removing folders... \n";
			run_command("adb shell su -c mount -o rw,remount /system >> " + log_filename + " 2>&1");
			const size_t paths_size = paths.size();
			for(size_t i = 0; i < paths_size; ++i){
				const string& path = paths[i];
				clear_line();
				display_progression(i, paths_size);
				cout << " Removing " << path << "... " << std::flush;
				remove_directory_phone(path);
			}
			clear_line();
			display_progression(paths_size-1, paths_size);
			run_command("adb shell su -c mount -o ro,remount /system >> " + log_filename + " 2>&1");
			cout << " Done.\n";
		}
		else cout << "There is no folder to remove.\n";
		
		cout << "\nDo you want to delete the file? [Y/n]\n";
		
		// gets user's choice
		for(string choice; getline(cin, choice);){
			if (choice.length() != valid_choice_length) choice = INVALID_CHOICE;
	
			const char& user_choice = choice[0];
			switch(user_choice){
			case yes:
				cout << "\nRemoving file... ";
				remove_file(temp_uninstall_filename);
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
	else cout << "There is no app to uninstall.\n";
}
