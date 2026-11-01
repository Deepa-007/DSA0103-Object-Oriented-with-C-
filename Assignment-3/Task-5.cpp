#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <chrono>

class Time {
private:
    int hours;
    int minutes;
    int seconds;

    void normalize() {
        if (seconds >= 60) {
            minutes += seconds / 60;
            seconds %= 60;
        }
        if (minutes >= 60) {
            hours += minutes / 60;
            minutes %= 60;
        }
    }

public:
    Time(int h = 0, int m = 0, int s = 0)
        : hours(h), minutes(m), seconds(s) {
        normalize();
    }

    long long toTotalSeconds() const {
        return hours * 3600LL + minutes * 60LL + seconds;
    }

    // Prefix increment (++t)
    Time& operator++() {
        ++seconds;
        normalize();
        return *this;
    }

    // Postfix increment (t++)
    Time operator++(int) {
        Time old = *this; // make copy
        ++seconds;
        normalize();
        return old;       // return copy by value
    }

    // Equality comparison
    bool operator==(const Time& other) const {
        return toTotalSeconds() == other.toTotalSeconds();
    }

    // Less-than comparison
    bool operator<(const Time& other) const {
        return toTotalSeconds() < other.toTotalSeconds();
    }

    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        os << std::setfill('0') << std::setw(2) << t.hours << ":"
           << std::setfill('0') << std::setw(2) << t.minutes << ":"
           << std::setfill('0') << std::setw(2) << t.seconds;
        return os;
    }
};

class Playlist {
private:
    std::vector<std::string> tracks;

public:
    Playlist(std::initializer_list<std::string> list) : tracks(list) {}

    size_t size() const { return tracks.size(); }

    // Non-const operator[] with range check
    std::string& operator[](int index) {
        if (index < 0 || static_cast<size_t>(index) >= tracks.size()) {
            std::cerr << "Error: Index " << index << " out of range [0, "
                      << tracks.size() - 1 << "]\n";
            throw std::out_of_range("Playlist index out of bounds: " + std::to_string(index));
        }
        return tracks[index];
    }

    // Const operator[] with range check
    const std::string& operator[](int index) const {
        if (index < 0 || static_cast<size_t>(index) >= tracks.size()) {
            std::cerr << "Error: Const Index " << index << " out of range [0, "
                      << tracks.size() - 1 << "]\n";
            throw std::out_of_range("Playlist const index out of bounds: " + std::to_string(index));
        }
        return tracks[index];
    }
};

// Predict Output Struct
struct Counter {
    int v;
    Counter& operator++() { ++v; return *this; } // prefix
    Counter operator++(int) { Counter old = *this; ++v; return old; } // postfix
};

int main() {

    // 1. Predict the Output Verification
    std::cout << "[Test 1] Predict the Output Program Verification:\n";
    Counter c{5};
    Counter a_c = c++;
    Counter b_c = ++c;
    std::cout << "Actual Output:   " << a_c.v << ' ' << b_c.v << ' ' << c.v << '\n';
    std::cout << "Expected Output: 5 7 7\n\n";

    // 2. Verify Time prefix vs postfix increment
    std::cout << "[Test 2] Time increment semantics:\n";
    Time t(0, 1, 30);
    std::cout << "  Initial Time t: " << t << "\n";
    Time a = t++; // postfix: a receives old value (00:01:30), t becomes 00:01:31
    std::cout << "  After a = t++:  a = " << a << ", t = " << t << "\n";
    Time b = ++t; // prefix: t becomes 00:01:32, b receives new value (00:01:32)
    std::cout << "  After b = ++t:  b = " << b << ", t = " << t << "\n\n";

    // 3. Test minute and hour rollover (59:59 -> 01:00:00)
    std::cout << "[Test 3] Rollover Test (00:59:59 -> 01:00:00):\n";
    Time rollTime(0, 59, 59);
    std::cout << "  Before increment: " << rollTime << "\n";
    ++rollTime;
    std::cout << "  After ++rollTime: " << rollTime << " (Rolled over correctly)\n\n";

    // 4. Playlist operator[] range checking
    std::cout << "[Test 4] Playlist Subscript Range Checking:\n";
    Playlist pl{"Track 1 - Prelude", "Track 2 - Allegro", "Track 3 - Rondo"};
    std::cout << "  pl[0]: " << pl[0] << "\n";
    std::cout << "  pl[2]: " << pl[2] << "\n";

    // Modify via non-const operator[]
    pl[1] = "Track 2 - Adagio (Remastered)";
    std::cout << "  pl[1] modified: " << pl[1] << "\n";

    // Test out of range [-1] and [size]
    try {
        std::cout << "  Testing negative index pl[-1]:\n";
        std::string s = pl[-1];
    } catch (const std::out_of_range& e) {
        std::cout << "  Caught expected exception: " << e.what() << "\n";
    }

    try {
        std::cout << "  Testing boundary index pl[3]:\n";
        std::string s = pl[3];
    } catch (const std::out_of_range& e) {
        std::cout << "  Caught expected exception: " << e.what() << "\n";
    }

    // 5. Benchmark 10,000,000 prefix vs postfix increments
    std::cout << "\n[Test 5] Benchmarking 10,000,000 prefix vs postfix increments:\n";
    const int ITERS = 10000000;
    Time testTime(0, 0, 0);

    auto startPre = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        ++testTime;
    }
    auto endPre = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> preDuration = endPre - startPre;

    testTime = Time(0, 0, 0);
    auto startPost = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        testTime++;
    }
    auto endPost = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> postDuration = endPost - startPost;

    std::cout << "  Prefix (++t) Duration:  " << preDuration.count() << " ms\n";
    std::cout << "  Postfix (t++) Duration: " << postDuration.count() << " ms\n";
    std::cout << "  Postfix overhead is due to creating and returning temporary copy objects.\n";

    return 0;
}
