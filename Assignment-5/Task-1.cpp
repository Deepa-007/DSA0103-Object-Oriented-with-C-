/**
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - CO5: Reliable Software with Runtime Polymorphism and Exceptions
 * Task B1: Implement and Validate Polymorphic Billing
 * Bloom's Level: K5 (Evaluate)
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

class Bill {
protected:
    std::string patient;

public:
    explicit Bill(const std::string& p) : patient(p) {}
    virtual ~Bill() = default;

    // TODO 1: Declare a pure virtual function total() const returning double, and a virtual function type() const returning "Bill"
    virtual double total() const = 0;
    virtual std::string type() const { return "Bill"; }

    std::string getPatient() const { return patient; }

    void print() const {
        std::cout << type() << ' ' << patient << ": " << total() << '\n';
    }
};

class OutPatient : public Bill {
    double consultation, tests;

public:
    OutPatient(const std::string& p, double c, double t)
        : Bill(p), consultation(c), tests(t) {}

    // TODO 2: Override total() as consultation + tests, and type() as "OP"
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};

class InPatient : public Bill {
    int days;
    double perDay, surgery;

public:
    InPatient(const std::string& p, int d, double rate, double s)
        : Bill(p), days(d), perDay(rate), surgery(s) {}

    // TODO 3: Override total() as days * perDay + surgery, and type() as "IP"
    double total() const override { return days * perDay + surgery; }
    std::string type() const override { return "IP"; }
};

class DayCareSurgery : public Bill {
    double packagePrice;

public:
    DayCareSurgery(const std::string& p, double pkg)
        : Bill(p), packagePrice(pkg) {}

    double total() const override { return packagePrice; }
    std::string type() const override { return "DayCare"; }
};

class Emergency : public Bill {
    double baseCharge, hourlyRate;
    double hours;

public:
    Emergency(const std::string& p, double base, double rate, double h)
        : Bill(p), baseCharge(base), hourlyRate(rate), hours(h) {}

    double total() const override { return baseCharge + (hourlyRate * hours); }
    std::string type() const override { return "Emergency"; }
};

// Payer Hierarchy from Task A3
class Payer {
public:
    virtual ~Payer() = default;
    virtual std::string getPayerName() const = 0;
    virtual double amountCovered(const Bill& b) const = 0;
};

class SelfPay : public Payer {
public:
    std::string getPayerName() const override { return "Self-Pay"; }
    double amountCovered(const Bill& b) const override {
        (void)b;
        return 0.0; // Patient pays 100%, payer covers 0
    }
};

class Insurance : public Payer {
    double coverLimit;
    double coPayPercent; // e.g. 10% co-payment by patient

public:
    Insurance(double limit, double coPay)
        : coverLimit(limit), coPayPercent(coPay) {}

    std::string getPayerName() const override { return "Private Insurance"; }

    double amountCovered(const Bill& b) const override {
        double billTotal = b.total();
        double eligible = std::min(billTotal, coverLimit);
        return eligible * (1.0 - coPayPercent / 100.0);
    }
};

class GovernmentScheme : public Payer {
    double fixedPackageRate;

public:
    explicit GovernmentScheme(double pkgRate) : fixedPackageRate(pkgRate) {}

    std::string getPayerName() const override { return "Government Health Scheme"; }

    double amountCovered(const Bill& b) const override {
        return std::min(b.total(), fixedPackageRate);
    }
};

int main() {
    std::cout << "========================================================\n";
    std::cout << "ASSIGNMENT 5 - TASK B1: POLYMORPHIC BILLING VERIFICATION\n";
    std::cout << "========================================================\n\n";

    // 1. Mandatory Complete the Code check from assignment prompt
    std::cout << "[Test 1] Complete the Code Verification:\n";
    {
        Bill* bills[] = {
            new OutPatient("Ravi", 500, 1200),
            new InPatient("Meena", 3, 2500, 40000)
        };
        double sum = 0;
        for (Bill* b : bills) {
            b->print(); // runtime polymorphism
            sum += b->total();
        }
        std::cout << "Total " << sum << '\n';
        for (Bill* b : bills) delete b; // virtual destructor runs the right destructor
    }
    std::cout << "Expected Output:\nOP Ravi: 1700\nIP Meena: 47500\nTotal 49200\n\n";

    // 2. Validate all 4 bill types against all 3 payers
    std::cout << "[Test 2] Comprehensive Matrix: 4 Bill Types x 3 Payers:\n";
    std::vector<std::unique_ptr<Bill>> allBills;
    allBills.push_back(std::make_unique<OutPatient>("Alice", 600, 1400));           // 2000
    allBills.push_back(std::make_unique<InPatient>("Bob", 4, 3000, 25000));         // 37000
    allBills.push_back(std::make_unique<DayCareSurgery>("Charlie", 18000));         // 18000
    allBills.push_back(std::make_unique<Emergency>("David", 5000, 1000, 6));        // 11000

    std::vector<std::unique_ptr<Payer>> payers;
    payers.push_back(std::make_unique<SelfPay>());
    payers.push_back(std::make_unique<Insurance>(50000.0, 10.0)); // 50k cover, 10% co-pay
    payers.push_back(std::make_unique<GovernmentScheme>(20000.0)); // 20k capped

    std::cout << std::left << std::setw(15) << "Bill Type" 
              << std::setw(12) << "Patient" 
              << std::setw(12) << "Gross Total" 
              << std::setw(15) << "Self-Pay" 
              << std::setw(18) << "Insurance Cover" 
              << std::setw(18) << "Govt Scheme Cover" << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (const auto& b : allBills) {
        std::cout << std::left << std::setw(15) << b->type() 
                  << std::setw(12) << b->getPatient() 
                  << std::setw(12) << std::fixed << std::setprecision(2) << b->total();
        for (const auto& p : payers) {
            std::cout << std::setw(18) << p->amountCovered(*b);
        }
        std::cout << "\n";
    }

    std::cout << "\nTask B1 Verification Completed Successfully.\n";
    return 0;
}
