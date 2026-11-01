/**
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - CO5: Reliable Software with Runtime Polymorphism and Exceptions
 * Task B2: Engineer Error Conditions
 * Bloom's Level: K5 (Evaluate)
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

// Predict Output Snippet
namespace PredictSnippetB2 {
    void validate(double amount) {
        if (amount < 0) throw std::invalid_argument("negative");
        if (amount > 1e6) throw std::out_of_range("too large");
        std::cout << "ok ";
    }
    void process(double amount) {
        try {
            validate(amount);
        } catch (const std::out_of_range& e) {
            std::cout << "[log " << e.what() << "] ";
            throw; // rethrow
        }
    }
}

// Complete the Code Classes
class ClaimError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

double approve(double billed, double cover) {
    // TODO 1: Throw std::invalid_argument("negative amount") if billed or cover is negative;
    // throw ClaimError("exceeds cover") if billed > cover; otherwise return billed
    if (billed < 0.0 || cover < 0.0) {
        throw std::invalid_argument("negative amount");
    }
    if (billed > cover) {
        throw ClaimError("exceeds cover");
    }
    return billed;
}

double settle(double billed, double cover) {
    try {
        return approve(billed, cover);
    } catch (const ClaimError& e) {
        // TODO 2: Print "audit: " followed by the message, then rethrow the SAME exception
        std::cout << "audit: " << e.what() << '\n';
        throw;
    }
}

// RAII Test Guard to prove stack unwinding destroys objects on exceptions
struct AllocationGuard {
    std::string resourceName;
    explicit AllocationGuard(const std::string& name) : resourceName(name) {
        std::cout << "  [Resource Acquired: " << resourceName << "]\n";
    }
    ~AllocationGuard() {
        std::cout << "  [Resource Safely Released: " << resourceName << "]\n";
    }
};

void simulateSevereFaults(int faultType) {
    AllocationGuard guard("PatientClaimRecord_#884");
    if (faultType == 1) {
        throw std::invalid_argument("Negative consultation charges detected (-500.00)");
    } else if (faultType == 2) {
        throw std::out_of_range("Unknown surgical package code: PKG-9999");
    } else if (faultType == 3) {
        // Parsing error simulation (std::stod)
        throw std::invalid_argument("std::stod: no conversion possible from string 'INV-XYZ'");
    } else if (faultType == 4) {
        // Bad alloc simulation
        throw std::bad_alloc();
    }
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "ASSIGNMENT 5 - TASK B2: ENGINEER ERROR CONDITIONS\n";
    std::cout << "========================================================\n\n";

    // 1. Predict Output snippet verification
    std::cout << "[Test 1] Predict Output Snippet Verification:\n";
    std::cout << "Actual output:   ";
    for (double a : {100.0, -5.0, 2e6}) {
        try {
            PredictSnippetB2::process(a);
            std::cout << "done ";
        } catch (const std::exception& e) {
            std::cout << "caught " << e.what() << ' ';
        }
    }
    std::cout << "\nExpected output: ok done caught negative [log too large] caught too large \n\n";

    // 2. Complete the Code Verification
    std::cout << "[Test 2] Complete the Code Prompt Verification:\n";
    double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
    for (auto& c : cases) {
        try {
            double paid = settle(c[0], c[1]);
            std::cout << "approved " << paid << '\n';
        } catch (const ClaimError& e) {
            std::cout << "claim rejected: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << '\n';
        }
    }
    std::cout << "\n";

    // 3. Testing 4 Engineered Error Conditions with RAII stack unwinding
    std::cout << "[Test 3] Engineered Fault Injection & RAII Cleanup:\n";
    for (int fault = 1; fault <= 4; ++fault) {
        std::cout << "Fault Scenario " << fault << ":\n";
        try {
            simulateSevereFaults(fault);
        } catch (const std::bad_alloc& e) {
            std::cout << "  Top-level catch handled memory exhaustion: std::bad_alloc\n";
        } catch (const std::exception& e) {
            std::cout << "  Top-level catch handled domain exception: " << e.what() << "\n";
        } catch (...) {
            std::cout << "  Top-level catch(...) caught unknown fatal exception safely.\n";
        }
        std::cout << "  Program remains fully alive and stable.\n\n";
    }

    std::cout << "Task B2 Verification Completed Successfully.\n";
    return 0;
}
