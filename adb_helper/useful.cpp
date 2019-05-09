#include "useful.h"

#include<stdexcept>
using std::runtime_error;

// runs command
void run_command(const string& command){
	int result = system(command.c_str());
	if(result != 0) throw runtime_error("Unable to run \'" + command + "\'. Aborting...");
}

// removes file
void remove_file(const string& filename){
	int result = remove(filename.c_str());
	if(result != 0) throw runtime_error("Unable to delete \'" + filename + "\'. Aborting...");
}

// removes directory
void remove_directory(const string& directory){
	const string command = "adb shell rm -rf " + directory;
	run_command(command);
}

// gets path
string get_path(const string& filepath){
	string path = filepath;
	while(!path.empty() && path.back() != '/') path.pop_back();
	return path;
}

// creates directory
void create_directory(const string& path){
	const string command = "mkdir -p "+ path;
	run_command(command);
}

// opens directory
void open_directory(const string& path){
	const string command = "xdg-open " + path;
	run_command(command);
}
