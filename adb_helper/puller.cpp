#include "puller.h"
#include "useful.h"

#include<stdexcept>
using std::runtime_error;

#include<fstream>
using std::ifstream;

#include<iostream>
using std::cout;

#include<string>
using std::to_string;

// lists phone model
void list_model()
// lists phone model into a file
{
	const string command = "adb shell getprop ro.product.model > " + temp_filename;
	run_command(command);
}

// gets phone model
const string get_model(){
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
void list_packages()
// lists packages and associated files into a file
{
	const string command = "adb shell pm list packages -f > " + packages_filename;
	run_command(command);
}	

// removes prefix
string remove_prefix(const string& sequence, const size_t& length){
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
		for(string name, path; getline(file, path, delimiter);){
			path = remove_prefix(path, length);
			getline(file, name);
			name.pop_back(); // removes sneaky carriage return
			if(!name.empty() && !path.empty()) {
				Package package { name, path };
				if(Type(package.System()) == type || type == Type::all) packages.push_back(package);
			}
		}
		file.close();
	}
	else throw runtime_error("Unable to open \'" + packages_filename + "\'. Aborting...");

	return packages;
}

// pulls a single package
void pull_package(const Package& package)
// the package must be valid, i.e. path and names are valid
{
	static const string model = get_model();
	// const string& name = package.Name();
	const string& path = package.Path();
	const string output_directory = pull_root_directory + model + get_path(path);
	
	create_directory(output_directory);

	const string command = "adb pull " + path + " " + output_directory;
	run_command(command);
}

// displays progression
void display_progression(const size_t& position, const size_t& total){
	size_t percentage = (position+1)*(100.0/total);
	string spercentage = to_string(percentage);
	while(spercentage.size() != 3) spercentage.insert(0, " ");
	cout << "\r[" << spercentage << "%]" << std::flush;
}

// opens output directory
void open_output_directory(){
	static const string model = get_model();
	static const string output_directory = pull_root_directory + model;
	open_directory(output_directory);
}

// packages puller
void puller(const Type& type)
// pulls packages from phone
{
	list_packages();
	vector<Package> packages = get_packages_information(type);

	if(packages.empty()) throw runtime_error("(packages puller) the list is empty.");
	else cout << "Pulling packages from phone...\n\n";

	const size_t size = packages.size();
	for(size_t i = 0; i < size; ++i){
		const Package& package = packages[i];
		display_progression(i, size);
		cout << " Pulling " << package.Name() << '\n';
		pull_package(package);
	}

	cout << "\nDone.\n";
	open_output_directory();
}
