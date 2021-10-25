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
	} while (option != 'N' && option != 'n');

	free(address_book->list); 

	return e_success;
}

Status list_contacts(AddressBook *address_book, int index)
{
	int option;

	menu_header("All Contacts:\n");
	printf("(Page %d of %d):\n", (index+1), (address_book->count));

		printf("===========================================================================================================\n");
		printf(":S.No  :Name                            :Phone No                        :Email ID                        :\n");
		printf("===========================================================================================================\n");

		//Print the first row.
		printf(":%d", address_book->list[index].si_no);
		if (address_book->list[index].si_no < 10) {
			for (int i = 0; i < 5; i++)
				printf(" ");
		} else {
			for (int i = 0; i < 4; i++)
				printf(" ");
		}
		printf(":%s", &address_book->list[index].name[0][0]);
		for (int i = 32 - strlen(&address_book->list[index].name[0][0]); i > 0; i--) {
			printf(" ");
		}

		int NumbOfPhones = sizeof address_book->list[index].phone_numbers / sizeof *address_book->list[index].phone_numbers;
		int NumbOfEmails = sizeof address_book->list[index].email_addresses / sizeof *address_book->list[index].email_addresses;
		int phonesPrinted = 0;
		int emailsPrinted = 0;

		if (NumbOfPhones > 0) {
			printf(":%s", &address_book->list[index].phone_numbers[0][0]);
			phonesPrinted++;
			for (int i = 32 - strlen(&address_book->list[index].phone_numbers[0][0]); i > 0; i--)
					printf(" ");
		} else {
			for (int i = 32 ; i > 0; i--) {
				printf(" ");
			}
		}

		if (NumbOfEmails > 0) {
			printf(":%s", &address_book->list[index].email_addresses[0][0]);
			emailsPrinted++;
			for (int i = 32 - strlen(&address_book->list[index].email_addresses[0][0]); i > 0; i--)
				printf(" ");
		} else {
			for (int i = 32 ; i > 0; i--) {
				printf(" ");
			}
		}
		printf(":\n");

		for (int k = 0; k < 4; k++) //Print multiple of sama info types.
		{
			printf(":      :                                "); //Fill whitespace.

			if (phonesPrinted < NumbOfPhones) {
				printf(":%s", &address_book->list[index].phone_numbers[phonesPrinted][0]);
				for (int m = 32 - strlen(&address_book->list[index].phone_numbers[phonesPrinted][0]); m > 0; m--)
					printf(" ");
				phonesPrinted++;
			} else
				for (int n = 32 ; n > 0; n--)
					printf(" ");

			if (emailsPrinted < NumbOfEmails) {
				printf(":%s", &address_book->list[index].email_addresses[emailsPrinted][0]);
				for (int l = 32 - strlen(&address_book->list[index].email_addresses[emailsPrinted][0]); l > 0; l--)
					printf(" ");
				emailsPrinted++;
			} else
				for (int p = 32 ; p > 0; p--)
					printf(" ");
			printf(":\n"); //end of row
		}

		printf("===========================================================================================================\n");

	int canMove = 0;
	do {
		printf("Exit: [0] | Next Page: [2] | Previous Page: [1]:   ");
		option = get_option(NUM, "");
		printf("%d\n", option);

		if (option == 0) {
			menu(address_book);
		} else if (option == 2) {
			if (index < (address_book->count-1)) {
				index++;
				list_contacts(address_book, index);
			} else {
				printf("Sorry, Youre on the last page.\n");
			}
		} else if (option == 1) {
			if (index > 0) {
				index--;
				list_contacts(address_book, index);
			} else {
				printf("Sorry, Youre on the first page.\n");
			}
		}
	} while (canMove == 0);

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

#ifndef __linux__
		system("cls");
#else
		system("clear");
#endif

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
		main_menu(); //prints main menu

		option = get_menu_option();


		if ((address_book->count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found! Would you like to add? Use Add Contacts");

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
			list_contacts(address_book, 0);
			break;
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
	strncpy(*contact.phone_numbers, " ", sizeof(contact.phone_numbers));
	strncpy(*contact.email_addresses, " ", sizeof(contact.email_addresses));

	do
	{
		menu_header("Add Contact:\n");
		printf("0. Back\n");
		printf("1. Name\t\t: %s\n", contact.name[0]);
		printf("2. Phone No 1\t: %s\n", contact.phone_numbers[0]);
		for (int i = 1; i < phoneCount; i++)
			printf("\t    %d\t: %s\n", i + 1, contact.phone_numbers[i]);
		printf("3. Email ID 1\t: %s\n", contact.email_addresses[0]);
		for (int i = 1; i < emailCount; i++)
			printf("\t    %d\t: %s\n", i + 1, contact.email_addresses[i]);

		printf("\nPlease select an option: ");
		option = get_option(NUM, "");

		switch (option)
		{
		case e_first_opt:
			break;
		case e_second_opt:
			printf("Enter the name: ");
			scanf("%s", &contact.name[0][0]);
			if (strlen(*contact.name) > NAME_LEN)
			{
				printf("ERROR. Invalid Length of Name. Please enter a key to continue: ");
				getchar();
			}
			break;
		case e_third_opt:
			if (phoneCount < PHONE_NUMBER_COUNT)
			{
				printf("Enter Phone Number %d: [Please reenter the same option of alternate Phone Number]: ", phoneCount + 1);
				scanf("%s", &contact.phone_numbers[phoneCount][0]);
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
			if (emailCount < EMAIL_ID_COUNT)
			{
				printf("Enter Email ID %d: [Please reenter the same option of alternate Email ID]: ", emailCount + 1);
				scanf("%s", &contact.email_addresses[emailCount][0]);
				if (strlen(&contact.email_addresses[emailCount][0]) > EMAIL_ID_LEN)
				{
					printf("ERROR. Invalid Length of Email. Please enter a key to continue: ");
					getchar();
				}
				else
					emailCount++;
			}
			else
			{
				printf("ERROR. You have reached maximum number of Emails. Please enter a key to continue: ");
				getchar();
			}
			break;
		}
		getchar();

	} while (option != 0);

	contact.si_no = address_book->count + 1;

	address_book->list[address_book->count] = contact;
	address_book->count++;

	return e_success;
}


Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	int found = 0;
	ContactInfo contact;

		switch (field)
		{
			case 1: //name
				for (int i = 0; i < address_book->count; i++)
				{
					ContactInfo contact = address_book->list[i];

					for (int j = 0; j < NAME_COUNT; j++)
					{
						if (strcmp(str, &contact.name[j][0]) == 0)
						{
							found = 1;
							j = NAME_COUNT; // to exit inner loop
							i = address_book->count; //to exit outer loop
							break;
						}
					}
				}
				break;
			case 2: //phone
				for (int i = 0; i < address_book->count; i++)
				{
					//ContactInfo contact = *(address_book->list + i*sizeof(ContactInfo));
					ContactInfo contact = address_book->list[i];

					for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
					{
						if (strcmp(str, &contact.phone_numbers[j][0]) == 0)
						{
							found = 1;
							j = NAME_COUNT; // to exit inner loop
							i = address_book->count; //to exit outer loop
							break;
						}
					}
				}
				break;
			case 3: //email
				for (int i = 0; i < address_book->count; i++)
				{
					ContactInfo contact = address_book->list[i];

					for (int j = 0; j < EMAIL_ID_COUNT; j++)
					{
						if (strcmp(str, &contact.email_addresses[j][0]) == 0)
						{
							found = 1;
							j = NAME_COUNT; // to exit inner loop
							i = address_book->count; //to exit outer loop
							break;
						}
					}
				}
				break;
			case 4: //serial no.
				for (int i = 0; i < address_book->count; i++)
				{
					ContactInfo contact = address_book->list[i];
						if (loop_count == contact.si_no)
						{
							found = 1;
							i = address_book->count; //to exit outer loop
							break;
						}
				}
				break;
			default:
				break;
		}

		//printing results:

		printf("####### Address Book #######\n");
		printf("####### Search Results:\n");

		if(found == 0)
		{
			printf("\nNo contacts where found with the given input.\n");
			

			char addContact;

			printf("Would you like to add a new contact? y/n\n");
			while ((getchar()) != '\n')
				; //cleaning buffer
			addContact = getchar();

			if (addContact == 'y')
				add_contacts(address_book);

			
			return e_no_match;
			
		}
		else
		{
			printf("===========================================================================================================\n");
			printf(":S.No  :Name                            :Phone No                        :Email ID                        :\n");
			printf("===========================================================================================================\n");

			//begin printing the first row of information
			printf(":%d", contact.si_no);
			if (contact.si_no < 10)
			{
				for (int i = 0; i < 5; i++)
					printf(" ");
			}
			else
			{
				for (int i = 0; i < 4; i++)
					printf(" ");
			}
			printf(":%s", &contact.name[0][0]);
			for (int i = 32 - strlen(&contact.name[0][0]); i > 0; i--)
			{
				printf(" ");
			}

			int NumbOfPhones = sizeof contact.phone_numbers / sizeof *contact.phone_numbers;
			int NumbOfEmails = sizeof contact.email_addresses / sizeof *contact.email_addresses;
			int phonesPrinted = 0;
			int emailsPrinted = 0;

			if (NumbOfPhones > 0)
			{
				printf(":%s", &contact.phone_numbers[0][0]);
				phonesPrinted++;
				for (int i = 32 - strlen(&contact.phone_numbers[0][0]); i > 0; i--)
					printf(" ");
			}
			else
				for (int i = 32 ; i > 0; i--)
					printf(" ");

			if (NumbOfEmails > 0)
			{
				printf(":%s", &contact.email_addresses[0][0]);
				emailsPrinted++;
				for (int i = 32 - strlen(&contact.email_addresses[0][0]); i > 0; i--)
					printf(" ");
			}
			else
				for (int i = 32 ; i > 0; i--)
					printf(" ");
			printf(":\n"); //end of first row

			for (int k = 0; k < 4; k++) //printing multiple phones and emails.
			{
				printf(":      :                                "); //empty si no and name.

				if (phonesPrinted < NumbOfPhones)
				{
					printf(":%s", &contact.phone_numbers[phonesPrinted][0]);
					for (int m = 32 - strlen(&contact.phone_numbers[phonesPrinted][0]); m > 0; m--)
						printf(" ");
					phonesPrinted++;
				}
				else
					for (int n = 32 ; n > 0; n--)
						printf(" ");

				if (emailsPrinted < NumbOfEmails)
				{
					printf(":%s", &contact.email_addresses[emailsPrinted][0]);
					for (int l = 32 - strlen(&contact.email_addresses[emailsPrinted][0]); l > 0; l--)
						printf(" ");
					emailsPrinted++;
				}
				else
					for (int p = 32 ; p > 0; p--)
						printf(" ");
				printf(":\n"); //end of row
			}

			printf("===========================================================================================================\n");

			char quit;
			quit = getchar(); //clearing buffer
			do
			{
				printf("Press: [q] | Cancel: [q]:   ");
				quit = getchar();
			} while (quit != 'q');
		}

}

Status search_contact(AddressBook *address_book)
{
	char str[32];
	int option;
	int flag = 0;

	menu_header("Search Contact by:\n");
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n");
	printf("Please select an option: ");

	while (flag == 0)
	{
		option = get_option(NUM, "");

		switch (option)
		{
		case 0:
			flag = 1;
			return e_back;
			break;
		case 1:
			printf("Enter the name: ");
			scanf("%s", str);
			search(str, address_book, 0, option, "", e_search);
			flag = 1;
			break;
		case 2:
			printf("Enter the phone: ");
			scanf("%s", str);
			search(str, address_book, 0, option, "", e_search);
			flag = 1;
			break;
		case 3:
			printf("Enter the email: ");
			scanf("%s", str);
			search(str, address_book, 0, option, "", e_search);
			flag = 1;
			break;
		case 4:
			printf("Enter the serial no.: ");
			int myInt;
			scanf("%d", &myInt);
			search("", address_book, myInt, option, "", e_search);
			flag = 1;
			break;
		default:
			printf("\nWrong input. Going back to main menu.");
			flag = 1;
			return e_back;
			break;
		}
	}
}

Status edit_contact(AddressBook *address_book)
{
	char str[32];
	int option;
	int flag = 0;

	menu_header("Search Contact to Edit by:\n");
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No\n");
	printf("3. Email ID\n");
	printf("4. Serial No\n");
	printf("Please select an option: ");

	while (flag == 0)
	{
		option = get_option(NUM, "");

		switch (option)
		{
		case 0:
			flag = 1;
			return e_back;
			break;
		case 1:
			printf("Enter the name: ");
			scanf("%s", str);
            for (int i = 0; i < address_book->count; i++) {
				printf("loop number: %d\n", i);
					ContactInfo contact = address_book->list[i];
                    for (int j = 0; j < NAME_COUNT; j++) {
						if (strcmp(str, &contact.name[j][0]) == 0) {
							printf("Prepping to call edit with name %s and index %d\n", str, i);
							edit(address_book, i);
							j = NAME_COUNT; // to exit inner loop
							i = address_book->count; //to exit outer loop
							break;
						}
					}
				}
			flag = 1;
			break;
		case 2:
			printf("Enter the phone: ");
			scanf("%s", str);
            for (int i = 0; i < address_book->count; i++) {
				ContactInfo contact = address_book->list[i];
                for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
					if (strcmp(str, &contact.phone_numbers[j][0]) == 0) {
						printf("Prepping to call edit with phone and index %d: ", i);
						edit(address_book, i);
						j = NAME_COUNT; // to exit inner loop
						i = address_book->count; //to exit outer loop
						break;
					}
				}
			}
			flag = 1;
			break;
		case 3:
			printf("Enter the email: ");
			scanf("%s", str);
			for (int i = 0; i < address_book->count; i++) {
				ContactInfo contact = address_book->list[i];
                for (int j = 0; j < EMAIL_ID_COUNT; j++) {
					if (strcmp(str, &contact.email_addresses[j][0]) == 0) {
						printf("Prepping to call edit with email and index %d: ", i);
						edit(address_book, i);
						j = NAME_COUNT; // to exit inner loop
						i = address_book->count; //to exit outer loop
						break;
					}
				}
			}
			flag = 1;
			break;
		case 4:
			printf("Enter the serial no.: ");
			int myInt;
			scanf("%d", &myInt);
            for (int i = 0; i < address_book->count; i++) {
				ContactInfo contact = address_book->list[i];
				if (myInt == contact.si_no) {
					printf("Prepping to call edit with serial and index %d: ", i);
					edit(address_book, i);
					i = address_book->count; //to exit outer loop
					break;
				}
			}
			flag = 1;
			break;
		default:
			printf("\nWrong input. Going back to main menu.");
			flag = 1;
			return e_back;
			break;
		}
	}
	return e_success;
}

Status edit(AddressBook *address_book, int index)
{
	//Similar structure to add_contact function to maintain UI consistency.
	ContactInfo replacementContact;
	int option;
	int phoneCount = 0;
	int emailCount = 0;

	strcpy(*replacementContact.name, " ");
	strncpy(*replacementContact.phone_numbers, " ", sizeof(replacementContact.phone_numbers));
	strncpy(*replacementContact.email_addresses, " ", sizeof(replacementContact.email_addresses));

	int loopStart = 0;

	do
	{
		if (loopStart == 0) {
			//menu_header("Edit Contact:\n");
			printf("Index: %d\n", index);
			printf("LoopStart 0\n");
			printf("0. Back\n");
			printf("1. Name\t\t: %s\n", address_book->list[index].name[0]);
			printf("2. Phone No 1\t: %s\n", address_book->list[index].phone_numbers[0]);
			for (int i = 1; i < phoneCount; i++)
				printf("\t    %d\t: %s\n", i + 1, address_book->list[index].phone_numbers[i]);
			printf("3. Email ID 1\t: %s\n", address_book->list[index].email_addresses[0]);
			for (int i = 1; i < emailCount; i++)
				printf("\t    %d\t: %s\n", i + 1, address_book->list[index].email_addresses[i]);
			loopStart = 1;
		} else {
			menu_header("Edit Contact:\n");
			printf("LoopStart 1\n");
			printf("0. Back\n");
			printf("1. Name\t\t: %s\n", replacementContact.name[0]);
			printf("2. Phone No 1\t: %s\n", replacementContact.phone_numbers[0]);
			for (int i = 1; i < phoneCount; i++)
				printf("\t    %d\t: %s\n", i + 1, replacementContact.phone_numbers[i]);
			printf("3. Email ID 1\t: %s\n", replacementContact.email_addresses[0]);
			for (int i = 1; i < emailCount; i++)
				printf("\t    %d\t: %s\n", i + 1, replacementContact.email_addresses[i]);

		}

		printf("\nPlease select an option: ");
		option = get_option(NUM, "");

		switch (option)
		{
		case e_first_opt:
			break;
		case e_second_opt:
			printf("Enter the name: ");
			scanf("%s", &replacementContact.name[0][0]);
			if (strlen(*replacementContact.name) > NAME_LEN)
			{
				printf("ERROR. Invalid Length of Name. Please enter a key to continue: ");
				getchar();
			}
			break;
		case e_third_opt:
			if (phoneCount < PHONE_NUMBER_COUNT)
			{
				printf("Enter Phone Number %d: [Please reenter the same option of alternate Phone Number]: ", phoneCount + 1);
				scanf("%s", &replacementContact.phone_numbers[phoneCount][0]);
				if (strlen(replacementContact.phone_numbers[phoneCount]) > NUMBER_LEN)
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
			if (emailCount < EMAIL_ID_COUNT)
			{
				printf("Enter Email ID %d: [Please reenter the same option of alternate Email ID]: ", emailCount + 1);
				scanf("%s", &replacementContact.email_addresses[emailCount][0]);
				if (strlen(&replacementContact.email_addresses[emailCount][0]) > EMAIL_ID_LEN)
				{
					printf("ERROR. Invalid Length of Email. Please enter a key to continue: ");
					getchar();
				}
				else
					emailCount++;
			}
			else
			{
				printf("ERROR. You have reached maximum number of Emails. Please enter a key to continue: ");
				getchar();
			}
			break;
		}
		getchar();

	} while (option != 0);

	replacementContact.si_no = (address_book->list[address_book->count-1].si_no) + 1;

	address_book->list[index] = replacementContact;

	return e_success;
}

Status delete (AddressBook *address_book, int index)
{
	unsigned int last_index = address_book->count - 1;
	ContactInfo contact_to_delete = address_book->list[index];
	ContactInfo last_contact = address_book->list[last_index];

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

		for (int i = 0; i < address_book->count; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			for (int j = 0; j < NAME_COUNT; j++)
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

		for (int i = 0; i < address_book->count; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
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

		for (int i = 0; i < address_book->count; i++)
		{
			ContactInfo *contact = &address_book->list[i];

			for (int j = 0; j < EMAIL_ID_COUNT; j++)
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

		for (int i = 0; i < address_book->count; i++)
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
