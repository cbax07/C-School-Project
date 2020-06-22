/* Corey Denny
 * SNHU CS210-Q5950
 */

#include <iostream>
#include <jni.h>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

vector<string> zooVector;  //vector of string objects created
std::string trackNum, name, type, subType, eggs, nurse;

class Animal {
public:
		Animal(string tempTrack, string tempName, string tempType, string tempSubType, string tempEggs, string tempNurse);
		std::string trackNum, name, type, subType, eggs, nurse;
};
Animal::Animal(string tempTrack, string tempName, string tempType, string tempSubType, string tempEggs, string tempNurse) {
	trackNum = tempTrack;
	name = tempName;
	type = tempType;
	subType = tempSubType;
	eggs = tempEggs;
	nurse = tempNurse;
}

void GenerateData()               //DO NOT TOUCH CODE IN THIS METHOD
{
     JavaVM *jvm;                      // Pointer to the JVM (Java Virtual Machine)
     JNIEnv *env;                      // Pointer to native interface
                                                              //================== prepare loading of Java VM ============================
     JavaVMInitArgs vm_args;                        // Initialization arguments
     JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
     options[0].optionString = (char*) "-Djava.class.path=";   // where to find java .class
     vm_args.version = JNI_VERSION_1_6;             // minimum Java version
     vm_args.nOptions = 1;                          // number of options
     vm_args.options = options;
     vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
                                                                          //=============== load and initialize Java VM and JNI interface =============
     jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
     delete options;    // we then no longer need the initialization options.
     if (rc != JNI_OK) {
            // TO DO: error processing...
            cin.get();
            exit(EXIT_FAILURE);
     }
     //=============== Display JVM version =======================================
     cout << "JVM load succeeded: Version ";
     jint ver = env->GetVersion();
     cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;

     jclass cls2 = env->FindClass("ZooFileWriter");  // try to find the class
     if (cls2 == nullptr) {
            cerr << "ERROR: class not found !";
     }
     else {                                  // if class found, continue
            cout << "Class MyTest found" << endl;
            jmethodID mid = env->GetStaticMethodID(cls2, "createZooFile", "()V");  // find method
            if (mid == nullptr)
                   cerr << "ERROR: method void createZooFile() not found !" << endl;
            else {
                   env->CallStaticVoidMethod(cls2, mid);                      // call method
                   cout << endl;
            }
     }


     jvm->DestroyJavaVM();
     cin.get();
}

void AddAnimal() {  // Add animal to file - function
	std::string trackNum, name, type, subType, eggs, nurse, newAnimalInfo; // variables for animal specifications
	bool menu = true;
	int userinput;
	while (menu == true) {  //Menu for user
		cout << "-----Animal Addition-----" << endl;
		cout << "Animal tracking number:" << endl;
		cin >> trackNum;
		if (trackNum.length() > 6 || trackNum.length() < 6) {  // Error checking for tracking number
			cout << "Tracking number is the wrong size. Must be 6 digits long." << endl;
			cin >> trackNum;
		}

		cout << "Animal name:" << endl;
		cin >> name;
		if (name.length() > 15) {  // Error checking for animal name
			cout << "Animal name is too large. Must be less than 15 characters long." << endl;
			cin >> name;
		}

		cout << "Animal type:" << endl;
		cin >> type;
		if (type.length() > 15) {  // Error checking for animal type
			cout << "Animal type is too large. Must be less than 15 characters long." << endl;
			cin >> type;
		}

		cout << "Animal sub-type:" << endl;
		cin >> subType;
		if (subType.length() > 15) {  // Error checking for animal sub-type
			cout << "Animal type is too large. Must be less than 15 characters long." << endl;
			cin >> subType;
		}

		if (type != "mammal" && type != "Mammal") {  // if not mammal, prompt for number of eggs
			cout << "Number of eggs:" << endl;
			cin >> eggs;
		} else if (type == "mammal" || type == "Mammal") {
			eggs = "0";
		}
		if (eggs.length() > 5) {  // Error checking for number of eggs
			cout << "Error: must be less than 5 digits long." << endl;
			cin >> subType;
		}

		if (type == "mammal" || type == "Mammal") {  // if mammal, prompt for nursing
			cout << "Is the animal nursing? 1 for yes or 0 for no." << endl;
			cin >> nurse;
		} else {
			nurse = "0";
		}

		string trackNumEx = trackNum.append("  ");  // block of code that checks and resizes user input for efficient storing
		unsigned int namesize = 17 - name.length();
		name.resize(name.length() + namesize, ' ');
		unsigned int typesize = 17 - type.length();
		type.resize(type.length() + typesize, ' ');
		unsigned int subTypesize = 17 - subType.length();
		subType.resize(subType.length() + subTypesize, ' ');
		unsigned int eggssize = 7 - eggs.length();
		eggs.resize(eggs.length() + eggssize, ' ');
		newAnimalInfo = trackNumEx + name + type + subType + eggs + nurse + "\n";

		bool menu2 = true;
		int userInput;
		while (menu2 == true) {  // stores new vector object with user provided info
			cout << "Enter 1 to save or 0 to cancel" << endl;
			cin >> userInput;
			if (userInput == 1) {
				Animal newAnimal(trackNum, name, type, subType, eggs, nurse);
				zooVector.push_back(newAnimalInfo);
				break;
			}
		}

		cout << "Would you like to add another animal? 1 for yes & 0 for no." << endl;
		cin >> userinput;
		if (userinput == 1) {
			AddAnimal();
			return;
		} else {
			cout << "Exiting now" << endl;
			menu = false;
		}
	}
}

void RemoveAnimal() {  // Iterates through the zoo vector in search of a tracking number. If found, the element is deleted.
	bool Removemenu = true;
	int userInput1;

	while (Removemenu == true) {
		string item;
		cout << "Enter tracking number of the animal you wish to delete" << endl;
		cin >> item;  // user input tracking number to search for
		item = item.append("  ");  // appends spaces to the end to match the tracking number input in the Add Animal function

		unsigned int i;

		for (i=0; i<zooVector.size(); i++) {  // Loop that looks for the tracking number
			int found = zooVector.at(i).find(item);
			//cout << found;
			if (found == 0) {
				cout << "Are you sure you want to delete this animal?\n 1 for yes or 2 for no." << endl;
				cin >> userInput1;
				if (userInput1 == 1) {
					zooVector.erase(zooVector.begin() + i);  // erases the object at i location
					cout << "Animal successfully deleted.\n\n" << endl;
					Removemenu = false;
				}
			}

		}
	}
}

void LoadDataFromFile() {  // Loads data from the file with the correct amount of spacing between variables
	ifstream inFS;
	inFS.open("zoodata.txt");
	string fileData;
	std::string trackNum, name, type, subType, eggs, nurse;

	while(inFS >> trackNum >> name >> type >> subType >> eggs >> nurse) {
		trackNum = trackNum.append("  ");
		unsigned int namesize = 17 - name.length();
		name.resize(name.length() + namesize, ' ');
		unsigned int typesize = 17 - type.length();
		type.resize(type.length() + typesize, ' ');
		unsigned int subTypesize = 17 - subType.length();
		subType.resize(subType.length() + subTypesize, ' ');
		unsigned int eggssize = 7 - eggs.length();
		eggs.resize(eggs.length() + eggssize, ' ');
		string newAnimalInfo = trackNum + name + type + subType + eggs + nurse + "\n";
		zooVector.push_back(newAnimalInfo);
	}
	cout << "Load complete." << endl;
	//cout << zooVector.size();
	inFS.close();
}

void SaveDataToFile() {  //If any changes are made, this function writes all data to the zoodata.txt file
     ofstream outFS("zoodata.txt");
     if (!outFS.is_open()) {
    	 cout << "Could not open the file." << endl;
    	 return;
     }
     unsigned int i;
     for (i=0; i<zooVector.size(); i++) {
    	 outFS << zooVector.at(i);
     }
     cout << "Save successful" << endl;
     outFS.close();
}

void DisplayAnimalData() {  // After "Load Data" function has been run, this function displays the data in tabular format
	cout << "\nTrack #|      Name      |      Type      |    Sub-Type    | Eggs | Nurse |" << endl;
	cout << "--------------------------------------------------------------------------\n";
	unsigned int i;
	for (i=0; i<zooVector.size(); i++) {
		cout << setw(68) << left << zooVector[i] << endl;
	}
	cout << "--------------------------------------------------------------------------\n" << endl;
}

void DisplayMenu() {  //Displays menu until program ends
	int tempNum = 0;
	bool menu = true;
	while (menu == true) {
     cout << "------------------------------" << endl;
     cout << "----Wildlife Zoo Interface----" << endl;
     cout << "------------------------------" << endl;
     cout << "1. Load Animal Data" << endl;
     cout << "2. Generate Data" << endl;
     cout << "3. Display Animal Data" << endl;
     cout << "4. Add Record" << endl;
     cout << "5. Delete Record" << endl;
     cout << "6. Save Animal Data" << endl;
     cout << "7. Quit\n"  << endl;

	cin >> tempNum;  // switch code for the seven different user options
	switch (tempNum) {
		case 1:
			LoadDataFromFile();
			break;
		case 2:
			GenerateData();
			break;
		case 3:
			DisplayAnimalData();
			break;
		case 4:
			AddAnimal();
			break;
		case 5:
			RemoveAnimal();
			break;
		case 6:
			SaveDataToFile();
			break;
		case 7:
			cout << "Exiting now" << endl;
			menu = false;
			break;
		default:
			cout << "Enter a valid number, 1-7." << endl;
			break;
		}
	}
}

int main() {
	//Animal Lion("1", "1", "1", "1", "1", "1");  //Test to make sure class works
	DisplayMenu();  //Displays the menu which loops until program ends
	return 1;
}
