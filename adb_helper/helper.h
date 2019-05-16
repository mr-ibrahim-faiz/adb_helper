#ifndef HELPER_H
#define HELPER_H

#include<string>
using std::string;

// symbolic names
constexpr char newline { '\n' };
constexpr char exit_character { 'x' };

// starts adb server
void start_server();

// stops adb server
void kill_server();

// displays main menu
void display_main_menu();

// displays pull menu
void display_pull_menu();

// app puller main function
void pull_app();

// puller main function
void pull();

// backs up files
void backup_files();

// file updater main function
void update_file(const string&);

#endif
