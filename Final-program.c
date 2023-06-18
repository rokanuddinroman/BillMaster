#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold user information
struct User {
    char username[20];
    char password[20];
    int role; // 0 for customer, 1 for admin
};

// Structure to hold product information
struct Product {
    char name[100];
    double price;
};

// Structure to hold category information
struct Category {
    char name[100];
    struct Product products[50];
    int productCount;
};

// Function to register a new user
void registerUser(struct User users[], int *count, int role) {
    printf("Enter username: ");
    scanf("%s", users[*count].username);

    printf("Enter password: ");
    scanf("%s", users[*count].password);

    users[*count].role = role;
    (*count)++;

    printf("Registration successful!\n");
}

// Function to authenticate a user
int authenticateUser(struct User users[], int count, char username[], char password[], int *loggedInUserIndex, int role) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            if (users[i].role == role) {
                *loggedInUserIndex = i;
                return 1; // Authentication successful
            } else {
                return 0; // Role mismatch, authentication failed
            }
        }
    }

    return 0; // Authentication failed
}

// Function to display available categories
void displayCategories(struct Category categories[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, categories[i].name);
    }
}

// Function to display products of a selected category
void displayProducts(struct Category category) {
    printf("Products in %s category:\n", category.name);
    for (int i = 0; i < category.productCount; i++) {
        printf("%d. %s - $%.2f\n", i + 1, category.products[i].name, category.products[i].price);
    }
}

// Function to add a product to the cart
void addToCart(struct Category category, int productIndex, struct Product cart[], int *cartItemCount) {
    cart[*cartItemCount] = category.products[productIndex];
    (*cartItemCount)++;
    printf("Product added to cart.\n");
}

// Function to display the contents of the cart
void displayCart(struct Product cart[], int cartItemCount) {
    printf("Cart:\n");
    for (int i = 0; i < cartItemCount; i++) {
        printf("%d. %s - $%.2f\n", i + 1, cart[i].name, cart[i].price);
    }
}

// Function to generate the invoice
void generateInvoice(struct Product cart[], int cartItemCount) {
    printf("\nInvoice:\n");
    printf("-------------------------\n");
    double total = 0.0;
    for (int i = 0; i < cartItemCount; i++) {
        printf("%d. %s - $%.2f\n", i + 1, cart[i].name, cart[i].price);
        total += cart[i].price;
    }
    printf("-------------------------\n");
    printf("Total: $%.2f\n", total);
}

// Function to save user data to a file
void saveUserData(struct User users[], int count) {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %d\n", users[i].username, users[i].password, users[i].role);
    }

    fclose(file);
}

// Function to load user data from a file
int loadUserData(struct User users[]) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%s %s %d", users[count].username, users[count].password, &users[count].role) != EOF) {
        count++;
    }

    fclose(file);
    return count;
}

// Function to save product data to a file
void saveProductData(struct Category categories[], int count) {
    FILE *file = fopen("products.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", categories[i].name);
        for (int j = 0; j < categories[i].productCount; j++) {
            fprintf(file, "%s\n", categories[i].products[j].name);
            fprintf(file, "%.2f\n",categories[i].products[j].price);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to load product data from a file
int loadProductData(struct Category categories[]) {

    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int count = 0;
    char line[1000];
    char productName[1000];
    double productPrice;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) > 0) {
            strcpy(categories[count].name, line);
            categories[count].productCount = 0; // Initialize product count for the category

            while (fgets(line, sizeof(line), file) != NULL) {
                line[strcspn(line, "\n")] = '\0';

                if (strlen(line) > 0) {
                    sscanf(line, "%[^$] $%lf", productName, &productPrice);
                    strcpy(categories[count].products[categories[count].productCount].name, productName);

                    // Read the next line for the product price
                    fgets(line, sizeof(line), file);
                    sscanf(line, "%lf", &productPrice);
                    categories[count].products[categories[count].productCount].price = productPrice;

                    categories[count].productCount++;
                } else {
                    break; // Empty line indicates end of products for the current category
                }
            }

            count++;
        }
    }

    fclose(file);
    return count;

}

int main() {
    struct User users[50];
    int userCount = loadUserData(users);

    struct Category categories[50];
    int categoryCount = loadProductData(categories);

    struct Product cart[50];
    int cartItemCount = 0; // Number of products in the cart

    int option;
    char username[20];
    char password[20];
    int loggedInUserIndex = -1;

    do {
        printf("1. Login as Customer\n");
        printf("2. Login as Admin\n");
        printf("3. Register as Customer\n");
        printf("4. Register as Admin\n");
        printf("5. Exit\n");
        printf("Select option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                // Customer login code
                printf("Enter username: ");
                scanf("%s", username);

                printf("Enter password: ");
                scanf("%s", password);

                if (authenticateUser(users, userCount, username, password, &loggedInUserIndex,0)) {
                    printf("Login successful!\n");
                    if (users[loggedInUserIndex].role == 0) {
                        // Customer menu
                        int categoryOption;
                        int selectedCategoryIndex;
                        int selectedProductIndex;

                        do {
                            printf("\n===== Customer Menu =====\n");
                            printf("1. View Categories\n");
                            printf("2. View Products\n");
                            printf("3. Add Product to Cart\n");
                            printf("4. View Cart\n");
                            printf("5. Checkout Now\n");
                            printf("6. Logout\n");
                            printf("Select option: ");
                            scanf("%d", &categoryOption);

                            switch (categoryOption) {
                                case 1:
                                    printf("\n===== Categories =====\n");
                                    displayCategories(categories, categoryCount);
                                    break;
                                case 2:
                                    printf("\n===== Products =====\n");
                                    printf("All Categories: \n");
                                    displayCategories(categories, categoryCount);
                                    printf("Enter the category number: ");
                                    scanf("%d", &selectedCategoryIndex);
                                    if (selectedCategoryIndex >= 1 && selectedCategoryIndex <= categoryCount) {
                                        displayProducts(categories[selectedCategoryIndex - 1]);
                                    } else {
                                        printf("Invalid category number.\n");
                                    }
                                    break;
                                case 3:
                                    printf("\n===== Add Product to Cart =====\n");
                                    printf("All Categories: \n");
                                    displayCategories(categories, categoryCount);
                                    printf("Enter the category number: ");
                                    scanf("%d", &selectedCategoryIndex);
                                    if (selectedCategoryIndex >= 1 && selectedCategoryIndex <= categoryCount) {
                                        displayProducts(categories[selectedCategoryIndex - 1]);
                                        printf("Enter the product number: ");
                                        scanf("%d", &selectedProductIndex);
                                        if (selectedProductIndex >= 1 && selectedProductIndex <= categories[selectedCategoryIndex - 1].productCount) {
                                            addToCart(categories[selectedCategoryIndex - 1], selectedProductIndex - 1, cart, &cartItemCount);
                                        } else {
                                            printf("Invalid product number.\n");
                                        }
                                    } else {
                                        printf("Invalid category number.\n");
                                    }
                                    break;
                                case 4:
                                    printf("\n===== View Cart =====\n");
                                    displayCart(cart, cartItemCount);
                                    break;
                                case 5:
                                    printf("\n===== Checkout Now =====\n");
                                    generateInvoice(cart, cartItemCount);
                                    cartItemCount = 0; // Clear the cart after checkout
                                    break;
                                case 6:
                                    printf("Logging out...\n");
                                    break;
                                default:
                                    printf("Invalid option. Try again.\n");
                                    break;
                            }
                        } while (categoryOption != 6);
                    }
                } else {
                    printf("Authentication failed. Please try again.\n");
                }
                break;
            case 2:
                // Admin login code
                printf("Enter username: ");
                scanf("%s", username);

                printf("Enter password: ");
                scanf("%s", password);

                if (authenticateUser(users, userCount, username, password, &loggedInUserIndex,1)) {
                    printf("Login successful!\n");
                    if (users[loggedInUserIndex].role == 1) {
                        // Admin menu
                        int adminOption;
                        do {
                            printf("\n===== Admin Menu =====\n");
                            printf("1. Add Category\n");
                            printf("2. Add Product to Category\n");
                            printf("3. View All Customers\n");
                            printf("4. Logout\n");
                            printf("Select option: ");
                            scanf("%d", &adminOption);

                            switch (adminOption) {
                                case 1:
                                    fflush(stdin);
                                    printf("Enter the category name: ");
                                    gets(categories[categoryCount].name);
                                    categories[categoryCount].productCount = 0;
                                    categoryCount++;
                                    printf("Category added successfully.\n");
                                    break;
                                case 2:
                                    printf("Enter the category number: ");
                                    int selectedCategoryIndex;
                                    scanf("%d", &selectedCategoryIndex);
                                    if (selectedCategoryIndex >= 1 && selectedCategoryIndex <= categoryCount) {
                                        fflush(stdin);
                                        printf("Enter the product name: ");
                                        gets(categories[selectedCategoryIndex - 1].products[categories[selectedCategoryIndex - 1].productCount].name);
                                        printf("Enter the product price: ");
                                        scanf("%lf", &categories[selectedCategoryIndex - 1].products[categories[selectedCategoryIndex - 1].productCount].price);
                                        categories[selectedCategoryIndex - 1].productCount++;
                                        printf("Product added to the category successfully.\n");
                                    } else {
                                        printf("Invalid category number.\n");
                                    }
                                    break;
                                case 3:
                                    printf("\n===== All Customers =====\n");
                                    for (int i = 0; i < userCount; i++) {
                                        if (users[i].role == 0) {
                                            printf("Username: %s\n", users[i].username);
                                            printf("Password: %s\n", users[i].password);
                                            printf("Role: Customer\n");
                                            printf("\n");
                                        }
                                    }
                                    break;
                                case 4:
                                    printf("Logging out...\n");
                                    break;
                                default:
                                    printf("Invalid option. Try again.\n");
                                    break;
                            }
                        } while (adminOption != 4);
                    }
                } else {
                    printf("Authentication failed. Please try again.\n");
                }
                break;
            case 3:
                // Customer registration
                registerUser(users, &userCount, 0); // Role 0 for customer
                break;
            case 4:
                // Admin registration
                registerUser(users, &userCount, 1); // Role 1 for admin
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
                break;
        }
    } while (option != 5);

    // Save user and product data to files
    saveUserData(users, userCount);
    saveProductData(categories, categoryCount);

    return 0;
}

