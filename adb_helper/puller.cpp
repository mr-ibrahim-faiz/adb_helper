#include "puller.h"
#include "shared.h"

#include<fstream>
using std::ifstream;

#include<iostream>
using std::cout;
using std::cerr;

#include<iomanip>
using std::setw;

#include<stdexcept>
using std::runtime_error;

// file names
const string packages_filename { "/tmp/adb.helper.packages" };
const string temp_filename { "/tmp/adb.helper.tmp" };

// symbolic names
constexpr char carriage_return { '\r' };
constexpr char delimiter { '=' };
const string package_extension { ".apk" };
const string package_prefix { "package:" };

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

// lists packages
void list_packages(const string& pattern)
// lists packages and associated files into a file
// a pattern can be specified to refine the results
{
	const string command = "adb shell \'pm list packages -f" + ((pattern.empty())? "" : " | grep -i " + pattern) + "\' > " + packages_filename + " 2>&1";
	run_command(command);
}	

// removes prefix
string remove_prefix(const string& sequence, const size_t& length)
// revmoves a number of characters from a sequence of string
{
	return sequence.substr(length);
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
				Package package { name, path };
				if(Type(package.System()) == type || type == Type::all) packages.push_back(package);
			}
		}
		file.close();
	}
	else cerr << "Error: Unable to open \'" + packages_filename + "\'.\n";

	return packages;
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
		const string error_message = "Error: " + string(e.what()) + "\n"; 
		write_to_file(log_filename, error_message);
		clear_line(150);
		cerr << "Error: unable to pull " << package.Name() << ".\n"; 
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
	const string error_message = "Error: " + string(e.what()) + "\n";
	write_to_file(log_filename, error_message);
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
			clear_line(150);
			display_progression(i, size);
			cout << " Pulling " << package.Name() <<  " " << std::flush;
			pull_package(package, mode);
		}

		clear_line(150);
		display_progression(size-1, size);
		cout << " Done.\n";
		open_output_directory();
	}
	else cout << "There is no app to pull.\n";
}
