#include <iostream>
#include <cstring>
#include <chrono>
#include <vector>

class Text {
private:
    char* data;
    size_t length;

public:
    static int copyCount;

    // Default and C-string constructor
    Text(const char* s = "") {
        if (s == nullptr) s = "";
        length = std::strlen(s);
        data = new char[length + 1];
        std::strcpy(data, s);
    }

    // Copy Constructor (Deep Copy)
    Text(const Text& other) : length(other.length) {
        ++copyCount;
        data = new char[length + 1];
        std::strcpy(data, other.data);
    }

    // Copy Assignment Operator with self-assignment guard
    Text& operator=(const Text& other) {
        if (this != &other) {
            char* newData = new char[other.length + 1];
            std::strcpy(newData, other.data);
            delete[] data;
            data = newData;
            length = other.length;
        }
        return *this;
    }

    // Destructor
    ~Text() {
        delete[] data;
        data = nullptr;
        length = 0;
    }

    size_t size() const { return length; }
    const char* c_str() const { return data; }

    // Concatenation operator +
    Text operator+(const Text& other) const {
        Text result;
        delete[] result.data; // free default empty buffer
        result.length = this->length + other.length;
        result.data = new char[result.length + 1];
        std::strcpy(result.data, this->data);
        std::strcat(result.data, other.data);
        return result;
    }

    // Compound assignment +=
    Text& operator+=(const Text& other) {
        char* newData = new char[this->length + other.length + 1];
        std::strcpy(newData, this->data);
        std::strcat(newData, other.data);
        delete[] data;
        data = newData;
        length = length + other.length;
        return *this;
    }

    // Comparison ==
    bool operator==(const Text& other) const {
        return (length == other.length) && (std::strcmp(data, other.data) == 0);
    }

    // Comparison !=
    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        os << t.data;
        return os;
    }
};

int Text::copyCount = 0;

int main() {
        // 1. Basic concatenation and printing
    std::cout << "[Test 1] Concatenation and Comparison:\n";
    Text t1("OTP Code: ");
    Text t2("482910");
    Text t3 = t1 + t2;
    std::cout << "  t1: " << t1 << "\n";
    std::cout << "  t2: " << t2 << "\n";
    std::cout << "  t3 = t1 + t2: " << t3 << "\n";
    std::cout << "  t3 == 'OTP Code: 482910': " << std::boolalpha << (t3 == Text("OTP Code: 482910")) << "\n";
    std::cout << "  t1 != t2: " << (t1 != t2) << "\n\n";

    // 2. Empty messages and long SMS messages
    std::cout << "[Test 2] Empty & Long SMS Messages:\n";
    Text emptyText;
    std::cout << "  Empty message length: " << emptyText.size() << ", content: '" << emptyText << "'\n";

    std::string longMsg(1600, 'X'); // 10 standard SMS segments
    Text longText(longMsg.c_str());
    std::cout << "  Long message length: " << longText.size() << " bytes\n\n";

    // 3. Self-assignment test
    std::cout << "[Test 3] Self-assignment (t1 = t1):\n";
    t1 = t1;
    std::cout << "  After self-assignment: " << t1 << " (Safe, no crash or dangling pointer)\n\n";

    // 4. Counting copies during a + b + c
    std::cout << "[Test 4] Counting Copies in a + b + c:\n";
    Text::copyCount = 0;
    Text a("Alpha-");
    Text b("Beta-");
    Text c("Gamma");
    Text sum = a + b + c;
    std::cout << "  Result: " << sum << "\n";
    std::cout << "  Copy constructor invocations: " << Text::copyCount << "\n\n";

    // 5. Compare custom Text class with std::string for 100,000 appends
    std::cout << "[Test 5] Performance Comparison for 10,000 concatenations:\n";
    const int CONCAT_COUNT = 10000;

    auto tStart = std::chrono::high_resolution_clock::now();
    Text customAccum;
    for (int i = 0; i < CONCAT_COUNT; ++i) {
        customAccum += Text("x");
    }
    auto tEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> customDuration = tEnd - tStart;

    auto sStart = std::chrono::high_resolution_clock::now();
    std::string stdAccum;
    for (int i = 0; i < CONCAT_COUNT; ++i) {
        stdAccum += "x";
    }
    auto sEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> stdDuration = sEnd - sStart;

    std::cout << "  Custom Text (" << CONCAT_COUNT << " reallocations): " << customDuration.count() << " ms\n";
    std::cout << "  std::string (" << CONCAT_COUNT << " geometric growth):  " << stdDuration.count() << " ms\n";

    return 0;
}
