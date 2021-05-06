#include "puller.h"
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
using std::streamsize;

#include<iomanip>
using std::setw;

#include<algorithm>
using std::find_if;

// file names
const string packages_filename { "/tmp/adb.helper.packages" };

// symbolic names
constexpr char delimiter { '=' };
const string empty_string { "" };
constexpr char carriage_return { '\r' };
constexpr char backslash { '\\' };
const string package_prefix { "package:" };

// gets terminal windows size
TerminalWindowSize get_terminal_window_size()
// retrieves the number of columns and rows of the terminal window
{
	TerminalWindowSize tws {};

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	const BOOL has_succeeded = GetConsoleScreenBufferInfo(output_handle, &csbi);

	if (has_succeeded) {
		tws.columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		tws.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}

	return tws;
}

// runs command
void run_command(const string& command)
// executes a system command
{
	int result = system(command.c_str());
	if(result != 0) throw runtime_error("Unable to run \'" + command + "\'.");
}

// creates file if it doesn't exist
void create_file(const string& filename){
	run_command("type nul > " + filename);
}

// removes file
void remove_file(const string& filename){
	int result = remove(filename.c_str());
	if(result != 0) cerr << "Unable to delete \'" + filename + "\'.";
}

// removes directory from computer
void remove_directory(const string& directory){
	const string command = "rmdir /Q /S " + directory;
	try { run_command(command); }
	catch(const runtime_error&){ cerr << "Error: unable to remove directory \'" << directory << "\'.\n"; }
}

// removes directory from phone
void remove_directory_phone(const string& directory){
	const string command = "adb shell su -c rm -rf " + directory;
	try { run_command(command); }
	catch(const runtime_error&){ cerr << "Error: unable to remove directory \'" << directory << "\'.\n"; }
}

// gets path
string get_path(const string& filepath){
	string path = filepath;
	while(!path.empty() && path.back() != '/') path.pop_back();
	return path;
}

// gets package path
string get_package_path(const string& name, const vector<Package>& packages){
	using const_iterator = typename vector<Package>::const_iterator;
	const_iterator found = find_if(packages.begin(), packages.end(), [&](const Package& package) -> bool { return name == package.Name(); });
	string path;
	if(found != packages.end()) path = (*found).Path();
	path = get_path(path);
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
	const string command = "if not exist \"" + path + "\" mkdir \""+ path + "\"";
	run_command(command);
}

// opens directory
void open_directory(const string& path){
	const string command = "explorer \"" + path + "\"";
	try { run_command(command); } // catches false positive when calling explorer
	catch (const runtime_error&) {};
}

// lists files
void list_files(const string& path_directory, const string& path_file, const string& pattern)
// lists files from a directory into a file
// a pattern can be used to refine the results
{
	const string command = "dir /b " + path_directory + "*" + pattern + " > " + path_file + " 2>&1";
	try { run_command(command); }
	catch(const runtime_error&) { 
		remove_file(path_file); // prevents invalid data from being used
		create_file(path_file);
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
	size_t percentage = size_t ( (position + 1)*(100.0 / total) );
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

// checks if character is parenthesis
bool isparenthesis(int c){
	if(c == '(' || c == ')') return true;
	return false;
}

// processes filename
string process_name_if(const string& name)
// makes sure the name of the file is valid
{
	string result;
	for(const char& character: name){
		if(isblank(character) || isparenthesis(character)) result.push_back(backslash);
		result += character;
	}
	return result;
}

// clears line
void clear_line()
// replaces a number of characters by spaces in the last line printed on screen 
{
	cout << carriage_return;
	TerminalWindowSize tws = get_terminal_window_size();
	const size_t length = (size_t) tws.columns - 1;
	cout << setw((streamsize)length) << empty_string;
	cout << carriage_return;
}

// lists packages
void list_packages(const string& pattern)
// lists packages and associated files into a file
// a pattern can be specified to refine the results
{
	const string command = "adb shell \"pm list packages -f" + ((pattern.empty())? "" : " | grep -i " + pattern) + "\" > " + packages_filename + " 2>&1";
	run_command(command);
}

// gets packages information
vector<Package> get_packages_information(const Type& type)
// gets packages information from file based on the type
{
	vector<Package> packages;
	ifstream file(packages_filename);
	if(file.is_open()){
		const size_t length = package_prefix.size();
		for(string raw, name, path; getline(file, raw);){
			const size_t position = raw.find_last_of(delimiter);
			path = raw.substr(0, position);
			name = raw.substr(position+1);
			path = remove_prefix(path, length);
			if(name.back()==carriage_return) name.pop_back(); // removes sneaky carriage return

			if(!name.empty() && !path.empty()) {
				Package package ( name, path );
				if(Type(package.System()) == type || type == Type::all) packages.push_back(package);
			}
		}
		file.close();
	}
	else cerr << "Error: Unable to open \'" + packages_filename + "\'.\n";

	return packages;
}

// trims line
string trim_line(const string& line, const size_t& number_of_ignored_characters)
// trims a string sequence so it can fit the terminal window
// 3 dots are added, e.g., azertyuio becomes aze...uio
{
	string line_trimmed { line };
	const string hidden_sequence { "..." };

	TerminalWindowSize tws = get_terminal_window_size();
	if(number_of_ignored_characters > tws.columns - hidden_sequence.length() - 1) return hidden_sequence;
	const size_t length = (size_t) tws.columns - number_of_ignored_characters - hidden_sequence.length() - 1;
	const size_t position = length/2;

	// if(name.size() + progression_output_length > length){
	if(line.length() > length){
		line_trimmed = line.substr(0, position);
		line_trimmed.append(hidden_sequence);
		line_trimmed.append(line.substr(line.size() - position));
	}

	return line_trimmed;
}
