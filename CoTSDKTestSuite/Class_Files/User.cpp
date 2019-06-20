#include "../Header_Files/pch.h"
#include "../Header_Files/User.h"

const int STARTING_YEAR = 1900;
const string MONTHS[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
const string DAYS[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

User::User(int id, string name, string pass) {
	/* Precondition: id must be (size of employees list in company) + 1*/
	userID = id;

	username = name;
	password = pass;
	numUsernameChanges = 0;
	cotAccountName = "null";
}

User::User(int id, string name, string pass, string cotLogin) {
	/* Precondition: id must be (size of employees list in company) + 1*/
	userID = id;

	username = name;
	password = pass;
	numUsernameChanges = 0;
	cotAccountName = cotLogin;
}

int User::getID() {
	return userID;
}

string User::getUsername() {
	return username;
}

//TODO prevent duplicate names
void User::changeUsername(string name) {
	numUsernameChanges++;
	username = name;
}

string User::getPassword() {
	return password;
}

bool User::changePassword(string pass) {
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

string User::getCotAccountName() {
	return cotAccountName;
}

User User::updateCotAccountName() {
	//TODO Use Person object function enable() (1.4.2.2) and catch specific error code for non-existent user
	cout << "Enter CoT Account Username" << endl;
	getline(cin, cotAccountName);
	return *this;
}

void User::sendMessage(User receiver) {
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
