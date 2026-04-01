#include <cstdio>
#include <string>

using namespace std;

// This version is "bad" because each block class repeats the same ideas:
// - each class stores x, y, z
// - each class has a constructor initializing x, y, z
// - each class has an isSolid() function
// - each class has a move() function
// - each class has an interact() function
//
// When many classes share the same structure and behavior,
// that is a hint that a common base class may be missing.

class WoodBlock {
private:
    // Position data appears here...
    double x, y, z;

    // ...plus WoodBlock-specific data.
    const string wood_type;

public:
    // This constructor initializes x, y, z...
    // but LavaBlock and TNTBlock will repeat the same pattern.
    WoodBlock(double x, double y, double z, const char *type)
        : x(x), y(y), z(z), wood_type(type) {}

    // This function is part of the common "block interface":
    // a block can answer whether it is solid.
    //
    // But each class defines its own version separately,
    // even though the idea is shared by all block types.
    bool isSolid() const {
        return true;
    }

    // Another repeated operation: every block can move to a new location.
    // The implementation is identical in all three classes.
    void move(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Another repeated idea: every block can be interacted with.
    // The message is different, but the overall operation is the same.
    void interact() {
        printf("Chopping down a %s wood block at (%.2f, %.2f, %.2f)\n",
               wood_type.c_str(), x, y, z);
    }
};

class LavaBlock {
private:
    // Same three coordinates again.
    // Repeated data members are often a sign that a base class is missing.
    double x, y, z;

public:
    // Same constructor shape again.
    LavaBlock(double x, double y, double z) : x(x), y(y), z(z) {}

    // Same kind of function again.
    bool isSolid() const {
        return false;
    }

    // Same exact move() implementation again.
    void move(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Same kind of function again.
    void interact() {
        printf("Scooping a lava block at (%.2f, %.2f, %.2f)\n", x, y, z);
    }
};

class TNTBlock {
private:
    // Same coordinates yet again.
    double x, y, z;

    // TNTBlock-specific data.
    bool lit;

public:
    // Same constructor pattern, plus one extra member.
    TNTBlock(double x, double y, double z) : x(x), y(y), z(z), lit(false) {}

    // Same common interface idea.
    bool isSolid() const {
        return true;
    }

    // Same exact move() implementation again.
    void move(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Same common operation idea: interact with a block.
    // This one also changes object state by lighting the TNT.
    void interact() {
        printf("Igniting a TNT block at (%.2f, %.2f, %.2f)\n", x, y, z);
        lit = true;
    }

    bool isLit() const {
        return lit;
    }
};

// This is another sign the design is not scaling well.
//
// We need one overload for WoodBlock...
void interactBlock(WoodBlock *block) {
    block->interact();
}

// ...another overload for LavaBlock...
void interactBlock(LavaBlock *block) {
    block->interact();
}

// ...and another overload for TNTBlock.
//
// If we add 10 more block types, we may need 10 more overloads.
// That is repetitive and hard to maintain.
void interactBlock(TNTBlock *block) {
    block->interact();
}

// A better design would introduce a common Block base class.
// Then the shared data (x, y, z) and shared operations
// (move, isSolid, interact) could live in one place.
//
// That would also let us write just one function:
//
//     void interactBlock(Block *block) {
//         block->interact();
//     }
//
// This avoids repetition and lets us treat all block types uniformly.
