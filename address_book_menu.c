#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book.h"
#include "address_book_fops.c"
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

	//free(address_book->list); //why set this free? if the person wants to continue using the address book, this will throw off the program.

	return e_success;
}

Status list_contacts(AddressBook *address_book, int index)
{
	printf("Index is: %d\n", index);
	int option;

	menu_header("All Contacts:\n");
	printf("(Page %d of %d):\n", index, (address_book->count));
	printf("0. Back\n");
	printf("1. Next Page\n");
	printf("2. Last Page\n");

	int startingIndex = WINDOW_SIZE * index;

	while ((startingIndex < (address_book->count)) && (startingIndex < (WINDOW_SIZE * (index + 1))))
	{
		printf("Loop: %c\n", startingIndex);
		printf("%p\n", (&address_book->list[startingIndex].name[1][0]));
		startingIndex++;
	}

	printf("\nPlease select an option: ");
	option = get_option(NUM, "");
	printf("%d\n", option);

	if (option == 0)
	{
		printf("Detected 0\n");
		menu(address_book);
	}
	else if (option == 1)
	{
		printf("Detected 1\n");
		if (index < (address_book->count / WINDOW_SIZE))
		{
			index++;
			list_contacts(address_book, index);
		}
	}
	else if (option == 2)
	{
		printf("Detected 2\n");
		if (index > 0)
		{
			index--;
			list_contacts(address_book, index);
		}
	}

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

	#ifdef __linux__
		system("clear");
	#else
		system("cls");
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
//			edit_contact(address_book);
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
	strcpy(contact.phone_numbers[0], " ");
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
					ContactInfo contact = *(address_book->list + i*sizeof(ContactInfo));

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
					ContactInfo contact = *(address_book->list + i*sizeof(ContactInfo));

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
					ContactInfo contact = *(address_book->list + i*sizeof(ContactInfo));
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

	printf("####### Address Book #######\n");
	printf("####### Search Contact by:\n");
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
			scanf("%s", str);
			search(str, address_book, 0, option, "", e_search);
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

/*
Status edit_contact(AddressBook *address_book)
{
	int option;
	char target_name[NAME_LEN];
	char target_phone[NUMBER_LEN];
	char target_email[EMAIL_ID_LEN];
	int target_serial;

	menu_header("Search Contact to Edit by:\n");
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
					return edit(address_book, i);
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
					return edit(address_book, i);
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
					return edit(address_book, i);
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
				return edit(address_book, i);
		}
		printf("Contact not found!\n");
		return e_no_match;
	}
	getchar();
	return e_success;
}


Status edit(AddressBook *address_book, int index)
{
	ContactInfo replacement;
	int option;

	strncpy(*replacement.name, " ", sizeof(replacement.name));
	strncpy(*replacement.phone_numbers, " ", sizeof(replacement.phone_numbers));
	strncpy(*replacement.email_addresses, " ", sizeof(replacement.email_addresses));

	while (option != 0)
	{
		menu_header("Edit Contact:\n");
		printf("0. Back\n");
		printf("1. Name\t\t: %s\n", replacement.name);
		printf("2. Phone No 1\t: %s\n", replacement.phone_numbers);
		printf("3. Email ID 1\t: %s\n", replacement.email_addresses);
		printf("\nPlease select an option: ");

		option = get_option(NUM, "");

		switch (option)
		{
		case e_first_opt:
			menu(address_book);
			break;
		case e_second_opt:
			printf("Enter the name: ");
			scanf("%s", replacement.name);
			fprintf(address_book->fp, *replacement.name);
			fprintf(address_book->fp, FIELD_DELIMITER);
			break;
		case e_third_opt:
			printf("Enter Phone Number: ");
			scanf("%s", replacement.phone_numbers);
			fprintf(address_book->fp, *replacement.phone_numbers);
			fprintf(address_book->fp, FIELD_DELIMITER);
			break;
		case e_fourth_opt:
			printf("Enter Email: ");
			scanf("%s", replacement.email_addresses);
			fprintf(address_book->fp, *replacement.email_addresses);
			fprintf(address_book->fp, FIELD_DELIMITER);
			break;
		}
		getchar();
	}
	ContactInfo oldContact = address_book->list[index];
	oldContact = replacement;
	return e_success;
} */

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
