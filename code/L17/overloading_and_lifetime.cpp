#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>


// Function overloading
// Functions with the same name but different argument lists are different functions
void print_it(long x) {
    printf("%ld\n", x);
}

void print_it(const char *s) {
    if (s) {
        printf("%s\n", s);
    } else {
        printf("NULL pointer!\n");
    }
}

void func_overloading_demo() {
    printf("\n--- Function overloading demo begins ---\n");
    print_it(528491l);
    print_it("Hello");

    // This is a common trap! NULL from C has a type of an integer!
    // The compiler will pick the "long" version of print_it
    print_it(NULL);
    print_it(nullptr); // Use nullptr instead!
    printf("--- Function overloading demo ends ---\n\n");
}

// Operators can also be overloaded
struct Complex {
    double real, im;

    // Change the meaning of Complex + Complex
    //                        (this) + (other)
    Complex operator+(const Complex &other) const {
        return Complex{real + other.real, im + other.im};
    }

    // Change the meaning of Complex + double
    //                        (this) + (other)
    Complex operator+(double real) const { // function + operator overloading
        return Complex{this->real + real, im};
    }

    // Change the meaning of Complex += Complex
    Complex &operator+=(const Complex &other) {
        real += other.real;
        im += other.im;

        // We can define the return type as void, but returning *this allows us
        // to do chaining like: Complex c3 = c2 += c1.
        return *this;
    }

    void print() const {
        printf("%f+%fi\n", real, im);
    }
};

// You can also overload an operator outside of a struct/class
Complex operator-(const Complex &lhs, const Complex &rhs) {
    return Complex{lhs.real - rhs.real, lhs.im - rhs.im};
}

void op_overloading_demo() {
    printf("\n--- Operator overloading demo begins ---\n");

    Complex c1{1, 0}, c2{0, 1};
    Complex c3 = c1 + c2;
    c3.print();

    c3 = c1.operator+(c2); // Same as c1 + c2
    c3.print();

    c1 += c2;
    c1.print();

    c1.operator+=(c2); // Again, same as c1 += c2
    c1.print();

    Complex c4 = c3 + 10; // Calls the "double" version of the operator+ overload
    c4.print();

    Complex c5 = c4 - c3;
    c5.print();

    c5 = operator-(c4, c3); // Same as c4 - c3
    c5.print();

    // Off topic: rvalue reference
    // In C++, you will sometimes see this:
    Complex &&ref = c2 + c4;
    // This is called a rvalue reference. Despite the jargon, it's simply
    // a reference to a temporary object returned by c2 + c4. Why do we
    // need this? It will be relevant for the move semantics in the next section :).
    ref.print();

    printf("\n--- Operator overloading demo ends ---\n");
}

// "Rule of three/five"
class Item {
public:
    Item(const char *s) {
        if (s) {
            size_t sz = strlen(s) + 1;

            // similar to malloc(sz * sizeof(char)); Not zero-initialized!
            name = new char[sz];
            strcpy(name, s);
            printf("Allocating name at %p\n", (void *)name);
        } else {
            name = nullptr;
        }
    }

    Item(const Item &o) : Item(o.name) {}

    Item(Item &&o) {
        name = o.name;
        o.name = nullptr;
        printf("Moved name to %p\n", (void *)name);
    }

    // Item &operator=(const Item &o) = default; // use the default method
    // Item &operator=(const Item &o) = delete;  // disallow copying

    Item &operator=(const Item &o) {
        printf("Assignment operator!\n");
        if (this != &o) {
            if (o.name) {
                char *buf = new char[strlen(o.name) + 1];
                strcpy(buf, o.name);

                delete[] name;
                name = buf;
            } else {
                delete[] name;
                name = nullptr;
            }
        }
        return *this;
    }

    Item &operator=(Item &&o) {
        printf("Move operator\n");
        if (this != &o) {
            delete[] name;
            name = o.name;
            o.name = nullptr;
        }
        return *this;
    }

    ~Item() {
        printf("Deleting name at %p", (void *)name);
        if (name) {
            printf(" (name=%s)\n", name);
        } else {
            printf("\n");
        }
        delete[] name;
    }

    void print() const {
        if (name) {
            printf("Item %s\n", name);
        } else {
            printf("Empty item\n");
        }
    }

private:
    char *name;
};

// "Rule of zero"
// BetterItem uses std::string, which already "knows" how to clean up, copy, and move.
// Therefore, the class probably doesn't need destructor, copy constructor/assignment,
// and move constructor/assignment
class BetterItem {
public:
    BetterItem(const char *n) : name(n) {}

    void print() const {
        printf("Item %s\n", name.c_str());
    }

private:
    std::string name;
};

void lifetime_demo() {
    printf("\n--- Object lifetime demo begins ---\n");

    Item *s = new Item("Keyboard");
    s->print();
    delete s;
    puts("---");

    Item s1("Book");
    s1.print();
    puts("---");

    Item s2("Car");
    s2 = s1; // Copy assignment. Handled by the "Item &operator=(const Item &o)""
    s2.print();
    puts("---");

    Item s3(s1); // Copy constructor. Handled by "Item(const Item &o)"
    s3.print();
    puts("---");

    s3 = Item("Chair"); // Move assignment. Handled by "Item &operator=(Item &&o)"
    s3.print();
    puts("---");

    // The following requires the "-fno-elide-constructors" flag to work
    Item s4(Item("Mug")); // Move constructor. Handled by "Item(Item &&o)"
    s4.print();
    puts("---");

    BetterItem s5("Pen");
    s5.print();
    printf("\n--- Object lifetime demo ends ---\n");
}

int main() {
    func_overloading_demo();
    op_overloading_demo();
    lifetime_demo();
    return 0;
}
