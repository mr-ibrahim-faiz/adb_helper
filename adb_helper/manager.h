#ifndef MANAGER_H
#define MANAGER_H

#include<string>
using std::string;

// installs a single package
void install_package(const string&);

// uninstalls a single package
void uninstall_package(const string&, const bool& = false);

// deletes data associated to a package
void clear_package(const string&, const bool& = false);

// disables package
void disable_package(const string&, const bool& = false);

// packages installer
void packages_installer(const string&);

// packages uninstaller
void packages_uninstaller();

#endif
