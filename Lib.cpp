#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

// Structure to represent a Book
struct Book
{
    int id;
    string title;
    string author;
    bool isAvailable;
    Book(int i, string t, string a) : id(i), title(t), author(a), isAvailable(true) {}
};

// Node for Linked List (to handle book checkouts)
struct Node
{
    Book* book;
    Node* next;
    Node(Book* b) : book(b), next(nullptr) {}
};

// Binary Search Tree for Book Management
class BookBST 
{
private:
    struct TreeNode 
    {
        Book* book;
        TreeNode* left;
        TreeNode* right;
        TreeNode(Book* b) : book(b), left(nullptr), right(nullptr) {}
    };
    TreeNode* root;
    void insert(TreeNode*& node, Book* book) 
    {
        if (node == nullptr) 
        {
            node = new TreeNode(book);
        } else if (book->id < node->book->id) 
        {
            insert(node->left, book);
        } else 
        {
            insert(node->right, book);
        }
    }

    void inorder(TreeNode* node) 
    {
        if (node) 
        {
            inorder(node->left);
            cout << "ID: " << node->book->id << ", Title: " << node->book->title << ", Author: " << node->book->author << ", Available: " << (node->book->isAvailable ? "Yes" : "No") << endl;
            inorder(node->right);
        }
    }

    TreeNode* search(TreeNode* node, int id) 
    {
        if (node == nullptr || node->book->id == id) return node;
        if (id < node->book->id) return search(node->left, id);
        return search(node->right, id);
    }

public:
    BookBST() : root(nullptr) {}
    void addBook(Book* book) 
    {
        insert(root, book);
    }

    void displayBooks() 
    {
        inorder(root);
    }

    Book* findBook(int id) 
    {
        TreeNode* result = search(root, id);
        return result ? result->book : nullptr;
    }
};

// Hash Table for User Management
class UserHashTable 
{
private:
    struct User 
    {
        int id;
        string name;
        vector<Book*> borrowedBooks;
        User(int i, string n) : id(i), name(n) {}
    };

    unordered_map<int, User*> users;
public:
    void addUser(int id, string name) 
    {
        if (users.find(id) == users.end()) 
        {
            users[id] = new User(id, name);
        }
    }

    User* getUser(int id) 
    {
        if (users.find(id) != users.end()) 
        {
            return users[id];
        }
        return nullptr;
    }

    void borrowBook(int userId, Book* book) 
    {
        if (users.find(userId) != users.end() && book && book->isAvailable) 
        {   users[userId]->borrowedBooks.push_back(book);
            book->isAvailable = false;
        }
    }

    void returnBook(int userId, int bookId) 
    {
        if (users.find(userId) != users.end()) 
        {
            auto& books = users[userId]->borrowedBooks;
            for (auto it = books.begin(); it != books.end(); ++it) 
            {
                if ((*it)->id == bookId) 
                {
                    (*it)->isAvailable = true;
                    books.erase(it);
                    break;
                }
            }
        }
    }

    void displayUser(int userId) 
    {
        if (users.find(userId) != users.end()) 
        {
            User* user = users[userId];
            cout << "User ID: " << user->id << ", Name: " << user->name << endl;
            cout << "Borrowed Books:" << endl;
            for (Book* book : user->borrowedBooks) 
            {
                cout << "  ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author << endl;
            }
        } else 
        {
            cout << "User not found." << endl;
        }
    }
};

// Queue for book requests
class BookRequestQueue 
{
private:
    queue<pair<int, int>> requests; // pair of userId and bookId

public:
    void addRequest(int userId, int bookId) 
    {
        requests.push({userId, bookId});
    }

    pair<int, int> processRequest() 
    {
        if (!requests.empty()) 
        {
            pair<int, int> request = requests.front();
            requests.pop();
            return request;
        }
        return {-1, -1}; // No requests
    }
};

// Library Management System
class LibrarySystem 
{
private:
    BookBST bookCollection;
    UserHashTable userTable;
    BookRequestQueue requestQueue;
    int bookIdCounter;
    int userIdCounter;

public:
    LibrarySystem() : bookIdCounter(1), userIdCounter(1) {}

    void addBook(string title, string author) 
    {
        Book* newBook = new Book(bookIdCounter++, title, author);
        bookCollection.addBook(newBook);
    }

    void addUser(string name) 
    {
        userTable.addUser(userIdCounter++, name);
    }

    void displayBooks() 
    {
        bookCollection.displayBooks();
    }

    void displayUser(int userId) 
    {
        userTable.displayUser(userId);
    }

    void borrowBook(int userId, int bookId) 
    {
        Book* book = bookCollection.findBook(bookId);
        userTable.borrowBook(userId, book);
    }

    void returnBook(int userId, int bookId) 
    {
        userTable.returnBook(userId, bookId);
    }

    void requestBook(int userId, int bookId) 
    {
        requestQueue.addRequest(userId, bookId);
    }

    void processRequests() 
    {
        pair<int, int> request;
        while ((request = requestQueue.processRequest()).first != -1) 
        {
            cout << "Processing request: User " << request.first << " for Book " << request.second << endl;
            borrowBook(request.first, request.second);
        }
    }
};

int main() 
{
    LibrarySystem library;
    library.addBook("The Great Gatsby", "F. Scott Fitzgerald");
    library.addBook("1984", "George Orwell");
    library.addBook("To Kill a Mockingbird", "Harper Lee");
    library.addUser("Alice");
    library.addUser("Bob");

    cout << "Displaying all books:" << endl;
    library.displayBooks();

    cout << "Displaying user Alice:" << endl;
    library.displayUser(1);

    cout << "Alice borrows '1984':" << endl;
    library.borrowBook(1, 2);

    cout << "Displaying user Alice after borrowing a book:" << endl;
    library.displayUser(1);

    cout << "Alice returns '1984':" << endl;
    library.returnBook(1, 2);

    cout << "Displaying user Alice after returning the book:" << endl;
    library.displayUser(1);

    cout << "Requesting books:" << endl;
    library.requestBook(1, 2);
    library.requestBook(2, 3);

    cout << "Processing requests:" << endl;
    library.processRequests();

    cout << "Displaying all books after processing requests:" << endl;
    library.displayBooks();
    return 0;
}
