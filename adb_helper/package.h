#ifndef PACKAGE_H
#define PACKAGE_H

#include<string>
using std::string;

// user-defined type Package
class Package {
public:
	// default constructor
	Package() noexcept = delete;

	// constructor taking a name and path as arguments
	explicit Package(const string, const string) noexcept;

	// retrieves the name
	string Name() const noexcept;

	// retrieves the path
	string Path() const noexcept;

	// tests if system
	bool System() const noexcept;

private:
	string name;
	string path; 
};

#endif
