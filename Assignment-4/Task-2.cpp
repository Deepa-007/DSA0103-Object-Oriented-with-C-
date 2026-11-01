
#include <iostream>
#include <string>

// Common Virtual Base Class
class Person {
protected:
    std::string name;
    int id;
    std::string phone;

public:
    Person(const std::string& n = "Unknown", int i = 0, const std::string& p = "0000000000")
        : name(n), id(i), phone(p) {
        std::cout << "Person Constructor called for: " << name << "\n";
    }

    virtual ~Person() {
        std::cout << "Person Destructor called for: " << name << "\n";
    }

    virtual void display() const {
        std::cout << "Person [ID: " << id << ", Name: " << name << ", Phone: " << phone << "]";
    }
};

// Derived Class 1: Virtual inheritance to resolve diamond
class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;

public:
    Student(const std::string& n, int i, const std::string& p,
            const std::string& prog, double g)
        : Person(n, i, p), programme(prog), cgpa(g) {
        std::cout << "Student Constructor called\n";
    }

    ~Student() override {
        std::cout << "Student Destructor called\n";
    }

    void display() const override {
        Person::display();
        std::cout << ", Programme: " << programme << ", CGPA: " << cgpa;
    }
};

// Derived Class 2: Virtual inheritance
class Staff : virtual public Person {
protected:
    std::string department;
    double salary;

public:
    Staff(const std::string& n, int i, const std::string& p,
          const std::string& dept, double sal)
        : Person(n, i, p), department(dept), salary(sal) {
        std::cout << "Staff Constructor called\n";
    }

    ~Staff() override {
        std::cout << "Staff Destructor called\n";
    }

    void display() const override {
        Person::display();
        std::cout << ", Department: " << department << ", Salary: Rs." << salary;
    }
};

// Most Derived Class: Multiple Inheritance with Diamond Resolved
class TeachingAssistant : public Student, public Staff {
private:
    int hoursPerWeek;

public:
    // Most derived class MUST initialize virtual base Person directly!
    TeachingAssistant(const std::string& n, int i, const std::string& p,
                      const std::string& prog, double g,
                      const std::string& dept, double sal, int hrs)
        : Person(n, i, p), // Direct call to virtual base
          Student(n, i, p, prog, g),
          Staff(n, i, p, dept, sal),
          hoursPerWeek(hrs) {
        std::cout << "TeachingAssistant Constructor called\n";
    }

    ~TeachingAssistant() override {
        std::cout << "TeachingAssistant Destructor called\n";
    }

    void display() const override {
        std::cout << "TA Profile -> ";
        Student::display();
        std::cout << ", Dept: " << department << ", TA Hours: " << hoursPerWeek << "\n";
    }
};

// Predict Output Structs from Task 2
namespace PredictSnippet {
    struct Person { Person() { std::cout << "Person "; } };
    struct Student : virtual Person { Student() { std::cout << "Student "; } };
    struct Staff : virtual Person { Staff() { std::cout << "Staff "; } };
    struct TeachingAssistant : Student, Staff {
        TeachingAssistant() { std::cout << "TA "; }
    };
}

int main() {

    // 1. Predict the Output Verification
    std::cout << "[Test 1] Predict the Output snippet verification:\n";
    std::cout << "Actual output:   ";
    {
        PredictSnippet::TeachingAssistant ta;
        std::cout << '\n';
    }
    std::cout << "Expected output: Person Student Staff TA \n";
    std::cout << "Reason: Virtual base Person is constructed exactly ONCE by the most derived class.\n\n";

    // 2. Full Hierarchy and Diamond Resolution Verification
    std::cout << "[Test 2] Full TeachingAssistant Object Creation:\n";
    {
        TeachingAssistant ta("Pooja Sharma", 4012, "9876543210",
                              "M.Tech Data Science", 9.42,
                              "Computer Science", 25000.0, 15);
        std::cout << "\nDisplaying TA details:\n";
        ta.display();
        std::cout << "\nScope exit - destroying TA:\n";
    }

    return 0;
}
