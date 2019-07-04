#pragma once
#include "pch.h"
#include "User.h"
#include "Project.h"

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
	Company(string companyName);

	Company(string companyName, int totalEmployees);

	vector<User> getEmployees();

	string getName();

	int getnumTotalEmployees();

	//Used to prevent artifical totalNumEmployee growth when loading from file
	void decrementNumTotalEmployees();

	void addEmployee();

	void addEmployee(User employee);

	bool removeEmployee();

	void displayEmployees();

	void updateEmployees(vector<User> newVector);

	void updateProjects(vector<Project> newVector);

	void changeCompanyName(string newName);

	vector<Project> getProjects();

	void addProject(Project project);

	void removeProject(int i);
};