#include "../Header_Files/pch.h"
#include "../Header_Files/User.h"
#include "../Header_Files/Project.h"

Project::Project(string projectName) {
	name = projectName;
}

Project::Project(User creator, string projectName) {
	members.push_back(creator);
	name = projectName;
}

void Project::addFile(string fileDir) {
	files.push_back(fileDir);
}

void Project::addMember(User member) {
	//TODO Add email validation
	members.push_back(member);
}

void Project::removeMember(int memberIndex) {
	members.erase(members.begin() + memberIndex);
}

void Project::changeName(string newName) {
	name = newName;
}

string Project::getName() {
	return name;
}

vector<User> Project::getMembers() {
	return members;
}

vector<string> Project::getFilesList() {
	return files;
}