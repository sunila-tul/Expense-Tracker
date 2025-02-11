#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
using namespace std;

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

class Expense {
public:
    string category;
    double amount;
    string date; // Format: YYYY-MM-DD

    Expense(string cat, double amt, string dt) : category(cat), amount(amt), date(dt) {}
    Expense() {}

    void display() const {
        cout << "Category: " << category << " | Amount: $" << fixed << setprecision(2) << amount << " | Date: " << date << endl;
    }
};

class User {
public:
    string username;
    string password;

    User(string usr, string pwd) : username(usr), password(pwd) {}
    User() {}
};

class ExpenseTracker {
private:
    vector<Expense> expenses;
    vector<User> users;
    string loggedInUser;
    double budgetLimit = 0.0;

public:
    void clearScreen() {
        system(CLEAR_SCREEN);
    }

    void saveUsersToFile() {
        ofstream file("users.txt");
        for (const auto &user : users) {
            file << user.username << " " << user.password << endl;
        }
        file.close();
    }

    void loadUsersFromFile() {
        ifstream file("users.txt");
        if (!file) return;
        User user;
        while (file >> user.username >> user.password) {
            users.push_back(user);
        }
        file.close();
    }

    void loadExpensesFromFile() {
        ifstream file("expenses.txt");
        if (!file) return;
        Expense exp;
        while (file >> exp.category >> exp.amount >> exp.date) {
            expenses.push_back(exp);
        }
        file.close();
    }

    bool signUp() {
        clearScreen();
        User newUser;
        cout << "Enter a username: ";
        cin >> newUser.username;
        cout << "Enter a password: ";
        cin >> newUser.password;
        
        for (const auto &user : users) {
            if (user.username == newUser.username) {
                cout << "Username already exists! Try a different one.\n";
                return false;
            }
        }
        
        users.push_back(newUser);
        saveUsersToFile();
        cout << "User registered successfully!\n";
        return true;
    }

    bool login() {
        clearScreen();
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        
        for (const auto &user : users) {
            if (user.username == username && user.password == password) {
                loggedInUser = username;
                cout << "Login successful!\n";
                return true;
            }
        }
        cout << "Invalid credentials. Try again.\n";
        return false;
    }

    void addExpense() {
        clearScreen();
        if (loggedInUser.empty()) {
            cout << "You must be logged in to add expenses.\n";
            return;
        }
        string category, date;
        double amount;
        cout << "Enter category: ";
        cin >> category;
        cout << "Enter amount: ";
        cin >> amount;
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> date;
        expenses.push_back(Expense(category, amount, date));
        cout << "Expense added successfully!\n";
    }

   
    void deleteExpense() {
    clearScreen();
    if (expenses.empty()) {
        cout << "No expenses recorded.\n";
        cout << "\nPress Enter to return to the menu...";
        cin.ignore(); // Ignore any leftover newlines
        cin.get();    // Wait for Enter key
        return;
    }

    int index;
    cout << "\n--- Expense List ---\n";
    for (size_t i = 0; i < expenses.size(); ++i) {
        cout << i + 1 << ". ";
        expenses[i].display();
    }

    cout << "Enter the expense number to delete (starting from 1): ";
    cin >> index;
    if (index < 1 || index > expenses.size()) {
        cout << "Invalid index!\n";
    } else {
        expenses.erase(expenses.begin() + index - 1);
        cout << "Expense deleted successfully!\n";
    }

    cout << "\nPress Enter to return to the menu...";
    cin.ignore(); // Ignore any leftover newlines
    cin.get();    // Wait for Enter key
}

    void viewExpenses() {
        clearScreen();
        if (expenses.empty()) {
            cout << "No expenses recorded.\n";
        } else {
            cout << "\n--- Expense List ---\n";
            for (size_t i = 0; i < expenses.size(); ++i) {
                cout << i + 1 << ". ";
                expenses[i].display();
            }
        }
        cout << "\nPress Enter to return to the menu...";
        cin.ignore(); // Ignore any leftover newlines
        cin.get();    // Wait for Enter key
    }

    void setBudgetLimit() {
        clearScreen();
        cout << "Enter your budget limit: $";
        cin >> budgetLimit;
        cout << "Budget limit set to $" << fixed << setprecision(2) << budgetLimit << "\n";
    }

    void showTotalExpenses() {
        clearScreen();
        double total = 0.0;
        for (const auto &exp : expenses) {
            total += exp.amount;
        }
        cout << "Total Expenses: $" << fixed << setprecision(2) << total << "\n";
        if (budgetLimit > 0 && total > budgetLimit) {
            cout << "Warning: You have exceeded your budget amount by $" << total-budgetLimit << "!\n";
        }
        cout << "\nPress Enter to return to the menu...";
    cin.ignore(); // Ignore any leftover newlines
    cin.get(); 
    }
};

int main() {
    ExpenseTracker tracker;
    tracker.loadUsersFromFile();
    tracker.loadExpensesFromFile();
    int choice;
    while (true) {
        tracker.clearScreen();
        cout << "\n1. Sign Up\n2. Login\n3. Exit\nEnter choice: ";
        cin >> choice;
        if (choice == 1) tracker.signUp();
        else if (choice == 2 && tracker.login()) break;
        else if (choice == 3) return 0;
    }
    do {
        tracker.clearScreen();
        cout << "\nExpense Tracker Menu:\n";
        cout << "1. Add Expense\n";
        cout << "2. View Expenses\n";
        cout << "3. Delete Expense\n";
        cout << "4. Show Total Expenses\n";
        cout << "5. Set Budget Limit\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: tracker.addExpense(); break;
            case 2: tracker.viewExpenses(); break;
            case 3: tracker.deleteExpense(); break;
            case 4: tracker.showTotalExpenses(); break;
            case 5: tracker.setBudgetLimit(); break;
            case 6: cout << "Exiting program.\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (true);
    return 0;

}


