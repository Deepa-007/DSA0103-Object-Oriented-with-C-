#include <iostream>
#include <string>
#include <chrono>
#include <vector>

class Prescription {
private:
    int n;
    std::string* medicines; // dynamically allocated array

public:
    static int copyConstructorCalls;

    // Parameterised Constructor
    explicit Prescription(int count)
        : n(count), medicines(new std::string[count]) {}

    // TODO 1: Copy constructor: allocate a NEW array of the same size and copy every medicine (deep copy)
    Prescription(const Prescription& other)
        : n(other.n), medicines(new std::string[other.n]) {
        ++copyConstructorCalls;
        for (int i = 0; i < n; ++i) {
            medicines[i] = other.medicines[i];
        }
    }

    // TODO 2: Copy assignment: guard against self-assignment, build a new array, copy, release the old array and return *this
    Prescription& operator=(const Prescription& other) {
        if (this != &other) {
            std::string* newMedicines = new std::string[other.n];
            for (int i = 0; i < other.n; ++i) {
                newMedicines[i] = other.medicines[i];
            }
            delete[] medicines;
            medicines = newMedicines;
            n = other.n;
        }
        return *this;
    }

    // TODO 3: Release the dynamically allocated array in Destructor
    ~Prescription() {
        delete[] medicines;
        medicines = nullptr;
    }

    void set(int i, const std::string& m) {
        if (i >= 0 && i < n) medicines[i] = m;
    }

    std::string get(int i) const {
        if (i >= 0 && i < n) return medicines[i];
        return "";
    }

    int size() const { return n; }
};

int Prescription::copyConstructorCalls = 0;

void passByValue(Prescription p) {
    (void)p;
}

void passByConstReference(const Prescription& p) {
    (void)p;
}

int main() {

    // 1. Mandatory Complete the Code check
    std::cout << "[Test 1] Complete the Code Verification:\n";
    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");

    Prescription q = p; // copy constructor (deep copy)
    q.set(1, "Cetirizine");

    Prescription r(1);
    r = p; // copy assignment (deep copy)
    p.set(0, "Ibuprofen");

    std::cout << "Actual Output:\n";
    std::cout << p.get(0) << ' ' << p.get(1) << " | "
              << q.get(0) << ' ' << q.get(1) << " | "
              << r.get(0) << ' ' << r.get(1) << '\n';
    std::cout << "Expected Output:\n";
    std::cout << "Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS\n\n";

    // 2. Test self-assignment (p = p)
    std::cout << "[Test 2] Testing Self-Assignment (p = p):\n";
    p = p;
    std::cout << "After p = p: " << p.get(0) << ' ' << p.get(1) << " (Integrity verified)\n\n";

    // 3. Count copies in pass-by-value vs pass-by-const-ref
    std::cout << "[Test 3] Pass-by-value vs Pass-by-const-ref copy counts:\n";
    Prescription::copyConstructorCalls = 0;
    passByValue(p);
    std::cout << "Copies made for passByValue: " << Prescription::copyConstructorCalls << "\n";

    Prescription::copyConstructorCalls = 0;
    passByConstReference(p);
    std::cout << "Copies made for passByConstReference: " << Prescription::copyConstructorCalls << "\n\n";

    // 4. Measure copy performance across data sizes (10, 1000, 100000)
    std::cout << "[Test 4] Benchmarking Deep Copy for 10, 1000, 100000 medicines:\n";
    for (int count : {10, 1000, 100000}) {
        Prescription source(count);
        for (int i = 0; i < count; ++i) {
            source.set(i, "Med_" + std::to_string(i));
        }
        auto start = std::chrono::high_resolution_clock::now();
        Prescription copied = source;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "  Count = " << count << ": Copy Time = " << elapsed.count() << " ms\n";
    }

      return 0;
}
