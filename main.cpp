#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
#include <conio.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cstring>

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

using namespace std;

class User {
private:
    struct user {
        string username;
        string password;
        float balance;
        float jackpot;
    };
   
public:
    user current_user;

    User(float initial_balance = 0.0f) {
        current_user.balance = initial_balance;
        current_user.jackpot = 1392742.0f;
    }

    float getBalance() const {
        return current_user.balance;
    }

    float getJackpot() const {
        return current_user.jackpot;
    }

    void addBalance(float amount) {
        current_user.balance += amount;
        save_user();
    }

    void subtractBalance(float amount) {
        current_user.balance -= amount;
        save_user();
    }

    void addToJackpot(float amount) {
        current_user.jackpot += amount;
    }

    void setUsername(const string& username) {
        current_user.username = username;
    }

    void setPassword(const string& password) {
        current_user.password = password;
    }

    void setBalance(float balance) {
        current_user.balance = balance;
    }

    string getUsername() const {
        return current_user.username;
    }

    string getPassword() const {
        return current_user.password;
    }

    void save_user() {
    ifstream infile("user_info.txt");
    stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    string content = buffer.str();
    stringstream new_content;
    bool user_found = false;
    size_t pos = 0;

    // Read line by line and update user information if the username matches
    string line;
    while (getline(buffer, line)) {
        if (line.find("Username: " + getUsername()) != string::npos) {
            line = "Username: " + getUsername() + " Password: " + getPassword() + " Balance: " + to_string(getBalance()) + " Jackpot: " + to_string(getJackpot());
            user_found = true;
        }
        new_content << line << "\n";
    }

    // If the user was not found, add a new line for the user
    if (!user_found) {
        new_content << "Username: " + getUsername() + " Password: " + getPassword() + " Balance: " + to_string(getBalance()) + " Jackpot: " + to_string(getJackpot()) + "\n";
    }

    ofstream outfile("user_info.txt");
    outfile << new_content.str();
    outfile.close();
}


    bool load_user(const string& username, const string& password) {
        ifstream file("user_info.txt");
        if (!file.is_open()) return false;

        string line;
        while (getline(file, line)) {
            if (line.find("Username: " + username) != string::npos) {
                size_t pos = line.find("Password: ");
                if (pos != string::npos && line.substr(pos + 10, password.length()) == password) {
                    size_t balance_pos = line.find("Balance: ");
                    size_t jackpot_pos = line.find("Jackpot: ");
                    if (balance_pos != string::npos && jackpot_pos != string::npos) {
                        float balance = stof(line.substr(balance_pos + 9, jackpot_pos - balance_pos - 9));
                        float jackpot = stof(line.substr(jackpot_pos + 9));
                        setUsername(username);
                        setPassword(password);
                        setBalance(balance);
                        current_user.jackpot = jackpot;
                        file.close();
                        return true;
                    }
                }
            }
        }
        file.close();
        return false;
    }
};

class UserManager {
public:
    void clear_screen() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void press_return() const {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nPress Enter to return.";
        cin.get();
        clear_screen();
    }

    string get_username_input() const {
        string username;
        while (true) {
            cout << "Enter username: ";
            cin >> username;
            if (username.length() >= 4) break;
            cout << "\nUsername must be at least 4 characters long.\n" << endl;
        }
        return username;
    }

    string get_password_input() const {
        string password;
        char ch;
        while ((ch = _getch()) != 13) { // 13 is ASCII value of Enter key
            if (ch == 8) { // 8 is ASCII value of Backspace key
                if (!password.empty()) {
                    cout << "\b \b";
                    password.pop_back();
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return password;
    }

    string get_confirmed_password() const {
        string password, confirm_password;
        while (true) {
            while (true) {
                cout << "Enter password: ";
                password = get_password_input();
                if (password.length() >= 6) break;
                cout << "\nPassword must be at least 6 characters long.\n" << endl;
            }

            cout << "Confirm password: ";
            confirm_password = get_password_input();

            if (password == confirm_password) break;
            cout << "\nPasswords do not match. Please try again.\n" << endl;
        }
        return password;
    }

    bool username_exists(const string& username) const {
        ifstream file("user_info.txt");
        if (!file.is_open()) return false;

        string line;
        while (getline(file, line)) {
            if (line.find("Username: " + username) != string::npos) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    bool verify_user(const string& username, const string& password) const {
        ifstream file("user_info.txt");
        if (!file.is_open()) return false;

        string line;
        while (getline(file, line)) {
            if (line.find("Username: " + username) != string::npos) {
                size_t pos = line.find("Password: ");
                if (pos != string::npos && line.substr(pos + 10) == password) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }
};

class Wallet
{
private:
    User& user; // Reference to User object
    UserManager userManager;

public:
    Wallet(User &user) : user(user) {}

    void view_wallet() {
        int choice;

        while (true) {
            cout << "+----------------------------------------+" << endl;
            cout << "|                WALLET                  |" << endl;
            cout << "+----------------------------------------+\n" << endl;

            cout << "Current Balance: $" << user.getBalance() << endl;

            cout << "\n+----------------------------------------+" << endl;
            cout << "| [1] Top-Up                             |" << endl;
            cout << "| [2] Withdraw                           |" << endl;
            cout << "+----------------------------------------+" << endl;
            cout << "| [3] Exit                               |" << endl;
            cout << "+----------------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                top_up();
                break;
            case 2:
                userManager.clear_screen();
                withdraw();
                break;
            case 3:
                userManager.clear_screen();
                return;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }

    void top_up() {
        float amount;

        while (true) {
            cout << "+----------------------------------------+" << endl;
            cout << "|                Top-Up                  |" << endl;
            cout << "+----------------------------------------+\n" << endl;

            cout << "Enter amount to top-up: ";

            if (!(cin >> amount)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid amount.\n" << endl;
                continue;
            } else if (amount <= 0) {
                userManager.clear_screen();
                cout << "Amount must be greater than 0.\n" << endl;
                continue;
            } else {
                user.addBalance(amount);
                cout << "\nSuccessfully updated balance!" << endl;
                userManager.press_return();
                return;
            }
        }
    }

    void withdraw() {
        float amount;

        while (true) {
            cout << "+----------------------------------------+" << endl;
            cout << "|                Withdraw                |" << endl;
            cout << "+----------------------------------------+\n" << endl;

            cout << "Enter amount to withdraw: ";

            if (!(cin >> amount)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid amount.\n" << endl;
                continue;
            } else if (amount <= 0) {
                userManager.clear_screen();
                cout << "Amount must be greater than 0.\n" << endl;
                continue;
            } else if (amount > user.getBalance()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "You do not have enough balance. Please enter a valid amount.\n" << endl;
                continue;
            } else {
                user.subtractBalance(amount);
                cout << "\nSuccessfully updated balance!" << endl;
                userManager.press_return();
                return;
            }
        }
    }

};


class Slot {
private:
    User &user;
    UserManager &userManager;
    Wallet &wallet;

public:
    Slot(User &user, UserManager &userManager, Wallet &wallet)
    : user(user), userManager(userManager), wallet(wallet) {}
        
    string slotArray[6] = {"7", "Orange", "Cherry", "Blueberry", "Watermelon", "Pear"};
    string colors[6] = {"\033[0;36m", "\e[0;33m", "\033[0;31m", "\033[0;34m", "\033[0;35m", "\033[0;32m"};
    const string RESET = "\033[0m";

    int choice;

    void SlotMenu() {
        while (true) {
            cout << " ======== Welcome to the Slot Machine! ======== " << endl << endl;
            cout << "    Test your luck? [1] Yes [2] Main Menu" << endl;
            cout << "                Choice: ";
            
            while (!(cin >> choice)) {
                cout << endl << "    Invalid selection. Please enter 1 or 2: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
    
            if (choice == 1) {
                if (user.getBalance() >= 15) {
                    slot();
                } else {
                    cout << endl << "You do not have enough balance to play the Slot Machine." << endl << endl;
                }
            } else if (choice == 2) {
                userManager.clear_screen();
                return;
            } else {
                cout << endl << "\t Invalid Input. Try again." << endl << endl;
            }
        }
    }

    void slot() {
        int arraySize = sizeof(slotArray) / sizeof(slotArray[0]);
        srand(static_cast<unsigned int>(time(0)));
     
        int randomIndex1 = rand() % arraySize;
        int randomIndex2 = rand() % arraySize;
        int randomIndex3 = rand() % arraySize;
    
        string reel1 = slotArray[randomIndex1];
        string reel2 = slotArray[randomIndex2];
        string reel3 = slotArray[randomIndex3];
    
        cout << endl << " ================ SLOT MACHINE ================" << endl << endl;
        cout << "  ||  "<< colors[randomIndex1] << reel1 << RESET << "  |  " 
             << colors[randomIndex2] << reel2 << RESET << "  |  " 
             << colors[randomIndex3] << reel3 << RESET << "  ||" << endl;
        cout << endl << " ==============================================" << endl;
    
        if ((reel1 == reel2) && (reel2 == reel3)) {
            cout << endl << "\t     JACKPOT! YOU WON $100" << endl;
            user.addBalance(100);
            
            cout << "\t     Current balance: " << user.getBalance() << endl << endl;
            
            cout << "\t Play again? [1] Yes [2] Main Menu: ";
            
            while (!(cin >> choice)) {
                cout << "    Invalid selection. Please enter 1 or 2: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
    
            if (choice != 1) {
                userManager.clear_screen();
                return;
            }
        } else {
            cout << "\t           You lose!" << endl;
            user.subtractBalance(15);
            cout << "\t     Current balance: " << user.getBalance() << endl << endl;
        }
    }
};


class Lotto
{
private:
    User &user;
    UserManager &userManager;
    Wallet &wallet;

public:
    Lotto(User &user, UserManager &userManager, Wallet &wallet)
        : user(user), userManager(userManager), wallet(wallet) {}

    void instructions(){
        cout << "+----------------------------------------+" << endl;
        cout << "|              INSTRUCTIONS              |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "\nObjective:\n" << endl;
        cout << "- The goal of the game is to match all 6 numbers of your bets with the 6 randomly" << endl;
        cout << " generated winning numbers." << endl;

        cout << "\nBetting:\n" << endl;
        cout << "- You will be prompted to enter 6 different bets." << endl;
        cout << "- Each bet must be a unique integer between 0 and the maximum number allowed." << endl;

        cout << "\nWinning Numbers:\n" << endl;
        cout << "- After entering your bets, the game will generate 6 random winning numbers." << endl;
        cout << "- These numbers are also unique and fall within the same range as your bets." << endl;

        cout << "\nMatching:\n" << endl;
        cout << "- The game will compare your bets with the winning numbers." << endl;
        cout << "- If all 6 of your bets match the winning numbers, you win!" << endl;
        cout << "- Otherwise, you lose." << endl;

        userManager.press_return();
    }

    void difficulty_menu(){
        int choice;

        while(true){
            cout << "+----------------------------------+" << endl;
            cout << "|        Difficulty Levels         | " << endl;
            cout << "+----------------------------------+" << endl;
            cout << "|                                  |" << endl;
            cout << "| [1] Easy         | 1 - 19 | $300 |" << endl;
            cout << "| [2] Intermediate | 1 - 39 | $200 |" << endl;
            cout << "| [3] Difficult    | 1 - 59 | $100 |" << endl;
            cout << "|                                  |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [0] Cancel                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "Choose difficulty: " ;

            if (!(cin >> choice)){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
                continue;
            }

            switch (choice){
                case 0:
                    userManager.clear_screen();
                    return;
                    break;
                case 1:
                    if (bet_balance_check(300)){
                        userManager.clear_screen();
                        cout << "+----------------------------------+" << endl;
                        cout << "|        EASY LEVEL: 1 - 19        |" << endl;
                        cout << "+----------------------------------+" << endl;
                        game_menu(19, 300);
                    }
                    break;
                case 2:
                    if (bet_balance_check(200)){
                        userManager.clear_screen();
                        cout << "+----------------------------------+" << endl;
                        cout << "|     INTERMEDIATE LEVEL: 1 - 39   |" << endl;
                        cout << "+----------------------------------+" << endl;
                        game_menu(39, 200);
                    }
                    break;
                case 3:
                    if (bet_balance_check(100)){
                        userManager.clear_screen();
                        cout << "+----------------------------------+" << endl;
                        cout << "|       DIFFICULT LEVEL: 1 - 59    |" << endl;
                        cout << "+----------------------------------+" << endl;
                        game_menu(59, 100);
                    }
                    break;
                default:
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter an option from the menu.\n" << endl;
                    break;
            }
        }
    }

    bool bet_balance_check(int bet){
        if (user.getBalance() <  bet){
            userManager.clear_screen();
            cout << "Insufficient balance. You only have $" << user.getBalance() << " for a $" << bet << " bet.\n" << endl;
            return false;
        }
        return true;
    }

    void game_menu(int max_num, int bet) {
            srand((unsigned) time(NULL));

            bool result = game(max_num);
            if (result) {
                cout << "\nCongratulations! You won $" << user.getJackpot() << "!" << endl;
                user.addBalance(user.getJackpot());
            } else {
                cout << "\nYou did not guess all of the winning numbers." << endl;
                cout << "You lose $" << bet << "." << endl;
                user.subtractBalance(bet);
                user.addToJackpot(bet);
            }

            userManager.press_return();
            return;
        }

    bool game(int max_num) {
            int arr_bet[6];
            int arr_wnums[6];

            get_bets(arr_bet, max_num);

            if (!verify_bets(arr_bet, max_num)) {
                return false;
            }

            userManager.clear_screen();
            cout << "+----------------------------------------+" << endl;
            cout << "|                RESULTS                 |" << endl;
            cout << "+----------------------------------------+\n" << endl;

            cout << "BETS:" << endl;
            for (int i = 0; i < 6; i++) {
                cout << "[ " << arr_bet[i] << " ] ";
            }

            get_winning_numbers(arr_wnums, max_num);

            for (int s = 0; s < 6; s++) {
                cout << "[ " << arr_wnums[s] << " ] ";
            }
            cout << "\n\n+----------------------------------------+" << endl;

            return bet_num_matches(arr_bet, arr_wnums);
        }

    void get_bets(int arr_bet[], int max_num) const {
            cout << "\nEnter your bets: " << endl;
            for (int i = 0; i < 6; i++) {
                arr_bet[i] = get_unique_bet(arr_bet, i, max_num);
            }
        }

    int get_unique_bet(int arr[], int size, int max_num) const {
            int element;
            bool unique;
            bool valid;

            do {
                cout << "[" << size + 1 << "] -- ";
                valid = true;

                if (!(cin >> element)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInvalid input. Please enter a valid integer.\n" << endl;
                    valid = false;
                }

                if (valid && (element > max_num || element <= 0)) {
                    cout << "\nInvalid input. Bets must be between 1 and " << max_num << ".\n" << endl;
                    valid = false;
                }

                if (valid) {
                    unique = is_unique(arr, size, element);
                    if (!unique) {
                        cout << "\nYou have already bet on this number. Duplicate bets are not allowed. Please enter a different bet.\n" << endl;
                    }
                }
            } while (!valid || !unique);

            return element;
        }

    bool is_unique(int arr[], int size, int element) const {
            for (int i = 0; i < size; ++i) {
                if (arr[i] == element) {
                    return false;
                }
            }
            return true;
        }

    bool verify_bets(int arr[], int max_num) {
            int choice;

            userManager.clear_screen();
            while (true) {
                cout << "+----------------------------------------+" << endl;
                cout << "|                  BETS                  | " << endl;
                cout << "+----------------------------------------+\n" << endl;
                for (int i = 0; i < 6; i++) {
                    cout << "[ " << arr[i] << " ] ";
                }

                cout << "\n\n+----------------------------------------+" << endl;
                cout << "| [1] Change a bet                       |" << endl;
                cout << "| [2] Finalize bets                      |" << endl;
                cout << "+----------------------------------------+" << endl;
                cout << "| [0] Cancel                             | " << endl;
                cout << "+----------------------------------------+" << endl;
                cout << "\nChoose option: ";

                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter an integer.\n" << endl;
                    continue;
                }

                switch (choice) {
                    case 0:
                        userManager.clear_screen();
                        main_menu();
                        break;
                    case 1:
                        userManager.clear_screen();
                        change_bet(arr, max_num);
                        break;
                    case 2:
                        return true;
                    default:
                        userManager.clear_screen();
                        cout << "Invalid choice. Please enter 1 or 2.\n" << endl;
                        break;
                }
            }
        }

    void change_bet(int arr[], int max_num) {
        int index, new_bet;

        while (true) {
            display_bets(arr);

            cout << "\nEnter the position of the bet you want to change (enter 0 to cancel): ";

            if (!(cin >> index)) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
                continue;
            } 
            else if (index == 0) {
                userManager.clear_screen();
                return;  
            } 
            else if (index < 1 || index > 6) {
                userManager.clear_screen(); 
                cout << "Invalid position. Please enter a number between 1 and 6.\n" << endl;
                continue;
            }
            else{
                index--;
                cout << "\nEnter the new bet: ";
                if (!(cin >> new_bet) || new_bet < 0 || new_bet > max_num || !is_unique(arr, 6, new_bet)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    userManager.clear_screen(); 
                    cout << "Invalid bet. Please enter a unique number between 0 and " << max_num << ".\n" << endl;
                    continue;
                }
                arr[index] = new_bet;
                userManager.clear_screen();
                break;
            }
        }
    }

    void display_bets(int arr[]) const {
            cout << "Your bets: " << endl;
            for (int j = 0; j < 6; j++) {
                cout << "[" << j + 1 << "] -- " << arr[j] << endl;
            }
        }

    void get_winning_numbers(int arr_wnums[], int max_num) const {
            cout << "\n\nWINNING NUMBERS:" << endl;

            for (int k = 0; k < 6; k++) {
                arr_wnums[k] = get_unique_num(arr_wnums, k, max_num);
            }
        }

    int get_unique_num(int arr[], int size, int max_num) const {
            int random_num;
            bool unique;
            do {
                random_num = 1 + rand() % max_num;
                unique = is_unique(arr, size, random_num);
            } while (!unique);
            return random_num;
        }

    bool bet_num_matches(int bets[], int nums[]) const {
            int match = 0;

            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 6; j++) {
                    if (bets[i] == nums[j]) {
                        match++;
                        break;
                    }
                }
            }

            return match == 6;
        }

    void main_menu(){
        int choice;

        while (true){
            cout << "+----------------------------------+" << endl;
            cout << "|        Welcome to Lotto!         |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "          !!! Jackpot !!!" << endl;
            cout << "             $" << int(user.getJackpot()) << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Start the Game               |" << endl;
            cout << "| [2] Instructions                 |" << endl;
            cout << "| [3] View Wallet                  |" << endl;
            cout << "| [4] Return to Main Menu          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter you choice: ";

            if (!(cin >> choice)){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    userManager.clear_screen();
                    difficulty_menu();
                    break;
                case 2:
                    userManager.clear_screen();
                    instructions();
                    break;
                case 3:
                    userManager.clear_screen();
                    wallet.view_wallet();
                    break;
                case 4:
                    userManager.clear_screen();
                    return;
                    break;
                default:
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter an option from the menu.\n" << endl;
                    break;
            }
        }
    }
};


class Baccarat
{
private:
    User &user;
    UserManager &userManager;
    Wallet &wallet;

    typedef struct Bets {
    string bet;
    float amount;
    } Bets;

public:
    Baccarat(User &user, UserManager &userManager, Wallet &wallet)
        : user(user), userManager(userManager), wallet(wallet) {}

    void baccarat_menu(){
        int choice;

        while(true){
            cout << "+----------------------------------+" << endl;
            cout << "|        Welcome to Baccarat!      |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Start the Game               |" << endl;
            cout << "| [2] Instructions                 |" << endl;
            cout << "| [3] View Wallet                  |" << endl;
            cout << "| [4] Return to Main Menu          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter you choice: ";

            if (!(cin >> choice)){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    userManager.clear_screen();
                    betting_menu();
                    break;
                case 2:
                    userManager.clear_screen();
                    instructions();
                    break;
                case 3:
                    userManager.clear_screen();
                    wallet.view_wallet();
                    break;
                case 4:
                    userManager.clear_screen();
                    return;
                    break;
                default:
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter an option from the menu.\n" << endl;
                    break;
            }
        }
        }

    void instructions(){
        cout << "+----------------------------------------+" << endl;
        cout << "|              INSTRUCTIONS              |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "\nObjective:\n";
        cout << " - The goal is to bet on the hand that you think will have a total value closest to 9." << endl;

        cout << "\nCard Values:\n";
        cout << " - 2-9: Face value" << endl;
        cout << " - 10, Jack, Queen, King: 0 points" << endl;
        cout << " - Ace: 1 point" << endl;

        cout << "\nGame Play:\n";
        cout << "1. Each round starts with the Player and Banker being dealt two cards each." << endl;
        cout << "2. If either the Player or Banker has a total of 8 or 9, it's called a 'natural' and the game ends." << endl;
        cout << "3. If neither has a natural, the Player and Banker may draw a third card based on specific rules." << endl;

        cout << "\nPlayer Rules:\n";
        cout << " - If the Player's total is 0-5, the Player draws a third card." << endl;
        cout << " - If the Player's total is 6-7, the Player stands." << endl;

        cout << "\nBanker Rules:\n";
        cout << " - If the Player stands, the Banker draws a third card on a total of 0-5 and stands on 6-7." << endl;
        cout << " - If the Player draws a third card, the Banker acts according to the following rules:\n";
        cout << "   - Banker's total 0-2: Draws a card." << endl;
        cout << "   - Banker's total 3: Draws a card unless the Player's third card is an 8." << endl;
        cout << "   - Banker's total 4: Draws a card if the Player's third card is 2-7." << endl;
        cout << "   - Banker's total 5: Draws a card if the Player's third card is 4-7." << endl;
        cout << "   - Banker's total 6: Draws a card if the Player's third card is a 6 or 7." << endl;
        cout << "   - Banker's total 7: Stands." << endl;

        cout << "\nWinning:\n";
        cout << " - The hand closest to 9 wins." << endl;
        cout << " - If both hands have the same total, it is a tie." << endl;

        userManager.press_return();
    }

    void betting_menu(){
        Bets bet;
        int betType = change_bet_type(bet);
        if (betType == 0){
            return;
        } else{
            bet.amount = get_bet_amount();
            if (bet.amount == 0){
                return;
            }
            userManager.clear_screen();
            verify_bet(bet);
        }
        
    }

    int change_bet_type(Bets &bet){
        int choice;
        while (true){
            cout << "+----------------------------------+" << endl;
            cout << "|         Bet on who wins!         | " << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [1] Banker                       |" << endl;
            cout << "| [2] Player                       |" << endl;
            cout << "| [3] Tie                          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [0] Cancel                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "Choose your bet: " ;

            if (!(cin >> choice)){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                userManager.clear_screen();
                cout << "\nInvalid input. Please enter an integer." << endl;
                continue;
            }

            switch (choice) {
                case 0: 
                    userManager.clear_screen();
                    return 0;
                case 1:
                    userManager.clear_screen();
                    bet.bet = "Banker";
                    return 1;
                case 2:
                    userManager.clear_screen();
                    bet.bet = "Player";
                    return 1;
                case 3:
                    userManager.clear_screen();
                    bet.bet = "Tie";
                    return 1;
                default:
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter an option from the menu.\n" << endl;
                    break;
            }
        }
    }

    float get_bet_amount(){
        float bet_amount;
        while (true) {
            cout << "Enter amount you want to bet (enter 0 to cancel): $";

            if (cin >> bet_amount) {
                if (bet_amount == 0) {
                    userManager.clear_screen();
                    return 0 ;
                }
                if (bet_amount > user.getBalance()) {
                    userManager.clear_screen();
                    cout << "Insufficient balance. You only have $" << user.getBalance() << " in your wallet.\n" << endl;
                } else if (bet_amount > 0) {
                    userManager.clear_screen();
                    return bet_amount;
                } else {
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter a positive integer.\n" << endl;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
            }
        }
    }

    void verify_bet(Bets &bet){
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "           "<< bet.bet << " for $" << bet.amount << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [1] Change bet                   |" << endl;
            cout << "| [2] Change bet amount            |" << endl;
            cout << "| [3] Finalize bet                 |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [0] Cancel                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "Choose your bet: " ;

            if (!(cin >> choice)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
                continue;
            }

            switch (choice) {
                case 0:
                    userManager.clear_screen();
                    return;
                case 1:
                    userManager.clear_screen();
                    change_bet_type(bet);
                    break;
                case 2:
                    userManager.clear_screen();
                    change_bet_amount(bet);
                    break;
                case 3:
                    userManager.clear_screen(); 
                    cout << "Your bet of $" << bet.amount << " on " << bet.bet << " is confirmed." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "\nPress Enter to continue.";
                    cin.get();
                    userManager.clear_screen();
                    game_menu(bet);
                    return;
                default:
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter an option from the menu.\n" << endl;
                    break;
            }
        }
    }

    void change_bet_amount(Bets &bet){
        bet.amount = get_bet_amount();
    }

    void game_menu(Bets &bet) {
        srand(static_cast<unsigned int>(time(0)));

        vector<int> player_hand = deal_hand();
        vector<int> banker_hand = deal_hand();

        int player_value = calculate_hand_value(player_hand);
        int banker_value = calculate_hand_value(banker_hand);

        
        display_hand("Player", player_hand);
        display_hand("Banker", banker_hand);

        
        if (player_value <= 5) {
            int player_third_card = rand() % 13 + 1;
            player_hand.push_back(player_third_card);
            player_value = calculate_hand_value(player_hand);
            cout << "Player has added a third card\n" << endl;
            display_hand("Player", player_hand);
        }

    
        if (player_hand.size() == 3) {
            if ((banker_value <= 2) || 
                (banker_value == 3 && player_hand[2] != 8) ||
                (banker_value == 4 && (player_hand[2] >= 2 && player_hand[2] <= 7)) ||
                (banker_value == 5 && (player_hand[2] >= 4 && player_hand[2] <= 7)) ||
                (banker_value == 6 && (player_hand[2] == 6 || player_hand[2] == 7))) {
                int banker_third_card = rand() % 13 + 1;
                banker_hand.push_back(banker_third_card);
                banker_value = calculate_hand_value(banker_hand);
                cout << "Banker has added a third card\n" << endl;
                display_hand("Banker", banker_hand);
            }
        }

        cout << "Press Enter to continue.";
        cin.get();
        userManager.clear_screen();
        determine_winner(bet, player_value, banker_value);
        cout << "\nPress Enter to return.";
        cin.get();
        userManager.clear_screen();
        baccarat_menu();
    }

    vector<int> deal_hand() {
        vector<int> hand;
        for (int i = 0; i < 2; ++i) {
            hand.push_back(rand() % 13 + 1);
        }
        return hand;
    }

    int calculate_hand_value(const vector<int> &hand) {
        int value = 0;
        for (int card : hand) {
            if (card > 9) card = 0;
            else if (card == 1) card = 1;
            value += card;
        }
        return value % 10;
    }

    void display_hand(const string &name, const vector<int> &hand) {
        cout << "------------------------------------------" << endl;
        cout <<  name << " Hand:\t";
        for (int card : hand) {
                cout << "[" << card_name(card) << "] ";
            }
        cout << "\n------------------------------------------" << endl;
        
        cout << "\nValue: " << calculate_hand_value(hand) << "\n" << endl;
    }

    string card_name(int card_value) {
        switch(card_value) {
            case 1: return "Ace";
            case 11: return "Jack";
            case 12: return "Queen";
            case 13: return "King";
            default: return to_string(card_value);
        }
    }

    void determine_winner(Bets &bet, int player_value, int banker_value) {
    
        cout << "+----------------------------------------+" << endl;
        cout << "|                RESULTS                 |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "             " << bet.bet << " for $" << bet.amount  << endl; 
        cout << "+----------------------------------------+" << endl;
        cout << "|    Player value    |   Banker value    |" << endl;
        cout << "|          " << player_value << "         |         " <<  banker_value << "         |" << endl;
        cout << "+----------------------------------------+\n" << endl;

        if (player_value > banker_value) {
            cout << "Winner: Player" << endl;
            if (bet.bet == "Player") {
                user.addBalance(bet.amount);
                cout << "\nCongratualations! You win $" << bet.amount << "!\n";
            } else {
                user.subtractBalance(bet.amount);
                cout << "\nYou lose $" << bet.amount << ". Better luck next time!\n";
            }
        } else if (banker_value > player_value) {
            cout << "Winner: Banker" << endl;
            if (bet.bet == "Banker") {
                user.addBalance(bet.amount * 0.95) ;  
                cout << "\nCongratualations! You win $" << bet.amount * 0.95 << "!\n";
            } else {
                user.subtractBalance(bet.amount);
                cout << "\nYou lose $" << bet.amount << ". Better luck next time!\n";
            }
        } else {
            cout << "It's a tie " << endl;;
            if (bet.bet == "Tie") {
                user.addBalance(bet.amount * 8);
                cout << "\nCongratulations! You win $" << bet.amount * 8 << "!\n";
            } else {
                user.subtractBalance(bet.amount);
                cout << "\nYou lose $" << bet.amount << ". Better luck next time!\n";
            }
        }
    }

};

class Soduko
{
private:
    User &user;
    UserManager &userManager;
    Wallet &wallet;


public:
    Soduko(User &user, UserManager &userManager, Wallet &wallet)
        : user(user), userManager(userManager), wallet(wallet) {}

    void displayNumberWithColor(int number, bool isUserInput) {
        if (isUserInput) {
            cout << ANSI_COLOR_YELLOW << number << ANSI_COLOR_RESET;
        }
        else {
            cout << ANSI_COLOR_BLUE << number << ANSI_COLOR_RESET;
        }
    }

    void soduko_menu() {
        srand(static_cast<unsigned int>(time(0)));
        int choice;
        bool betConfirmed = false;
        int betAmount = 0;

        int board[9][9];
        bool fixed[9][9] = { false };
        bool userInput[9][9] = { false };

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|       Welcome to Sudoku!         |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Start the Game               |" << endl;
            cout << "| [2] Instructions                 |" << endl;
            cout << "| [3] View Wallet                  |" << endl;
            cout << "| [4] Return to Main Menu          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter you choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                while (true) {
                    int betAmount = get_bet_amount();
                    if (betAmount == -1 ){
                        break;
                    }
                    else if (betAmount > 0){
                        int betResult = verify_bet(betAmount);
                        if (betResult == 0){
                            break;
                        }
                        else if (betResult == 2) {
                            puzzles(board, fixed, userInput, betAmount);
                            break;
                        }
                    }
                }
                break;
            case 2:
                userManager.clear_screen();
                instructions();
                break;
            case 3:
                userManager.clear_screen();
                wallet.view_wallet();
                break;
            case 4:
                userManager.clear_screen();
                return;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }

    void instructions() {
        cout << "+----------------------------------------+" << endl;
        cout << "|              INSTRUCTIONS              |" << endl;
        cout << "+----------------------------------------+" << endl;

        cout << "\nObjectives:" << endl;
        cout << " - To fill a 9x9 grid with numbers so that each column, each row, and each of the nine \n" << endl;
        cout << "   3x3 subgrids contain all the numbers from 1 to 9." << endl;

        cout << "\nBasic Rules:" << endl;
        cout << " 1. Grid Layout" << endl;
        cout << "   - The grid is divided into nine 3x3 subgrids." << endl;
        cout << "   - Each row has 9 cells." << endl;
        cout << "   - Each column has 9 cells." << endl;
        cout << " 2. Starting Grid" << endl;
        cout << "   - The puzzle starts with some cells filled with numbers." << endl;
        cout << "   - The difficulty of the puzzle depends on the number and placement of these givens." << endl;
        cout << " 3. Filling the Grid" << endl;
        cout << "   - Each row must contain the numbers 1-9 without repetition." << endl;
        cout << "   - Each column must contain the numbers 1-9 without repetition." << endl;
        cout << "   - Each 3x3 subgrid must contain the numbers 1-9 without repetition." << endl;
        cout << " 4. No Guessing" << endl;
        cout << "   - A valid sudoku puzzle should have only one solution and can be solved using logic alone." << endl;

        cout << "\nMechanics of the game:" << endl;
        cout << " 1. Choose a row" << endl;
        cout << " 2. Choose a column" << endl;
        cout << " 3. Input a number from 1-9" << endl;
        cout << "\nWinning:" << endl;
        cout << " - The game is won when all rows and columns are filled correctly." << endl;

        userManager.press_return();
    }

    int get_bet_amount() {
        int bet_amount = 0;

        while (true) {
            cout << "Enter amount you want to bet (enter 0 to cancel): $";

            if (cin >> bet_amount) {
                if (bet_amount == 0) {
                    userManager.clear_screen();
                    return -1;
                }
                if (bet_amount > user.getBalance()) {
                    userManager.clear_screen();
                    cout << "Insufficience balance. You only have $" << user.getBalance() << " in your wallet.\n" << endl;
                }
                else if (bet_amount > 0) {
                    userManager.clear_screen();
                    return bet_amount;
                }
                else {
                    userManager.clear_screen();
                    cout << "Invalid input. Please enter a positive integer.\n" << endl;
                }
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Invalid input. Please enter an integer.\n" << endl;
            }
        }
    }

    int verify_bet(int bet_amount) {
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|          Verify your bet         |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "               $" << bet_amount << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [1] Change bet                   |" << endl;
            cout << "| [2] Confirm bet                  |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [0] Cancel                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                return 1;
            case 2:
                userManager.clear_screen();
                cout << "Your bet of $" << bet_amount << " has been confirmed." << endl;
                user.subtractBalance(bet_amount);
                userManager.press_return();
                userManager.clear_screen();
                return 2;
            case 0:
                userManager.clear_screen();
                return 0;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                break;
            }
        }
    }

    void puzzles(int board[9][9], bool fixed[9][9], bool userInput[9][9], int betAmount) {
        int randomPuzzle = 1 + rand() % 3;

        int board1[9][9] = {
            {0, 0, 0, 2, 6, 0, 7, 0, 1},
            {6, 8, 0, 0, 7, 0, 0, 9, 0},
            {1, 9, 0, 0, 0, 4, 5, 0, 0},
            {8, 2, 0, 1, 0, 0, 0, 4, 0},
            {0, 0, 4, 6, 0, 2, 9, 0, 0},
            {0, 5, 0, 0, 0, 3, 0, 2, 8},
            {0, 0, 9, 3, 0, 0, 0, 7, 4},
            {0, 4, 0, 0, 5, 0, 0, 3, 6},
            {7, 0, 3, 0, 1, 8, 0, 0, 0}
        };

        int board2[9][9] = {
            {1, 0, 0, 4, 8, 9, 0, 0, 6},
            {7, 3, 0, 0, 0, 0, 0, 4, 0},
            {0, 0, 0, 0, 0, 1, 2, 9, 5},
            {0, 0, 7, 1, 2, 0, 6, 0, 0},
            {5, 0, 0, 7, 0, 3, 0, 0, 8},
            {0, 0, 6, 0, 9, 5, 7, 0, 0},
            {9, 1, 4, 6, 0, 0, 0, 0, 0},
            {0, 2, 0, 0, 0, 0, 0, 3, 7},
            {8, 0, 0, 5, 1, 2, 0, 0, 4}
        };

        int board3[9][9] = {
            {6, 5, 9, 0, 1, 0, 2, 8, 0},
            {1, 0, 0, 0, 5, 0, 0, 3, 0},
            {2, 0, 0, 8, 0, 0, 0, 1, 0},
            {0, 0, 0, 1, 3, 5, 0, 7, 0},
            {8, 0, 0, 9, 0, 0, 0, 0, 2},
            {0, 0, 3, 0, 7, 8, 6, 4, 0},
            {3, 0, 2, 0, 0, 9, 0, 0, 4},
            {0, 0, 0, 0, 0, 1, 8, 0, 0},
            {0, 0, 8, 7, 6, 0, 0, 0, 0}
        };

        switch (randomPuzzle) {
        case 1:
            memcpy(board, board1, sizeof(board1));
            break;
        case 2:
            memcpy(board, board2, sizeof(board2));
            break;
        case 3:
            memcpy(board, board3, sizeof(board3));
            break;
        }
        start_game(board, fixed, userInput, betAmount);
    }

    void start_game(int board[9][9], bool fixed[9][9], bool userInput[9][9], int betAmount) {
        int choice;
        int row, col, num;
        int tries = 3;

        memset(userInput, false, sizeof(bool) * 9 * 9);

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] != 0) {
                    fixed[i][j] = true;
                }
            }
        }

        while (true) {
            sudoku_board(board, userInput);

            if (!get_user_input(row, col, num)) {
                continue;
            }

            if (row == -1 || col == -1 || num == -1) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nYou just forfeited $" << betAmount << ". Press Enter to return.";
                cin.get();
                userManager.clear_screen();
                break;
            }

            if (row >= 0 && row < 9 && col >= 0 && col < 9 && num >= 1 && num <= 9) {
                if (!fixed[row][col]) {
                    board[row][col] = num;
                    userInput[row][col] = true;
                    userManager.clear_screen();
                } else {
                    userManager.clear_screen();
                    cout << "You cannot change the initially given number.\n" << endl;
                }
            } else {
                userManager.clear_screen();
                cout << "\nInvalid input.\n" << endl;
            }

            if (board_complete(board)) {
                sudoku_board(board, userInput);
                cout << "\n[1] Submit Board [2] Continue Editting: ";
                
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    userManager.clear_screen();
                    cout << "Please enter a valid choice from the menu.\n" << endl;
                    continue;
                    }

                if (choice == 1) {
                    if (solution_checker(board)) {
                        float winnings = bet_multiplier(tries, betAmount);
                        cout << "\nYou have solved the puzzle!" << endl;
                        cout << "You have won $" << winnings << endl;
                        break;
                    } else {
                        cout << "\nPuzzle not solved." << endl;
                        tries--;
                        if (tries <= 0) {
                            cout << "You have reached the maximum amount of tries. $" << betAmount << " will be deducted from your wallet." << endl;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "\nPress 'enter' to return to menu.";
                            cin.get();
                            userManager.clear_screen();
                            user.subtractBalance(betAmount);
                            break;
                        } else {
                            if (tries == 1) {
                                cout << "You only have " << tries << " try left to solve the puzzle." << endl;
                            } else {
                                cout << "You only have " << tries << " tries left to solve the puzzle." << endl;
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "\nPress 'enter' to continue playing.";
                            cin.get();
                            userManager.clear_screen();
                        }
                    }
                } else if (choice == 2) {
                    userManager.clear_screen();
                } else {
                    cout << "Invalid choice." << endl;
                }
            }
        }
    }

    bool get_user_input(int& row, int& col, int& num) {
        cout << "\nEnter row (1-9) or -1 to exit: ";
        if (!(cin >> row)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            userManager.clear_screen();
            cout << "Invalid input. Please only enter numbers from 1-9.\n" << endl;
            return false;
        }

        if (row == -1) return true;

        if (row < 1 || row > 9) {
            userManager.clear_screen();
            cout << "Row out of bounds. Please enter a number from 1-9.\n" << endl;
            return false;
        }

        cout << "Enter column (1-9) or -1 to exit: ";
        if (!(cin >> col)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            userManager.clear_screen();
            cout << "Invalid input. Please only enter numbers from 1-9.\n" << endl;
            return false;
        }

        if (col == -1) return true;

        if (col < 1 || col > 9) {
            userManager.clear_screen();
            cout << "Column out of bounds. Please enter a number from 1-9.\n" << endl;
            return false;
        }

        cout << "Enter number (1-9) or -1 to exit: ";
        if (!(cin >> num)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            userManager.clear_screen();
            cout << "Invalid input. Please only enter numbers from 1-9.\n" << endl;
            return false;
        }

        if (num == -1) return true;

        if (num < 1 || num > 9) {
            userManager.clear_screen();
            cout << "Number out of bounds. Please enter a number from 1-9.\n" << endl;
            return false;
        }

        row--;
        col--;
        return true;
    }

    void sudoku_board(int grid[9][9], bool userInput[9][9]) {
        cout << "   ";
        for (int col = 0; col < 9; col++) {
            cout << col + 1 << " ";
            if (col == 2 || col == 5)
                cout << "| ";
        }
        cout << endl;
        cout << "  ----------------------\n";

        for (int row = 0; row < 9; row++) {
            cout << row + 1 << "| ";
            for (int col = 0; col < 9; col++) {
                if (grid[row][col] != 0) {
                    displayNumberWithColor(grid[row][col], userInput[row][col]);
                    cout << " ";
                }
                else {
                    cout << grid[row][col] << " ";
                }
                if (col == 2 || col == 5)
                    cout << "| ";
            }
            cout << endl;

            if (row == 2 || row == 5) {
                cout << "  ----------------------" << endl;
            }
        }
        cout << ANSI_COLOR_RESET;
    }   

    bool board_complete(int board[9][9]) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool solution_checker(int board[9][9]) {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                if (board[row][col] != 0) {
                    int num = board[row][col];
                    board[row][col] = 0;
                    if (!is_number_valid(board, row, col, num)) {
                        board[row][col] = num;
                        return false;
                    }
                    board[row][col] = num;
                }
            }
        }
        return true;
    }

    bool is_number_valid(int grid[9][9], int row, int col, int num) {
        for (int x = 0; x < 9; x++)
            if (grid[row][x] == num)
                return false;

        for (int x = 0; x < 9; x++)
            if (grid[x][col] == num)
                return false;

        int startRow = row - row % 3, startCol = col - col % 3;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (grid[i + startRow][j + startCol] == num)
                    return false;

        return true;
    }

    float bet_multiplier(int tries, int betAmount) {
        float multiplier;

        if (tries == 0) {
            multiplier = 2.5;
        }
        else if (tries == 1) {
            multiplier = 2.0;
        }
        else {
            multiplier = 1.5;
        }

        float winnings = betAmount * multiplier;
        user.addBalance(winnings);

        return winnings;
    }
};


class Casino {
private:
    User& user; 
    UserManager& userManager; 
    Wallet wallet; 
    Lotto lotto; 
    Baccarat baccarat;
    Soduko soduko;

public:
    Casino(User& user, UserManager& userManager)
        : user(user), userManager(userManager), wallet(user), lotto(user, userManager, wallet), baccarat(user, userManager, wallet), soduko(user, userManager, wallet) {}

    void sign_up() {
        cout << "+----------------------------------+" << endl;
        cout << "|              Sign-Up             |" << endl;
        cout << "+----------------------------------+" << endl;

        string username;
        while (true) {
            username = userManager.get_username_input();
            if (!userManager.username_exists(username)) break;
            cout << "Username already exists. Please choose a different username.\n";
        }
        user.setUsername(username);

        string password = userManager.get_confirmed_password();
        user.setPassword(password);

        user.save_user();

        userManager.clear_screen();
        cout << "Sign-up successful! Welcome to \"What Are The Odds?\" " << username << "!" << endl;
        userManager.press_return();
    }

    void log_in() {
        cout << "+----------------------------------+" << endl;
        cout << "|              Log-In              |" << endl;
        cout << "+----------------------------------+" << endl;

        string username = userManager.get_username_input();
        cout << "Enter password: ";
        string password = userManager.get_password_input();

        if (user.load_user(username, password)) {
            userManager.clear_screen();
            cout << "Login successful! Welcome back " << username << "!" << endl;
            userManager.press_return();
            game_menu();
        } else {
            userManager.clear_screen();
            cout << "Invalid username or password. Please try again." << endl;
            userManager.press_return();
        }
    }

    void main_menu() {
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|       What Are The Odds?         |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Sign Up                      |" << endl;
            cout << "| [2] Log In                       |" << endl;
            cout << "| [3] Exit                         |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                sign_up();
                break;
            case 2:
                userManager.clear_screen();
                log_in();
                break;
            case 3:
                userManager.clear_screen();
                exit(0);
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }

    void game_menu(){
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|       What Are The Odds?         |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Luck Games                   |" << endl;
            cout << "| [2] Card Games                   |" << endl;
            cout << "| [3] Logic Games                  |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [4] Wallet                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [5] Log-Out                      |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                luck_games_menu();
                break;
            case 2:
                userManager.clear_screen();
                card_games_menu();
                break;
            case 3:
                userManager.clear_screen();
                logic_games_menu();
                break;
            case 4:
                userManager.clear_screen();
                wallet.view_wallet();
                break;
            case 5:
                userManager.clear_screen();
                main_menu();
                break;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }

    void luck_games_menu(){
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|            Luck Games            |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Slot                         |" << endl;
            cout << "| [2] Color Game                   |" << endl;
            cout << "| [3] Lotto                        |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [4] Wallet                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [5] Return to main menu          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                Slot.SlotMenu();
                break;
            case 2:
                userManager.clear_screen();
                //color game
                break;
            case 3:
                userManager.clear_screen();
                lotto.main_menu();
                break;
            case 4:
                userManager.clear_screen();
                wallet.view_wallet();
                break;
            case 5:
                userManager.clear_screen();
                game_menu();
                break;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }

    void card_games_menu(){
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|            Card Games            |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] BlackJack                    |" << endl;
            cout << "| [2] Baccarat                     |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [3] Wallet                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [4] Return to main menu          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                //blackjack
                break;
            case 2:
                userManager.clear_screen();
                baccarat.baccarat_menu();
                break;
            case 3:
                userManager.clear_screen();
                wallet.view_wallet();
                break;
            case 4:
                userManager.clear_screen();
                game_menu();
                break;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }

    void logic_games_menu(){
        int choice;

        while (true) {
            cout << "+----------------------------------+" << endl;
            cout << "|           Logic Games            |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| Select an option:                |" << endl;
            cout << "| [1] Soduko                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [2] Wallet                       |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << "| [3] Return to main menu          |" << endl;
            cout << "+----------------------------------+" << endl;
            cout << " Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
                continue;
            }

            switch (choice) {
            case 1:
                userManager.clear_screen();
                soduko.soduko_menu();
                break;
            case 2:
                userManager.clear_screen();
                wallet.view_wallet();
                break;
            case 3:
                userManager.clear_screen();
                game_menu();
                break;
            default:
                userManager.clear_screen();
                cout << "Please enter a valid choice from the menu.\n" << endl;
            }
        }
    }
};

int main() {
    User user; 
    UserManager userManager; 

    Casino casino(user, userManager); 

    casino.main_menu();
    return 0;
}
