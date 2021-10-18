#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h"

int get_option(int type, const char *msg)
{
	printf("%s: ", msg);

	if (type == NUM) {
		int num;
		scanf("%d", &num);
		return num;
	} else if (type == CHAR) {
		return fgetc(stdin);
	} else {
		fgetc(stdin);
		return -1;
	}
}

MenuOptions get_menu_option() {
	switch (get_option(NUM, ""))
		{
		case 0:
			return e_exit;
		case 1:
			return e_add_contact;
		case 2:
			return e_search_contact;
		case 3:
			return e_edit_contact;
		case 4:
			return e_delete_contact;
		case 5:
			return e_list_contacts;
		case 6:
			return e_save;
		default:
			return -1;
		}
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	// system("cls");

	printf("#######  Address Book  #######\n");
	if (*str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_menu_option();

		if ((address_book->count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
		case e_add_contact:
			add_contacts(address_book);
			break;
		case e_search_contact:
			search_contact(address_book);
			break;
		case e_edit_contact:
			edit_contact(address_book);
			break;
		case e_delete_contact:
			delete_contact(address_book);
			break;
		case e_list_contacts:
			break;
			/* Add your implementation to call list_contacts function here */
		case e_save:
			save_file(address_book);
			// save_prompt(address_book); PROMPTS THE SAVE MENU
			break;
		case e_exit:
			exit(0);
			break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	ContactInfo person;
	int option;

	strncpy(*person.name, " ", sizeof(person.name));
	strncpy(*person.phone_numbers, " ", sizeof(person.phone_numbers));
	strncpy(*person.email_addresses, " ", sizeof(person.email_addresses));

	do
	{
		menu_header("Add Contact:\n");
		printf("0. Back\n");
		printf("1. Name\t\t: %s\n", person.name);
		printf("2. Phone No 1\t: %s\n", person.phone_numbers);
		printf("3. Email ID 1\t: %s\n", person.email_addresses);
		printf("\nPlease select an option: ");

		option = get_option(NUM, "");

		switch (option)
		{
		case e_first_opt:
			menu(address_book);
			break;
		case e_second_opt:
			printf("Enter the name: ");
			scanf("%s", person.name);
			fprintf(address_book->fp, *person.name);
			fprintf(address_book->fp, "\n");
			break;
		case e_third_opt:
			printf("Enter Phone Number 1: [Please reenter the same option of alternate Phone Number]: ");
			scanf("%s", person.phone_numbers);
			fprintf(address_book->fp, *person.phone_numbers);
			fprintf(address_book->fp, "\n");
			break;
		case e_fourth_opt:
			printf("Enter Email ID 1: [Please reenter the same option of alternate Email ID]: ");
			scanf("%s", person.email_addresses);
			fprintf(address_book->fp, *person.email_addresses);
			fprintf(address_book->fp, "\n");
			break;
		}
		getchar();

	} while (option != e_first_opt);

	return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */

	int option;

	do
	{
		menu_header("Delete Contact By:\n");
		printf("0. Back\n");
		printf("1. Name\n");
		printf("2. Phone No\n");
		printf("3. Email ID\n");
		printf("4. Serial No\n");
		printf("\nPlease select an option: ");

		option = get_option(NUM, "");

		switch (option)
		{
		case e_first_opt:
			menu(address_book);
			break;
		case e_second_opt:
			printf("Enter the name: ");
			//scanf("%s", person.name);
			break;
		case e_third_opt:
			printf("Enter Phone Number: ");
			//scanf("%s", person.phone_numbers);
			break;
		case e_fourth_opt:
			printf("Enter Email ID: ");
			//scanf("%s", person.email_addresses);
			break;
		}
		getchar();

	} while (option != e_first_opt);
}
