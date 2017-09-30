/*
Name:Nate Clark
Student number:132386160
Email:nclark7@myseneca.ca
Workshop:Milestone 4(final)
Section:M
Date:April 9th 2017
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>


// Define statements:
#define LINEAR 1
#define FORM 0
#define STOCK 1
#define CHECKOUT 0
#define MAX_ITEM_NO 500
#define MAX_QTY 999
#define SKU_MAX 999
#define SKU_MIN 100
#define DATAFILE "items.txt"



// Constant TAX value:
const double TAX = 0.13;



// Item structure:
struct Item {
	double price;
	int sku;
	int isTaxed;
	int quantity;
	int minQuantity;
	char name[21];
};

// Function Prototypes Milestone 1:
void welcome(void);
void printTitle(void);
void printFooter(double gTotal);
void flushKeyboard(void);
void pause(void);
int getInt(void);
double getDouble(void);
int getIntLimited(int lowerLimit, int upperLimit);
double getDoubleLimited(double lowerLimit, double upperLimit);
int yes(void);
void GroceryInventorySystem(void);
int menu(void);

// Function Prototypes Milesone 2:
double totalAfterTax(struct Item item);
int isLowQuantity(struct Item item);
struct Item itemEntry(int sku);
void displayItem(struct Item item, int linear);
void listItems(const struct Item item[], int noOfItems);
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);

// Function Prototypes Milestone 3:
void search(const struct Item item[], int numofRecs);
void updateItem(struct Item* itemptr);
void addItem(struct Item item[], int *NoOfRecs, int sku);
void addOrUpdateItem(struct Item item[], int* NoOfRecs);
void adjustQuantity(struct Item item[], int NoOfRecs, int stock);

// Function Prototypes Milestone 4:
void saveItem(struct Item item, FILE* dataFile);
int loadItem(struct Item* item, FILE* dataFile);
int saveItems(const struct Item item[], char fileName[], int NoOfRecs);
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);

//main function
int main()
{
	GroceryInventorySystem();
	return 0;
}

// Function Implementations Milestone 1:
void welcome(void){
	printf("---=== Grocery Inventory System ===---\n\n");
}

void printTitle(void)
{
	printf("Row |SKU| Name               | Price  |Taxed| Qty | Min |   Total    |Atn\n");
	printf("----+---+--------------------+--------+-----+-----+-----+------------|---\n");
}

void printFooter(double Total)
{
	printf("--------------------------------------------------------+----------------\n");
	if (Total > 0)
	{
		printf("                                           Grand Total: |%12.2lf\n", Total);
	}
}

void flushKeyboard(void)
{
	while (getchar() != '\n')
		;
}

void pause(void)
{
	printf("Press <ENTER> to continue...");
	flushKeyboard();
}

int getInt(void)
{
	char charactercheck = 'x';
	int intcheck;
	while (charactercheck != '\n'){
		scanf("%d%c", &intcheck, &charactercheck);
		if (charactercheck != '\n')
		{
			flushKeyboard();
			printf("Invalid integer, please try again: ");
		};
	}
	return intcheck;
}

int getIntLimited(int lowerlimit, int upperlimit)
{
	int valueint = 0;
	//run this while loop while the input value is greater than the top value or lower than the input value
	while (valueint >= upperlimit || valueint <= lowerlimit){
		valueint = getInt();
		if (valueint > upperlimit || valueint < lowerlimit)
		{
			//invalid value should be %d < %d < %d but 
			printf("Invalid value, %d < value < %d: ", lowerlimit, upperlimit);
		}
		else if (valueint >= lowerlimit || valueint <= upperlimit)
		{
			return valueint;
		}
	}
	return 0;
}

double getDouble()
{
	char charactercheck = 'x';
	double doublecheck;
	while (charactercheck != '\n'){
		scanf("%lf%c", &doublecheck, &charactercheck);
		if (charactercheck != '\n')
		{
			flushKeyboard();
			printf("Invalid number, please try again: ");
		};
	}
	return doublecheck;
}

double getDoubleLimited(double lowerLimit, double upperlimit)
{

	double valuedouble = 0;
	while (valuedouble >= upperlimit || valuedouble <= lowerLimit){
		valuedouble = getDouble();

		if (valuedouble > upperlimit || valuedouble < lowerLimit)
		{
			printf("Invalid value, %lf< value < %lf: ", lowerLimit, upperlimit);
		}
		else if (valuedouble >= lowerLimit || valuedouble <= upperlimit)
		{
			return valuedouble;
		}
	}
	//fail safe so compiler won't throw errors
	return 0;
}

int yes(void)
{
	char readchar = ' ';
	int yesflag = 0;
	int flag = 0;
	//done and good
	while (!flag){
		scanf("%c", &readchar);
		flushKeyboard();
		if ((readchar == 'y') || (readchar == 'Y')){
			flag = 1;
			yesflag = 1;
		}
		else if ((readchar == 'n') || (readchar == 'N')){
			flag = 1;
			yesflag = 0;
		}
		else
		{
			printf("Only (Y)es or (N)o are acceptable: ");
		}
	}
	return yesflag;
}

int menu(void)
{
	int choice;
	//put this in to display welcome message on unix, as the tester file wasn't running my tester

	printf("1- List all items\n");
	printf("2- Search by SKU\n");
	printf("3- Checkout an item\n");
	printf("4- Stock an item\n");
	printf("5- Add new item or update item\n");
	printf("6- delete item\n");
	printf("7- Search by name\n");
	printf("0- Exit program\n");
	printf("> ");
	choice = getIntLimited(0, 7);
	return choice;
}

void GroceryInventorySystem(void)
{
	struct Item item[MAX_ITEM_NO];
	int numrecordsread = 0;
	welcome();
	if (loadItems(item, DATAFILE, &numrecordsread) == 0)
	{
		printf("Could not read from %s.\n",DATAFILE);
	}
	else
	{
		int returnvalue = 0;
		while (!returnvalue)
		{
			//needs work and testing
			switch (menu())
			{
			case 0:
				printf("Exit the program? (Y)es/(N)o): ");
				returnvalue = yes();
				break;
			case 1:
				listItems(item, numrecordsread);
				pause();
				break;
			case 2:
				//search items
				search(item, numrecordsread);
				pause();
				break;
			case 3:
				//checkout
				adjustQuantity(item,numrecordsread,CHECKOUT);
				if (saveItems(item, DATAFILE, numrecordsread) == 0){
					printf("could not update data file %s\n",DATAFILE);
				}
				pause();
				break;
			case 4:
				//stock
				adjustQuantity(item, numrecordsread, STOCK);
				if (saveItems(item, DATAFILE, numrecordsread) == 0){
					printf("could not update data file %s\n", DATAFILE);
				}
				pause();
				break;
			case 5:
				//add or update item
				addOrUpdateItem(item,&numrecordsread);
				if (saveItems(item, DATAFILE, numrecordsread) == 0){
					printf("could not update data file %s\n", DATAFILE);
				}
				pause();
				break;
			case 6:
				printf("Not implemented!\n");
				pause();
				break;
			case 7:
				printf("Not implemented!\n");
				pause();
				break;
			}
		}
	}
}

// Function Implementations Milestone 2:
double totalAfterTax(struct Item item)
{
	//done and works
	double total;
	double taxtotal;
	total = item.price * item.quantity;
	if (item.isTaxed == 1) {
		taxtotal = total * TAX;
		total = total + taxtotal;
	}
	return total;
}

int isLowQuantity(struct Item item)
{
	//done and works
	int flag;
	if (item.quantity <= item.minQuantity)
	{
		flag = 1;
	}
	else{
		flag = 0;
	}
	return flag;
}

struct Item itemEntry(int sku)
{
	//done

	struct Item newitem;
	newitem.sku = sku;
	printf("        SKU: %d\n", newitem.sku);

	printf("       Name: ");
	scanf("%20[^\n]", newitem.name);
	flushKeyboard();

	printf("      Price: ");
	newitem.price = getDoubleLimited(0.01,1000.00);

	printf("   Quantity: ");
	newitem.quantity = getIntLimited(1,MAX_QTY);

	printf("Minimum Qty: ");
	newitem.minQuantity = getIntLimited(0,100);

	printf("   Is Taxed: ");
	newitem.isTaxed = yes();
	//test
	return newitem;
}

void displayItem(struct Item item, int linear){
	double totalprice = 0.0;
	double totaltax = 0.0;
	//done
	//need tide detergent to round up
	totalprice = item.price * item.quantity;
	if (linear)
	{
		if (item.quantity <= item.minQuantity) {
			if (item.isTaxed == 1) {
				totaltax = totalprice * TAX;
				totalprice += totaltax;
				printf("|%d|%-20s|%8.2lf|  Yes|%4d |%4d |%12.2f|***\n", item.sku, item.name, item.price, item.quantity, item.minQuantity, totalprice);
			}
			else
				printf("|%d|%-20s|%8.2lf|   No|%4d |%4d |%12.2f|***\n", item.sku, item.name, item.price, item.quantity, item.minQuantity, totalprice);
		}
		else
		{
			if (item.isTaxed == 1) {
				totaltax = totalprice * TAX;
				totalprice += totaltax;
				printf("|%d|%-20s|%8.2lf|  Yes|%4d |%4d |%12.2f|\n", item.sku, item.name, item.price, item.quantity, item.minQuantity, totalprice);
			}
			else
				printf("|%d|%-20s|%8.2lf|   No|%4d |%4d |%12.2f|\n", item.sku, item.name, item.price, item.quantity, item.minQuantity, totalprice);
		}
	}
	else if (!linear)
	{
		if (item.quantity <= item.minQuantity) {
			printf("        SKU: %d\n", item.sku);
			printf("       Name: %s\n", item.name);
			printf("      Price: %.2f\n", item.price);
			printf("   Quantity: %d\n", item.quantity);
			printf("Minimum Qty: %d\n", item.minQuantity);
			if (item.isTaxed) {
				printf("   Is Taxed: Yes\n");
			}
			else {
				printf("   Is Taxed: No\n");
			}
			printf("WARNING: Quantity low, please order ASAP!!!\n");
		}
		else {
			printf("        SKU: %d\n", item.sku);
			printf("       Name: %s\n", item.name);
			printf("      Price: %.2lf\n", item.price);
			printf("   Quantity: %d\n", item.quantity);
			printf("Minimum Qty: %d\n", item.minQuantity);
			if (item.isTaxed) {
				printf("   Is Taxed: Yes\n");
			}
			else {
				printf("   Is Taxed: No\n");
			}
		}
	}
}

void listItems(const struct Item item[], int noOfItems)
{
	//done
	double grandTotal = 0.0;
	double taxTotal = 0.0;
	double itemprice = 0.0;
	int i;
	printTitle();
	for (i = 0; i < noOfItems; i++)
	{

		printf("%-4d", i + 1);//printing row number
		displayItem(item[i], 1);
		if (item[i].isTaxed)
		{
			itemprice = item[i].price * item[i].quantity;
			taxTotal = itemprice * TAX;
			grandTotal = grandTotal + taxTotal;
		}
		grandTotal = item[i].price * item[i].quantity + grandTotal;//add total price of the current Item element in the array to the grand value
	}
	printFooter(grandTotal);

}

int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index)
{
	//works again
	int i = 0;
	int truevalue = 0;
	int foundflag = 0;
	for (i = 0; i < NoOfRecs; i++){
		if (item[i].sku == sku) {
			*index = i;//index is right
			foundflag = 1;
		}
	}
	if (foundflag == 1){
		truevalue = 1;
	}
	else
		truevalue = 0;
	return truevalue;
}

//Milestone 3 function implementations
void search(const struct Item item[], int numofRecs)
{
	//good to go
	int sku = 0;
	int index = 0;
	int skuwasfound;
	printf("Please enter the SKU: ");
	sku = getIntLimited(SKU_MIN, SKU_MAX);
	skuwasfound = locateItem(item, numofRecs, sku, &index);
	if (skuwasfound)
		displayItem(item[index], FORM);
	else
		printf("Item not found!\n");//delete in search before submitting
}

void updateItem(struct Item* itemptr)
{
	struct Item newitem;
	//itemptr = &newitem;
	printf("Enter new data:\n");
	newitem = itemEntry(itemptr->sku);
	printf("Overwrite old data? (Y)es/(N)o: ");
	if (yes()){
		/*
		If user responds yes, overwrite the target of itemptr by the Item instance
		*/
		*(itemptr) = newitem;
		printf("--== Updated! ==--\n");
	}
	else{
		printf("--== Aborted! ==--\n");
	}
}

void addItem(struct Item item[], int *NoOfRecs, int sku)
{
	//done
	struct Item Newitem;
	if (*NoOfRecs == MAX_ITEM_NO)
	{
		printf("Can not add new item; Storage Full!\n");
	}
	else
	{
		Newitem = itemEntry(sku);
		printf("Add Item? (Y)es/(N)o: ");
		if (yes())
		{
			/*
			//If the user replies yes,
			set the Item after the last one in the item array to the one
			you just got from the user and add one to the target of
			NoOfRecs pointer and print
			*/
			item[*NoOfRecs] = Newitem;//need proper updating of item working, as well as increasing number of records properly
			*NoOfRecs += 1;
			printf("--== Added! ==--\n");
		}
		else
		{
			printf("--== Aborted! ==--\n");
		}
	}
}

void addOrUpdateItem(struct Item item[], int* NoOfRecs)
{
	//needs work, not displaying correct sku number
	int searchSku = 0;
	int foundflag = 0;
	int index = 0;
	printf("Please enter the SKU: ");
	searchSku = getIntLimited(SKU_MIN, SKU_MAX);//search sku is getting the right number
	foundflag = locateItem(item, *NoOfRecs, searchSku, &index);
	if (foundflag)
	{
		displayItem(item[index], FORM);
		printf("Item already exists, Update? (Y)es/(N)o: ");
		if (yes())
		{
			updateItem(&item[index]);
		}
		else
		{
			printf("--== Aborted! ==--\n");
		}
	}
	else
	{
		addItem(item, NoOfRecs, searchSku);
	}
}

void adjustQuantity(struct Item item[], int NoOfRecs, int stock)
{
	//works
	int searchsku = 0;
	int i = 0;
	int itemfound = 0;
	int index = 0;
	int input = 0;
	printf("Please enter the SKU: ");
	//call
	searchsku = getIntLimited(SKU_MIN, SKU_MAX);
	for (i = 0; i <= NoOfRecs; i++)
	{
		if (item[i].sku == searchsku)
		{
			itemfound = 1;
			index = i;
		}
	}
	if (itemfound == 1)
	{
		displayItem(item[index], FORM);
		if (stock == STOCK)
		{
			printf("Please enter the quantity to stock; Maximum of %d or 0 to abort: ", MAX_QTY - item[index].quantity);
			input = getIntLimited(0, MAX_QTY - item[index].quantity);
		}
		else if (stock == CHECKOUT)
		{
			printf("Please enter the quantity to checkout; Maximum of %d or 0 to abort: ", item[index].quantity);
			input = getIntLimited(0, item[index].quantity);
		}
		
		if (input == 0)
		{
			printf("--== Aborted! ==--\n");
		}
		else{
			if (stock == STOCK)
			{
				item[index].quantity += input;
				printf("--== Stocked! ==--\n");
			}
			else if (stock == CHECKOUT)
			{
				item[index].quantity -= input;
				printf("--== Checked out! ==--\n");
			}
			if (isLowQuantity(item[index]) == 1)
			{
				printf("Quantity is low, please reorder ASAP!!!\n");
			}
		}
	}
	else
	{
		printf("SKU not found in storage!\n");
	}
}

//function implementation Milestone 4
// Function Prototypes Milestone 4:

void saveItem(struct Item item, FILE* dataFile)
{
	fprintf(dataFile, "%d,%d,%d,%.2f,%d,%s\n", item.sku, item.quantity, item.minQuantity, item.price, item.isTaxed, item.name);
}

int loadItem(struct Item* item, FILE* dataFile)
{
	int rv = 0;
	if (dataFile != NULL)
	{
		rv = fscanf(dataFile, "%d,%d,%d,%lf,%d,%[^\n]\n", &item->sku, &item->quantity, &item->minQuantity, &item->price, &item->isTaxed, item->name);
	}

	return rv;
}//done

int saveItems(const struct Item item[], char fileName[], int NoOfRecs)
{
	FILE *fp = NULL;
	int i;
	fp = fopen(fileName,"w");
	int returnvalue;
	if (fp != NULL)
	{
		for (i = 0; i < NoOfRecs; i++){
			saveItem(item[i],fp);
	}
		returnvalue = 1;
	} else
	{
		returnvalue = 0;
	}
	fclose(fp);
	return returnvalue;
}

int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr)
{
	FILE *fp = NULL;
	int i;
	fp = fopen(fileName, "r");
	int returnvalue;
	int numofloadeditems = 0;
	int ttlcuritms = 19;
	if (fp != NULL)
	{
		for (i = 0; i < ttlcuritms; i++){
			loadItem(&item[i], fp);
			numofloadeditems++;
		}
		*NoOfRecsPtr = numofloadeditems;
		returnvalue = 1;
		fclose(fp);
	}
	else
	{
		returnvalue = 0;
	}
	
	return returnvalue;
}