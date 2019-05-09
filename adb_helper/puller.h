#ifndef PULLER_H
#define PULLER_H

#include "package.h"

#include<vector>
using std::vector;

// file names
const string packages_filename { "packages.txt" };
const string temp_filename { "/tmp/puller_temp.txt" };

// directory names
const string pull_root_directory { "~/Desktop/" };

// symbolic names
constexpr char newline { '\n' };
constexpr char exit_character { 'x' };
const string INVALID_CHOICE = "0";
const string package_prefix { "package:" };

// user-defined type Type
enum class Type {
	nosystem, system, all
};

// symbolic names
constexpr char delimiter { '=' };

// lists phone model
void list_model();

// gets phone model
const string get_model();

// lists packages
void list_packages();

// removes prefix
string remove_prefix(const string&, const size_t&);

// gets packages information
vector<Package> get_packages_information(const Type&);

// pulls a single package
void pull_package(const Package&);

// displays progression
void display_progression(const size_t&, const size_t &);

// opens output directory
void open_output_directory();

// packages puller
void puller(const Type&);

#endif
