#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    int copies;

public:
    Book(const std::string& t, const std::string& a, const std::string& i, int c)
        : title(t), author(a), isbn(i), copies(c) {
        if (c < 0) {
            throw std::invalid_argument("Number of copies cannot be negative: " + std::to_string(c));
        }
    }

    Book(const std::string& t)
        : Book(t, "Unknown Author", "ISBN-PENDING", 1) {}

    Book()
        : Book("Untitled Record", "Unknown Author", "000-0-00-000000-0", 0) {}

    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getIsbn() const { return isbn; }
    int getCopies() const { return copies; }

    void setCopies(int c) {
        if (c < 0) throw std::invalid_argument("Copies cannot be negative.");
        copies = c;
    }

    void display() const {
        std::cout << "Title: \"" << title << "\", Author: " << author
                  << ", ISBN: " << isbn << ", Copies: " << copies << "\n";
    }
};

// Class demonstrating explicit constructor requirement
class ExplicitBookID {
    int id;
public:
    explicit ExplicitBookID(int val) : id(val) {}
    int getId() const { return id; }
};

void printId(const ExplicitBookID& b) {
    std::cout << "Book ID: " << b.getId() << "\n";
}

int main() {
        // 1. Create Book objects with every constructor and display
    std::cout << "[Test 1] Creating Book objects with all 3 constructors:\n";
    Book b1("Clean Architecture", "Robert C. Martin", "978-0134494166", 5); // Full details
    Book b2("Introduction to Algorithms");                                  // Donated book (delegates)
    Book b3;                                                                // Empty record (delegates)

    std::cout << "  b1 (Full):    "; b1.display();
    std::cout << "  b2 (Donated): "; b2.display();
    std::cout << "  b3 (Default): "; b3.display();

    // 2. Test negative number of copies validation
    std::cout << "\n[Test 2] Testing invalid (negative) copies validation:\n";
    try {
        std::cout << "  Attempting to create Book with -3 copies...\n";
        Book invalidBook("Corrupted Record", "Jane Doe", "000-000", -3);
        invalidBook.display();
    } catch (const std::invalid_argument& e) {
        std::cout << "  Successfully caught expected exception: " << e.what() << "\n";
    }

    // 3. Array of Books (demonstrates requirement for default constructor)
    std::cout << "\n[Test 3] Allocating an array of 3 Book objects:\n";
    Book catalog[3]; // Calls default constructor 3 times
    catalog[0] = Book("Operating Systems", "Silberschatz", "978-1118063330", 3);
    catalog[1] = Book("Database System Concepts", "Korth", "978-0073523323", 4);
    for (int i = 0; i < 3; ++i) {
        std::cout << "  catalog[" << i << "]: ";
        catalog[i].display();
    }

    // 4. Explicit constructor demonstration
    std::cout << "\n[Test 4] Demonstrating 'explicit' constructor:\n";
    ExplicitBookID validId(101);
    printId(validId);
    // printId(202); // Compile error: no known conversion from 'int' to 'ExplicitBookID'
    std::cout << "  Note: Passing raw integer (e.g. printId(202)) fails compilation when explicit is specified.\n";
    return 0;
}
