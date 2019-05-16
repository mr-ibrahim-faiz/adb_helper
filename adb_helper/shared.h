#ifndef SHARED_H
#define SHARED_H

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
const string INVALID_CHOICE = "0";

// runs command
void run_command(const string&);

// creates file if it doesn't exist
void create_file(const string&);

// removes file
void remove_file(const string&);

// removes directory from computer
void remove_directory(const string&);

// removes directory from phone
void remove_directory_phone(const string&);

// gets path
string get_path(const string&);

// gets file name
string get_filename(const string&);

// creates directory
void create_directory(const string&);

// opens directory
void open_directory(const string&);

// lists files
void list_files(const string&, const string&, const string& = ".");

// gets data from a file
vector<string> get_lines(const string&);

// displays progression
void display_progression(const size_t&, const size_t &);

// writes to a file
void write_to_file(const string&, const string&, const ios_base::openmode& = ios_base::app);

// processes filename
string process_name_if(const string&);

// clears line
void clear_line(const int&);

#endif
