#include <cstdio>

// Chapter 1: The need for function templates
// The two max functions below have the same implementation and only differ in data type.
// We do not like this repetition.

int max_int(int x, int y) {
    if (x < y) {
        return y;
    } else {
        return x;
    }
}

double max_double(double x, double y) {
    if (x < y) {
        return y;
    } else {
        return x;
    }
}

// A function template lets us use a placeholder type (T here)
// instead of writing separate functions for each type.
// By convention, we often use an uppercase T as the placeholder type.
// Use T1, T2, etc. if multiple type parameters are needed.
template <typename T>
T max(T x, T y) {
    if (x < y) {
        return y;
    } else {
        return x;
    }
}

// A template can also have non-type parameters.
// These are not function arguments. Instead, they are compile-time constants
// baked into the instantiated function.
template <int N>
void print() {
    printf("%d\n", N);
}

void showFuncTemplateMotivation() {
    int xint = 1, yint = 2;
    double xdouble = 1.0, ydouble = 2.0;

    printf("max=%d\n", max<int>(xint, yint)); // Explicitly specifying T=int
    printf("max=%d\n", max(xint, yint));      // Template argument deduction
    printf("max=%f\n", max(xdouble, ydouble));

    // printf("max=%f\n", max(xint, ydouble));
    // The code above won't compile,
    // because max(xint, ydouble) does not match max(T, T) directly:
    // the two arguments have different types.

    print<10>();
    // print<xint>(); // This will not compile: xint is not a compile-time constant
}

// Chapter 2: Which types are allowed to be passed to a template?
// In general, a type works with a template if it supports the operations
// that the template uses.

// Example 1: Having the required members and member functions
struct Point {
    int x, y;
    int sum() const { return x + y; }
};

struct Coord3D {
    int x, y, z;
    int sum() const { return x + y + z; }
};

struct Line { // Doesn't have x and y members
    int length;
};

struct PurePoint { // Doesn't have the sum function
    int x, y;
};

// Accepts any type with x and y members, and a sum member function
template <typename T>
void print_xy(const T &p) {
    printf("(%d, %d)\n", p.x, p.y);
    printf("sum=%d\n", p.sum());
}

void showAllowedType1() {
    Point point = {1, 2};
    print_xy(point);

    Coord3D coord = {1, 2, 3};
    print_xy(coord);

    // This will not work because Line has no x and y members
    // Line line = {1};
    // print_xy(line);

    // This will not work because PurePoint has no sum member function
    // PurePoint pp = {1, 2};
    // print_xy(pp);
}

// Example 2: Supporting required operators
// Recall that max<T> uses "<" to compare elements.
class Foo {
public:
    int x;
};

class Bar {
public:
    int x;

    // Overload the "<" operator
    bool operator<(const Bar &other) const {
        return x < other.x;
    }
};

void showAllowedType2() {
    // The following will not work, since Foo does not support comparison with <
    // Foo f1{1}, f2{2};
    // printf("%d\n", max<Foo>(f1, f2).x);

    Bar b1 = {1}, b2 = {2};
    printf("max=%d\n", max<Bar>(b1, b2).x);
}

// Chapter 3: Better max
// If an object is expensive to copy, it is often better to pass by const reference.
template <typename T>
const T &better_max(const T &x, const T &y) {
    if (x < y) {
        return y;
    } else {
        return x;
    }
}

void showBetterMax() {
    Bar b1 = {1}, b2 = {2};
    printf("max=%d\n", better_max<Bar>(b1, b2).x);
}

// Chapter 4: Function template specialization
// This is a more advanced feature and less commonly needed than ordinary templates.
template <typename T>
void print_number(T num) {
    printf("num=%d\n", num); // This only makes sense for integer-like types
}

// Explicit specialization for double
template <>
void print_number<double>(double num) {
    printf("num=%f\n", num);
}

void showFuncSpecialization() {
    print_number(1);
    print_number('a');
    print_number(2.4);
}

// Chapter 5: Class templates
// The same idea can be applied to classes.

class PairInt {
public:
    int first;
    int second;
};

class PairDouble {
public:
    double first;
    double second;
};

// A class template for Pair
template <typename T>
class Pair {
public:
    T first;
    T second;
};

// We can use a class template within a function template
template <typename T>
T pair_max(const Pair<T> &p) {
    if (p.first > p.second) {
        return p.first;
    } else {
        return p.second;
    }
}

// We can define class templates with multiple type parameters
template <typename T1, typename T2>
class OddTriplet {
public:
    T1 first;
    T2 second;
    double third; // A concrete type can also appear inside the class
};

void showClassTemplate() {
    Pair<int> p1 = {1, 2};
    printf("(%d, %d)\n", p1.first, p1.second);

    Pair<double> p2 = {1.0, 2.0};
    printf("(%f, %f)\n", p2.first, p2.second);

    printf("max: %f\n", pair_max<double>(p2));

    OddTriplet<int, double> p3 = {1, 2.0, 3.0};
    printf("(%d, %f, %f)\n", p3.first, p3.second, p3.third);
}

// Chapter 6: Template classes can have methods
template <typename T>
class Tuple {
private:
    T elem1, elem2;

public:
    // We pass by const reference to avoid copying into the parameters
    Tuple(const T &e1, const T &e2) : elem1(e1), elem2(e2) {}

    T &getElem1() {
        return elem1;
    }

    void setElem1(const T &new_e);
};

// Defining a class-template method outside the class body
template <typename T>
void Tuple<T>::setElem1(const T &new_e) {
    elem1 = new_e;
}

void showClassTemplateMethods() {
    Tuple<int> tuple(1, 2);
    tuple.setElem1(2);
    printf("elem1: %d\n", tuple.getElem1());
}

int main() {
    showFuncTemplateMotivation();
    showAllowedType1();
    showAllowedType2();
    showBetterMax();
    showFuncSpecialization();
    showClassTemplate();
    showClassTemplateMethods();
}
