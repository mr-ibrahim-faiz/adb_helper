#ifndef PULLER_H
#define PULLER_H

#include "shared.h"

#include<vector>
using std::vector;

// directory names
const string pull_root_directory { "~/Documents/Android/" };

// user-defined type Mode
// defines how a package is stored in the computer when pulled
enum class Mode {
	raw, name
};

// lists phone model
void list_model();

// gets phone model
const string get_model();

// removes prefix
string remove_prefix(const string&, const size_t&);

// pulls a file or directory from device
void pull_file(const string&, const string&);

// pulls a single package
void pull_package(const Package&, const Mode& = Mode::raw);

// opens output directory
void open_output_directory();

// packages puller
void packages_puller(const Type&, const string& = string{});

#endif
