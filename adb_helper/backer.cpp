#include "backer.h"
#include "shared.h"

#include<iostream>
using std::cerr;

#include<stdexcept>
using std::runtime_error;

// backs up directory
int backup_directory(const string& directory_path)
// pulls a directory from phone and saves it to the backup root directory
// returns 0 if the operation succeeded,
// returns 1 otherwise
{
	const string command = "adb pull " + directory_path + " " + backup_root_directory + " >> " + log_filename; 
	try {
		run_command(command);
	}
	catch(runtime_error& e){
		const string error_message = "Error: " + string(e.what()) + "\n"; 
		write_to_file(log_filename, error_message);
		clear_line(150);
		cerr << "Error: unable to pull " << directory_path << ".\n";
		return 1;
	}
	return 0;
}

// backs up and removes directory
void backup_and_remove_directory(const string& directory_path)
// pulls a directory from phone, saves it to the backup root directory and removes the directory from phone
{
	const int result = backup_directory(directory_path);
	if(result == 0) remove_directory_phone(directory_path);
}
