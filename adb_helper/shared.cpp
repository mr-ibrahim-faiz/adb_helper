#include "shared.h"

#include<stdexcept>
using std::runtime_error;

#include<fstream>
using std::ifstream;
using std::fstream;

#include<string>
using std::to_string;

#include<iostream>
using std::cerr;
using std::cout;

#include<iomanip>
using std::setw;

// symbolic names
const string empty_string { "" };
constexpr char carriage_return { '\r' };
constexpr char backslash { '\\' };

// runs command
void run_command(const string& command)
// executes a system command
{
	int result = system(command.c_str());
	if(result != 0) throw runtime_error("Unable to run \'" + command + "\'.");
}

// creates file if it doesn't exist
void create_file(const string& filename){
	run_command("touch " + filename);
}

// removes file
void remove_file(const string& filename){
	int result = remove(filename.c_str());
	if(result != 0) cerr << "Unable to delete \'" + filename + "\'.";
}

// removes directory from computer
void remove_directory(const string& directory){
	const string command = "rm -rdf " + directory;
	try { run_command(command); }
	catch(runtime_error& e){ cerr << "Error: unable to remove directory \'" << directory << "\'.\n"; }
}

// removes directory from phone
void remove_directory_phone(const string& directory){
	const string command = "adb shell rm -rf " + directory;
	try { run_command(command); }
	catch(runtime_error& e){ cerr << "Error: unable to remove directory \'" << directory << "\'.\n"; }
}

// gets path
string get_path(const string& filepath){
	string path = filepath;
	while(!path.empty() && path.back() != '/') path.pop_back();
	return path;
}

// gets file name
string get_filename(const string& filepath){
	const string path = get_path(filepath);
	const size_t size = path.size();
	string filename = filepath;
	filename.erase(0, size);
	return filename;
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

// lists files
void list_files(const string& path_directory, const string& path_file, const string& pattern)
// lists files from a directory into a file
// a pattern can be used to refine the results
{
	const string command = "ls " + path_directory + " > /tmp/list_backup.txt 2>&1 ; grep -i " + pattern + " /tmp/list_backup.txt > " + path_file + " ; rm /tmp/list_backup.txt";
	try { run_command(command); }
	catch(runtime_error& e) { 
		remove_file(path_file); // prevents invalid data from being used
		run_command("touch " + path_file);
		write_to_file(log_filename, "Error: unable to list files.\n"); 
	}
}

// gets data from a file
vector<string> get_lines(const string& filename)
// gets data from a file line by line into a vector of string
{
	vector<string> lines;
	ifstream file(filename);
	if(file.is_open()){
		for(string line; getline(file, line);) if(!line.empty()) lines.push_back(line);
		file.close();
	}
	else cerr << "Unable to open \'" + filename + "\'.\n";	
	return lines;
}

// displays progression
void display_progression(const size_t& position, const size_t& total){
	size_t percentage = (position+1)*(100.0/total);
	cout << "[" << setw(3) << percentage << "%]";
}

// writes to a file
void write_to_file(const string& filename, const string& stream, const ios_base::openmode& mode)
// writes a string of character into a file
{
	fstream file(filename, mode);
	if(file.is_open()){
		file << stream;
		file.close();
	}
	else cout << "Error: unable to open \'" << filename << "\'.\n";
}

// processes filename
string process_name_if(const string& name)
// makes sure the name of the file is valid
{
	string result;
	for(const char& character: name){
		if(isblank(character)) result.push_back(backslash);
		result += character;
	}
	return result;
}

// clears line
void clear_line(const int& length)
// replaces a number of characters by spaces in the last line printed on screen 
{
	cout << carriage_return;
	cout << setw(length) << empty_string;
	cout << carriage_return;
}
