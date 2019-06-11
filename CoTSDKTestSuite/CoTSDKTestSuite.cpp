// CoTSDKTestSuite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <windows.h>
#include <chrono>
#include <direct.h>
#include <filesystem>

//#include <libs/boost/filesystem/filesystem.hpp>
using namespace std;

class User;
const string COMPANY_FILE_ADDRESS = "AAAcompany_info.txt";
const string MESSAGES_DIR = "AAAMessages";
int convertInt(string str);

const int STARTING_YEAR = 1900;
const string MONTHS[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
const string DAYS[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// General user class for imaginary software
class User {
	/*User class that represents an employee within this company.
	  === Attributes ===
	  int userID: constant identifier for User
	  string username: username of employee within records
	  string password: password for user to access software
	  string cotAccountName: username for user in CoT. Will be an email address
	  int numUsernameChanges: metric that logs how many times a user has changed their username. Useful for internal data.
	  vector<string> pastPasswords: a collection of passwords used previously by the user to avoid repeat use for security purposes.

	  === Functions ===
	  string getID(): returns userID of User
	  string getUsername(): returns the username of User
	  void changeUsername(string): changes username of User and increments numUsernameChanges
	  string getPassword(): returns password of User
	  bool changePassword(string): changes password of user if 1. it is not the same as the current password, and 2. it is not present within pastPasswords
	  string getCotAccountName(): returns the cotAccountName of User
	  bool updateCotAccountName(): prompts ser with login from sdk to verify new cotAccountName. Returns true or false depending on the outcome of auth

	  === Special ===
	  2 constructors. 1st for newly created User without established CoT account, and 2nd to be used for loading pre-defined User objects
	  with already authenticated accounts*/

private:
	int userID;
	string username;
	string password;
	string cotAccountName;
	int numUsernameChanges;
	vector<string> pastPasswords;

public:
	User(int id, string name, string pass) {
		/* Precondition: id must be (size of employees list in company) + 1*/
		userID = id;

		username = name;
		password = pass;
		numUsernameChanges = 0;
		cotAccountName = "null";
	}

	User(int id, string name, string pass, string cotLogin) {
		/* Precondition: id must be (size of employees list in company) + 1*/
		userID = id;

		username = name;
		password = pass;
		numUsernameChanges = 0;
		cotAccountName = cotLogin;
	}

	int getID() {
		return userID;
	}

	string getUsername() {
		return username;
	}

	void changeUsername(string name) {
		numUsernameChanges++;
		username = name;
	}

	string getPassword() {
		return password;
	}

	bool changePassword(string pass) {
		if (pass == password or find(pastPasswords.begin(), pastPasswords.end(), pass) != pastPasswords.end()) {
			cout << "That password has been used before!" << endl;
			return false;
		}
		else {
			pastPasswords.push_back(password);
			password = pass;
			cout << "Password changed." << endl;
			return true;
		}
	}

	string getCotAccountName() {
		return cotAccountName;
	}

	User updateCotAccountName() {
		//TODO Use Person object function enable() (1.4.2.2) and catch specific error code for non-existent user
		cout << "Enter CoT Account Username" << endl;
		getline(cin, cotAccountName);
		return *this;
	}

	void sendMessage(User receiver) {
		cout << "What Would You Like To Say?: " << endl;
		string messageContent;
		getline(cin, messageContent);

		//Gets current date
		time_t now = time(NULL);
		struct tm tm;
		localtime_s(&tm, &now);
		string dt = DAYS[tm.tm_wday] + ", " + MONTHS[tm.tm_mon] + " " + to_string(tm.tm_mday) + " " + to_string(tm.tm_year + STARTING_YEAR) + " | ";

		//To deal with min values under 10
		int currMin = tm.tm_min;
		string formattedMin;
		if (currMin < 10)
			formattedMin = '0' + to_string(currMin);
		else
			formattedMin = to_string(currMin);

		//To deal with special hour cases
		if (tm.tm_hour > 12)
			dt = dt + to_string(tm.tm_hour % 12) + ":" + formattedMin + " " + "PM";
		else if (tm.tm_hour == 12)
			dt = dt + to_string(tm.tm_hour) + ":" + formattedMin + " " + "PM";
		else if (tm.tm_hour == 0)
			dt = dt + "12" + ":" + formattedMin + "" + "AM";
		else
			dt = dt + to_string(tm.tm_hour) + ":" + formattedMin + " " + "AM";

		//Creates conversation text file
		string receiverDirectory = "Messages/" + to_string(receiver.getID());
		ofstream receiverOutfile(receiverDirectory + "/" + to_string(userID) + "conversation.txt", ios::app);
		receiverOutfile << "(" + to_string(userID) + ") " + username + ": " + messageContent + " (" + dt + ")" << endl;
		receiverOutfile.close();


		string senderDirectory = "Messages/" + to_string(userID);
		ofstream senderOutfile(senderDirectory + "/" + to_string(receiver.getID()) + "conversation.txt", ios::app);
		senderOutfile << "(" + to_string(userID) + ") " + username + ": " + messageContent + " (" + dt + ")" << endl;
		senderOutfile.close();
		//throw EXCEPTION_ILLEGAL_INSTRUCTION;
	}

};

//TODO add documentation
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
	Project(User creator, string projectName) {
		members.push_back(creator);
		name = projectName;
	}

	void addFile(string fileDir) {
		files.push_back(fileDir);
	}

	void addMember(User member) {
		//TODO Add email validation
		members.push_back(member);
	}

	void removeMember(int memberIndex) {
		members.erase(members.begin() + memberIndex);
	}

	void changeName(string newName) {
		name = newName;
	}

	string getName() {
		return name;
	}

	vector<User> getMembers() {
		return members;
	}

	vector<string> getFilesList() {
		return files;
	}
};

class Company {
	/*Company class that contains employees
	  === Attributes ===
	  string name: name of company
	  vector<User> employee: User vector of current employees within company
	  int numTotalEmployees: number of total that have or are working at the company. Used to create unique identifiers for employees

	  === Functions ===
	  string getEmployees(): returns the vector of employees
	  string getName(): returns the company name
	  string getPassword(): returns password of User
	  void addEmployee(): prompts user for employee information and adds new User to employees
	  void addEmployee(User): overwritten function that serves as a tool for developers using this software. Allows a current user object to be
									   passed in and added directly to employees. Not used in this build.
	  bool removeEmployee(): prompts user for employee information and removes matching User object from employees
	  void displayEmployees(): iterates through employees and displays formatted username and password information for all employees
	  void updateEmployees(vector<User>): assigns employees to new vector. Used when updating information from employees to prevent memory issues
	  void changeCompanyName(string): updates name attribute

	  === Special ===
	  2 Constructors. One for new Company, and one for loading saved company.*/

private:
	string name;
	vector<User> employees;
	int numTotalEmployees;
	vector<Project> projects;

public:
	Company(string companyName) {
		name = companyName;
		numTotalEmployees = 0;
	}

	Company(string companyName, int totalEmployees) {
		name = companyName;
		numTotalEmployees = totalEmployees;
	}

	vector<User> getEmployees() {
		return employees;
	}

	string getName() {
		return name;
	}

	int getnumTotalEmployees() {
		return numTotalEmployees;
	}

	//Used to prevent artifical totalNumEmployee growth when loading from file
	void decrementNumTotalEmployees() {
		numTotalEmployees--;
	}

	void addEmployee() {
		string empName;
		string empPass;

		cout << "Enter Employee Name: " << endl;
		getline(cin, empName);
		cout << "Enter Employee Password: " << endl;
		getline(cin, empPass);

		employees.push_back(User(numTotalEmployees, empName, empPass));

		string directory = "Messages/" + to_string(numTotalEmployees);
		_mkdir(directory.c_str());

		numTotalEmployees++;
	}

	void addEmployee(User employee) {
		employees.push_back(employee);
		numTotalEmployees++;
	}

	bool removeEmployee() {
		cout << "Enter Employee Name: " << endl;
		string name;
		getline(cin, name);

		for (int i = 0; i < (int)employees.size(); i++) {
			if (employees[i].getUsername() == name) {
				employees.erase(employees.begin() + i);
				return true;
			}
		}

		cout << "Employee not listed" << endl;
		return false;
	}

	void displayEmployees() {
		cout << "Employees: " << endl;
		for (int i = 0; i < employees.size(); i++) {
			User user = employees[i];
			cout << "	Username: " + user.getUsername() + ", Password: " + user.getPassword() + ", CoT Account: " + user.getCotAccountName() << endl;
		}
	}

	void updateEmployees(vector<User> newVector) {
		employees = newVector;
	}

	void updateProjects(vector<Project> newVector) {
		projects = newVector;
	}

	void changeCompanyName(string newName) {
		name = newName;
	}

	vector<Project> getProjects() {
		return projects;
	}

	void addProject(Project project) {
		projects.push_back(project);
	}

	void removeProject(int i) {
		projects.erase(projects.begin() + i);
	}
};


// Prompt to get information when updating employee username
void getNewUsername(Company &company, int userIndex) {
	vector<User> employees = company.getEmployees();

	cout << "Enter New Employee Name: " << endl;
	string newName;
	getline(cin, newName);

	/*//Update all message histroy with new name
	vector<string> messages;
	ifstream reader;
	ofstream writer;

	for (int i = 0; i < company.getnumTotalEmployees(); i++) {
		//Fix all files in changed user folder
		//TODO only not working on files in sender folder
		//TODO time delay doesnt affect it

		string dir = "Messages/" + to_string(employees[userIndex].getID()) + "/" + to_string(i) + "conversation.txt";
		reader.open(dir);

		string message;
		string messageContent;
		while (getline(reader, message)) {
			stringstream parsed(message);
			
			getline(parsed, messageContent); 
			
			if (messageContent.substr(0, messageContent.find(": ")) == employees[userIndex].getUsername()) {
				messages.push_back(newName + messageContent.substr(messageContent.find(": "), messageContent.length() - 1));
			}
			else {
				messages.push_back(messageContent);
			}
		}
		reader.close();

		//Writes new messages back to file
		writer.open(dir);
		for (int i = 0; i < messages.size(); i++) {
			writer << messages[i] << endl;
		}

		writer.close();

		//Fixes all other files with user conversation
		dir = "Messages/" + to_string(i) + "/" + to_string(employees[userIndex].getID()) + "conversation.txt";

		//Writes new messages back to file
		writer.open(dir);
		for (int i = 0; i < messages.size(); i++) {
			writer << messages[i] << endl;
		}

		messages.clear();
	} */

	//Updates Employee name
	employees[userIndex].changeUsername(newName);
	company.updateEmployees(employees);

}

// Prompt to get information when updating employee password
void getNewPassword(Company &company, int userIndex) {
	vector<User> employees = company.getEmployees();

	cout << "Enter New Employee Password: " << endl;
	string newPass;
	getline(cin, newPass);

	employees[userIndex].changePassword(newPass);
	company.updateEmployees(employees);
}

//Prompt to authenticate cot account and add to employee
void assignCoTAccount(Company &company, int userIndex) {
	vector<User> employees = company.getEmployees();

	employees[userIndex] = employees[userIndex].updateCotAccountName();
	company.updateEmployees(employees);
}

//Prompt to get new company name when changing company name
void getNewCompanyName(Company &company) {
	cout << "Enter New Company Name: " << endl;
	string newName;
	getline(cin, newName);

	company.changeCompanyName(newName);
}

//Converts inputted string into an int and handles exception if thrown. Used in parsing menu choice input.
int convertInt(string str) {
	try {
		return stoi(str);
	}
	catch (exception e) {
		return -1;
	}
}

//Helper function to see if directory exists within given space
bool dirExists(string dir) {
	struct stat buffer;
	return (stat(dir.c_str(), &buffer) == 0);
}

void save(Company company) {
	// Save data
	// First line is the company name
	// Employee data is ordered as (ID), (Username), (Password), (CoT Username)
	// CoT Username = "null" if it DNE
	ofstream writer;
	writer.open(COMPANY_FILE_ADDRESS);
	vector<User> saveEmployees = company.getEmployees();
	
	writer << company.getName() + ", " + to_string(company.getnumTotalEmployees()) << endl;
	for (int i = 0; i < (int)saveEmployees.size(); i++) {
		writer << to_string(saveEmployees[i].getID()) + "," + saveEmployees[i].getUsername() +
			"," + saveEmployees[i].getPassword() + "," + saveEmployees[i].getCotAccountName() << endl;
	}

	writer.close();
}

//Loads company on startup
//TODO Add folder if not present when loading file
Company load() {
	//load data
	ifstream reader;
	reader.open(COMPANY_FILE_ADDRESS);

	string header;
	getline(reader, header);

	vector<string> headerVector;
	stringstream headerStream(header);
	string headerToken;

	while (getline(headerStream, headerToken, ','))
		headerVector.push_back(headerToken);

	//Create company and populate employees
	Company company(headerVector[0], convertInt(headerVector[1]));
	string employeeInfo;

	while (getline(reader, employeeInfo)) {
		vector<string> infoVector;
		stringstream infoStream(employeeInfo);
		string infoToken;

		while (getline(infoStream, infoToken, ','))
			infoVector.push_back(infoToken);

		company.addEmployee(User(convertInt(infoVector[0]), infoVector[1], infoVector[2], infoVector[3]));
		infoVector.clear();

		company.decrementNumTotalEmployees();
	}

	return company;

}

//Prompts user for save/load choice and proceeds accordingly
Company getBootPrompt() {
	cout << "Boot Menu: (Enter number of option)" << endl;
	cout << "1. Create New Company (will overwrite previous company)" << endl;
	cout << "2. Load Saved Company" << endl;

	string bootChoice;
	getline(cin, bootChoice);

	switch (convertInt(bootChoice)) {
	case 1: {
		//TODO add existing check or not
		string directory = "Messages";

		//TODO will only work on windows with admin
		system("erase Messages /s /q > nul"); 
		system("rmdir Messages /s /q > nul");
		_mkdir(directory.c_str());

		string companyName;
		cout << "Enter Company Name: " << endl;
		getline(cin, companyName);
		return Company(companyName);
		break;
	}

	case 2:
		return load();
		break;

	default:
		cout << "That is not a valid option" << endl;
		return getBootPrompt();
		break;
	};
}

void sendMessagePrompt(Company &company, int userIndex) {
	cout << "Enter Name of Receiving Employee: " << endl;
	string receiveEmployee;
	getline(cin, receiveEmployee);

	vector<User> employees = company.getEmployees();
	for (int j = 0; j < (int) employees.size(); j++) {
		if (employees[j].getUsername() == receiveEmployee) {
			//Send Message
			employees[userIndex].sendMessage(employees[j]);
			return;
		}
	}
	cout << "Employee Not Listed" << endl;
}

//Displays messages of user
void displayMessages(Company &company, int userIndex) {
	ifstream reader;
	// TODO quick fix for now. Inefficient with large total num of employees
	//TODO need way to identify with whom different message threads are engaging
	for (int i = 0; i < company.getnumTotalEmployees(); i++) {
		reader.open("Messages/" + to_string(company.getEmployees()[userIndex].getID()) + "/" + to_string(i) +"conversation.txt");

		string message;
		while (getline(reader, message)) {
			cout << message << endl;
		}

		cout << "----------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		reader.close();
	}
}

bool containsName(vector<User> projectMembers, string name) {
	for (int i = 0; i < projectMembers.size(); i++) {
		if (projectMembers[i].getUsername() == name)
			return true;
	}

	return false;
}

void addMemberPrompt(Company &company, int projectIndex) {
	cout << "Enter Name of Employee To Be Added: " << endl;
	string addName;
	getline(cin, addName);

	vector<User> employees = company.getEmployees();
	vector<Project> projects = company.getProjects();
	vector<User> projectMembers = projects[projectIndex].getMembers();
	for (int i = 0; i < employees.size(); i++) {
		if (employees[i].getUsername() == addName && !containsName(projectMembers, addName)) {
			projects[projectIndex].addMember(employees[i]);
			company.updateProjects(projects);
			cout << "Member Added" << endl;
			return;
		}
	}
	cout << "Employee Could Not Be Added" << endl;
}

void removeMemberPrompt(Company &company, int projectIndex) {
	cout << "Enter Name of Employee To Be Removed: " << endl;
	string addName;
	getline(cin, addName);

	vector<Project> projects = company.getProjects();
	for (int i = 0; i < projects[projectIndex].getMembers().size(); i++) {
		if (projects[projectIndex].getMembers()[i].getUsername() == addName) {
			projects[projectIndex].removeMember(i);
			company.updateProjects(projects);
			cout << "Member Removed" << endl;
			return;
		}
	}
	cout << "Employee Not Found" << endl;
}

void createProject(Company &company, int userIndex) {
	cout << "Enter a Name For This Project" << endl;
	string name;
	getline(cin, name);

	company.addProject(Project(company.getEmployees()[userIndex], name));

	bool addingMembers = true;
	while (addingMembers) {
		cout << "Want To Add Another Member? (Enter Number of Option)" << endl;
		cout << "	1. Yes" << endl;
		cout << "	2. No" << endl;

		string choice;
		getline(cin, choice);

		switch (convertInt(choice)) {
		case 1:
			addMemberPrompt(company, company.getProjects().size() - 1);
			break;

		case 2:
			addingMembers = false;
			break;

		default:
			cout << "That is not a valid option." << endl;
			break;
		}
	}
}

//function to login to employee options menu
int login(Company &company) {
	cout << "Login Menu" << endl;
	cout << "Please enter Username: " << endl;
	string name;
	getline(cin, name);

	vector<User> employees = company.getEmployees();
	for (int i = 0; i < employees.size(); i++) {
		if (employees[i].getUsername() == name) {
			cout << "Enter Password: " << endl;
			string pass;
			getline(cin, pass);

			if (employees[i].getPassword() == pass) {
				cout << "Login Successful" << endl;
				return i;
			}

			cout << "Incorrect Password" << endl;
			return -1; //Represents failed login User
		}
	}
	cout << "Employee Not Listed" << endl;
	return -1; //Represents failed login User
}

void getNewProjectName(Company &company, int projectIndex) {
	cout << "Enter New Name For Project" << endl;
	string newName;
	getline(cin, newName);

	company.getProjects()[projectIndex].changeName(newName);
}

//Helper Method to list members in a project
string listMembers(Company &company, vector<User> projectMembers) {
	string membersList;
	for (int k = 0; k < projectMembers.size(); k++) {
		if (k == projectMembers.size() - 1)
			membersList += projectMembers[k].getUsername();

		else
			membersList += projectMembers[k].getUsername() + ", ";
	}

	return membersList;
}

void displayProjects(Company &company, int userIndex) {
	vector<Project> projects = company.getProjects();

	//For scenario where no projects at all
	if (projects.size() == 0) {
		cout << "No Projects Listed" << endl;
		return;
	}

	vector<string> projectList;
	cout << "Projects" << endl;

	for (int i = 0; i < projects.size(); i++) {
		vector<User> projectMembers = projects[i].getMembers();

		for (int j = 0; j < projectMembers.size(); j++) {

			if (projectMembers[j].getUsername() == company.getEmployees()[userIndex].getUsername()) {

				string membersList = listMembers(company, projectMembers);

				projectList.push_back("Name: " + projects[i].getName() + " | Members: " + membersList);
			}
		}
	}
	//runs this if employee checked isnt in project members
	if (projectList.size() == 0) {
		cout << "No Projects Listed" << endl;
	}
	else {
		for (auto &projectListing : projectList) {
			cout << projectListing << endl;
		}
	}
}

void checkDirectories() {
	if (!dirExists(COMPANY_FILE_ADDRESS)) {
		ofstream receiverOutfile(COMPANY_FILE_ADDRESS, ios::app);
		receiverOutfile.close();
	}

	if (!dirExists(MESSAGES_DIR)) {
		_mkdir(MESSAGES_DIR.c_str());
	}
}

//TODO save projects in file
void showProjectMenu(Company &company, int userIndex) {
	//TODO Need to limit search to projects with userIndex as member
	cout << "Enter Target Project Name" << endl;
	string projectName;
	getline(cin, projectName);

	int projectIndex= -1;
	vector<Project> projects = company.getProjects();
	for (int i = 0; i < projects.size(); i++) {
		if(projects[i].getName() == projectName && containsName(projects[i].getMembers(), company.getEmployees()[userIndex].getUsername())){
			projectIndex = i;
			break;
		}
	}

	//TODO under construction
	bool projectRunning = true;
	while (projectRunning && projectIndex != -1) {
		cout << projects[projectIndex].getName() + " Project Menu" << endl;
		cout << "	1: Change Project Name" << endl;
		cout << "	2: Add Member" << endl;
		cout << "	3: Remove Member" << endl;
		cout << "	4: Display Members" << endl;
		cout << "	5: Add File/Folder To Project (UNDER CONSTRUCTION)" << endl;
		cout << "	6: Delete Project" << endl;
		cout << "	7: Previous Menu" << endl;

		string choice;
		getline(cin, choice);
		switch (convertInt(choice)) {
		case 1:
			getNewProjectName(company, projectIndex);
			break;

		case 2:
			addMemberPrompt(company, projectIndex);
			break;

		case 3:
			removeMemberPrompt(company, projectIndex);

		case 4:
			cout << "Members: " + listMembers(company, company.getProjects()[projectIndex].getMembers()) << endl;
			break;

		case 5:
			break;

		case 6:
			company.removeProject(projectIndex);
			projectRunning = false;
			break;

		case 7:
			projectRunning = false;
			break;

		default:
			cout << "That is not a valid option" << endl;
			break;
		}
	}
	
	if (projectIndex == -1) {
		cout << "Project Not Found" << endl;
	}
}

void showEmployeeMenu(Company &company) {
	//Login is outside loop so user doesnt have to login every time loop iterates
	int userIndex = login(company);

	bool employeeRunning = true;
	while (employeeRunning && userIndex != -1) {
		cout << company.getName() + " Employee Menu: (Enter number of option)" << endl;
		cout << "	1. Change Employee Name" << endl;
		cout << "	2. Change Employee Password" << endl;
		cout << "	3. Assign CoT Account" << endl;
		cout << "	4. Send Message" << endl;
		cout << "	5. Display Messages" << endl;
		cout << "	6. Create Project" << endl;
		cout << "	7. Display Projects" << endl;
		cout << "	8. Project Menu" << endl;
		cout << "	9. Previous Menu" << endl;

		string choice;
		getline(cin, choice);

		switch (convertInt(choice)) {
		case 1:
			getNewUsername(company, userIndex);
			break;

		case 2:
			getNewPassword(company, userIndex);
			break;

		case 3:
			assignCoTAccount(company, userIndex);
			break;

		case 4:
			sendMessagePrompt(company, userIndex);
			break;

		case 5:
			displayMessages(company, userIndex);
			break;

		case 6:
			createProject(company, userIndex);
			break;

		case 7:
			displayProjects(company, userIndex);
			break;

		case 8:
			showProjectMenu(company, userIndex);
			break;

		case 9:
			employeeRunning = false;
			break;

		default:
			cout << "That is not a valid option" << endl;
			break;
		}

		save(company);
	}
}

//Main controller interface
// int _tmain (int argc, TCHAR *argv[])
int main() {
	checkDirectories();
	Company company = getBootPrompt();

	bool running = true;
	while (running) {
		cout << company.getName() + " Menu: (Enter number of option)" << endl;
		cout << "	1. Add Employee" << endl;
		cout << "	2. Remove Employee" << endl;
		cout << "	3. Display Employees" << endl;
		cout << "	4. Employee Options" << endl;
		cout << "	5. Change Company Name" << endl;
		cout << "	6. Quit Program" << endl;

		string choice;
		getline(cin, choice);

		switch (convertInt(choice)) {
		case 1:
			company.addEmployee();
			break;

		case 2:
			company.removeEmployee();
			break;

		case 3:
			company.displayEmployees();
			break;

		case 4:
			showEmployeeMenu(company);
			break;

		case 5:
			getNewCompanyName(company);
			break;

		case 6:
			running = false;
			break;

		default:
			cout << "That is not a valid option" << endl;
			break;
		}

		save(company);
	}


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
