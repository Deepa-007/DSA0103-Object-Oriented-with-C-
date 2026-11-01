#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; // percent

public:
    Cart() = default;

    // TODO 1: add(): store the item and price if there is space, and return *this so that calls can be chained
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    // TODO 2: setDiscount(): use this-> to tell the member apart from the parameter named discount, and return *this
    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }

    void display() const {
        std::cout << "Cart (" << n << " items, Discount: " << discount << "%): Total = Rs."
                  << std::fixed << std::setprecision(2) << total() << "\n";
    }
};

int main() {

    // 1. Mandatory Complete the Code check
    std::cout << "[Test 1] Complete the Code Verification:\n";
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c; // pointer to an object
    p->add("Oil", 180);
    std::cout << "Actual output:   " << std::fixed << std::setprecision(2) << p->total() << '\n';
    std::cout << "Expected output: 324.00\n\n";

    // 2. Four ways to reach an object
    std::cout << "[Test 2] Demonstrating Four Ways to Reach an Object:\n";
    // (a) Directly by name
    Cart directCart;
    directCart.add("Milk", 30).add("Bread", 40);
    std::cout << "  (a) Directly by name: Total = " << directCart.total() << "\n";

    // (b) Through a pointer with ->
    Cart* ptrCart = &directCart;
    ptrCart->add("Eggs", 60);
    std::cout << "  (b) Through pointer (->): Total = " << ptrCart->total() << "\n";

    // (c) Through a reference
    Cart& refCart = directCart;
    refCart.setDiscount(5);
    std::cout << "  (c) Through reference (.): Total = " << refCart.total() << "\n";

    // (d) Through dynamically allocated pointer (new/delete)
    Cart* dynCart = new Cart();
    dynCart->add("Apples", 150).add("Bananas", 60).setDiscount(10);
    std::cout << "  (d) Dynamic pointer (new): Total = " << dynCart->total() << "\n";
    delete dynCart;

    // 3. Array of pointers to find cart with highest total
    std::cout << "\n[Test 3] Finding Cart with Highest Total across Active Customers:\n";
    Cart customerCarts[4];
    customerCarts[0].add("Tea", 120).add("Sugar", 45);                 // 165.00
    customerCarts[1].add("Coffee", 350).add("Cookies", 150);          // 500.00
    customerCarts[2].add("Basmati Rice", 450).add("Ghee", 600);       // 1050.00
    customerCarts[3].add("Snacks", 80);                               // 80.00

    Cart* activeCarts[4] = { &customerCarts[0], &customerCarts[1], &customerCarts[2], &customerCarts[3] };

    Cart* highestCart = activeCarts[0];
    int highestIndex = 0;
    for (int i = 1; i < 4; ++i) {
        if (activeCarts[i]->total() > highestCart->total()) {
            highestCart = activeCarts[i];
            highestIndex = i;
        }
    }

    std::cout << "  Highest Cart is Customer #" << highestIndex + 1
              << " with Total: Rs." << highestCart->total() << "\n";

        return 0;
}
