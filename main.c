#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char title[50];
    char author[50];
    char category[50];
    int pages;
    int price;
    int quantity;
} Book;

typedef struct{
    int id;
    char name[50];
}borrower;

void saveBorrowedBook(Book book, int borrowed, borrower user);
void returnBorrowedBook(Book book, int borrowed, borrower user);
int total_number_of_books = 0;

// Function to add a book
void addBook(Book book[], int *count) {
    printf("Enter book title: ");
    scanf("%s", book[*count].title);
    printf("Enter author name: ");
    scanf("%s", book[*count].author);
    printf("Enter category: ");
    scanf("%s", book[*count].category);
    printf("Enter number of pages: ");
    scanf("%d", &book[*count].pages);
    printf("Enter number of price: ");
    scanf("%d", &book[*count].price);
    printf("Enter quantity: ");
    scanf("%d", &book[*count].quantity);
    (*count)++;
    total_number_of_books += book[*count].quantity;
}

// Function to save the books
void saveBooks(Book book[], int count) {
    FILE *file = fopen("books.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %s %d %d %d\n", book[i].title, book[i].author, book[i].category, book[i].pages, book[i].price, book[i].quantity);
    }
    fclose(file);
}

// Function to load the books
void loadBooks(Book book[], int *count) {
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%s %s %s %d %d %d", book[*count].title, book[*count].author, book[*count].category, &book[*count].pages, &book[*count].price, &book[*count].quantity);
        (*count)++;
      total_number_of_books += book[*count].quantity;
    }
    fclose(file);
}

// Function to display the books
void displayBook(Book book[], int count) {
    printf("%-20s %-20s %-20s %-10s %-10s\n", "Book title", "Author name", "Category", "Pages", "quantity");
    for (int i = 0; i < count; i++) {
        printf("%-20s %-20s %-20s %-10d %-10d\n", book[i].title, book[i].author, book[i].category, book[i].pages, book[i].quantity);
    }
}

// Function to display the books by author
void displayBooksByAuthor(Book book[], int count, char author[]) {
    printf("%-20s %-20s %-20s %-10s %-10s\n", "Book title", "Author name", "Category", "Pages", "quantity");
    for (int i = 0; i < count; i++) {
        if (strcmp(book[i].author, author) == 0) {
            printf("%-20s %-20s %-20s %-10d %-10d %-10d\n", book[i].title, book[i].author, book[i].category, book[i].pages, book[i].price, book[i].quantity);
        }
    }
}

// Function to display the books by category
void displayBooksByCategory(Book book[], int count, char category[]) {
    printf("%-20s %-20s %-20s %-10s %-10s\n", "Book title", "Author name", "Category", "Pages", "quantity");
    for (int i = 0; i < count; i++) {
        if (strcmp(book[i].category, category) == 0) {
            printf("%-20s %-20s %-20s %-10d %-10d %-10d\n", book[i].title, book[i].author, book[i].category, book[i].pages, book[i].price, book[i].quantity);
        }
    }
}

// Function to find a book by its title
int findBookByTitle(Book book[], int count, char title[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(book[i].title, title) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to borrow a book
void borrowBook(Book book[], int count) {
    char title[50];
    borrower user;
    char name[50];
    printf("Please enter your id: ");
    scanf("%d", &user.id);
    printf("Please enter your name: ");
    scanf("%s", name);
    strcpy(user.name, name);
    printf("Enter book title: ");
    scanf("%s", title);
    int index = findBookByTitle(book, count, title);
    if (index == -1) {
        printf("Book not found!\n");
        return;
    }
    if (book[index].quantity <= 0) {
        printf("Book is not available!\n");
        return;
    }
    book[index].quantity--;
  total_number_of_books -= 1;
    printf("Book borrowed successfully!\n");
    saveBooks(book, count);
    saveBorrowedBook(book[index], 1, user);
}

// Function to return a book
void returnBook(Book book[], int count) {
    char title[50];
    borrower user;
    char name[50];
    printf("Please enter your id: ");
    scanf("%d", &user.id);
    printf("Please enter your name: ");
    scanf("%s", name);
    strcpy(user.name, name);
    printf("Enter book title: ");
    scanf("%s", title);
    int index = findBookByTitle(book, count, title);
    if (index == -1) {
        printf("Entry not found!\n");
        return;
    }
    book[index].quantity++;
  total_number_of_books += 1;
    printf("Book returned successfully!\n");
    saveBooks(book, count);
    // book[index].quantity--;
    returnBorrowedBook(book[index], 0, user);
}

// Function to save the borrowed book details
void saveBorrowedBook(Book book, int borrowed, borrower user) {
    FILE *file = fopen("borrower.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    char action[10];
    if (borrowed) {
        strcpy(action, "Borrowed");
    }
    else {
      strcpy(action, "Returned");
    }
    fprintf(file, "%d %s %s %s %s\n", user.id, user.name, book.title, book.author, action);
    fclose(file);
}

void displayBookCount(int count) {
  printf("Total number of books = %d\n", count); 
}

void returnBorrowedBook(Book book, int borrowed, borrower user) {
    
  const char *file_name = "borrower.txt";

    const char *temp_file_name = "temp.txt";

    FILE *originalFile, *tempFile;
    char line[100];

    originalFile = fopen(file_name, "r");
    tempFile = fopen(temp_file_name, "w");

    if (originalFile == NULL || tempFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    char action[100];

    if(borrowed){
      strcpy(action, "Borrowed");
    }

    snprintf(line, sizeof(line), "%d %s %s %s %s\n", user.id, user.name, book.title, book.author, action);

    while (fgets(line, sizeof(line), originalFile) != NULL) {
        if (strstr(line, book.title) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(originalFile);
    fclose(tempFile);

    if (remove(file_name) != 0) {
        perror("Error deleting original file");
        exit(EXIT_FAILURE);
    }

    if (rename(temp_file_name, file_name) != 0) {
        perror("Error renaming file");
        exit(EXIT_FAILURE);
    }

    printf("Book entry deleted successfully.\n");
}

int main() {
 Book book[50];
 int count = 0;
 int choice;

 loadBooks(book, &count);

 while (1) {
    printf("\n\nWelcome to Library Management System\n");
    printf("Please choose an option: \n");
    printf("1. Add a book\n");
    printf("2. Display book information\n");
    printf("3. Display books by author\n");
    printf("4. Display books by category\n");
    printf("5. Display book count\n");
    printf("6. Borrow a book\n");
    printf("7. Return a book\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      addBook(book, &count);
      saveBooks(book, count);
      break;
    case 2:
      displayBook(book, count);
      break;
    case 3:
      {
        char author[50];
        printf("Enter author name: ");
        scanf("%s", author);
        displayBooksByAuthor(book, count, author);
      }
      break;
    case 4:
      {
        char category[50];
        printf("Enter category: ");
        scanf("%s", category);
        displayBooksByCategory(book, count, category);
      }
    case 5:
      {
      displayBookCount(count);
      break;
      }
    case 6:
      borrowBook(book, count);
      break;
    case 7:
      returnBook(book, count);
      break;
    case 8:
      return 0;
    default:
      printf("Invalid choice!\n");
    }
 }
 return 0;
}
