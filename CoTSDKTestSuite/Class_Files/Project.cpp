#include "../Header_Files/pch.h"
#include "../Header_Files/User.h"
#include "../Header_Files/Project.h"

Project::Project(string projectName) {
	name = projectName;

	//TODO take project name and conver to WCHAR[]
	WCHAR* circleName = new WCHAR[projectName.length()];
	swprintf(circleName, 128, L"%s", projectName);

	WCHAR pass[] = L"2237b";
	InitApp(201, pass);

	CoTCircle projectCircle;

	CoTSdk::createCircle(&projectCircle,
		circleName,
		L"Created by CoTSDKSuite",
		NULL, 0
	);

}

Project::Project(User creator, string projectName) {
	members.push_back(creator);
	name = projectName;

	//TODO take project name and conver to WCHAR[]
	WCHAR* circleName = new WCHAR[projectName.length()];
	swprintf(circleName, 128, L"%s", projectName);

	WCHAR pass[] = L"aaaa";
	InitApp(201, pass);

	CoTCircle projectCircle;

	createCircle(&projectCircle,
		circleName,
		L"Created by CoTSDKSuite",
		NULL, 0
	);
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