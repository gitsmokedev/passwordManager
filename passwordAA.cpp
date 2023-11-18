#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>

using namespace std;


struct PasswordList
{
    string websiteOrApp;
    string userName;
    string password;    
};

//  Function Prototypes
bool userContinue();
void displayTitle();
void raiseError();
void savePasswords(const vector<PasswordList>& passwords);
int programClose();
int displayMenu(const string [], int, int);
string generatePassword(int length);
vector<PasswordList> loadPasswords();

int main() {

    const char* dir = "./passwords.txt";

    struct stat sb;

    if (stat(dir, &sb) != 0) {
        ofstream file("passwords.txt");
        //cout << "Path is valid";
    } 
    
    int userChoice = 0;
    const string MENU[4] = {
        "Generate Passwords",
        "Display Passwords",
        "Store Passwords",
        "Exit Program"
    };

    displayTitle(); //   Display Title to the user.

    while (userChoice != 4)
    {
       vector<PasswordList> savedPasswords = loadPasswords();
       userChoice = displayMenu(MENU, sizeof(MENU)/sizeof(MENU[0]), userChoice);

       switch (userChoice)
       {
        case 1: {
            do {
                    int passwordLength;
                    cout << "Enter the length of the password: ";
                    cin >> passwordLength;
                    string newPassword = generatePassword(passwordLength);
                    cout << "\nGenerated password: " << newPassword << "\n" << endl;
            } while (userContinue());
                break;
        }

        case 2: {
            do {
                if (!savedPasswords.empty()) {
                        cout << "Saved Passwords:\n";
                        for (size_t i = 0; i < savedPasswords.size(); i++) {
                            cout << i + 1 << ". Website/Application: " << savedPasswords[i].websiteOrApp << endl;
                            cout << "Username/Password: " << savedPasswords[i].userName << " - " << savedPasswords[i].password << endl;
                        }
                    } else {
                        cout << "No saved passwords found.\n";
                    }
            } while (userContinue());
                break;      
        }

        case 3: {
            do {
                PasswordList newPasswordEntry;
                cout << "Enter the name of the website or application: ";
                cin >> newPasswordEntry.websiteOrApp;
                cout << "Enter the username: ";
                cin >> newPasswordEntry.userName;
                int passwordLength;
                cout << "Enter the length of the password: ";
                cin >> passwordLength;
                newPasswordEntry.password = generatePassword(passwordLength);

                savedPasswords.push_back(newPasswordEntry);
                savePasswords(savedPasswords);
                cout << "Generated and stored password for " << newPasswordEntry.websiteOrApp << endl;
            } while (userContinue());
                break;
        }

        case 4: {
            programClose();
            break;
        } 
        
        default:
            raiseError();
            break;
       }
    }
    
    return 0;
}


//  Functions for the program.
void displayTitle() {
    cout << "Welcome to Ayo's Password Manager." << endl;
    cout << "My main functionality is to simplify your password usage and storage" << endl;
}

void raiseError() {
    cout << "You have made an invalid selection." << endl;
}

bool userContinue() {
    char userResponse;
    cout << "Would you like to continue? Enter 'Y' or 'N': ";
    cin >> userResponse;
    while (toupper(userResponse) != 'Y' && toupper(userResponse) != 'N') {
        raiseError();
        cin >> userResponse;
    }
    return toupper(userResponse) == 'Y';
}

int displayMenu(const string a[], int size, int selection = 0) {
    cout << "   __  __                  \n"
            "  |  \\/  | ___ _ __  _   _ \n"
            "  | |\\/| |/ _ | '_ \\| | | |\n"
            "  | |  | |  __| | | | |_| |\n"
            "  |_|  |_|\\___|_| |_|\\__,_|\n"
            "\n";

    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". " << a[i] << endl;
    }
    cout << "Select an option: ";
    cin >> selection;
    while (selection <= 0 || selection > size) {
        raiseError();
        cin >> selection;
    }
    return selection;
}

string generatePassword(int length) {
    const string charset = 
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    const int charsetSize = charset.size();
    string password;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < length; i++) {
        int index = rand() % charsetSize;
        password += charset[index];
    }

    return password;
}

void savePasswords(const vector<PasswordList>& passwords) {
    ofstream file("passwords.txt");
    if (file.is_open()) {
        for (const auto& password : passwords) {
            file << "Website/Application: " << password.websiteOrApp << '\n'
                 << "Username/Password: " << password.userName << " - " << password.password << '\n'
                 << "--------------------------\n";  // Add the delimiter
        }
        //file.close();
        cout << "Passwords saved to 'passwords.txt'." << endl;
    } else {
        cerr << "Error: Unable to open 'passwords.txt' for saving passwords." << endl;
    }
}

vector<PasswordList> loadPasswords() {
    vector<PasswordList> passwords;
    ifstream file("passwords.txt");
    if (file.is_open()) {
        PasswordList entry;
        string line;
        bool readingEntry = false;
        while (getline(file, line)) {
            if (line == "--------------------------") {
                if (readingEntry) {
                    passwords.push_back(entry);
                    entry.websiteOrApp = "";  // Clear the entry for the next password
                    entry.userName = "";
                    entry.password = "";
                    readingEntry = false;
                }
            } else if (line.find("Website/Application: ") == 0) {
                entry.websiteOrApp = line.substr(21);
                readingEntry = true;
            } else if (line.find("Username/Password: ") == 0) {
                string usernamePassword = line.substr(19);
                size_t pos = usernamePassword.find(" - ");
                if (pos != string::npos) {
                    entry.userName = usernamePassword.substr(0, pos);
                    entry.password = usernamePassword.substr(pos + 3);
                }
            }
        }
        //file.close();
    } else {
        cerr << "Error: Unable to open 'passwords.txt' for loading passwords." << endl;
    }
    return passwords;
}

int programClose() {
    cout << "Exiting Password Manager." << endl;
    return 0;
}

