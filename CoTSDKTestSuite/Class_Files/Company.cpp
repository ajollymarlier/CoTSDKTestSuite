#include "../Header_Files/pch.h"
#include "../Header_Files/User.h"
#include "../Header_Files/Project.h"
#include "../Header_Files/Company.h"

Company::Company(string companyName) {
	name = companyName;
	numTotalEmployees = 0;
}

Company::Company(string companyName, int totalEmployees) {
	name = companyName;
	numTotalEmployees = totalEmployees;
}

vector<User> Company::getEmployees() {
	return employees;
}

string Company::getName() {
		return name;
}

int Company::getnumTotalEmployees() {
	return numTotalEmployees;
}

//Used to prevent artifical totalNumEmployee growth when loading from file
void Company::decrementNumTotalEmployees() {
		numTotalEmployees--;
}

//Prevent duplicate username addition
void Company::addEmployee() {
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
	system("cls");

	cout << "Employee Added" << endl;
}

void Company::addEmployee(User employee) {
	employees.push_back(employee);
	numTotalEmployees++;
}

bool Company::removeEmployee() {
	cout << "Enter Employee Name: " << endl;
	string name;
	getline(cin, name);

	system("cls");

	for (int i = 0; i < (int)employees.size(); i++) {
		if (employees[i].getUsername() == name) {
			employees.erase(employees.begin() + i);
			cout << "Employee Removed" << endl;
			return true;
		}
	}

	cout << "Employee not listed" << endl;
	return false;
}

void Company::displayEmployees() {
	system("cls");

	cout << "Employees: " << endl;
	for (int i = 0; i < employees.size(); i++) {
		User user = employees[i];
		cout << "	Username: " + user.getUsername() + ", Password: " + user.getPassword() + ", CoT Account: " + user.getCotAccountName() << endl;
	}
}

void Company::updateEmployees(vector<User> newVector) {
	employees = newVector;
}

void Company::updateProjects(vector<Project> newVector) {
	projects = newVector;
}

void Company::changeCompanyName(string newName) {
	name = newName;
}

vector<Project> Company::getProjects() {
	return projects;
}

void Company::addProject(Project project) {
	projects.push_back(project);
}

void Company::removeProject(int i) {
	projects.erase(projects.begin() + i);
}