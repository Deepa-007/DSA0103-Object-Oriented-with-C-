#include <iostream>
#include <string>
#include <iomanip>

class Auditorium {
private:
    std::string eventName;
    int rows;
    int seatsPerRow;
    char** seatMap; // 2D dynamically allocated array of characters

public:
    // Dynamic Constructor
    Auditorium(const std::string& name, int r, int s)
        : eventName(name), rows(r), seatsPerRow(s), seatMap(nullptr) {
        std::cout << "[CONSTRUCTOR] Allocating Auditorium seat map for '" << eventName
                  << "' (" << rows << " rows x " << seatsPerRow << " seats)\n";
        if (rows > 0 && seatsPerRow > 0) {
            seatMap = new char*[rows];
            for (int i = 0; i < rows; ++i) {
                seatMap[i] = new char[seatsPerRow];
                for (int j = 0; j < seatsPerRow; ++j) {
                    seatMap[i][j] = '.'; // '.' means available
                }
            }
        }
    }

    // Destructor releasing every row allocation and printing summary
    ~Auditorium() {
        std::cout << "[DESTRUCTOR] Closing Event '" << eventName << "'. Releasing memory...\n";
        int booked = 0, total = rows * seatsPerRow;
        if (seatMap != nullptr) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < seatsPerRow; ++j) {
                    if (seatMap[i][j] == 'B') ++booked;
                }
                delete[] seatMap[i]; // release each row
            }
            delete[] seatMap; // release pointer array
            seatMap = nullptr;
        }
        std::cout << "  Closing Summary: " << booked << " booked out of "
                  << total << " seats (" << (total > 0 ? (booked * 100.0 / total) : 0.0) << "% occupancy)\n";
    }

    bool bookSeat(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow) return false;
        if (seatMap[r][s] == 'B') return false; // already booked
        seatMap[r][s] = 'B';
        return true;
    }

    bool cancelSeat(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow) return false;
        if (seatMap[r][s] == '.') return false; // not booked
        seatMap[r][s] = '.';
        return true;
    }

    size_t getHeapMemoryBytes() const {
        if (rows <= 0 || seatsPerRow <= 0) return 0;
        return (rows * sizeof(char*)) + (rows * seatsPerRow * sizeof(char));
    }
};

// Predict Output Struct from Question 3
struct Logger {
    std::string n;
    Logger(const std::string& s) : n(s) { std::cout << '+' << n << ' '; }
    Logger(const Logger& o) : n(o.n + "c") { std::cout << "copy" << n << ' '; }
    ~Logger() { std::cout << '-' << n << ' '; }
};

void use(Logger) { std::cout << "use "; }

int main() {

    // 1. Stack Allocation
    std::cout << "--- 1. Stack Scope Object ---\n";
    {
        Auditorium stackEvent("Tech Symposium", 3, 5);
        stackEvent.bookSeat(0, 0);
        stackEvent.bookSeat(0, 1);
        stackEvent.bookSeat(2, 4);
        std::cout << "  Seats booked on stack object.\n";
    } // stackEvent destructor called here automatically

    // 2. Block Scope Object
    std::cout << "\n--- 2. Block Scope Object ---\n";
    {
        std::cout << "  Entering inner block...\n";
        Auditorium blockEvent("Annual Concert", 2, 4);
        blockEvent.bookSeat(1, 1);
        std::cout << "  Exiting inner block...\n";
    } // blockEvent destructor called here

    // 3. Heap Allocation (new and delete)
    std::cout << "\n--- 3. Heap Scope Object (new/delete) ---\n";
    Auditorium* heapEvent = new Auditorium("Drama Festival", 4, 6);
    heapEvent->bookSeat(1, 2);
    heapEvent->cancelSeat(1, 2);
    delete heapEvent; // explicit release

    // 4. Zero rows edge case
    std::cout << "\n--- 4. Edge Case: Zero Rows ---\n";
    {
        Auditorium zeroEvent("Canceled Show", 0, 0);
    }

    // 5. Memory measurement table
    std::cout << "\n--- 5. Heap Memory Calculations for 2D Seat Maps ---\n";
    int dimensions[][2] = {{10, 20}, {100, 200}, {1000, 2000}};
    std::cout << std::left << std::setw(15) << "Grid (R x S)"
              << std::setw(20) << "Pointer Array (B)"
              << std::setw(20) << "Seat Elements (B)"
              << std::setw(15) << "Total Heap (B)" << "\n";
    for (auto& d : dimensions) {
        int r = d[0], s = d[1];
        size_t ptrBytes = r * sizeof(char*);
        size_t charBytes = r * s * sizeof(char);
        size_t totalBytes = ptrBytes + charBytes;
        std::string gridStr = std::to_string(r) + " x " + std::to_string(s);
        std::cout << std::left << std::setw(15) << gridStr
                  << std::setw(20) << ptrBytes
                  << std::setw(20) << charBytes
                  << std::setw(15) << totalBytes << "\n";
    }

    // 6. Predict the Output Verification
    std::cout << "\n--- 6. Predict the Output Code Verification ---\n";
    std::cout << "Running Logger test snippet:\n";
    {
        Logger a("A");
        {
            Logger b("B");
        }
        use(a);
        Logger* p = new Logger("P");
        delete p;
        std::cout << "end ";
    }
    std::cout << "\n";
    return 0;
}
