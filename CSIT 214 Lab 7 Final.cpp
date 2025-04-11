#include <iostream>     // For input/output operations
#include <string>       // For using std::string
#include <vector>       // For using std::vector
#include <ctime>        // For handling dates and time
#include <limits>       // For numeric_limits (used with cin.ignore)

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

// Book class represents a book in the library
class Book {
public:
    string Title;
    string Author;
    string ISBN;
};

// Member class represents a library member
class Member {
public:
    string Name;
    string PhoneNumber;
    string MembershipID;
};

// Transaction class represents a book borrowing record
class Transaction {
public:
    Member member;      // The member borrowing the book
    Book book;          // The borrowed book
    string DueDate;     // Due date in YYYY-MM-DD format
    bool Returned = false;  // Status of return

    double calculateLateFee() const;     // Calculates late fee if any
    static const double dailyLateFee;    // Daily late fee rate
};

const double Transaction::dailyLateFee = 0.50;

// Calculates how late the book is and applies a fee if returned late
double Transaction::calculateLateFee() const {
    if (Returned) return 0.0;

    time_t now = time(0);
    tm* currentDate = localtime(&now);

    int dueYear, dueMonth, dueDay;
    sscanf(DueDate.c_str(), "%d-%d-%d", &dueYear, &dueMonth, &dueDay);

    tm dueTM = {0};
    dueTM.tm_year = dueYear - 1900;
    dueTM.tm_mon = dueMonth - 1;
    dueTM.tm_mday = dueDay;
    time_t dueTime = mktime(&dueTM);

    double secondsLate = difftime(now, dueTime);
    int daysLate = static_cast<int>(secondsLate / (60 * 60 * 24));

    return daysLate > 0 ? daysLate * dailyLateFee : 0.0;
}

// Adds books to the library vector from user input
void CreateBook(vector<Book>& library) {
    char response;

    while (true) {
        cout << "Would you like to add a book to the library? (y/n) " << endl;
        cin >> response;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (response == 'y' || response == 'Y') {
            Book NewBook;
            cout << "Please enter name of book " << endl;
            std::getline(cin, NewBook.Title);
            cout << "Please enter Author's name " << endl;
            std::getline(cin, NewBook.Author);
            cout << "Please enter ISBN " << endl;
            std::getline(cin, NewBook.ISBN);
            library.push_back(NewBook);
            cout << "\n=== Books in Library ===\n";
            continue;
        }
        else if (response == 'n' || response == 'N') {
            break;
        }
        else {
            cout << "Invalid input, please enter y/Y or n/N " << endl;
        }
    }
}

// Registers new members to the library
void RegisterMember(vector<Member>& members) {
    char response;

    while(true) {
        cout << "Would you like to add a member to the members list? (y/n) " << endl;
        cin >> response;
        cin.ignore();

        if (response == 'y' || response == 'Y') {
            Member NewMember;
            cout << "Please enter member's name " << endl;
            getline(cin, NewMember.Name);
            cout << "Please enter member's phone number (no dashes) " << endl;
            getline(cin, NewMember.PhoneNumber);
            cout << "Please enter Membership ID " << endl;
            getline(cin, NewMember.MembershipID);
            members.push_back(NewMember);
            continue;
        }
        else if (response == 'n' || response == 'N')  {
            break;
        }
        else {
            cout << "Invalid input, please enter y/Y or n/N" << endl;
        }
    }
}

// Handles the borrowing process between a member and a book
void BookTransaction(vector<Transaction>& transactions, const vector<Member>& members, const vector<Book>& books) {
    char response;

    while(true) {
        cout << "Would you like to make a transaction? (y/n) " << endl;
        cin >> response;
        cin.ignore();

        if (response == 'y' || response == 'Y') {
            Transaction NewTransaction;
            cout << "Available members: " << endl;
            for (int i =0; i < members.size(); i++) {
                cout << i+1 << ".";
                cout << members[i].Name << endl;
            }
            cout << "Please select member (Number): " << endl;
            int MemberChoice;
            cin >> MemberChoice;
            NewTransaction.member = members[MemberChoice - 1];

            cout << "Available books: " << endl;
            for (int SizeTransactions = 0; SizeTransactions < books.size(); SizeTransactions++) {
                cout << SizeTransactions + 1 << ".";
                cout << books[SizeTransactions].Title << " Written by " << books[SizeTransactions].Author << endl;
            }

            cout << "Select book (number): ";
            int bookChoice;
            cin >> bookChoice;
            cin.ignore();
            NewTransaction.book = books[bookChoice-1];

            cout << "Enter due date (YYYY-MM-DD): ";
            getline(cin, NewTransaction.DueDate);

            transactions.push_back(NewTransaction);
        }
        else if (response == 'n' || response == 'N') {
            break;
        }
        else {
            cout << "Invalid input, please enter y/Y or n/N" << endl;
        }
    }
}

// Handles returning a borrowed book and calculates late fees
void ReturnBook(vector<Transaction>& transactions) {
    if (transactions.empty()) {
        cout << "No borrowings to return.\n";
        return;
    }

    cout << "\n=== Return a Book ===\n";
    for (size_t i = 0; i < transactions.size(); i++) {
        if (!transactions[i].Returned) {
            cout << i+1 << ". " << transactions[i].book.Title 
                 << " borrowed by " << transactions[i].member.Name
                 << " (Due: " << transactions[i].DueDate << ")";
            
            double fee = transactions[i].calculateLateFee();
            if (fee > 0) cout << " - Late fee: $" << fee;
            cout << endl;
        }
    }

    int choice;
    cout << "Enter borrowing number to mark as returned: ";
    cin >> choice;
    cin.ignore();

    if (choice > 0 && choice <= transactions.size()) {
        transactions[choice-1].Returned = true;

        double fee = transactions[choice-1].calculateLateFee();
        if (fee > 0) {
            cout << "Late fee of $" << fee << " applied.\n";
        }
        cout << "Book marked as returned.\n";
    } else {
        cout << "Invalid selection.\n";
    }
}

// Displays all books currently in the library
void DisplayAllBooks(const vector<Book>& library) {
    if (library.empty()) {
        cout << "The library has no books yet.\n";
        return;
    }

    cout << "\n=== Books in Library ===\n";
    for (const auto& book : library) {
        cout << "Title: " << book.Title << endl
             << "Author: " << book.Author << endl
             << "ISBN: " << book.ISBN << endl
             << "----------------------\n";
    }
}

// Displays all registered library members
void DisplayAllMembers(const vector<Member>& members) {
    if (members.empty()) {
        cout << "The library has no members yet.\n";
        return;
    }

    cout << "\n=== Members in Library ===\n";
    for (const auto& member : members) {
        cout << "Name: " << member.Name << endl
             << "Phone: " << member.PhoneNumber << endl
             << "Membership ID: " << member.MembershipID << endl
             << "----------------------\n";
    }
}

// Allows users to search for books by title, author, or ISBN
void SearchBooks(const vector<Book>& library) {
    if (library.empty()) {
        cout << "The library has no books yet.\n";
        return;
    }

    cout << "\n=== Search Books ===\n"
         << "1. Search by Title\n"
         << "2. Search by Author\n"
         << "3. Search by ISBN\n"
         << "Enter choice (1-3): ";

    int choice;
    cin >> choice;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string searchTerm;
    cout << "Enter search term: ";
    getline(cin, searchTerm);

    cout << "\n=== Search Results ===\n";
    bool found = false;

    for (const auto& book : library) {
        bool match = false;

        switch(choice) {
            case 1: match = (book.Title == searchTerm); break;
            case 2: match = (book.Author == searchTerm); break;
            case 3: match = (book.ISBN == searchTerm); break;
            default: cout << "Invalid choice.\n"; return;
        }

        if (match) {
            cout << "Title: " << book.Title << endl
                 << "Author: " << book.Author << endl
                 << "ISBN: " << book.ISBN << endl
                 << "----------------------\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No matching books found.\n";
    }
}

// Entry point of the program, displays the main menu and handles user choices
int main() {
    vector<Book> library;
    vector<Member> members;
    vector<Transaction> transactions;

    int choice;
    do {
        cout << "\n=== Library Management System ===" << endl;
        cout << "1. Add Books" << endl;
        cout << "2. Register Members" << endl;
        cout << "3. Borrow Books" << endl;
        cout << "4. Return Books" << endl;
        cout << "5. Display All Books" << endl;
        cout << "6. Display All Members" << endl;
        cout << "7. Search Books" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1: CreateBook(library); break;
            case 2: RegisterMember(members); break;
            case 3: BookTransaction(transactions, members, library); break;
            case 4: ReturnBook(transactions); break;
            case 5: DisplayAllBooks(library); break;
            case 6: DisplayAllMembers(members); break;
            case 7: SearchBooks(library); break;
            case 8: cout << "Exiting program. Goodbye!" << endl; break;
            default: cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
