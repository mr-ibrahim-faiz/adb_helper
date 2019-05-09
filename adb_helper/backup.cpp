#include "backup.h"
#include "useful.h"

// backups directory
void backup_directory(const string& directory){
	const string command = "adb pull " + directory + " " + backup_root_directory; 
	run_command(command);
	remove_directory(directory);
	open_directory(backup_root_directory);
}
