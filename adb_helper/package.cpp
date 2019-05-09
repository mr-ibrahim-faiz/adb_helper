#include "package.h"

// constructor taking a name and path as arguments
Package::Package(const string name, const string path) noexcept
	: name{name}
	, path{path}{}
	
// retrieves the name
string Package::Name() const noexcept {
	return name;
}

// retrieves the path
string Package::Path() const noexcept {
	return path;
}

// tests if system
bool Package::System() const noexcept {
	if(path.find("system") != string::npos) return true;
	return false;
}
