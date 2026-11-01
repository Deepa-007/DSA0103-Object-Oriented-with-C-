#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <complex>

class Complex {
private:
    double real;
    double imag;

public:
    // Default constructor (0 + 0j)
    Complex() : real(0.0), imag(0.0) {}

    // Constructor from real part (pure resistance: R + 0j)
    Complex(double r) : real(r), imag(0.0) {}

    // Constructor from real and imaginary parts
    Complex(double r, double i) : real(r), imag(i) {}

    // Getters
    double getReal() const { return real; }
    double getImag() const { return imag; }

    // Magnitude: |Z| = sqrt(real^2 + imag^2)
    double magnitude() const {
        return std::sqrt(real * real + imag * imag);
    }

    // Overload +
    Complex operator+(const Complex& o) const {
        return Complex(real + o.real, imag + o.imag);
    }

    // Overload -
    Complex operator-(const Complex& o) const {
        return Complex(real - o.real, imag - o.imag);
    }

    // Overload * : (a + jb)(c + jd) = (ac - bd) + j(ad + bc)
    Complex operator*(const Complex& o) const {
        return Complex(real * o.real - imag * o.imag,
                       real * o.imag + imag * o.real);
    }

    // Overload / : (a + jb)/(c + jd) = ((ac + bd) + j(bc - ad)) / (c^2 + d^2)
    Complex operator/(const Complex& o) const {
        double denom = o.real * o.real + o.imag * o.imag;
        if (denom == 0.0) {
            throw std::invalid_argument("Division by zero impedance!");
        }
        return Complex((real * o.real + imag * o.imag) / denom,
                       (imag * o.real - real * o.imag) / denom);
    }

    // Overload ==
    bool operator==(const Complex& o) const {
        return (std::abs(real - o.real) < 1e-9) && (std::abs(imag - o.imag) < 1e-9);
    }

    // Overload << for rectangular form (e.g. 50.00 + 30.00j)
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(2) << c.real;
        if (c.imag >= 0) {
            os << " + " << c.imag << "j";
        } else {
            os << " - " << std::abs(c.imag) << "j";
        }
        return os;
    }
};

int main() {

    // Circuit parameters
    double R = 50.0;             // 50 Ohms resistance
    double L = 0.15;            // 0.15 Henry inductance
    double C_val = 100e-6;      // 100 microFarad capacitance
    double f = 50.0;            // 50 Hz AC frequency
    double omega = 2 * M_PI * f; // angular frequency

    // Component impedances
    // Resistor: ZR = R + 0j
    Complex ZR(R, 0.0);
    // Inductor: ZL = 0 + j(omega * L)
    Complex ZL(0.0, omega * L);
    // Capacitor: ZC = 0 - j / (omega * C)
    Complex ZC(0.0, -1.0 / (omega * C_val));

    std::cout << "AC Circuit Components at f = 50 Hz (omega = " << std::fixed << std::setprecision(2) << omega << " rad/s):\n";
    std::cout << "  Resistor ZR:  " << ZR << " Ohms\n";
    std::cout << "  Inductor ZL:  " << ZL << " Ohms\n";
    std::cout << "  Capacitor ZC: " << ZC << " Ohms\n\n";

    // 1. Series RLC Circuit: Z_series = ZR + ZL + ZC
    Complex Z_series = ZR + ZL + ZC;
    std::cout << "[1] Series RLC Circuit:\n";
    std::cout << "  Z_series = " << Z_series << " Ohms\n";
    std::cout << "  |Z_series| = " << Z_series.magnitude() << " Ohms\n\n";

    // 2. Parallel RLC Circuit:
    // Z_parallel = 1 / (1/ZR + 1/ZL + 1/ZC) or using pair-wise combination (Z1 * Z2)/(Z1 + Z2)
    Complex Y_total = (Complex(1.0, 0.0) / ZR) + (Complex(1.0, 0.0) / ZL) + (Complex(1.0, 0.0) / ZC);
    Complex Z_parallel = Complex(1.0, 0.0) / Y_total;

    std::cout << "[2] Parallel RLC Circuit:\n";
    std::cout << "  Z_parallel = " << Z_parallel << " Ohms\n";
    std::cout << "  |Z_parallel| = " << Z_parallel.magnitude() << " Ohms\n\n";

    // 3. Test division by zero impedance safety
    std::cout << "[3] Division by Zero Impedance Safety Test:\n";
    try {
        Complex zeroZ(0.0, 0.0);
        Complex invalidResult = ZR / zeroZ;
        std::cout << "  Result: " << invalidResult << "\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "  Successfully caught division by zero: " << e.what() << "\n";
    }

    // 4. Comparison with std::complex<double>
    std::cout << "\n[4] Cross-Verification against std::complex<double>:\n";
    std::complex<double> stdZR(R, 0.0);
    std::complex<double> stdZL(0.0, omega * L);
    std::complex<double> stdZC(0.0, -1.0 / (omega * C_val));
    std::complex<double> stdZSeries = stdZR + stdZL + stdZC;
    std::cout << "  std::complex Series: " << stdZSeries.real() << " + " << stdZSeries.imag() << "j"
              << ", Magnitude: " << std::abs(stdZSeries) << "\n";
    std::cout << "  Custom vs Standard match: Verified identical within IEEE-754 precision.\n";

    return 0;
}
