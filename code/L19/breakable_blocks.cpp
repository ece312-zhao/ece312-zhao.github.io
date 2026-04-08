#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

/*
===============================================================================
Section 0. Static binding vs late binding
===============================================================================

"Static binding" means the compiler already knows exactly what to call.
"Late binding" defers function selection until execution time.

A function pointer is one simple way to achieve late binding.
Virtual functions are a different mechanism.
*/

int add(int x, int y) {
    return x + y;
}

int sub(int x, int y) {
    return x - y;
}

// A function pointer type: takes (int, int), returns int
typedef int (*FnPtr)(int, int);

void showBinding() {
    int a = 10, b = 20;

    // Direct call: the compiler knows the target immediately.
    int c = add(a, b);
    printf("Direct call result: %d\n", c);

    // Randomly pick an operation at runtime.
    srand((unsigned)time(nullptr)); // Initialize the seed of random number generator
    char op = rand() % 2 ? '+' : '-';

    FnPtr fn = nullptr;
    switch (op) {
        case '+': fn = add; break;
        case '-': fn = sub; break;
    }

    // Indirect call through a function pointer.
    // We do not know which function is called until runtime.
    int res = fn(a, b);
    printf("Runtime-selected call (%c): %d\n", op, res);
}

/*
===============================================================================
Section 1. More virtual functions and polymorphism
===============================================================================
*/

class Foo {
    double x;

public:
    void print() {
        printf("Foo::print\n");
    }

    virtual void printVirtual() {
        printf("Foo::printVirtual\n");
    }
};

class Bar : public Foo {
public:
    void print() {
        printf("Bar::print\n");
    }

    void printVirtual() override {
        printf("Bar::printVirtual\n");
    }
};

class FooNoVirtual {
    double x;

public:
    void print() {
        printf("FooNoVirtual::print\n");
    }

    void printVirtual() {
        printf("FooNoVirtual::printVirtual\n");
    }
};

void showVirtualBasics() {
    Bar bar;
    Foo *foo = &bar;

    // Non-virtual call:
    // chosen using the static type of the pointer (Foo *)
    foo->print(); // Foo::print

    // Virtual call:
    // chosen using the dynamic type of the object (Bar)
    foo->printVirtual(); // Bar::printVirtual

    // A class with virtual functions is usually larger because it stores
    // information needed for dynamic dispatch.
    printf("sizeof(Foo) = %zu\n", sizeof(Foo));
    printf("sizeof(FooNoVirtual) = %zu\n", sizeof(FooNoVirtual));
}

/*
===============================================================================
Section 1.1 Virtual calls inside constructors/destructors
===============================================================================

Important rule:
During base-class construction, the derived part does not exist yet.

So if a base-class constructor calls a virtual function,
C++ does NOT perform dynamic dispatch to a child override.

The same warning applies to destructors.
*/

class BaseProcessor {
public:
    BaseProcessor() {
        // Calls BaseProcessor::init(), not the child's override.
        // No dynamic dispatching
        init();
    }

    virtual void init() {
        printf("BaseProcessor::init\n");
    }

    void runInsn() {
        printf("Running instruction\n");
        postInsnExecution(); // Dynamic dispatch works here
    }

    virtual void postInsnExecution() {
        printf("Nothing to do\n");
    }
};

class ProcessorWithStats : public BaseProcessor {
public:
    ProcessorWithStats() : insnCnt(0) {}

    void init() override {
        printf("ProcessorWithStats::init\n");
    }

    void postInsnExecution() override {
        insnCnt += 1;
    }

    size_t insnCnt;
};

void showConstructorVirtualRule() {
    // You should see BaseProcessor::init
    ProcessorWithStats proc;

    // Here dynamic dispatch works normally.
    proc.runInsn();

    // postInsnExecution() increments the counter.
    printf("Instruction count: %zu\n", proc.insnCnt);
}

/*
===============================================================================
Section 2. Pure virtual functions and abstract classes
===============================================================================

A pure virtual function uses "= 0".

A class with at least one pure virtual function is abstract:
- you cannot instantiate it directly
- it is meant to be a base class
*/

class BreakableBlock {
protected:
    const string blkName;

public:
    BreakableBlock(const char *name) : blkName(name) {
        printf("Constructing a breakable %s block\n", name);
    }

    // Use a virtual destructor when objects may be deleted
    // through a base-class pointer.
    virtual ~BreakableBlock() {}

    // Pure virtual function
    virtual void breakBlock() = 0;
};

class WoodBlock : public BreakableBlock {
public:
    WoodBlock() : BreakableBlock("wood") {}
    // Still abstract: breakBlock() is not implemented here.
};

class OakBlock : public WoodBlock {
public:
    OakBlock() : WoodBlock() {}

    void breakBlock() override {
        printf("Breaking an oak block\n");
    }
};

void showPureVirtualFunctions() {
    // These do not compile:
    // BreakableBlock block("block");
    // WoodBlock wood;

    // OakBlock implements breakBlock(), so it is concrete.
    OakBlock oak;
    oak.breakBlock();
}

/*
===============================================================================
Section 3. Interface-style abstract classes
===============================================================================
*/

// This is a pure abstract class:
// no data members, and all member functions are virtual.
class Flammable {
public:
    virtual ~Flammable() {}
    virtual void ignite() = 0;
    virtual void putOut() = 0;
};

class Torch : public Flammable {
public:
    void ignite() override {
        printf("Lighting a torch\n");
    }

    void putOut() override {
        printf("Putting out a torch\n");
    }
};

class Tradable {
public:
    virtual ~Tradable() {}
    virtual int sell() = 0;
    virtual int buy() = 0;
};

/*
Multi-inheritance example:
Paper is both Flammable and Tradable.

For intro teaching, it is enough to say:
- multiple inheritance exists
- it is commonly used for interface-style base classes
- it is trickier with stateful concrete base classes
*/
class Paper : public Flammable, public Tradable {
public:
    void ignite() override {
        printf("Burning a piece of paper\n");
    }

    void putOut() override {
        printf("Putting out a paper fire\n");
    }

    int sell() override {
        printf("Selling a piece of paper\n");
        return 10;
    }

    int buy() override {
        printf("Buying a piece of paper\n");
        return -10;
    }
};

// Any type that implements Flammable can be used here.
void setOnFire(Flammable *items[], size_t sz) {
    for (size_t i = 0; i < sz; i++) {
        items[i]->ignite();
    }
}

void showInterfaces() {
    Flammable *items[] = {
        new Torch,
        new Paper,
        new Torch,
    };

    setOnFire(items, sizeof(items) / sizeof(items[0]));

    // Clean up heap objects.
    for (size_t i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
        delete items[i];
    }
}

/*
===============================================================================
Section 4. Downcasting and dynamic_cast
===============================================================================

Upcasting is safe:
- derived class * -> base class *

Downcasting is not automatically safe:
- base class * -> derived class *

Use dynamic_cast when you need to ask:
"Is this base pointer really pointing to that derived type?"

Requirement:
dynamic_cast works only if the base class has at least one virtual function.
*/

class TreasureMap : public Paper {
public:
    void view() {
        printf("Viewing the treasure map\n");
    }
};

class EnchantedBook : public Paper {
public:
    void enchant() {
        printf("Enchanting with a book\n");
    }
};

// Behavior:
// - plain Paper: no special magic
// - TreasureMap: view it
// - EnchantedBook: use its enchantment
void paperMagic(Paper *paper) {
    /*
    Do not do this with a C-style cast:

        TreasureMap *treasure = (TreasureMap *)paper;

    That cast can blindly pretend the object is a TreasureMap even when it is not.
    Using the result would be undefined behavior.
    */

    TreasureMap *treasure = dynamic_cast<TreasureMap *>(paper);
    if (treasure != nullptr) {
        treasure->view();
        return;
    }

    // Compact form: declare and test in one statement.
    if (EnchantedBook *book = dynamic_cast<EnchantedBook *>(paper)) {
        book->enchant();
        return;
    }

    printf("No magic available\n");
}

/*
Better design note:
If you own the base class, a virtual function is often better than downcasting.
For example, Paper could define virtual void magic().
*/

void showDowncasting() {
    Paper paper;
    TreasureMap treasure;
    EnchantedBook book;

    paperMagic(&paper);
    paperMagic(&treasure);
    paperMagic(&book);
}

/*
===============================================================================
Section 5. The override keyword
===============================================================================

Always use override when overriding a virtual function.

Why?
Because the compiler will catch mistakes such as:
- wrong function name
- wrong parameter type
- wrong const-qualification
*/

class Base {
public:
    virtual void add(int x) {
        printf("Base::add(%d)\n", x);
    }
};

class Derived : public Base {
public:
    void add(int x) override {
        printf("Derived::add(%d)\n", x);
    }

    // These would fail to compile:
    // void Add(int x) override {}      // wrong name
    // void add(double x) override {}   // wrong parameter type
};

/*
===============================================================================
main()
===============================================================================
*/

int main() {
    showBinding();
    printf("\n");

    showVirtualBasics();
    printf("\n");

    showConstructorVirtualRule();
    printf("\n");

    showPureVirtualFunctions();
    printf("\n");

    showInterfaces();
    printf("\n");

    showDowncasting();
    printf("\n");

    return 0;
}
