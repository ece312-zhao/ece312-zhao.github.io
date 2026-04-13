#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Chapter 0: Dynamic array
// In C, we learned about dynamic memory management and dynamic arrays.
//
// A very small C-style dynamic array for integers only.
typedef struct {
    int *arr;
    size_t size, cap;
} DynArrC;

DynArrC *allocateDynArr() {
    DynArrC *darr = (DynArrC *)malloc(sizeof(DynArrC));
    if (!darr) return NULL;

    darr->size = 0;
    darr->cap = 10;
    darr->arr = (int *)calloc(darr->cap, sizeof(*darr->arr));
    if (!darr->arr) {
        free(darr);
        return NULL;
    }
    return darr;
}

void freeDynArr(DynArrC *darr) {
    if (darr) {
        free(darr->arr);
    }
    free(darr);
}

void appendElemDynArr(DynArrC *darr, int elem) {
    if (!darr) return;

    if (darr->size == darr->cap) {
        size_t new_cap = darr->cap * 2;
        int *new_arr = (int *)realloc(darr->arr, new_cap * sizeof(int));
        if (!new_arr) {
            return; // Keep the old array unchanged if realloc fails
        }
        darr->cap = new_cap;
        darr->arr = new_arr;
    }

    darr->arr[darr->size] = elem;
    darr->size += 1;
}

int getElemDynArr(DynArrC *darr, size_t idx) {
    if (darr && idx < darr->size) { // Don't forget the bounds check
        return darr->arr[idx];
    }
    return 0;
}

// However, the C implementation above has many limitations:
// * It only stores integers
// * It requires manual memory management
// * It does not support the array subscript [] operator on DynArrC itself

// Chapter 1: C++'s Standard Template Library (STL) provides std::vector,
// a well-implemented dynamic array container.
//
// Despite the name "vector", it is not limited to mathematical vectors.
// It can store many kinds of objects.

// A toy class that will be used throughout the example
class Foo {
    int x, y;

public:
    Foo() : x(0), y(0) {} // Default constructor

    Foo(int x, int y) : x(x), y(y) {
        printf("Constructor!\n");
    }

    Foo(const Foo &other) : x(other.x), y(other.y) {
        printf("Copy Constructor!\n");
    }
};

// Make std::vector and many more identifiers accessible without std::
using namespace std;

void showVector() {
    // We can optionally bring in a single name:
    using std::vector;

    // std::vector is a template class (will be covered next lecture):
    // the same container idea can work for different element types.
    vector<int> vec1;                      // A vector of int
    vector<double> vec2 = {0.1, 0.2, 0.3}; // A vector of double (initialized with 3 elements)
    vector<Foo> vecFoo;                    // A vector of objects
    vector<Foo *> vecFooPtr;               // A vector of pointers to objects

    // We can access vector elements through [] or .at()
    printf("2nd element is %f\n", vec2[1]);
    printf("2nd element is %f\n", vec2.at(1));
    printf("------\n");

    // [] does not do bounds checking.
    // Accessing an out-of-range index with [] is undefined behavior.
    //
    // .at() checks bounds and throws an exception (will be covered next week)
    // if the index is invalid.
    // If that exception is not handled, the program will terminate.
    //
    // printf("4th element is %f\n", vec2[3]);   // Undefined behavior
    // printf("4th element is %f\n", vec2.at(3)); // Throws an exception

    // Append elements to the back of the vector
    vec1.push_back(1);
    printf("vec1 size: %zu; cap: %zu\n", vec1.size(), vec1.capacity());

    vec1.push_back(2);
    printf("vec1 size: %zu; cap: %zu\n", vec1.size(), vec1.capacity());

    vecFoo.reserve(4); // Reserve space for at least 4 Foo objects

    // push_back vs emplace_back
    Foo foo;
    printf("------\n");
    vecFoo.push_back(foo); // Inserts an existing object at the end
    printf("------\n");
    vecFoo.emplace_back(10, 20); // Constructs a Foo directly at the end
    printf("------\n");
    // For objects, emplace_back may avoid creating an extra temporary
    // and may avoid an extra copy/move.

    // Delete the last element from the vector
    vecFoo.pop_back();

    // Iterating through the vector using array indexing.
    // A better way is discussed in Chapter 3
    for (size_t i = 0; i < vec1.size(); ++i) {
        printf("idx=%zu: %d\n", i, vec1[i]);
    }
    printf("------\n");

    // Note that we do not manually free the memory used by vector.
    // The vector object cleans up its own resources automatically.
}

// Chapter 2: std::list
// STL also provides an implementation of a doubly linked list.
void showList() {
    std::list<Foo> listFoo;                   // A linked list storing Foo
    std::list<int> listInt = {1, 2, 3, 4, 5}; // An int linked list

    printf("Front: %d, Back: %d\n", listInt.front(), listInt.back());
    printf("------\n");

    // list also has push_back and emplace_back. Both are O(1) operations.
    listFoo.push_back(Foo(1, 2));
    listFoo.emplace_back(1, 2);
    printf("------\n");

    // list also supports push_front and emplace_front (both are O(1)), which vector does not
    listInt.push_front(0);
    listInt.emplace_front(-1);

    // Deleting from the front/back of a list is also O(1)
    listInt.pop_back();
    listInt.pop_front();

    // However, list does not support array subscript [].
    // So how do we iterate through it?
}

// Chapter 3: Iterator
// An iterator is an object representing a position in a container.
// It lets us traverse the container while hiding implementation details.
typedef struct ListC {
    int data;
    struct ListC *next;
} ListC;

void showIterator() {
    // Think about how we traverse a C string / char array
    char s[] = "We live in a twilight world";
    for (char *ptr = s; *ptr != '\0'; ++ptr) {
        printf("%c", *ptr);
    }
    printf("\n------\n");

    // This is how we traverse a linked list in C
    ListC node = {0, NULL};
    for (ListC *cur = &node; cur; cur = cur->next) {
        printf("%d ", cur->data);
    }
    printf("\n------\n");

    // Iterators generalize this idea and hide implementation details
    std::list<int> listInt = {1, 2, 3};

    // Iterator objects
    std::list<int>::iterator begin = listInt.begin();
    std::list<int>::iterator end = listInt.end(); // One past the last element

    for (std::list<int>::iterator it = begin; it != end; ++it) {
        printf("%d ", *it);
    }
    printf("\n------\n");

    // Use auto to avoid writing the long iterator type name.
    // Compiler is smart enough to figure out the type of begin2.
    // Note that auto has a different meaning in C!
    auto begin2 = listInt.begin();
    for (auto it = begin2; it != listInt.end(); ++it) {
        printf("%d ", *it);
    }
    printf("\n------\n");

    // An even simpler way to iterate over a container: Range-based for loop
    for (int d : listInt) {
        // Roughly like:
        // for (auto it = listInt.begin(); it != listInt.end(); ++it) {
        //     auto d = *it;
        // }
        printf("%d ", d);
    }
    printf("\n------\n");

    // Works on vectors and many other container/range types too
    std::vector<int> vecInt = {4, 5, 6};
    for (auto d : vecInt) {
        printf("%d ", d);
    }
    printf("\n------\n");

    // If we want to modify the actual elements, use a reference
    for (auto &d : vecInt) {
        d += 1;
    }

    for (auto d : vecInt) {
        printf("%d ", d);
    }
    printf("\n------\n");

    // When iterating through a container of objects, const reference is often
    // preferred if we do not want to modify the objects or make copies.
    std::vector<Foo> vecFoo;
    vecFoo.emplace_back(1, 2);
    vecFoo.emplace_back(2, 3);

    for (const auto &foo : vecFoo) {
        (void)foo; // Read from foo here
    }
    printf("------\n");

    // WARNING:
    // Be careful about adding/removing elements while iterating.
    // Some operations can invalidate iterators.
    for (auto it = vecInt.begin(); it != vecInt.end(); ++it) {
        if (*it % 2) {
            // If vecInt grows and reallocates, 'it' may become invalid.
            // Using an invalid iterator is undefined behavior.
            //
            // This is intentionally a BAD example to illustrate the danger.
            // Example of code you should NOT write:
            // vecInt.push_back(*it * 3 + 1);
        }
    }

    // Some containers support erasing while iterating, but you must do it carefully.
    for (auto it = listInt.begin(); it != listInt.end(); ) {
        if (*it % 2 == 0) {
            it = listInt.erase(it); // erase returns the next valid iterator
        } else {
            ++it;
        }
    }

    for (int d : listInt) {
        printf("%d ", d);
    }
    printf("\n------\n");
}

// Chapter 4: std::set and std::unordered_set
// These containers store unique elements.
void showSet() {
    std::set<int> intSet = {1, 1, 2, 2, 3, 3};

    // Range-based for loops also work here
    printf("Set size: %zu\n", intSet.size());
    for (auto d : intSet) {
        printf("%d ", d);
    }
    printf("\n------\n");

    // Testing for membership
    if (intSet.find(3) != intSet.end()) {
        printf("3 is in the set\n");
    }

    printf("4 is in the set? %zu\n", intSet.count(4));
    printf("------\n");

    // Adding/removing elements
    intSet.insert(10);
    intSet.emplace(10); // No effect here because 10 is already present
    intSet.erase(2);

    std::unordered_set<int> unorderedSet = {1, 2, 3};
    for (auto d : unorderedSet) {
        printf("%d ", d);
    }
    printf("\n------\n");

    // unordered_set has a similar API to set, but uses hashing.
    // It does not keep elements in sorted order.
    //
    // In general:
    // * set keeps elements ordered
    // * unordered_set does not keep order, but often has faster average lookup
    //
    // For set, keys must work with the comparison used by the set.
    // For unordered_set, keys must be hashable and equality-comparable.
}

// Chapter 5: std::map and std::unordered_map
// A map stores key-value pairs, like a dictionary.
void showMap() {
    std::map<int, int> gradeBook = {{1, 90}, {2, 100}}; // student id -> grade

    // Accessing the value associated with a given key
    printf("Student 2's grade is %d\n", gradeBook[2]);
    printf("Student 2's grade is %d\n", gradeBook.at(2));
    printf("------\n");

    // operator[] inserts a new key if the key is not already present.
    // The mapped value is value-initialized (0 for int here).
    //
    // .at() throws an exception if the key does not exist.
    printf("Student 3's grade is %d\n", gradeBook[3]); // gradeBook[3] is 0 afterwards
    // printf("Student 4's grade is %d\n", gradeBook.at(4)); // Throws

    printf("------\n");

    gradeBook[5] = 90; // A new entry is created automatically if needed

    for (const auto &p : gradeBook) {
        // p is a reference to a pair storing key and value
        // It's equivalent to "for (const std::pair<const int, int> &p : gradeBook)"
        printf("Student %d's grade is %d\n", p.first, p.second);
    }
    printf("------\n");

    // map supports other APIs similar to set
    printf("Student 8 is in the map? %zu\n", gradeBook.count(8));
    printf("------\n");

    // unordered_map uses hashing and does not keep keys sorted.
    std::unordered_map<int, int> unsortedGradeBook = {{1, 90}, {2, 100}};
    for (const auto &p : unsortedGradeBook) {
        printf("Student %d's grade is %d\n", p.first, p.second);
    }
}

int main() {
    showVector();
    showList();
    showIterator();
    showSet();
    showMap();
}
