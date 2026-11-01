
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

// ==============================================================================
// REPAIRED DESIGN: CLEAN INHERITANCE HIERARCHY
// ==============================================================================

class Staff {
protected:
    std::string name;
    int id;
    double basic;
    static constexpr double DA_RATE = 0.42; // Constant instead of magic number

public:
    Staff(const std::string& n, int i, double b = 0.0)
        : name(n), id(i), basic(b) {}

    virtual ~Staff() = default; // Essential virtual destructor for polymorphic base

    virtual double pay() const = 0; // Pure virtual function

    std::string getName() const { return name; }
    int getId() const { return id; }
    double getBasic() const { return basic; }

    virtual void display() const {
        std::cout << "ID: " << id << " | Name: " << std::left << std::setw(18) << name
                  << " | Pay: Rs. " << std::fixed << std::setprecision(2) << pay() << "\n";
    }
};

class Professor : public Staff {
private:
    double research;

public:
    Professor(const std::string& n, int i, double b, double r)
        : Staff(n, i, b), research(r) {}

    double pay() const override {
        return basic + (basic * DA_RATE) + research;
    }
};

class LabAssistant : public Staff {
public:
    LabAssistant(const std::string& n, int i, double b)
        : Staff(n, i, b) {}

    double pay() const override {
        return basic + (basic * DA_RATE);
    }
};

class GuestFaculty : public Staff {
private:
    double hours;
    double rate;

public:
    GuestFaculty(const std::string& n, int i, double h, double r)
        : Staff(n, i, 0.0), hours(h), rate(r) {}

    double pay() const override {
        return hours * rate;
    }
};

int main() {

    // Polymorphic processing of all staff in a single loop
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Dr. Anand Kumar", 101, 75000.0, 15000.0));
    payroll.push_back(std::make_unique<LabAssistant>("P. Rajesh", 204, 28000.0));
    payroll.push_back(std::make_unique<GuestFaculty>("Ms. Shalini Iyer", 305, 32.5, 1200.0));

    double totalPayroll = 0.0;
    std::cout << "College Staff Payroll Report:\n";
    std::cout << "--------------------------------------------------------\n";
    for (const auto& staffMember : payroll) {
        staffMember->display();
        totalPayroll += staffMember->pay();
    }
    std::cout << "--------------------------------------------------------\n";
    std::cout << "Total Monthly Payroll Expenditure: Rs. "
              << std::fixed << std::setprecision(2) << totalPayroll << "\n\n";

       return 0;
}
