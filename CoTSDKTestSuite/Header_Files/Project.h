#pragma once
#include "pch.h"
#include "User.h"

class Project {
	/*
	=== Attributes ===
	vector<User> members: list of User objects listed as members of initialized project
	string name: the name of the project listed
	vector<string> files: list of file directories added to project

	=== Functions ===
	addFile(string): UNDER CONSTRUCTION
	addMember(User): adds User object to members list
	removeMember(int): removes member by index from members list
	changeName(string): changes name of project
	getName(): returns name of project
	getMembers(): returns list of members of project
	getFilesList(): returns list of file directories
	*/

private:
	vector<User> members;
	string name;
	vector<string> files;
	//Add circle to project
	//Add files

public:
	Project(string projectName);

	Project(User creator, string projectName);

	void addFile(string fileDir);

	void addMember(User member);

	void removeMember(int memberIndex);

	void changeName(string newName);

	string getName();

	vector<User> getMembers();

	vector<string> getFilesList();
};