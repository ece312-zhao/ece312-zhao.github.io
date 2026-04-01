#include <cstdio>
#include <string>

using namespace std;

// This is our base class.
// It stores the data shared by all block types.
class Block {
protected:
    const string blk_name; // Common attribute: every block has a name
    double x, y, z;        // Common attribute: every block has a location
    bool is_solid;         // Common attribute: every block knows whether it is solid

public:
    // Base-class constructor.
    // Derived classes will call this to initialize the shared Block part.
    Block(const char *name, double x, double y, double z, bool solid)
        : blk_name(name), x(x), y(y), z(z), is_solid(solid) {
        printf("Constructing a generic block\n");
    }

    // Virtual destructor.
    // This is important because we may delete a derived object
    // through a Block * pointer.
    virtual ~Block() {
        printf("Destructing a generic block\n");
    }

    // Common method inherited by all child classes.
    bool isSolid() const { return is_solid; }

    // Another common method inherited by all child classes.
    void move(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Non-virtual function.
    // Calls to describe() use static dispatch:
    // the compiler chooses the version based on the variable type.
    void describe() const {
        printf("This is a generic %s block\n", blk_name.c_str());
    }

    // A virtual function.
    // Child classes can override this to provide specialized behavior.
    virtual void interact() {
        printf("Interacting with a %s block at (%.2f, %.2f, %.2f)\n",
               blk_name.c_str(), x, y, z);
    }
};

// WoodBlock is a derived class.
// It inherits the common Block data and methods,
// and adds wood-specific data/behavior.
class WoodBlock : public Block {
private:
    const string wood_type; // Extra attribute needed only by wood blocks

public:
    // Constructor chaining:
    // first construct the Block part, then initialize wood_type.
    WoodBlock(double x, double y, double z, const char *type)
        : Block("Wood", x, y, z, true), wood_type(type) {
        printf("Constructing a %s wood block\n", wood_type.c_str());
    }

    // Derived destructor.
    // Because Block's destructor is virtual, deleting through Block *
    // will still call this first, then Block's destructor.
    ~WoodBlock() override {
        printf("Destructing a %s wood block\n", wood_type.c_str());
    }

    // This has the same name as Block::describe(),
    // but Block::describe() is NOT virtual.
    // So calls through Block * will still use Block::describe().
    void describe() const {
        printf("This is a %s wood block\n", wood_type.c_str());
    }

    // Override the parent class's virtual function.
    void interact() override {
        printf("Chopping down a %s wood block at (%.2f, %.2f, %.2f)\n",
               wood_type.c_str(), x, y, z);
    }
};

// Another derived class.
class LavaBlock : public Block {
public:
    LavaBlock(double x, double y, double z)
        : Block("Lava", x, y, z, false) {}

    void describe() const {
        printf("This is a lava block\n");
    }

    // Override Block::interact with lava-specific behavior.
    void interact() override {
        printf("Scooping a lava block at (%.2f, %.2f, %.2f)\n", x, y, z);
    }
};

// Another derived class.
class TNTBlock : public Block {
private:
    bool lit; // Extra attribute needed only by TNT blocks

public:
    TNTBlock(double x, double y, double z)
        : Block("TNT", x, y, z, true), lit(false) {}

    bool isLit() const { return lit; }

    void describe() const {
        printf("This is a TNT block\n");
    }

    // Override Block::interact with TNT-specific behavior.
    // This one also changes object state.
    void interact() override {
        printf("Igniting a TNT block at (%.2f, %.2f, %.2f)\n", x, y, z);
        lit = true;
    }
};

// This function takes a Block *.
// Because interact() is virtual, the correct version is chosen
// based on the actual object type at runtime.
void interactBlock(Block *block) {
    block->interact();
}

// This function shows static dispatch.
// describe() is NOT virtual, so the compiler chooses Block::describe()
// because block has type Block *.
void describeBlock(Block *block) {
    block->describe();
}

// This function also takes a Block *.
// Because Block has a virtual destructor, deleting a derived object
// through this pointer is safe.
void deleteBlock(Block *block) {
    delete block;
}

int main() {
    WoodBlock wood(1, 1, 1, "Oak");
    TNTBlock tnt(2, 2, 2);
    LavaBlock lava(3, 3, 3);

    printf("--- Direct calls ---\n");
    wood.describe();   // WoodBlock::describe
    tnt.describe();    // TNTBlock::describe
    lava.describe();   // LavaBlock::describe

    printf("\n--- Static dispatch through Block * ---\n");
    describeBlock(&wood); // Block::describe
    describeBlock(&tnt);  // Block::describe
    describeBlock(&lava); // Block::describe

    printf("\n--- Dynamic dispatch through Block * ---\n");
    interactBlock(&wood); // WoodBlock::interact
    interactBlock(&tnt);  // TNTBlock::interact
    interactBlock(&lava); // LavaBlock::interact

    printf("\n--- Virtual destructor demo ---\n");
    WoodBlock *cherry = new WoodBlock(1, 2, 3, "Cherry");
    deleteBlock(cherry);

    return 0;
}

/*
Knowledge points shown in this example

1. Inheritance
   - WoodBlock, LavaBlock, and TNTBlock inherit from Block.

2. Shared members in the base class
   - blk_name, x, y, z, and is_solid belong in Block.

3. Extra members in derived classes
   - WoodBlock adds wood_type.
   - TNTBlock adds lit.

4. Constructor chaining
   - The base-class constructor runs before the derived-class constructor.

5. Destructor chaining
   - The derived destructor runs before the base destructor.

6. Static dispatch
   - describe() is not virtual.
   - When called through Block *, the compiler chooses Block::describe()
     based on the pointer type.

7. Dynamic dispatch
   - interact() is virtual.
   - When called through Block *, the program chooses the correct
     overridden version at runtime based on the actual object type.

8. Virtual destructor
   - Deleting through Block * is safe because Block::~Block() is virtual.
*/
