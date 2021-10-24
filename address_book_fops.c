#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status AddressBookInit(AddressBook *address_book)
{
	address_book->count = 0;
	address_book->list = (ContactInfo *)calloc(100, sizeof(ContactInfo));

	return e_success;
}

Status load_file(AddressBook *address_book)
{
	int ret = 0;

	// Checks for file existence
	if ((address_book->fp = fopen(DEFAULT_FILE, "r")))
	{
		fclose(address_book->fp);
		ret = 1;
	}

	if (ret == 1)
	{

		address_book->fp = fopen(DEFAULT_FILE, "a+");
		AddressBookInit(address_book);
		int counter = 0;
		char line[1000];
		char *tok;

		// parses csv file and adds contacts to list if there are any existing contacts
		while(fgets(line, sizeof(line), address_book->fp) != NULL)
		{
			tok = strtok(line, ",");
			while(tok != NULL)
			{
				// adds serial
				address_book->list[counter].si_no = atoi(tok);
				printf("%d\n", address_book->list[counter].si_no);
				tok = strtok(NULL, ",");
				// adds name

				strcpy(address_book->list[counter].name[0], tok);
				printf("%s\n", address_book->list[counter].name[0]);
				tok = strtok(NULL, ",");

				//adds phone numbers
				// for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
				// {
				// 	strcpy(address_book->list[counter].phone_numbers[i], tok);
				// 	printf("%s\n", address_book->list[counter].phone_numbers[i]);
				// 	tok = strtok(NULL, ",");
				// }

				// //adds emails
				// for (int i = 0; i < EMAIL_ID_COUNT; i++)
				// {
				// 	printf("TEST2\n");
				// 	strcpy(address_book->list[counter].email_addresses[i], tok);
				// 	printf("TEST3\n");
				// 	printf("%s\n", address_book->list[counter].email_addresses[i]);
				// 	tok = strtok(NULL, ",");
				// }
				counter++;
				address_book->count++;
			}
		}
	}
	else
	{
		AddressBookInit(address_book);
		address_book->fp = fopen(DEFAULT_FILE, "w");
	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
    /*
     * Write contacts back to file.
     * Re write the complete file currently
     */
    
    address_book->fp = fopen(DEFAULT_FILE, "w");
    for (int i = 0; i < address_book->count; i++)
    {
        fprintf(address_book->fp, "%d", address_book->list[i].si_no);
        fprintf(address_book->fp, FIELD_DELIMITER);
        fprintf(address_book->fp, "%s", address_book->list[i].name[0]);
        fprintf(address_book->fp, FIELD_DELIMITER);

        for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
        {
            fprintf(address_book->fp, "%s", address_book->list[i].phone_numbers[j]);
            fprintf(address_book->fp, FIELD_DELIMITER);
        }

        for (int j = 0; j < EMAIL_ID_COUNT; j++)
        {
            fprintf(address_book->fp, "%s", address_book->list[i].email_addresses[j]);
            fprintf(address_book->fp, FIELD_DELIMITER);
        }

        fputs("\n", address_book->fp);
    }


        if (address_book->fp == NULL)
        {
            return e_fail;
        }

    /* 
     * Add the logic to save the file
     * Make sure to do error handling
     */

    fclose(address_book->fp);

    return e_success;
}
