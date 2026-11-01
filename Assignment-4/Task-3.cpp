#include <iostream>
#include <string>

// Predict the Output snippet
namespace PredictTask3 {
    struct Engine {
        Engine() { std::cout << "Engine "; }
        ~Engine() { std::cout << "~Engine "; }
    };
    struct Vehicle {
        Vehicle() { std::cout << "Vehicle "; }
        ~Vehicle() { std::cout << "~Vehicle "; }
    };
    struct Car : Vehicle {
        Engine e;
        Car() { std::cout << "Car "; }
        ~Car() { std::cout << "~Car "; }
    };
}

// Complete the Code Classes
class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address { // used as a member class (composition)
    std::string city;
    int pin;

public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Student : public Person {
    long long regNo;
    Address home;

public:
    // TODO 1: Write the constructor: initialise the Person base, regNo and the home Address member in initializer list
    Student(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), regNo(r), home(c, p) {}

    void print() const {
        std::cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n';
    }
};

int main() {

    // 1. Predict Output snippet verification
    std::cout << "[Test 1] Predict the Output snippet:\n";
    std::cout << "Actual output:   ";
    {
        {
            PredictTask3::Car c;
            std::cout << "| ";
        }
        std::cout << '\n';
    }
    std::cout << "Expected output: Vehicle Engine Car | ~Car ~Engine ~Vehicle \n";
    std::cout << "Construction order: Base (Vehicle) -> Member (Engine) -> Derived (Car)\n";
    std::cout << "Destruction order:  Derived (~Car) -> Member (~Engine) -> Base (~Vehicle)\n\n";

    // 2. Complete the Code Verification
    std::cout << "[Test 2] Complete the Code Prompt Verification:\n";
    Student s("Kavya", 19, 192311001, "Salem", 636001);
    std::cout << "Actual output:   ";
    s.print();
    std::cout << "Expected output: 192311001 Kavya (19) Salem-636001\n\n";

    return 0;
}
