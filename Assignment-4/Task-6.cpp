#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

// Abstract Base Class
class Vehicle {
protected:
    std::string regNo;
    std::string model;
    double dailyRate;
    bool isRented;

public:
    Vehicle(const std::string& reg, const std::string& m, double rate)
        : regNo(reg), model(m), dailyRate(rate), isRented(false) {}

    virtual ~Vehicle() = default;

    virtual double calculateRentalCharge(int daysRented, int daysLate) const = 0;
    virtual void displayDetails() const = 0;

    std::string getRegNo() const { return regNo; }
    std::string getModel() const { return model; }
    double getDailyRate() const { return dailyRate; }
    bool getStatus() const { return isRented; }

    void rent() { isRented = true; }
    void returnVehicle() { isRented = false; }
};

// Concrete Class 1: Bicycle
class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, const std::string& m, double rate)
        : Vehicle(reg, m, rate) {}

    double calculateRentalCharge(int daysRented, int daysLate) const override {
        double baseCharge = daysRented * dailyRate;
        double lateFee = daysLate * (dailyRate * 1.20); // 20% late penalty
        return baseCharge + lateFee;
    }

    void displayDetails() const override {
        std::cout << "[Bicycle] Reg: " << regNo << " | Model: " << model
                  << " | Rate: Rs." << dailyRate << "/day | Status: "
                  << (isRented ? "Rented" : "Available") << "\n";
    }
};

// Intermediate Class: MotorVehicle
class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel; // percentage 0 - 100
    std::string serviceDueDate;

public:
    MotorVehicle(const std::string& reg, const std::string& m, double rate,
                 double level, const std::string& sDate)
        : Vehicle(reg, m, rate), fuelOrChargeLevel(level), serviceDueDate(sDate) {}
};

// Concrete Class 2: TwoWheeler
class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const std::string& reg, const std::string& m, double rate,
               double fuel, const std::string& sDate)
        : MotorVehicle(reg, m, rate, fuel, sDate) {}

    double calculateRentalCharge(int daysRented, int daysLate) const override {
        double base = daysRented * dailyRate;
        double late = daysLate * (dailyRate * 1.50); // 50% late penalty
        return base + late;
    }

    void displayDetails() const override {
        std::cout << "[Two-Wheeler] Reg: " << regNo << " | Model: " << model
                  << " | Fuel: " << fuelOrChargeLevel << "% | Next Service: " << serviceDueDate
                  << " | Rate: Rs." << dailyRate << "/day\n";
    }
};

// Concrete Class 3: Car
class Car : public MotorVehicle {
protected:
    int seatingCapacity;

public:
    Car(const std::string& reg, const std::string& m, double rate,
        double fuel, const std::string& sDate, int seats)
        : MotorVehicle(reg, m, rate, fuel, sDate), seatingCapacity(seats) {}

    double calculateRentalCharge(int daysRented, int daysLate) const override {
        double base = daysRented * dailyRate;
        double late = daysLate * (dailyRate * 1.75); // 75% late penalty
        double insuranceSurcharge = 200.0 * daysRented;
        return base + late + insuranceSurcharge;
    }

    void displayDetails() const override {
        std::cout << "[Car] Reg: " << regNo << " | Model: " << model
                  << " | Seats: " << seatingCapacity << " | Fuel: " << fuelOrChargeLevel
                  << "% | Rate: Rs." << dailyRate << "/day\n";
    }
};

// Concrete Class 4: ElectricCar
class ElectricCar : public Car {
private:
    double batteryCapacityKWh;
    double ratePerKWh;

public:
    ElectricCar(const std::string& reg, const std::string& m, double rate,
                double chargeLevel, const std::string& sDate, int seats,
                double batteryKWh, double kwhRate)
        : Car(reg, m, rate, chargeLevel, sDate, seats),
          batteryCapacityKWh(batteryKWh), ratePerKWh(kwhRate) {}

    double calculateRentalCharge(int daysRented, int daysLate) const override {
        double base = daysRented * dailyRate;
        double late = daysLate * (dailyRate * 1.75);
        // Energy recharge calculation based on kWh consumed
        double energyConsumedKWh = batteryCapacityKWh * ((100.0 - fuelOrChargeLevel) / 100.0);
        double rechargeFee = energyConsumedKWh * ratePerKWh;
        return base + late + rechargeFee;
    }

    void displayDetails() const override {
        std::cout << "[Electric Car] Reg: " << regNo << " | Model: " << model
                  << " | Battery: " << batteryCapacityKWh << " kWh (" << fuelOrChargeLevel << "%)"
                  << " | Rate: Rs." << dailyRate << "/day\n";
    }
};

// ==============================================================================
// EXTENSION DEMONSTRATION: ElectricBus ADDED WITHOUT MODIFYING EXISTING CLASSES
// ==============================================================================
class ElectricBus : public Vehicle {
private:
    int passengerCapacity;
    double batteryCapacityKWh;

public:
    ElectricBus(const std::string& reg, const std::string& m, double rate,
                int passengers, double batteryKWh)
        : Vehicle(reg, m, rate), passengerCapacity(passengers), batteryCapacityKWh(batteryKWh) {}

    double calculateRentalCharge(int daysRented, int daysLate) const override {
        double base = daysRented * dailyRate;
        double late = daysLate * (dailyRate * 2.0); // 100% late penalty
        double commercialTax = 500.0 * daysRented;
        return base + late + commercialTax;
    }

    void displayDetails() const override {
        std::cout << "[Electric Bus (NEW EXTENSION)] Reg: " << regNo << " | Model: " << model
                  << " | Passengers: " << passengerCapacity
                  << " | Battery: " << batteryCapacityKWh << " kWh | Rate: Rs." << dailyRate << "/day\n";
    }
};

int main() {
       std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("TN-01-B-101", "Hercules Road-Trip", 150.0));
    fleet.push_back(std::make_unique<TwoWheeler>("TN-01-TW-202", "Honda Activa 6G", 400.0, 75.0, "2026-12-15"));
    fleet.push_back(std::make_unique<Car>("TN-01-C-303", "Hyundai Creta", 2200.0, 60.0, "2026-11-30", 5));
    fleet.push_back(std::make_unique<ElectricCar>("TN-01-EC-404", "Tata Nexon EV", 2800.0, 45.0, "2027-01-10", 5, 40.5, 12.0));

    // Demonstrate new extension added seamlessly
    fleet.push_back(std::make_unique<ElectricBus>("TN-01-EB-505", "BYD K9 Electric Bus", 8500.0, 42, 324.0));

    std::cout << "Fleet Inventory Listing:\n";
    std::cout << "--------------------------------------------------------\n";
    for (const auto& v : fleet) {
        v->displayDetails();
    }
    std::cout << "--------------------------------------------------------\n\n";

    std::cout << "Simulating Rental Charges (3 days rented + 1 day late return):\n";
    for (const auto& v : fleet) {
        v->rent();
        double charge = v->calculateRentalCharge(3, 1);
        std::cout << "  Vehicle " << v->getRegNo() << " (" << v->getModel()
                  << ") -> Total Charge: Rs." << std::fixed << std::setprecision(2) << charge << "\n";
        v->returnVehicle();
    }

      return 0;
}
