// Learning goals for this demo:
// - See how C++ classes bundle state and behavior together
//      - Learn what member functions (methods) are and how they are called
//      - Understand the difference between public and private members
//      - See when and why to use 'this'
//      - Understand constructors the member initializer list
// - Learn references as aliases to existing objects
//      - Contrast references with pointers
// - See examples of method overloading and operator overloading
// - Understand why namespaces help prevent name collisions
//      - See std::string as a standard library class under namespace std
//

#include <stdio.h>
#include <cstdio>   // Preferred C++ header form for C stdio facilities
#include <string>

// ============================================================
// Part 1: Basic concepts of classes and objects
// ============================================================

// Old-fashioned C-style ADT:
// data is stored in a struct, and functions operate on that data.
typedef struct {
    const char *color;
    unsigned brightness;  // ranges from 0 to 10
    bool is_on;

    // Invariant:
    // - when is_on == false, brightness == 0
    // - when is_on == true,  1 <= brightness <= 10
} LampInC;

void LampTurnOn(LampInC *lamp) {
    if (!lamp) return;
    lamp->is_on = true;
    lamp->brightness = 10;
}

void LampTurnOff(LampInC *lamp) {
    if (!lamp) return;
    lamp->is_on = false;
    lamp->brightness = 0;
}

void LampPrint(const LampInC *lamp) {
    if (!lamp) return;

    if (lamp->is_on) {
        printf("Lamp emits %s light with a brightness of %u\n",
               lamp->color, lamp->brightness);
    } else {
        printf("Lamp is off\n");
    }
}

void LampSetBrightness(LampInC *lamp, unsigned brightness) {
    if (!lamp || brightness > 10) return;

    if (brightness == 0) {
        LampTurnOff(lamp);
    } else {
        if (!lamp->is_on) {
            LampTurnOn(lamp);
        }
        lamp->brightness = brightness;
    }
}

void useLampInC() {
    LampInC lamp = {"blue", 0, false};
    LampPrint(&lamp);

    LampTurnOn(&lamp);
    LampSetBrightness(&lamp, 5);
    LampPrint(&lamp);
}

// A transitional example between C and C++:
// the data and the operations are now grouped together.
struct Lamp {
    // Data members
    const char *color;
    unsigned brightness;  // ranges from 0 to 10
    bool is_on;

    void turnOn() {  // member function (method)
        is_on = true;
        brightness = 10;
    }

    void turnOff() {
        is_on = false;
        brightness = 0;
    }

    // Mark as const because printing should not modify the object.
    void print() const {
        if (is_on) {
            printf("Lamp emits %s light with a brightness of %u\n",
                   color, brightness);
        } else {
            printf("Lamp is off\n");
        }
    }

    void setBrightness(unsigned brightness) {
        if (brightness > 10) return;

        if (brightness == 0) {
            turnOff();
        } else {
            if (!is_on) {
                turnOn();
            }

            // 'this' is a pointer to the current object.
            // Use it when a parameter name shadows a data member name.
            this->brightness = brightness;
        }
    }
};

void useLampInCpp() {
    Lamp lamp = {"blue", 0, false};
    lamp.print();

    lamp.turnOn();
    lamp.setBrightness(8);
    lamp.print();

    printf("Is lamp on: %d\n", lamp.is_on);

    // Problem: the data is still public.
    // A user can break the invariant directly, for example:
    // lamp.is_on = false;   // but brightness might still stay nonzero
}

// A better C++ class with proper access control.
class BetterLamp {
private:
    // Private data is inaccessible from outside the class.
    // This helps the class preserve its own invariants.
    const char *color;
    unsigned brightness;  // ranges from 0 to 10
    bool is_on;

public:
    // Constructors initialize an object when it is created.
    //
    // The member initializer list is preferred:
    // - members are initialized before the constructor body runs
    // - members are initialized in the order they are declared above,
    //   NOT in the order written in the initializer list
    BetterLamp(const char *color)
        : color(color), brightness(0), is_on(false) {
        printf("Created a lamp with %s color\n", color);
    }

    BetterLamp() : color("red"), brightness(0), is_on(false) {
        printf("Created a default %s lamp\n", color);
    }

    void turnOn() {
        is_on = true;
        brightness = 10;
    }

    void turnOff() {
        is_on = false;
        brightness = 0;
    }

    // A const member function promises not to modify the object.
    void print() const {
        if (is_on) {
            printf("Lamp emits %s light with a brightness of %u\n",
                   color, brightness);
        } else {
            printf("Lamp is off\n");
        }
    }

    void setBrightness(unsigned brightness) {
        if (brightness > 10) return;

        if (brightness == 0) {
            turnOff();
        } else {
            if (!is_on) {
                turnOn();
            }
            this->brightness = brightness;
        }
    }
};

// A small example showing why the member initializer list matters.
class Foo {
public:
    // This would not compile because x and y are const members:
    //
    // Foo(int a) {
    //     x = a;
    //     y = a;
    // }

    Foo(int a) : x(a), y(x) {}

    // Again: data members are initialized in declaration order.
    // The following does NOT initialize x with a first:
    //
    // Foo(int a) : y(a), x(y) {}

    const int x, y;
};

void useBetterLamp() {
    BetterLamp lamp("green");

    lamp.print();

    lamp.turnOn();
    lamp.setBrightness(7);
    lamp.print();

    // lamp.is_on = false;   // error: private member

    BetterLamp def_lamp;
    def_lamp.turnOn();
    def_lamp.print();
}

void setMildlyGlowing(BetterLamp &lamp) {
    lamp.setBrightness(3);
}

void useLampReference() {
    BetterLamp lamp("green");

    // This creates a copy using the compiler-generated copy constructor
    // (covered in the next lecture).
    BetterLamp lamp_cpy = lamp;
    lamp_cpy.turnOn();
    lamp.print();

    // A reference is an alias for an existing object.
    // Use the reference as if it were the object itself.
    //
    // Properties of references:
    // 1. cannot be null
    // 2. must be initialized when created
    // 3. cannot be modified to refer to a different object later
    BetterLamp &lampref = lamp;
    lampref.turnOn();
    lamp.print();

    // References are also common as function parameters.
    setMildlyGlowing(lamp);
    lamp.print(); // The brightness should be 3
}

void useLampPointer() {
    // new allocates memory and constructs the object.
    BetterLamp *lamp = new BetterLamp("red");
    lamp->setBrightness(5);
    lamp->print();
    delete lamp;  // delete destroys the object and frees the memory

    // new[] constructs each element using the default constructor.
    BetterLamp *lamp_arr = new BetterLamp[10];
    for (int i = 0; i < 10; i++) {
        lamp_arr[i].setBrightness(i);
        lamp_arr[i].print();
    }

    // delete[] destroys every element, then frees the array memory.
    delete[] lamp_arr;

    // delete lamp_arr;   // wrong: must use delete[] for arrays
    // Also, do not use delete on memory allocated by malloc().
}

// ============================================================
// Part 2: Other useful C++ features
// ============================================================

// Function overloading:
// same function name, different parameter lists.
// The compiler chooses the correct one based on the argument types.
//
// Important rule:
// return type alone cannot distinguish overloads.
class Point {
public:
    Point(int x, int y) : x(x), y(y) {}

    void setLoc(int x, int y) {
        printf("Set location via integers\n");
        this->x = x;
        this->y = y;
    }

    void setLoc(double x, double y) {
        printf("Set location via floating points\n");
        this->x = (int)x;
        this->y = (int)y;
    }

    // Operator overloading lets user-defined types work naturally with operators.
    // Here we overload '+' so we can write:
    // Point c = a + b;
    Point operator+(const Point &other) const {
        return Point(x + other.x, y + other.y);
    }

    void print() const {
        printf("Point(%d, %d)\n", x, y);
    }

private:
    int x, y;
};

void showOverloading() {
    Point p1(1, 1), p2(2, 2);
    p1.setLoc(2, 2);
    p2.setLoc(3.0, 3.0);
    Point p3 = p1 + p2;
    p3.print();
}

// Namespaces prevent name collisions.
namespace Fruit {
void buy_apple(unsigned c) {
    printf("Bought %u juicy apples\n", c);
}
}  // namespace Fruit

namespace Company {
// Same function name and same parameter list as Fruit::buy_apple,
// but in a different namespace.
void buy_apple(unsigned c) {
    printf("Bought %u shares of Apple stock\n", c);
}
}  // namespace Company

void showNamespace() {
    Fruit::buy_apple(10);
    Company::buy_apple(10);

    // std::string belongs to the standard namespace std.
    std::string s("This is a standard string!\n");
    printf("String s=%s\n", s.c_str());
}

int main() {
    useLampInC();
    useLampInCpp();
    useBetterLamp();
    useLampReference();
    useLampPointer();
    showOverloading();
    showNamespace();
    return 0;
}