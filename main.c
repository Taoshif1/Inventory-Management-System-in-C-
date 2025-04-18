#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Item {
    char name[50];
    int quantity;
    float price;
    char category[30];
};

struct Item stack[MAX];
int top = -1;

void getItemDetails(struct Item *newItem) {
    printf("Enter item name: ");
    scanf(" %[^\n]", newItem->name);

    // Input validation for quantity (positive integer)
    while (1) {
        printf("Enter quantity: ");
        scanf("%d", &newItem->quantity);
        if (newItem->quantity >= 0) {
            break;
        } else {
            printf("Quantity can't be negative! Please enter again.\n");
        }
    }

    // Input validation for price (positive float)
    while (1) {
        printf("Enter price: ");
        scanf("%f", &newItem->price);
        if (newItem->price > 0) {
            break;
        } else {
            printf("Price should be a positive value! Please enter again.\n");
        }
    }

    printf("Enter category: ");
    scanf(" %[^\n]", newItem->category);
}

void addItem() {
    if (top == MAX - 1) {
        printf("Inventory is full!\n");
        return;
    }
    struct Item newItem;
    getItemDetails(&newItem);
    stack[++top] = newItem;
    printf("'%s' added to inventory.\n", newItem.name);
}

void sellItem() {
    if (top == -1) {
        printf("Inventory is empty!\n");
        return;
    }
    char name[50];
    printf("Enter item name to sell: ");
    scanf(" %[^\n]", name);

    for (int i = 0; i <= top; i++) {
        if (strcmp(stack[i].name, name) == 0) {
            int qty;
            printf("Enter quantity to sell: ");
            scanf("%d", &qty);

            if (stack[i].quantity < qty) {
                printf("Not enough stock!\n");
                return;
            }

            stack[i].quantity -= qty;
            printf("Sold %d of %s.\n", qty, stack[i].name);
            if (stack[i].quantity == 0) {
                // Shift left to delete
                for (int j = i; j < top; j++) {
                    stack[j] = stack[j + 1];
                }
                top--;
                printf("%s is out of stock and removed from inventory.\n", name);
            }
            return;
        }
    }
    printf("Item not found!\n");
}

void viewInventory() {
    if (top == -1) {
        printf("Inventory is empty!\n");
        return;
    }

    printf("\n%-20s %-10s %-10s %-15s\n", "Name", "Quantity", "Price", "Category");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i <= top; i++) {
        printf("%-20s %-10d %-10.2f %-15s\n", stack[i].name, stack[i].quantity, stack[i].price, stack[i].category);
    }
}

void searchItem() {
    char name[50];
    printf("Enter item name to search: ");
    scanf(" %[^\n]", name);
    for (int i = 0; i <= top; i++) {
        if (strcmp(stack[i].name, name) == 0) {
            printf("Item found: %s - Qty: %d, Price: %.2f, Category: %s\n",
                   stack[i].name, stack[i].quantity, stack[i].price, stack[i].category);
            return;
        }
    }
    printf("Item not found!\n");
}

void updateItem() {
    char name[50];
    printf("Enter item name to update: ");
    scanf(" %[^\n]", name);
    for (int i = 0; i <= top; i++) {
        if (strcmp(stack[i].name, name) == 0) {
            printf("Enter new details for %s:\n", name);
            getItemDetails(&stack[i]);
            printf("Item updated successfully.\n");
            return;
        }
    }
    printf("Item not found!\n");
}

void countItems() {
    printf("Total items in inventory: %d\n", top + 1);
}

void lowStockAlert() {
    printf("Low stock items (Qty < 5):\n");
    int found = 0;
    for (int i = 0; i <= top; i++) {
        if (stack[i].quantity < 5) {
            printf(" - %s (%d pcs)\n", stack[i].name, stack[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("No low stock items!\n");
}

void sortByQuantity() {
    struct Item temp;
    for (int i = 0; i <= top; i++) {
        for (int j = i + 1; j <= top; j++) {
            if (stack[i].quantity < stack[j].quantity) {
                temp = stack[i];
                stack[i] = stack[j];
                stack[j] = temp;
            }
        }
    }
    printf("Inventory sorted by quantity (high to low).\n");
}

void filterByCategory() {
    char cat[30];
    printf("Enter category to filter: ");
    scanf(" %[^\n]", cat);
    int found = 0;
    printf("Items in category '%s':\n", cat);
    for (int i = 0; i <= top; i++) {
        if (strcmp(stack[i].category, cat) == 0) {
            printf(" - %s (%d pcs, %.2f BDT)\n", stack[i].name, stack[i].quantity, stack[i].price);
            found = 1;
        }
    }
    if (!found) printf("No items found in that category!\n");
}

void saveToFile() {
    FILE *fp = fopen("inventory.txt", "w");
    if (fp == NULL) {
        printf("Error opening file to save inventory.\n");
        return;
    }
    for (int i = 0; i <= top; i++) {
        fprintf(fp, "%s %d %.2f %s\n", stack[i].name, stack[i].quantity, stack[i].price, stack[i].category);
    }
    fclose(fp);
    printf("Inventory saved to 'inventory.txt'\n");
}

void loadFromFile() {
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) {
        printf("No previous inventory found. Starting fresh.\n");
        return;
    }

    struct Item temp;
    while (fscanf(fp, " %[^\n] %d %f %[^\n]", temp.name, &temp.quantity, &temp.price, temp.category) != EOF) {
        stack[++top] = temp;
    }
    fclose(fp);
}

int main() {
    int choice;
    loadFromFile();
    do {
        printf("\n===== INVENTORY MANAGER =====\n");
        printf("1. Add Item\n");
        printf("2. Sell Item\n");
        printf("3. View Inventory\n");
        printf("4. Search Item\n");
        printf("5. Count Items\n");
        printf("6. Update Item\n");
        printf("7. Low Stock Alert\n");
        printf("8. Sort by Quantity\n");
        printf("9. Filter by Category\n");
        printf("10. Save & Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addItem(); break;
            case 2: sellItem(); break;
            case 3: viewInventory(); break;
            case 4: searchItem(); break;
            case 5: countItems(); break;
            case 6: updateItem(); break;
            case 7: lowStockAlert(); break;
            case 8: sortByQuantity(); break;
            case 9: filterByCategory(); break;
            case 10: saveToFile(); printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 10);

    return 0;
}
