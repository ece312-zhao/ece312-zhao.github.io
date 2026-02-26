
// FREE RESPONSE EXAMPLE FROM RECITATION : ONE POSSIBLE SOLUTION
// Write code that can keep track of books in a library. Each Book struct should record book title,
// author, and whether the book is checked out. A Library should contain all Book structs, as well as 
// a total count of all books.
// Define the Book struct, and write a function that can create a Book given a title and author. 
// Define the Library struct, and write a function that can add a Book to the Library. 
// Finally, write a function that will sort Books in the Library by author name.

// NOTE: this is one of many possible solutions, and it is not necessarily the best solution


typedef struct {
    char* author;
    char* title;
    int checkedOut;
} Book;

typedef struct {
    Book* books;
    int totalBooks;
} Library;

Book* createBook(char* title, char* author) {

    Book* book = malloc(sizeof(Book));
    book->title = malloc(sizeof(char) * 100);
    book->author = malloc(sizeof(char) * 100);
    strcpy(book->title, title);
    strcpy(book->author, author);
    book->checkedOut = 0;
    return book;
}

// extra function for better modularity/readability
void copyBook(Book* source, Book* dest) {
    // assuming dest has not been initialized yet
    dest->author = malloc(sizeof(char) * 100);
    dest->title = malloc(sizeof(char) * 100);
    strcpy(dest->title, source->title);
    strcpy(dest->author, source->author);
    dest->checkedOut = source->checkedOut;
}

void addBook(Library* library, Book* toAdd) {
    library->books = realloc(library->books, sizeof(Book) * (library->totalBooks + 1)); // not very efficient to realloc every time, but it works
    copyBook(toAdd, &(library->books[library->totalBooks]));
    library->totalBooks++;
}

// helper function for sorting books
void swapBooks(Library* library, int idx) {
    char tempAuthor [100];
    char tempTitle[100];
    int tempCheckedOut;

    strcpy(tempAuthor, library->books[idx].author);
    strcpy(tempTitle, library->books[idx].title);
    tempCheckedOut = library->books[idx].checkedOut;

    strcpy(library->books[idx].author, library->books[idx+1].author);
    strcpy(library->books[idx].title, library->books[idx+1].title);
    library->books[idx].checkedOut = library->books[idx+1].checkedOut;

    strcpy(library->books[idx+1].author, tempAuthor);
    strcpy(library->books[idx+1].title, tempTitle);
    library->books[idx+1].checkedOut = tempCheckedOut;
}

void sortBooks(Library* library) {
    int swapped = 1;
    while(swapped) {
        swapped = 0;
        for(int i = 1; i < library->totalBooks; i++) {
            if (strcmp(library->books[i-1], library->books[i]) > 0) {
                swapBooks(library, i-1);
                swapped = 1;
            }
        }
    }
}

