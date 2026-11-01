#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

class Money {
private:
    long long paise; // store entire amount in smallest unit (paise)

public:
    // Constructor converting rupees and paise into total paise
    Money(long long rupees = 0, int p = 0)
        : paise(rupees * 100 + p) {}

    // Direct paise constructor
    static Money fromPaise(long long p) {
        Money m;
        m.paise = p;
        return m;
    }

    long long getPaise() const { return paise; }

    // TODO 1: Overload + as a member function that returns a new Money object
    Money operator+(const Money& other) const {
        Money result;
        result.paise = this->paise + other.paise;
        return result;
    }

    // Overload - as a member function
    Money operator-(const Money& other) const {
        Money result;
        result.paise = this->paise - other.paise;
        return result;
    }

    // Overload += as a member function returning reference to *this
    Money& operator+=(const Money& other) {
        this->paise += other.paise;
        return *this;
    }

    // Overload < as a member function
    bool operator<(const Money& other) const {
        return this->paise < other.paise;
    }

    // TODO 2: Overload == as a member, and << as a friend that prints "Rs 200.00"
    bool operator==(const Money& other) const {
        return this->paise == other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long absPaise = std::abs(m.paise);
        long long r = absPaise / 100;
        long long p = absPaise % 100;
        if (m.paise < 0) {
            os << "-Rs " << r << '.' << std::setfill('0') << std::setw(2) << p;
        } else {
            os << "Rs " << r << '.' << std::setfill('0') << std::setw(2) << p;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Money& m) {
        long long r = 0;
        int p = 0;
        is >> r >> p;
        m.paise = r * 100 + p;
        return is;
    }
};

int main() {

    // 1. Mandatory Complete the Code check
    std::cout << "[Test 1] Complete the Code Prompt Verification:\n";
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';

    // 2. Addition carrying paise into rupees (Rs 0.75 + Rs 0.50 = Rs 1.25)
    std::cout << "\n[Test 2] Testing carry of paise into rupees:\n";
    Money p1(0, 75);
    Money p2(0, 50);
    Money pSum = p1 + p2;
    std::cout << "  " << p1 << " + " << p2 << " = " << pSum
              << " (Expected: Rs 1.25)\n";

    // 3. Subtraction producing a negative amount
    std::cout << "\n[Test 3] Testing negative balance handling:\n";
    Money diff = p1 - p2;
    std::cout << "  " << p1 << " - " << p2 << " = " << diff << "\n";
    Money neg = p2 - p1;
    std::cout << "  " << p2 << " - " << p1 << " = " << neg << " (Negative balance)\n";

    // 4. Billing counter comparison: With vs Without operator overloading
    std::cout << "\n[Test 4] Billing calculation readability:\n";
    Money item1(150, 50), item2(85, 25), item3(45, 0);
    Money totalBill = item1 + item2 + item3;
    std::cout << "  Items: " << item1 << ", " << item2 << ", " << item3 << "\n";
    std::cout << "  Total Bill (item1 + item2 + item3): " << totalBill << "\n";

    // 5. Compare Money (integer paise) vs double for 1,000,000 additions
    std::cout << "\n[Test 5] Precision & Drift test: Money vs double (1,000,000 additions of 0.10):\n";
    const int N = 1000000;
    Money moneyAccum(0, 0);
    Money tenPaise(0, 10);
    for (int i = 0; i < N; ++i) {
        moneyAccum += tenPaise;
    }

    double doubleAccum = 0.0;
    for (int i = 0; i < N; ++i) {
        doubleAccum += 0.10;
    }

    std::cout << "  Money result:  " << moneyAccum << " (Paise: " << moneyAccum.getPaise() << ")\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "  Double result: Rs " << doubleAccum << " (Shows floating-point accumulation drift)\n";
    std::cout << std::resetiosflags(std::ios_base::floatfield);

    return 0;
}
