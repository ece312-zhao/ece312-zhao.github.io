#include <cstddef>
#include <cstdio>
#include <map>
#include <new>
#include <stdexcept>
#include <vector>

// Chapter 1: Error handling via function arguments and return value

// Return the index of the first number matching the target
int find_number(const std::vector<int> &vec, int target) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return i;
        }
    }
    return -1; // Sentinel value meaning "not found", as an array index is non-negative
}

// Suppose our program wants to treat division by zero as an error condition.
// The return value is already used for the result of division,
// so how can we also communicate error status?
double divide(double x, double y) {
    return x / y;
}

// Solution 1: Pass a reference to a boolean indicating whether division failed
double divide(double x, double y, bool &err) {
    if (y == 0) {
        err = true;
        return 0.0; // We should ignore 0.0
    } else {
        err = false;
        return x / y;
    }
}

// Solution 2: Return a boolean status code and use a reference for the result
bool divide(double x, double y, double &res) {
    if (y == 0) {
        return false;
    } else {
        res = x / y;
        return true;
    }
}

void use_divide() {
    double x = 10, y = 20;
    bool err;
    double res = divide(x, y, err);

    if (err) { // Check for errors before using it
        printf("Divide by zero!\n");
    } else {
        printf("Result = %f\n", res);
    }

    double z = 0;
    res = divide(x, z, err);
    if (err) {
        printf("Divide by zero!\n");
    } else {
        printf("Result = %f\n", res);
    }

    // The code looks messy because we must check the error state after each call.
    // Things become even messier if there are multiple possible errors.
    //
    // Another issue with returning a bool status code is that constructors
    // do not have return values.
    printf("=== End of use_divide ===\n\n");
}

// Chapter 2: Error handling via exceptions
// An exception signals that something went wrong and transfers control
// to a matching exception handler.
double div_w_exception(double x, double y) {
    if (y == 0.0) {
        printf("Before throw\n");
        throw "Divide by zero"; // You can throw a value of any data type
        printf("After throw\n"); // Never executes
    }
    printf("After branch\n"); // This will not execute if a divide-by-zero error occured
    return x / y;
}

void use_divide_exception() {
    double x = 10, y = 20, z = 0;

    try {
        double res = div_w_exception(x, y);
        printf("Result = %f\n", res); // This prints

        res = div_w_exception(x, z);
        printf("Result = %f\n", res);
    } catch (const char *e) {
        printf("Encountered an exception: %s\n", e);
    }

    // Execution continues after the try-catch statement
    printf("Normal execution\n");
    printf("=== End of use_divide_exception ===\n\n");
}
// IMPORTANT NOTE:
// An exception does not automatically undo side effects that already happened.
// For example, in the code above, the first "Result =" line is still printed.

// Chapter 3: Advanced try-throw-catch
void multi_catch() {
    try {
        throw -1;
    } catch (int x) {
        printf("Caught an integer exception of %d\n", x);
    } catch (double) {
        printf("Caught a double exception and we do not care about its value\n");
    } catch (const char *s) {
        printf("Caught a string exception of %s\n", s);
    }
}

void uncaught() {
    try {
        throw -1;
    } catch (double) {
        // No implicit int-to-double conversion is performed here
        printf("Caught a double exception and we do not care about its value\n");
    }

    // The thrown int has no matching catch block here.
    // This is an uncaught exception (assuming it's not caught by its callers, see later),
    // so the program terminates.
}

class Base {};
class Derived : public Base {};

void inheritance_demo() {
    try {
        throw Derived();
    } catch (const Base &) {
        // Child classes are up-casted into the parent class
        printf("Caught a Base exception\n");
    } catch (const Derived &) {
        // This block will never be reached because the Base handler appears first.
        // Flip the order of "catch (Base)" and "catch (Derived)" if you want to catch Derived
        printf("Caught a Derived exception\n");
    }
}

void use_try_throw_catch() {
    try {
        multi_catch();
    } catch (const char *) {
        printf("Caught an exception thrown from a catch block\n");
    }

    // uncaught(); // This would terminate the program
    inheritance_demo();
    printf("=== End of use_try_throw_catch ===\n\n");
}

// Chapter 4: Stack unwinding
// When an exception occurs, the program first searches for a matching handler
// in the current function. If none is found, it walks up the call stack
// until a matching handler is found.

void f3() {
    printf("Entering f3\n");
    throw 3;
    printf("Leaving f3\n");
}

void f2() {
    printf("Entering f2\n");
    try {
        f3();
    } catch (double) {
        printf("Caught a double exception in f2\n");
    }
    printf("Leaving f2\n");
}

void f1() {
    printf("Entering f1\n");
    f2();
    printf("Leaving f1\n");
}

void f0() {
    printf("Entering f0\n");
    try {
        f1();
    } catch (...) {
        printf("Caught a generic exception in f0\n");
        // The exception object survives stack unwinding;
        // it is not just an ordinary local stack variable.
    }
    printf("Leaving f0\n");
}

void use_stack_unwinding() {
    f0();
    printf("=== End of use_stack_unwinding ===\n\n");
}

// Chapter 5: Catch exceptions thrown by STL containers
void catch_std_exceptions() {
    std::vector<int> vec = {1, 2, 3};

    try {
        printf("4th element is %d\n", vec.at(3));
    } catch (const std::out_of_range &) {
        printf("Out-of-range access!\n");
    }

    std::map<int, int> map = {{1, 2}, {2, 3}};
    try {
        printf("Looking up key=3. Value=%d\n", map.at(3));
    } catch (const std::out_of_range &) {
        printf("Key not found!\n");
    }

    try {
        int *arr = new int[1000000000000ULL];
        delete[] arr;
    } catch (const std::bad_alloc &) {
        printf("Failed to allocate an array\n");
    }
}

int main() {
    use_divide();
    use_divide_exception();
    use_try_throw_catch();
    use_stack_unwinding();
    catch_std_exceptions();
}
