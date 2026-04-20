#include <iomanip>
#include <iostream>
#include <string>

// Chapter 1: Basic concept of iostream
void show_iostream() {
    // Send the string "Hello world" to an output stream connected to the console
    // std::endl inserts a newline and flushes the stream
    // std::cout is a std::ostream ("output stream"), and it supports << operator
    std::cout << "Hello world" << std::endl;
    std::cout << "We can also print a number like " << 10 << std::endl;

    // We can also read from an input stream
    // std::cin is a std::istream ("input stream"), and it supports >> operator
    int x = 0;
    if (std::cin >> x) { // Attempt to read an integer from the console into x
        std::cout << "x is " << x << std::endl;
    } else {
        // x was not successfully assigned by this extraction
        std::cin.clear(); // Clear the failed state
        std::cin.ignore(10000, '\n'); // Ignore input until a '\n' is found
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); is a better version
        std::cout << "Bad input\n";
    }
    std::cout << "=== End of show_iostream ===\n" << std::endl;
}

// Chapter 2: Manipulators
// A manipulator is used with >> or << to change a stream's behavior or formatting
void show_manipulator() {
    char buf[10] = "";
    // If the user types more than 9 non-whitespace characters,
    // the extra characters remain in the input stream (prevent buffer overflow).
    // The next extraction may read those leftover characters.
    // Why 9? Because of the ending '\0'!
    std::cin >> std::setw(10) >> buf;

    int y = 0;
    // If the user entered more than 9 characters for buf,
    // this extraction may read the leftover characters instead.
    std::cin >> y;
    std::cout << "buf=" << buf << " | y=" << y << std::endl;

    // Change the output base
    std::cout << "hex=" << std::hex << 63 << std::endl; // hexadecimal
    std::cout << "dec=" << std::dec << 63 << std::endl; // decimal

    // Set the floating-point format
    std::cout << std::fixed << std::setprecision(2) << 3.1415925 << std::endl;
    std::cout << 2.718281 << std::endl; // The effect of these manipulators is persistent

    std::cout << "=== End of show_manipulator ===\n" << std::endl;
}

// Chapter 3: Overloading stream operators for a class
class Block {
public:
    Block(const char *s, double x, double y, double z) : x(x), y(y), z(z), name(s) {}
    Block() : Block("Uninitialized", 0, 0, 0) {}

    double x, y, z;
    std::string name;
};

// Return the stream reference so that output can be chained
std::ostream &operator<<(std::ostream &os, const Block &blk) {
    os << blk.name << " (" << blk.x << ", " << blk.y << ", " << blk.z << ")";
    return os;
}

std::istream &operator>>(std::istream &is, Block &blk) {
    is >> blk.name >> blk.x >> blk.y >> blk.z;
    return is;
}

void show_stream_operator_overloading() {
    Block b("Grass", 10, 10, 10);
    std::cout << b << "\n";

    // The code above is roughly the same as:
    auto &os = operator<<(std::cout, b);
    operator<<(os, "\n");

    // Returning the stream reference allows chaining
    Block b2;
    std::cin >> b2;
    std::cout << b2 << std::endl;
}

// Chapter 4: Printing inherited classes
class Base {
public:
    int x;
    Base(int x) : x(x) {}
};

std::ostream &operator<<(std::ostream &os, const Base &b) {
    return os << "Base x=" << b.x;
}

class Derived : public Base {
public:
    Derived(int x) : Base(x) {}
};

std::ostream &operator<<(std::ostream &os, const Derived &b) {
    return os << "Derived x=" << b.x;
}

void show_inherited_class() {
    Derived d{10};
    std::cout << d << std::endl;

    Base &b = d;
    std::cout << b << std::endl; // Prints "Base x=". operator<< itself is not virtual
}

// We define a virtual method responsible for printing and call it from operator<<
class BaseFixed {
public:
    int x;
    BaseFixed(int x) : x(x) {}

    virtual std::ostream &print(std::ostream &os) const {
        return os << "Base x=" << x;
    }
};

std::ostream &operator<<(std::ostream &os, const BaseFixed &b) {
    return b.print(os); // Calls the virtual function print
}

class DerivedFixed : public BaseFixed {
public:
    DerivedFixed(int x) : BaseFixed(x) {}

    std::ostream &print(std::ostream &os) const override {
        return os << "Derived x=" << x;
    }
};

void show_fixed_inherited_class() {
    DerivedFixed d{10};
    std::cout << d << std::endl;

    BaseFixed &b = d;
    std::cout << b << std::endl; // Prints "Derived x=" because print() is virtual
}

int main() {
    show_iostream();
    show_manipulator();
    show_stream_operator_overloading();
    show_inherited_class();
    show_fixed_inherited_class();
    return 0;
}
