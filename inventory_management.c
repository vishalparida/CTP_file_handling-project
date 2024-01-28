#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int additem()
{
    FILE *inventory=fopen("inventory.csv","a+");
    FILE *inventoryrep=fopen("inventory_report.txt","a");
    int currentId, flag=0;
    int itemid, price, qty; char name[40];
    printf("Enter new Item Id: ");
    scanf("%d", &itemid);
    getchar();
    while (fscanf(inventory, "%d,%*[^,],%*d,%*d", &currentId) == 1){
        if(currentId==itemid){
            flag=1;
            break;
        }
    }
    if(flag==1){
        printf("Item ID already exists ! Enter again...\n");
        return 0;
    }
    printf("Enter new item's name: ");
    gets(name);
    printf("Enter the quantity of item you want to add: ");
    scanf("%d", &qty);
    printf("Enter the price/item: ");
    scanf("%d", &price);
    fprintf(inventory, "%d,%s,%d,%d\n", itemid, name, qty, price);
    time_t current_time;
    time(&current_time);
    char* date_time = ctime(&current_time);
    fprintf(inventoryrep, "A new item named %s, with item ID %d, quantity %d and price/item %d was added on %s", name, itemid, qty, price, date_time);
    fclose(inventory);
    fclose(inventoryrep);
    return 1;
}
int update(){
    FILE *inventoryp1=fopen("inventory.csv","r+");
    FILE *inventoryp2=fopen("inventory_copy.csv","w+");
    FILE *inventoryp3=fopen("inventory_report.txt","a");
    int id, newqty, newprice;
    printf("Enter the ID of the item you want to update: ");
    scanf("%d", &id);
    printf("Enter new quantity: ");
    scanf("%d", &newqty);
    printf("Enter new price: ");
    scanf("%d", &newprice);
    int found = 0;
    int currentId, currentQuantity, currentPrice;
    char currentName[50];
    while (fscanf(inventoryp1, "%d,%49[^,],%d,%d", &currentId, currentName, &currentQuantity, &currentPrice) == 4) {
        if (currentId == id) {
            printf("Previous Values: \n");
            printf("%d, %s, %d, %d\n", id, currentName, currentQuantity, currentPrice);
            printf("Updated Values: \n");
            printf("%d, %s, %d, %d\n", currentId, currentName, newqty, newprice);
            time_t current_time;
            time(&current_time);
            char* date_time = ctime(&current_time);
            if(newqty>currentQuantity){
                fprintf(inventoryp3, "The quantity of Item with Item ID %d was increased by %d on %s", currentId, (newqty-currentQuantity), date_time);
            }
            else if(newqty<currentQuantity){
                fprintf(inventoryp3, "The quantity of Item with Item ID %d was decreased by %d on %s", currentId, (currentQuantity-newqty), date_time);
            }
            if(newprice>currentPrice){
                fprintf(inventoryp3, "The price of the Item with Item ID %d was increased by %d on %s", currentId, (newprice-currentPrice), date_time);
            }
            else if(newprice<currentPrice){
                fprintf(inventoryp3, "The price of the Item with Item ID %d was decreased by %d on %s", currentId, (currentPrice-newprice), date_time);
            }
            fprintf(inventoryp2, "%d,%s,%d,%d\n", currentId, currentName, newqty, newprice);
            found=1;
            continue;
        }
        fprintf(inventoryp2, "%d,%s,%d,%d\n", currentId, currentName, currentQuantity, currentPrice);
    }
    if(found==0){printf("Item ID not found !!!");}
    fclose(inventoryp1);
    fclose(inventoryp2);
    fclose(inventoryp3);
    remove("inventory.csv");
    rename("inventory_copy.csv", "inventory.csv");
    return 1;
}
int delete(){
    FILE *inventoryp1=fopen("inventory.csv","r+");
    FILE *inventoryp2=fopen("inventory_copy.csv","w+");
    FILE *inventoryp3=fopen("inventory_report.txt","a");
    int id;
    printf("Enter the ID of the item you want to delete: ");
    scanf("%d", &id);
    int found = 0;
    int currentId, currentQuantity, currentPrice;
    char currentName[50];
    while (fscanf(inventoryp1, "%d,%49[^,],%d,%d", &currentId, currentName, &currentQuantity, &currentPrice) == 4) {
        if (currentId == id){
            time_t current_time;
            time(&current_time);
            char* date_time = ctime(&current_time);
            fprintf(inventoryp3, "The record of %s with Item ID %d, quantity %d and price/item %d was deleted on %s", currentName, currentId, currentQuantity, currentPrice, date_time);
            found=1;
            continue;
        }
        fprintf(inventoryp2, "%d,%s,%d,%d\n", currentId, currentName, currentQuantity, currentPrice);
    }
    if(found==0){printf("Item ID not found !!!");}
    fclose(inventoryp1);
    fclose(inventoryp2);
    fclose(inventoryp3);
    remove("inventory.csv");
    rename("inventory_copy.csv", "inventory.csv");
    return 1;
}
int viewInventory() {
    FILE *fp = fopen("inventory.csv", "r");
    int currentId, currentQuantity, currentPrice;
    char currentName[50];
    printf("| Item_ID | Item_Name         | Quantity | Price/item |\n");
    printf("|---------|-------------------|----------|------------|\n");
    while (fscanf(fp, "%d,%49[^,],%d,%d", &currentId, currentName, &currentQuantity, &currentPrice) == 4) {
        printf("| %-7d | %-17s | %-8d | %-10d |\n", currentId, currentName, currentQuantity, currentPrice);
    }
    fclose(fp);
    return 1;
}

int report(){
    FILE *fp=fopen("inventory_report.txt","r");
    FILE *fp1=fopen("inventory.csv","r");
    printf("----\t----\t----INVENTORY REPORT----\t----\t----\n");
    int c=fgetc(fp);
    while(c!=EOF){
        printf("%c",c);
        c=fgetc(fp);
    }
    int price,qty,sum=0;
    while (fscanf(fp1, "%*d,%*49[^,],%d,%d", &qty, &price) == 2){
        sum+=(qty*price);
    }
    printf("--------------------------------------");
    printf("|TOTAL INVENTORY VALUE = %d |", sum);
    printf("--------------------------------------\n\n");
    fclose(fp);
    fclose(fp1);
    return 1;
}
int main()
{
    int true=1, choice, check;
    printf("--- --- --- --- --- --- --- ---WELCOME TO INVENTORY--- --- --- --- --- --- --- ---\n");
    while(true)
    {
        check=0;
        printf("1. Add a new item\n");
        printf("2. Update an existing item\n");
        printf("3. Delete an existing item\n");
        printf("4. View inventory\n");
        printf("5. Generate inventory report. \n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
            check=additem();
            break;
            case 2:
            check=update();
            break;
            case 3:
            check=delete();
            break;
            case 4:
            check=viewInventory();
            break;
            case 5:
            check=report();
            break;
            case 6:
            printf("--- --- -Thank You- --- ---\n");
            true=0;
            break;
            default:
            printf("Wrong Choice !\n");
            printf("Enter again:\n");
        }
        if(check==1)
        {
            printf("Operation Successful...\n\n");
        }
    }// returns 0
    return 0;
}
