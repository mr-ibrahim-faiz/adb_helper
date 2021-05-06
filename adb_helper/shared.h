#ifndef SHARED_H
#define SHARED_H

#include "Windows.h"
#include "package.h"

#include<string>
using std::string;

#include<vector>
using std::vector;

#include<fstream>
using std::ios_base;

// filenames
const string log_filename { "/tmp/adb.helper.log" };

// symbolic names
constexpr char yes { 'y' };
constexpr char no { 'n' };
constexpr size_t valid_choice_length { 1 };
constexpr size_t progression_output_length { 7 };
const string INVALID_CHOICE = "0";
const string empty_space { " " };

// user-defined type Type
// defines the type of a package
enum class Type {
	nosystem, system, all
};

// user-defined type TerminalWindowSize
struct TerminalWindowSize {
	SHORT columns;
	SHORT rows;
};

// gets terminal windows size
TerminalWindowSize get_terminal_window_size();

// runs command
void run_command(const string& command);

// creates file if it doesn't exist
void create_file(const string& filename);

// removes file
void remove_file(const string& filename);

// removes directory from computer
void remove_directory(const string& directory);

// removes directory from phone
void remove_directory_phone(const string& directory);

// gets path
string get_path(const string& filepath);

// gets package path
string get_package_path(const string& name, const vector<Package>& packages);

// gets file name
string get_filename(const string& filepath);

// creates directory
void create_directory(const string& path);

// opens directory
void open_directory(const string& path);

// lists files
void list_files(const string& path_directory, const string& path_file, const string& pattern= ".");

// gets data from a file
vector<string> get_lines(const string& filename);

// displays progression
void display_progression(const size_t& position, const size_t & total);

// writes to a file
void write_to_file(const string& filename, const string& stream, const ios_base::openmode& mode = ios_base::app);

// checks if character is parenthesis
bool isparenthesis(int c);

// processes filename
string process_name_if(const string& name);

// clears line
void clear_line();

// lists packages
void list_packages(const string& pattern = string{});

// gets packages information
vector<Package> get_packages_information(const Type& type = Type::all);

// trims line
string trim_line(const string& line, const size_t& number_of_ignored_characters = 0);

#endif
