#ifndef USEFUL_H
#define USEFUL_H

#include<string>
using std::string;

// runs command
void run_command(const string&);

// removes file
void remove_file(const string&);

// removes directory
void remove_directory(const string&);

// gets path
string get_path(const string&);

// creates directory
void create_directory(const string&);

// opens directory
void open_directory(const string&);

#endif
