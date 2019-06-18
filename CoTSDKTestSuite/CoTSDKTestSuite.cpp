// CoTSDKTestSuite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Header_Files/pch.h"
#include "Header_Files/User.h"
#include "Header_Files/Project.h"
#include "Header_Files/Company.h"

const string COMPANY_FILE_ADDRESS = "company_info.txt";
const string MESSAGES_DIR = "Messages";
int convertInt(string str);

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

//Saves employee, company, and project data in company_info.txt
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

	writer << endl;
	vector<Project> saveProjects = company.getProjects();

	for (int i = 0; i < (int)saveProjects.size(); i++) {
		writer << saveProjects[i].getName() + "," + to_string(saveProjects[i].getMembers().size()) + "," + 
			to_string(saveProjects[i].getFilesList().size()) << endl;

		for (auto &member : saveProjects[i].getMembers()) {
			writer << member.getUsername() << endl;
		}

		for (auto &filePath : saveProjects[i].getFilesList()) {
			writer << filePath << endl;
		}
	}

	writer.close();
}

//Loads company on startup from company_info.txt
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

	//Reads and parses employee info
	while (getline(reader, employeeInfo) && employeeInfo != "") {
		vector<string> eInfoVector;
		stringstream eInfoStream(employeeInfo);
		string eInfoToken;

		while (getline(eInfoStream, eInfoToken, ','))
			eInfoVector.push_back(eInfoToken);

		company.addEmployee(User(convertInt(eInfoVector[0]), eInfoVector[1], eInfoVector[2], eInfoVector[3]));
		eInfoVector.clear();

		company.decrementNumTotalEmployees();
	}

	//Reads second layer of project info
	string projectInfo;
	while (getline(reader, projectInfo) && projectInfo != "") {
		vector<string> pInfoVector;
		stringstream pInfoStream(projectInfo);
		string pInfoToken;

		//Reads project info line
		while (getline(pInfoStream, pInfoToken, ','))
			pInfoVector.push_back(pInfoToken);

		Project temp = Project(pInfoVector[0]);
		string memberName;

		//Reads subsequent employee name lines
		for (int i = 0; i < convertInt(pInfoVector[1]); i++) {
			getline(reader, memberName);
			
			vector<User> companyEmployees = company.getEmployees();
			for (int j = 0; j < (int)companyEmployees.size(); j++) {
				if (companyEmployees[j].getUsername() == memberName) {
					temp.addMember(companyEmployees[j]);
					break;
				}
			}
		}

		//Reads subsequent file directory lines
		string fileDir;
		for (int i = 0; i < convertInt(pInfoVector[2]); i++) {
			getline(reader, fileDir);
			temp.addFile(fileDir);
		}

		company.addProject(temp);
	}

	reader.close();
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

//Prompts to send message between two employees
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
	cout << "----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

	ifstream reader;
	// TODO quick fix for now. Inefficient with large total num of employees
	//TODO need way to identify with whom different message threads are engaging
	for (int i = 0; i < company.getnumTotalEmployees(); i++) {
		reader.open("Messages/" + to_string(company.getEmployees()[userIndex].getID()) + "/" + to_string(i) +"conversation.txt");

		string message;
		while (getline(reader, message)) {
			cout << message << endl;
		}

		//TODO When folder doesnt exist, line still prints
		cout << "----------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		reader.close();
	}
}

//Checks if name is present in employees list
bool containsName(vector<User> projectMembers, string name) {
	for (int i = 0; i < projectMembers.size(); i++) {
		if (projectMembers[i].getUsername() == name)
			return true;
	}

	return false;
}

//Prompts user to add employee to company
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

//Prompts to remove user from company
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

//Prompts to create project, add members, and add to company
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

//Prompts to update project name
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

//Displays all projects with logged in user as a member
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

//checks to see if company_info.txt and Messages folder exists
void checkDirectories() {
	if (!dirExists(COMPANY_FILE_ADDRESS)) {
		ofstream receiverOutfile(COMPANY_FILE_ADDRESS, ios::app);
		receiverOutfile.close();
	}

	if (!dirExists(MESSAGES_DIR)) {
		_mkdir(MESSAGES_DIR.c_str());
	}
}

//Open dialog to select file and add to project
bool browseFile(Company &company, int projectIndex) {
	char buff[FILENAME_MAX];
	char* workingPath = GetCurrentDir(buff, FILENAME_MAX);

	char filename[MAX_PATH];

	OPENFILENAMEA ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	//ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0"; // File type filter
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	//ofn.lpstrTitle = "Select a File, yo!"; //Sets window title
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		vector<Project> projects = company.getProjects();

		string filenamepath(filename);
		if (!(filenamepath.substr(filenamepath.length() - 4) == ".tef") || true/*TODO check contains*/) {
			projects[projectIndex].addFile(filename);

			company.updateProjects(projects);
			cout << "Added \"" << filename << "\" To Project. \n";
		}
		else {
			cout << "Unable To Add File" << endl;
		}

		//Changes working path back to where .cpp file is
		_chdir(workingPath);
		return true;
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: cout << "CDERR_DIALOGFAILURE" << endl;   break;
		case CDERR_FINDRESFAILURE: cout << "CDERR_FINDRESFAILURE" << endl;  break;
		case CDERR_INITIALIZATION: cout << "CDERR_INITIALIZATION" << endl;  break;
		case CDERR_LOADRESFAILURE: cout << "CDERR_LOADRESFAILURE" << endl;  break;
		case CDERR_LOADSTRFAILURE: cout << "CDERR_LOADSTRFAILURE" << endl;  break;
		case CDERR_LOCKRESFAILURE: cout << "CDERR_LOCKRESFAILURE" << endl;  break;
		case CDERR_MEMALLOCFAILURE: cout << "CDERR_MEMALLOCFAILURE" << endl;; break;
		case CDERR_MEMLOCKFAILURE: cout << "CDERR_MEMLOCKFAILURE" << endl;  break;
		case CDERR_NOHINSTANCE: cout << "CDERR_NOHINSTANCE" << endl;     break;
		case CDERR_NOHOOK: cout << "CDERR_NOHOOK" << endl;          break;
		case CDERR_NOTEMPLATE: cout << "CDERR_NOTEMPLATE" << endl;      break;
		case CDERR_STRUCTSIZE: cout << "CDERR_STRUCTSIZE" << endl;      break;
		case FNERR_BUFFERTOOSMALL: cout << "FNERR_BUFFERTOOSMALL" << endl;  break;
		case FNERR_INVALIDFILENAME: cout << "FNERR_INVALIDFILENAME" << endl; break;
		case FNERR_SUBCLASSFAILURE: cout << "FNERR_SUBCLASSFAILURE" << endl; break;
		default: cout << "No File Selected." << endl;
		}

		return false;
	}
}

void displayFiles(Company &company, int projectIndex) {
	if (company.getProjects()[projectIndex].getFilesList().size() == 0)
		cout << "No Files Exist For This Project" << endl;
	else {
		for (auto filePath : company.getProjects()[projectIndex].getFilesList()) {
			cout << filePath << endl;
		}
	}
}

//Shows menu for project functionality
void showProjectMenu(Company &company, int userIndex) {
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

	bool projectRunning = true;
	while (projectRunning && projectIndex != -1) {
		cout << projects[projectIndex].getName() + " Project Menu" << endl;
		cout << "	1: Change Project Name" << endl;
		cout << "	2: Add Member" << endl;
		cout << "	3: Remove Member" << endl;
		cout << "	4: Display Members" << endl;
		cout << "	5: Add File To Project" << endl;
		cout << "	6. Display Files In Project" << endl;
		cout << "	7: Delete Project" << endl;
		cout << "	8: Previous Menu" << endl;

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
			break;

		case 4:
			cout << "Members: " + listMembers(company, company.getProjects()[projectIndex].getMembers()) << endl;
			break;

		case 5:
			browseFile(company, projectIndex);
			break;

		case 6:
			displayFiles(company, projectIndex);
			break;

		case 7:
			company.removeProject(projectIndex);
			projectRunning = false;
			break;

		case 8:
			projectRunning = false;
			break;

		default:
			cout << "That is not a valid option" << endl;
			break;
		}

		save(company);
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
