#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100

typedef struct Book {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    struct Book* next;
} Book;

Book* head = NULL;
int book_count = 0;

Book* createBook(int id, const char* title, const char* author) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strncpy(newBook->title, title, MAX_TITLE_LENGTH);
    strncpy(newBook->author, author, MAX_AUTHOR_LENGTH);
    newBook->next = NULL;
    return newBook;
}

void addBook(const char* title, const char* author) {
    Book* newBook = createBook(++book_count, title, author);
    if (head == NULL) {
        head = newBook;
    } else {
        Book* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newBook;
    }
    printf("Book added successfully: %s by %s\n", title, author);
}

void removeBook(int id) {
    Book* current = head;
    Book* previous = NULL;
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Book not found with ID: %d\n", id);
        return;
    }
    if (previous == NULL) {
        head = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
    printf("Book with ID %d removed successfully.\n", id);
}

void listBooks() {
    Book* current = head;
    if (current == NULL) {
        printf("No books in the library.\n");
        return;
    }
    while (current != NULL) {
        printf("ID: %d, Title: %s, Author: %s\n", current->id, current->title, current->author);
        current = current->next;
    }
}

void searchBook(const char* title) {
    Book* current = head;
    while (current != NULL) {
        if (strncmp(current->title, title, MAX_TITLE_LENGTH) == 0) {
            printf("Book found: ID: %d, Title: %s, Author: %s\n", current->id, current->title, current->author);
            return;
        }
        current = current->next;
    }
    printf("Book not found with title: %s\n", title);
}

void saveToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    Book* current = head;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s\n", current->id, current->title, current->author);
        current = current->next;
    }
    fclose(file);
    printf("Books saved to file: %s\n", filename);
}

void loadFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char title[MAX_TITLE_LENGTH];
        char author[MAX_AUTHOR_LENGTH];
        sscanf(line, "%d,%99[^,],%99[^\n]", &id, title, author);
        Book* newBook = createBook(id, title, author);
        newBook->next = head;
        head = newBook;
    }
    fclose(file);
    printf("Books loaded from file: %s\n", filename);
}

int main() {
    int choice;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int id;
    char filename[100];

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. List Books\n");
        printf("4. Search Book\n");
        printf("5. Save to File\n");
        printf("6. Load from File\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // consume newline character left in buffer

        switch (choice) {
            case 1:
                printf("Enter title: ");
                fgets(title, MAX_TITLE_LENGTH, stdin);
                title[strcspn(title, "\n")] = 0;  // remove newline character
                printf("Enter author: ");
                fgets(author, MAX_AUTHOR_LENGTH, stdin);
                author[strcspn(author, "\n")] = 0;  // remove newline character
                addBook(title, author);
                break;
            case 2:
                printf("Enter book ID to remove: ");
                scanf("%d", &id);
                removeBook(id);
                break;
            case 3:
                listBooks();
                break;
            case 4:
                printf("Enter title to search: ");
                fgets(title, MAX_TITLE_LENGTH, stdin);
                title[strcspn(title, "\n")] = 0;  // remove newline character
                searchBook(title);
                break;
            case 5:
                printf("Enter filename to save: ");
                fgets(filename, 100, stdin);
                filename[strcspn(filename, "\n")] = 0;  // remove newline character
                saveToFile(filename);
                break;
            case 6:
                printf("Enter filename to load: ");
                fgets(filename, 100, stdin);
                filename[strcspn(filename, "\n")] = 0;  // remove newline character
                loadFromFile(filename);
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }
    return 0;
}
