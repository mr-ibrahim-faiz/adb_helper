#include "helper.h"
#include "puller.h"
#include "backup.h"
#include "shared.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;

#include<stdexcept>
using std::runtime_error;

// starts adb server
void start_server(){
	const string command = "adb devices > " + log_filename + " 2>&1";
	run_command(command);
}

// stops adb server
void kill_server(){
	const string command = "adb kill-server >> " + log_filename + " 2>&1";
	run_command(command);
}

// displays main menu
void display_main_menu()
// informs the user on available choices
{
	cout << "[1] Pull\n";
	cout << "[2] Backup files\n";
	cout << "[3] Install apps from Backup directory\n";
	cout << "[x] Exit\n";	
}

// displays pull menu
void display_pull_menu()
// informs the user on available choices
{
	cout << "[1] Pull third-party apps\n";
	cout << "[2] Pull system apps\n";
	cout << "[3] Pull all apps\n";
	cout << "[4] Pull app\n";
	cout << "[5] Pull hosts\n";
	cout << "[6] Pull build.prop\n";
	cout << "[x] Main menu\n";
}

// app puller main function
void pull_app()
// pulls an app using a pattern or name provided by the user
{
	cout << "Please enter a package name or pattern:\n> ";
	string pattern;
	getline(cin, pattern);
	cout << newline;
	if(!pattern.empty()) packages_puller(Type::all, pattern);
	else cout << "Aborting...\n";
}

// puller main function
void pull()
try
{
	// displays main menu
	display_pull_menu();

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if (choice.length() != valid_choice_length) choice = INVALID_CHOICE;

		char& user_choice = choice[0];
		if(user_choice != exit_character) cout << newline;

		switch (user_choice) {
		case '1': 
			packages_puller(Type::nosystem);
			break;

		case '2':
			packages_puller(Type::system);
			break;

		case '3':
			packages_puller(Type::all);
			break;

		case '4':
			pull_app();
			break;

		case '5':
		{
			const string path_hosts { "/etc/hosts" };
			update_file(path_hosts);
		}
			break;

		case '6':
		{
			const string path_build_prop { "/system/build.prop" };
			update_file(path_build_prop);
		}

		case exit_character:
			break;

		default:
			cout << "Please enter a valid choice.\n";
			break;
		}

		if (user_choice == exit_character) break;
		else {
			cout << newline;
			display_pull_menu();
		}
	}

	return;
}
catch(runtime_error& e){
	const string error_message = "Error: " + string(e.what()) + "\n";
	write_to_file(log_filename, error_message);
	cerr << "Error: unable to pull file(s).\n\n";
	pull();
}

// backs up files
void backup_files()
// pulls specific directories from phone, saves them to the pull root directory and removes them from the phone
{
	// backs up images
	const string device_dcim { "/sdcard/DCIM" };
	const string device_pictures { "/sdcard/Pictures" };
	const string sdcard_dcim { "/storage/2065-AB5B/DCIM" };

	// backs up other files 
	const string device_sounds { "/sdcard/Sounds" };
			
	// backs up LP/Backup
	const string device_LP { "/sdcard/LuckyPatcher/Backup" };

	// backs up TWRP
	const string sdcard_twrp { "/storage/2065-AB5B/TWRP" };

	const vector<string> paths { device_dcim, device_pictures, sdcard_dcim, device_sounds, device_LP, sdcard_twrp };

	if(!paths.empty()){
		cout << "Backing up file(s)...\n";

		const size_t size = paths.size();
		for(size_t i = 0; i < size; ++i){
			const string& path = paths[i];
			clear_line(150);
			display_progression(i, size);
			cout << " Backing up " << path <<  " " << std::flush;
			backup_and_remove_directory(path);
		}

		clear_line(150);
		display_progression(size-1, size);
		cout << " Done.\n";
		open_directory(backup_root_directory);
	}
	else cout << "There are no files to pull.\n";
}

// file updater main function
void update_file(const string& filepath)
// pulls a file from phone,
// allows the user to modify the pulled filed before pushing it back to the phone
{
	cout << "Pulling file... ";
	pull_file(filepath, pull_root_directory);
	cout << "Done.\n";

	cout << "\nDo you want to modify the file? [Y/n]\n";
		
	// gets user's choice
	for(string choice; getline(cin, choice);){
		if(choice.length() != valid_choice_length) choice = INVALID_CHOICE;
				
		char& user_choice = choice[0];
		switch(user_choice){
		case yes:
		{
			cout << newline;

			const string filename = get_filename(filepath);
			string filepath_updated = pull_root_directory + "/" + filename;

			run_command("vim " + filepath_updated);

			cout << "Pushing file... ";
			run_command("adb push " + filepath_updated + " /sdcard/ > /dev/null 2>&1");
			run_command(string("adb shell \'su -c \"mount -o rw,remount /system\"") + "\' >> " + log_filename + " 2>&1");
			run_command("adb shell \'su -c \"mv /sdcard/" + filename + " " + filepath + "\"" + "\' >> " + log_filename + " 2>&1" );
			run_command("adb shell \'su -c \"chmod 644 " + filepath + "\"" + "\' >> " + log_filename + " 2>&1");
			run_command("adb shell \'su -c \"chown root " + filepath + "\"" + "\' >> " + log_filename + " 2>&1");
			run_command("adb shell \'su -c \"chgrp root " + filepath + "\"" + "\' >> " + log_filename + " 2>&1");
			run_command(string("adb shell \'su -c \"mount -o ro,remount /system\"") + "\' >> " + log_filename + " 2>&1");
			cout << "Done.\n";
			run_command("adb reboot");
		}
			return;

		case no:
			return;

		default:
			cout << "\nPlease enter a valid choice.\n";
			break;
		}
	}
}
