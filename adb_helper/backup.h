#ifndef BACKUP_H
#define BACKUP_H

#include<string>
using std::string;

// directory names
const string backup_root_directory { "~/Documents/Android" };

// backups directory
void backup_directory(const string&);

#endif
