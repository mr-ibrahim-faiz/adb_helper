#include "backer.h"
#include "shared.h"

#include<iostream>
using std::cerr;
using std::endl;

#include<stdexcept>
using std::runtime_error;

// backs up directory
int backup_directory(const string& directory_path)
// pulls a directory from phone and saves it to the backup root directory
// returns 0 if the operation succeeded,
// returns 1 otherwise
{
	const string directory_thumbnails = directory_path + "/.thumbnails";
	const string directory_gallery2 = directory_path + "/.Gallery2";
	const string command = "adb pull " + directory_path + " " + backup_root_directory + " >> " + log_filename; 
	try {
		remove_directory_phone(directory_thumbnails);
		remove_directory_phone(directory_gallery2);
		run_command(command);
	}
	catch(runtime_error& e){
		const string log_message = "Error: " + string(e.what()) + "\n";
		write_to_file(log_filename, log_message);
		clear_line();
		const string error_message = "Error: unable to pull " + directory_path + ".";
		cerr << trim_line(error_message) << endl;
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
