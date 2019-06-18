#pragma once
#include "pch.h"

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
	User(int id, string name, string pass);

	User(int id, string name, string pass, string cotLogin);

	int getID();

	string getUsername();

	void changeUsername(string name);

	string getPassword();

	bool changePassword(string pass);

	string getCotAccountName();

	User updateCotAccountName();

	void sendMessage(User receiver);
};