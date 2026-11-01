#include <iostream>
#include <vector>
#include <memory>

// Predict Output snippet
namespace PredictTask4 {
    struct Account {
        double rate() const { return 3.5; }
        virtual double vrate() const { return 3.5; }
        virtual ~Account() = default;
    };
    struct SeniorAccount : Account {
        double rate() const { return 4.0; }
        double vrate() const override { return 4.0; }
    };
}

// Repaired Account Hierarchy
class RepairedAccount {
public:
    virtual double rate() const { return 3.5; }
    virtual ~RepairedAccount() {
        std::cout << "[~RepairedAccount Base Destructor]\n";
    }
};

class RepairedSeniorAccount : public RepairedAccount {
private:
    double* history;

public:
    RepairedSeniorAccount() : history(new double[100]) {
        for (int i = 0; i < 100; ++i) history[i] = 4.0;
    }

    double rate() const override { return 4.0; }

    ~RepairedSeniorAccount() override {
        std::cout << "[~RepairedSeniorAccount Derived Destructor: Releasing history buffer]\n";
        delete[] history;
    }
};

int main() {

    // 1. Predict the Output snippet verification
    std::cout << "[Test 1] Predict the Output snippet:\n";
    {
        PredictTask4::SeniorAccount s;
        PredictTask4::Account* p = &s;
        PredictTask4::Account copy = s; // Sliced object!
        std::cout << "Actual output:   " << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';
        std::cout << "Expected output: 3.5 4 3.5\n";
        std::cout << "Explanation:\n"
                  << "  - p->rate(): early bound at compile-time to Account::rate() -> 3.5\n"
                  << "  - p->vrate(): dynamic dispatch at runtime via vtable to SeniorAccount::vrate() -> 4.0\n"
                  << "  - copy.vrate(): object sliced to base Account, calling Account::vrate() -> 3.5\n\n";
    }

    // 2. Repaired Polymorphic Account Hierarchy Verification
    std::cout << "[Test 2] Repaired Program Execution (Virtual Functions & Destructor):\n";
    std::vector<std::unique_ptr<RepairedAccount>> accounts;
    accounts.push_back(std::make_unique<RepairedAccount>());
    accounts.push_back(std::make_unique<RepairedSeniorAccount>());

    for (size_t i = 0; i < accounts.size(); ++i) {
        std::cout << "  Account " << i << " rate: " << accounts[i]->rate() << "%\n";
    }
    std::cout << "Releasing accounts through smart pointers:\n";
    accounts.clear(); // Correct destructors invoked in derived-to-base order

    return 0;
}
