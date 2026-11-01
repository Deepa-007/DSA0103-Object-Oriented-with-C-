/**
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - CO5: Reliable Software with Runtime Polymorphism and Exceptions
 * Task B3: Virtual Functions vs switch vs Exceptions: The Cost
 * Bloom's Level: K5 (Evaluate)
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <variant>

// Virtual Function Implementation
class PolyBill {
public:
    virtual ~PolyBill() = default;
    virtual double total() const = 0;
};

class PolyOP : public PolyBill {
    double consultation, tests;
public:
    PolyOP(double c, double t) : consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
};

class PolyIP : public PolyBill {
    int days;
    double rate, surgery;
public:
    PolyIP(int d, double r, double s) : days(d), rate(r), surgery(s) {}
    double total() const override { return days * rate + surgery; }
};

// Switch-based Implementation
enum class BillType { OP, IP };

struct SwitchBill {
    BillType type;
    double param1; // consultation or days
    double param2; // tests or dailyRate
    double param3; // 0 or surgery

    double total() const {
        switch (type) {
            case BillType::OP: return param1 + param2;
            case BillType::IP: return (param1 * param2) + param3;
            default: return 0.0;
        }
    }
};

// std::variant Implementation
struct VarOP { double c, t; double total() const { return c + t; } };
struct VarIP { int d; double r, s; double total() const { return d * r + s; } };
using VarBill = std::variant<VarOP, VarIP>;

int main() {
    std::cout << "========================================================\n";
    std::cout << "ASSIGNMENT 5 - TASK B3: PERFORMANCE & ARCHITECTURAL COST\n";
    std::cout << "========================================================\n\n";

    // 1. Small Data Correctness Check (10 bills)
    std::cout << "[B3.1] Correctness Verification on 10 Sample Bills:\n";
    std::vector<PolyBill*> polyList;
    std::vector<SwitchBill> switchList;

    for (int i = 0; i < 5; ++i) {
        polyList.push_back(new PolyOP(500 + i * 50, 1000 + i * 100));
        switchList.push_back({BillType::OP, 500.0 + i * 50, 1000.0 + i * 100, 0.0});
    }
    for (int i = 0; i < 5; ++i) {
        polyList.push_back(new PolyIP(3 + i, 2000, 15000 + i * 5000));
        switchList.push_back({BillType::IP, static_cast<double>(3 + i), 2000.0, 15000.0 + i * 5000});
    }

    double polyTotal = 0.0;
    for (auto b : polyList) polyTotal += b->total();

    double switchTotal = 0.0;
    for (const auto& b : switchList) switchTotal += b.total();

    std::cout << "  Virtual Functions Total: Rs. " << std::fixed << std::setprecision(2) << polyTotal << "\n";
    std::cout << "  Switch Statement Total:  Rs. " << switchTotal << "\n";
    std::cout << "  Totals Match: " << std::boolalpha << (polyTotal == switchTotal) << "\n\n";

    for (auto b : polyList) delete b;

    // 2. Scaled Benchmark: 1,000, 1,000,000, and 10,000,000 bills
    std::cout << "[B3.2] Dispatch Mechanism Scaled Benchmarking:\n";
    std::cout << std::left << std::setw(15) << "Bill Count" 
              << std::setw(25) << "Virtual Functions (ms)" 
              << std::setw(25) << "Switch Dispatch (ms)" 
              << std::setw(25) << "std::variant/visit (ms)" << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (int count : {1000, 1000000, 10000000}) {
        // Switch benchmark
        std::vector<SwitchBill> sBills(count, {BillType::OP, 500.0, 1200.0, 0.0});
        auto startS = std::chrono::high_resolution_clock::now();
        double sumS = 0.0;
        for (int i = 0; i < count; ++i) sumS += sBills[i].total();
        auto endS = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> timeS = endS - startS;

        // Virtual benchmark (using pointers)
        // For 10M we use a smaller repeated buffer to prevent out-of-memory
        int testBatch = (count > 1000000) ? 1000000 : count;
        int repeat = count / testBatch;
        std::vector<PolyBill*> pBills(testBatch);
        for (int i = 0; i < testBatch; ++i) {
            if (i % 2 == 0) pBills[i] = new PolyOP(500.0, 1200.0);
            else pBills[i] = new PolyIP(3, 2500.0, 40000.0);
        }

        auto startP = std::chrono::high_resolution_clock::now();
        double sumP = 0.0;
        for (int r = 0; r < repeat; ++r) {
            for (int i = 0; i < testBatch; ++i) sumP += pBills[i]->total();
        }
        auto endP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> timeP = endP - startP;

        for (int i = 0; i < testBatch; ++i) delete pBills[i];

        // Variant benchmark
        std::vector<VarBill> vBills(testBatch, VarOP{500.0, 1200.0});
        auto startV = std::chrono::high_resolution_clock::now();
        double sumV = 0.0;
        for (int r = 0; r < repeat; ++r) {
            for (int i = 0; i < testBatch; ++i) {
                sumV += std::visit([](const auto& b) { return b.total(); }, vBills[i]);
            }
        }
        auto endV = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> timeV = endV - startV;

        (void)sumS; (void)sumP; (void)sumV;

        std::cout << std::left << std::setw(15) << count 
                  << std::setw(25) << timeP.count() 
                  << std::setw(25) << timeS.count() 
                  << std::setw(25) << timeV.count() << "\n";
    }

    std::cout << "\nTask B3 Verification Completed Successfully.\n";
    return 0;
}
