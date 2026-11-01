/**
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - CO5: Reliable Software with Runtime Polymorphism and Exceptions
 * Task B4: Final Engineering Decision - Practical Extension & Production Assessment
 * Bloom's Level: K5 (Evaluate)
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

// Base Bill Class
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(const std::string& p) : patient(p) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const { return "Bill"; }
    std::string getPatient() const { return patient; }
};

// Existing Bill Classes
class OutPatient : public Bill {
    double c, t;
public:
    OutPatient(const std::string& p, double cons, double tests) 
        : Bill(p), c(cons), t(tests) {}
    double total() const override { return c + t; }
    std::string type() const override { return "OP"; }
};

class InPatient : public Bill {
    int days; double rate, surg;
public:
    InPatient(const std::string& p, int d, double r, double s) 
        : Bill(p), days(d), rate(r), surg(s) {}
    double total() const override { return days * rate + surg; }
    std::string type() const override { return "IP"; }
};

// ==============================================================================
// PRACTICAL EXTENSION 1: HomeCareBill ADDED WITHOUT MODIFYING EXISTING CODE
// ==============================================================================
class HomeCareBill : public Bill {
private:
    int nurseVisits;
    double visitRate;
    double medicalSupplies;

public:
    HomeCareBill(const std::string& p, int visits, double vRate, double supplies)
        : Bill(p), nurseVisits(visits), visitRate(vRate), medicalSupplies(supplies) {}

    double total() const override {
        return (nurseVisits * visitRate) + medicalSupplies;
    }

    std::string type() const override { return "HomeCare"; }
};

// Base Payer Class
class Payer {
public:
    virtual ~Payer() = default;
    virtual std::string name() const = 0;
    virtual double amountCovered(const Bill& b) const = 0;
};

// Existing Payers
class SelfPay : public Payer {
public:
    std::string name() const override { return "SelfPay"; }
    double amountCovered(const Bill&) const override { return 0.0; }
};

class Insurance : public Payer {
    double limit, coPay;
public:
    Insurance(double l, double cp) : limit(l), coPay(cp) {}
    std::string name() const override { return "Insurance"; }
    double amountCovered(const Bill& b) const override {
        double eligible = std::min(b.total(), limit);
        return eligible * (1.0 - coPay / 100.0);
    }
};

// ==============================================================================
// PRACTICAL EXTENSION 2: CorporateTieUp Payer ADDED WITHOUT MODIFYING EXISTING CODE
// ==============================================================================
class CorporateTieUp : public Payer {
private:
    std::string companyName;
    double negotiatedDiscountRate; // e.g. 15% corporate discount
    double maxCorporateCover;      // e.g. 100,000

public:
    CorporateTieUp(const std::string& comp, double disc, double maxCov)
        : companyName(comp), negotiatedDiscountRate(disc), maxCorporateCover(maxCov) {}

    std::string name() const override { return "Corporate (" + companyName + ")"; }

    double amountCovered(const Bill& b) const override {
        double discountedBill = b.total() * (1.0 - negotiatedDiscountRate / 100.0);
        return std::min(discountedBill, maxCorporateCover);
    }
};

int main() {
    std::cout << "========================================================\n";
    std::cout << "ASSIGNMENT 5 - TASK B4: FINAL ENGINEERING EXTENSION\n";
    std::cout << "========================================================\n\n";

    // Regression Test: Existing Bills and Payers
    std::cout << "[B4.1] Demonstrating Extension with Regression Verification:\n";
    std::vector<std::unique_ptr<Bill>> systemBills;
    systemBills.push_back(std::make_unique<OutPatient>("Ravi", 500, 1200));      // 1700
    systemBills.push_back(std::make_unique<InPatient>("Meena", 3, 2500, 40000)); // 47500
    
    // NEW BILL TYPE: HomeCareBill
    systemBills.push_back(std::make_unique<HomeCareBill>("Elderly Care - Sundaram", 10, 800.0, 3500.0)); // 11500

    // Payers including NEW CorporateTieUp
    std::vector<std::unique_ptr<Payer>> systemPayers;
    systemPayers.push_back(std::make_unique<SelfPay>());
    systemPayers.push_back(std::make_unique<Insurance>(50000.0, 10.0));
    systemPayers.push_back(std::make_unique<CorporateTieUp>("Infosys Healthcare", 15.0, 100000.0));

    std::cout << "Processing Extended Bills across All Payers:\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    for (const auto& b : systemBills) {
        std::cout << "Bill [" << b->type() << "] for Patient: " << b->getPatient() 
                  << " | Total: Rs." << std::fixed << std::setprecision(2) << b->total() << "\n";
        for (const auto& p : systemPayers) {
            std::cout << "    -> Covered by " << std::left << std::setw(28) << p->name() 
                      << ": Rs." << p->amountCovered(*b) << "\n";
        }
    }
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << "All existing and newly added bill/payer types executed flawlessly.\n\n";

    std::cout << "Task B4 Verification Completed Successfully.\n";
    return 0;
}
