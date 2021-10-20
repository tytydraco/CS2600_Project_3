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
	printf("%s", msg);

	if (type == NUM)
	{
		int num;
		scanf("%d", &num);
		return num;
	}
	else if (type == CHAR)
	{
		return fgetc(stdin);
	}
	else
	{
		fgetc(stdin);
		return -1;
	}
}

MenuOptions get_menu_option()
{
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

		if (option == 'Y' || option == 'y')
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

	// #ifdef __linux__
	// 	system("clear");
	// #else
	// 	system("cls");
	// #endif

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
	printf("\nPlease select an option: ");
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
			getchar();
			printf("Done. Please enter a key to continue: ");
			getchar();
			break;
		case e_exit:
			save_prompt(address_book);
			exit(0);
			break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	ContactInfo contact;
	int option;
	int phoneCount = 0;
	int emailCount = 0;

	strcpy(*contact.name, " ");
	strcpy(contact.phone_numbers[0], " ");
	strncpy(*contact.email_addresses, " ", sizeof(contact.email_addresses));

	do
	{
		menu_header("Add Contact:\n");
		printf("0. Back\n");
		printf("1. Name\t\t: %s\n", contact.name[0]);
		printf("2. Phone No 1\t: %s\n", contact.phone_numbers[0]);
		for (int i = 1; i < phoneCount; i++)
			printf("\t    %d\t: %s\n", i+1, contact.phone_numbers[i]);
		printf("3. Email ID 1\t: %s\n", contact.email_addresses[0]);
		for (int i = 1; i < emailCount; i++)
			printf("\t    %d\t: %s\n", i+1, contact.email_addresses[i]);

		printf("\nPlease select an option: ");
		option = get_option(NUM, "");

		switch (option)
		{
		case e_first_opt:
			break;
		case e_second_opt:
			printf("Enter the name: ");
			scanf("%s", &contact.name[0]);
			if (strlen(*contact.name) > NAME_LEN)
			{
				printf("ERROR. Invalid Length of Name. Please enter a key to continue: ");
				getchar();
			}
			break;
		case e_third_opt:
			if(phoneCount < PHONE_NUMBER_COUNT)
			{	
				printf("Enter Phone Number %d: [Please reenter the same option of alternate Phone Number]: ", phoneCount+1);
				scanf("%s", &contact.phone_numbers[phoneCount]);
				if (strlen(contact.phone_numbers[phoneCount]) > NUMBER_LEN)
				{
					printf("ERROR. Invalid Length of Phone Number. Please enter a key to continue: ");
					getchar();
				}
				else
					phoneCount++;
			}
			else
			{
				printf("ERROR. You have reached maximum number of Phone Numbers. Please enter a key to continue: ");
				getchar();
			}
			break;
		case e_fourth_opt:
			if(emailCount < EMAIL_ID_COUNT)
			{
				printf("Enter Email ID %d: [Please reenter the same option of alternate Email ID]: ", emailCount + 1);
				scanf("%s", &contact.email_addresses[emailCount]);
				if (strlen(contact.email_addresses[emailCount]) > EMAIL_ID_LEN)
				{
					printf("ERROR. Invalid Length of Email. Please enter a key to continue: ");
					getchar();
				}
				else 
					emailCount++;
			} else
			{
				printf("ERROR. You have reached maximum number of Emails. Please enter a key to continue: ");
				getchar();
			}
			break;
		}
		getchar();

	} while (option != 0);

	contact.si_no = address_book->count + 1;
	fprintf(address_book->fp, "%d", contact.si_no);
	fprintf(address_book->fp, FIELD_DELIMITER);
	fprintf(address_book->fp, *contact.name);
	fprintf(address_book->fp, FIELD_DELIMITER);

	for (int i = 0; i < phoneCount; i++)
	{
		fprintf(address_book->fp, "%s", contact.phone_numbers[i]);
		fprintf(address_book->fp, FIELD_DELIMITER);
	}

	for (int i = 0; i < emailCount; i++)
	{
		fprintf(address_book->fp, contact.email_addresses[i]);
		fprintf(address_book->fp, FIELD_DELIMITER);
	}

	address_book->count++;

	/*Testing for adding contact to contact list*/
	// printf("Name: %s\n", address_book->list[address_book->count].name[0]);
	// printf("TEST2\n");
	// printf("Phone: %s\n", address_book->list[address_book->count].phone_numbers[0]);
	// printf("Email: %s\n", address_book->list[address_book->count].email_addresses[0]);

	// printf("TEST1\n");
	// address_book->list->name[0] = "hello";
	// address_book->list[address_book->count - 1] = contact;
	// printf("TEST2\n");

	fprintf(address_book->fp, NEXT_ENTRY);

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

Status delete (AddressBook *address_book, int index)
{
	unsigned int last_index = address_book->count - 1;
	ContactInfo contact_to_delete = address_book->list[index];
	ContactInfo last_contact = address_book->list[last_index];

	printf("SWAPPING %d with %d\n", index, last_index);

	/*
	 * Swap the last contact with the contact that should be deleted.
	 * This results in the contact to be deleted being the last contact in the list.
	 * We then decrement the count such that the contact to be deleted is no longer accessible.
	 */
	address_book->list[last_index] = contact_to_delete;
	address_book->list[index] = last_contact;

	address_book->count--;

	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */

	int option;
	char target_name[NAME_LEN];
	char target_phone[NUMBER_LEN];
	char target_email[EMAIL_ID_LEN];
	int target_serial;

	menu_header("Search Contact to Delete Contact by:\n");
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n");
	printf("\nPlease select an option: ");

	option = get_option(NUM, "");

	switch (option)
	{
	case 0:
		menu(address_book);
		break;
	case 1:

		printf("Enter the Name: ");
		scanf("%s", target_name);

		for (int i = 0; i < address_book->count - 1; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			for (int j = 0; j < NAME_COUNT - 1; j++)
			{
				if (strcmp(target_name, contact->name[j]) == 0)
				{
					return delete (address_book, i);
				}
			}
		}

		printf("Contact not found!\n");
		return e_no_match;
	case 2:

		printf("Enter Phone Number: ");
		scanf("%s", target_phone);

		for (int i = 0; i < address_book->count - 1; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			for (int j = 0; j < PHONE_NUMBER_COUNT - 1; j++)
			{
				if (strcmp(target_phone, contact->phone_numbers[j]) == 0)
					return delete (address_book, i);
			}
		}

		printf("Contact not found!\n");
		return e_no_match;
	case 3:

		printf("Enter Email ID: ");
		scanf("%s", target_email);

		for (int i = 0; i < address_book->count - 1; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			for (int j = 0; j < EMAIL_ID_COUNT - 1; j++)
			{
				if (strcmp(target_email, contact->email_addresses[j]) == 0)
					return delete (address_book, i);
			}
		}

		printf("Contact not found!\n");
		return e_no_match;
	case 4:

		printf("Enter Serial No: ");
		scanf("%d", &target_serial);

		for (int i = 0; i < address_book->count - 1; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			if (target_serial == contact->si_no)
				return delete (address_book, i);
		}

		printf("Contact not found!\n");
		return e_no_match;
	}
	getchar();

	return e_success;
}
