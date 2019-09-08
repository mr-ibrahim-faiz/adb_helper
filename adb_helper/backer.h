#ifndef BACKER_H
#define BACKER_H

#include<string>
using std::string;

// directory names
const string backup_root_directory { "D:\\Android\\" };

// backs up directory
int backup_directory(const string&);

// backs up and removes directory
void backup_and_remove_directory(const string&);

#endif
