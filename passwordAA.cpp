#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <openssl/evp.h>

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
vector<PasswordList> searchPasswords();


int main() {

    const char* dir = "./passwords.txt";

    struct stat sb;

    if (stat(dir, &sb) != 0) {
        ofstream file("passwords.txt");
        //std::cout << "Path is valid";
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
       //while (!isdigit(userChoice)) {
        //raiseError();
        //std::cin >> userChoice;
       //}
       switch (userChoice)
       {
        case 1: {
            do {
                    int passwordLength;
                    std::cout << "Enter the length of the password: ";
                    std::cin >> passwordLength;
                    string newPassword = generatePassword(passwordLength);
                    std::cout << "\nGenerated password: " << newPassword << "\n" << endl;
            } while (userContinue());
                break;
        }

        case 2: {
            do {
                if (!savedPasswords.empty()) {

                    std::cout << "\n+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+\n"
                                 "|S|a|v|e|d| |P|a|s|s|w|o|r|d|s|\n"
                                 "+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+\n";
                    for (size_t i = 0; i < savedPasswords.size(); i++) {
                        std::cout <<"\n" << i + 1 << ". Website/Application: " << savedPasswords[i].websiteOrApp << endl;
                        std::cout << "Username: " << savedPasswords[i].userName << endl;
                        std::cout << "Password: " << savedPasswords[i].password << endl;
                    }
                } else {
                    std::cout << "No saved passwords found.\n";
                }

                char searchOrNot;
                std::cout << "Would you like to search for a password? Enter Y/N: ";
                std::cin >> searchOrNot;

                while (toupper(searchOrNot) != 'Y' && toupper(searchOrNot) != 'N') {
                    raiseError();
                    std::cin >> searchOrNot;
                }

                if (toupper(searchOrNot) == 'Y') {
                    vector<PasswordList> searchResults = searchPasswords();

                    if (!searchResults.empty()) {
                        std::cout 
                            << "                           _         , __                  _         \n"
                            << "  ()                      | |       /|/  \\                | |         \n"
                            << "  /\\  _   __,   ,_    __  | |        |___/  _   ,         | |_|_  ,  \n"
                            << " /  \\|/  /  |  /  |  /    |/ \\       | \\   |/  / \\_|   |  |/  |  / \\_\n"
                            << "/(__/|__/\\_/|_/   |_/\\___/|   |_/    |  \\_/|__/ \\/  \\_/|_/|__/|_/ \\/ \n";


                        for (size_t i = 0; i < searchResults.size(); i++) {
                            std::cout << "\n\n" << i + 1 << ". Website/Application: " << searchResults[i].websiteOrApp << endl;
                            std::cout << "Username: " << searchResults[i].userName << endl;
                            std::cout << "Password: " << searchResults[i].password << endl;
                        }
                    } else {
                        std::cout << "No matching passwords found.\n";
                    }
                }
            } while (userContinue());
            break;
        }


        case 3: {
            do {
                PasswordList newPasswordEntry;
                std::cout << "Enter the name of the website or application: ";
                std::cin.ignore();
                getline(std::cin, newPasswordEntry.websiteOrApp);
                std::cout << "Enter the username: ";
                getline(std::cin, newPasswordEntry.userName);

                std::cout << "Would you like to generate a password? Enter (Y/N): ";
                char generateChoice;
                std::cin >> generateChoice;
                if (toupper(generateChoice) == 'N'){
                    std::cout << "Enter the password you would like to save: ";
                    std::cin.ignore();
                    getline(std::cin, newPasswordEntry.password);
                }
                else {
                    int passwordLength;
                    std::cout << "Enter the length of the password: ";
                    std::cin >> passwordLength;
                    newPasswordEntry.password = generatePassword(passwordLength);
                }
                savedPasswords.push_back(newPasswordEntry);
                savePasswords(savedPasswords);
                std::cout << "Generated and stored password for " << newPasswordEntry.websiteOrApp << endl;
            } while (userContinue());
                break;
        }

        case 4: {
            ifstream file("passwords.txt");
            file.close();
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
    std::cout << "\n+-+-+-+-+-+ +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+\n"
                 "|A|y|o|'|s| |P|a|s|s|w|o|r|d| |M|a|n|a|g|e|r|\n"
                 "+-+-+-+-+-+ +-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+\n";

    std::cout << "\nMy main functionality is to simplify your password usage and storage" << endl;
}

void raiseError() {
    std::cout << "\nYou have made an invalid selection." << endl;
    std::cout << "Try again! Enter a valid option: ";
}

bool userContinue() {
    char userResponse;
    std::cout << "Would you like to continue? Enter 'Y' or 'N': ";
    std::cin >> userResponse;
    while (toupper(userResponse) != 'Y' && toupper(userResponse) != 'N') {
        raiseError();
        std::cin >> userResponse;
    }
    return toupper(userResponse) == 'Y';
}

int displayMenu(const string a[], int size, int selection = 0) {
    std::cout << "   __  __                  \n"
            "  |  \\/  | ___ _ __  _   _ \n"
            "  | |\\/| |/ _ | '_ \\| | | |\n"
            "  | |  | |  __| | | | |_| |\n"
            "  |_|  |_|\\___|_| |_|\\__,_|\n"
            "\n";

    for (int i = 0; i < size; i++) {
        std::cout << i + 1 << ". " << a[i] << endl;
    }
    std::cout << "Select an option: ";
    std::cin >> selection;
    while (selection <= 0 || selection > size) {
        raiseError();
        std::cin >> selection;
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

//  Save Function // 
void savePasswords(const vector<PasswordList>& passwords) {
    ofstream file("passwords.txt");
    if (file.is_open()) {
        for (const auto& password : passwords) {
            file << "Website/Application: " << password.websiteOrApp << '\n'
                 << "Username: " << password.userName << '\n'
                 << "Password: " << password.password << '\n'
                 << "--------------------------\n";  // Add the delimiter
        }
        //file.close();
        std::cout << "Passwords saved to 'passwords.txt'." << endl;
    } else {
        cerr << "Error: Unable to open 'passwords.txt' for saving passwords." << endl;
    }
}

//  Search Function  //
vector<PasswordList> searchPasswords() {
    ifstream file("passwords.txt");
    string line;
    string webAppName;
    vector<PasswordList> foundPasswords;

    std::cout << "What is the website name you would like to find? ";
    std::cin >> webAppName;
    bool found = false;

    while (getline(file, line)) {
        if (line.find("Website/Application: " + webAppName) != string::npos) {
            PasswordList entry;
            entry.websiteOrApp = line.substr(21); // Extract website/app name
            getline(file, line); // Read the next line (Username)
            entry.userName = line.substr(10); // Extract username
            getline(file, line); // Read the next line (Password)
            entry.password = line.substr(10); // Extract password
            foundPasswords.push_back(entry);
            found = true;
        }
    }

    if (!found) {
        std::cout << "Your search was not found!" << endl;
    }

    return foundPasswords;
}

//  Display Passwords  //
vector<PasswordList> loadPasswords() {
    vector<PasswordList> passwords; //  Create vector to return after password is loaded.
    ifstream file("passwords.txt");
    if (file.is_open()) {
        PasswordList entry; //  Create a temporary structure to store read values.
        string line;    //  Store each line in the passwords in string.

        while (getline(file, line)) {
            if (line.find("Website/Application: ") != string::npos) {
                entry.websiteOrApp = line.substr(21);
            } else if (line.find("Username: ") != string::npos) {
                entry.userName = line.substr(10);
            } else if (line.find("Password: ") != string::npos) {
                entry.password = line.substr(10);

                passwords.push_back(entry); //  Store passwords in the vector to be returned

                entry.websiteOrApp = "";  // Clear the entry for the next password
                entry.userName = "";
                entry.password = "";
            }
        }
        //file.close();
    } else {
        //  Throws error when unable to access file.
        std::cout << "Error: Unable to open 'passwords.txt' for loading passwords." << endl;
    }
    return passwords;
}

//  Exit Message  //
int programClose() {
    std::cout << "Exiting Password Manager." << endl;
    return 0;
}

