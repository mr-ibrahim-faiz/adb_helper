#include "puller.h"
#include "shared.h"

#include<fstream>
using std::ifstream;

#include<iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::flush;

#include<iomanip>
using std::setw;

#include<stdexcept>
using std::runtime_error;

// file names
const string temp_filename { "/tmp/adb.helper.tmp" };

// symbolic names
constexpr char carriage_return { '\r' };
const string package_extension { ".apk" };

// lists phone model
void list_model()
// saves the phone model into a file
{
	const string command = "adb shell getprop ro.product.model > " + temp_filename + " 2>&1";
	run_command(command);
}

// gets phone model
const string get_model()
// retrieves the phone model from a temporray file
// the file contains only the phone model
// the temporary file is then removed
{
	list_model();
	string model;
	ifstream file(temp_filename);
	if(file.is_open()){
		file >> model;
		file.close();
	}
	remove_file(temp_filename);
	return model;
}

// removes prefix
string remove_prefix(const string& sequence, const size_t& length)
// revmoves a number of characters from a sequence of string
{
	return sequence.substr(length);
}

// pulls a file or directory from device
void pull_file(const string& path_remote, const string& path_local){
	const string command = "adb pull " + path_remote + " " + path_local + " >> " + log_filename + " 2>&1";
	run_command(command);
}

// pulls a single package
void pull_package(const Package& package, const Mode& mode)
// the package must be valid, i.e. path and names are valid
{
	static const string model = get_model();
	const string& name = package.Name();
	const string& path = package.Path();
	const string output_directory = pull_root_directory + model + ((mode==Mode::raw)? get_path(path) : "/" + name + package_extension);

	create_directory((mode==Mode::raw)? output_directory : get_path(output_directory));
	try {
		pull_file(path, output_directory);
	}
	catch(runtime_error& e){
		const string log_message = "Error: " + string(e.what()) + "\n";
		write_to_file(log_filename, log_message);
		clear_line();
		const string error_message = "Error: unable to pull " + package.Name() + ".";
		cerr << trim_line(error_message) << endl;
	}
}

// opens output directory
void open_output_directory()
try
{
	static const string model = get_model();
	static const string output_directory = pull_root_directory + model;
	open_directory(output_directory);
}
catch(runtime_error& e){
	const string log_message = "Error: " + string(e.what()) + "\n";
	write_to_file(log_filename, log_message);
}

// packages puller
void packages_puller(const Type& type, const string& pattern)
// pulls packages from phone
// a pattern can be used to pull a specific package or a set of packages
{
	list_packages(pattern);
	vector<Package> packages = get_packages_information(type);

	if(!packages.empty()){
		cout << "Pulling package(s) from phone...\n";

		const Mode mode = (pattern.empty())? Mode::raw : Mode::name;

		const size_t size = packages.size();
		for(size_t i = 0; i < size; ++i){
			const Package& package = packages[i];
			clear_line();
			display_progression(i, size);
			cout << empty_space;

			const string message_pulling = "Pulling " + package.Name() + " ";
			cout << trim_line(message_pulling) << flush;
			pull_package(package, mode);
		}

		clear_line();
		display_progression(size-1, size);
		cout << " Done.\n";
		open_output_directory();
	}
	else cout << "There is no app to pull.\n";
}
