#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book_fops.h"

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
		

		
		//************* populating address_book from file.
		char chr;
		int counter = 0;
		int iterator = 0;


		chr = getc(address_book->fp);

		while((chr != EOF) )
		{
			ContactInfo *ptr = (ContactInfo*)calloc(1, sizeof(ContactInfo));
			
			while ((chr != '\n') && (chr != EOF))
			{

				while (chr != ',') //get name
				{
					ptr->name[0][counter] = chr;
					counter++;				
					chr = getc(address_book->fp);
				}
				
				
				counter = 0;

				for (int i = 0; i < 5; i++) //get phones
				{	
					chr = getc(address_book->fp); // the previous char was a comma
					while (chr != ',') //iterating through phones
					{
						ptr->phone_numbers[i][counter] = chr;
						counter++;
						chr = getc(address_book->fp);
					}
					counter = 0;
				}

				
				

				for (int i = 0; i < 5; i++) //get emails
				{
					counter = 0;

					chr = getc(address_book->fp); // the previous char was a comma
					while (chr != ',') //iterating through emails
					{
						ptr->email_addresses[i][counter] = chr;
						counter++;
						chr = getc(address_book->fp);
					}
				}
				

				ptr->si_no = iterator + 1; //assigning serial no value
				chr = getc(address_book->fp); // the previous char was a comma, now it is EOF or \n
				counter = 0;

				
				address_book->list[iterator] = *ptr; //writting person into addressbook
				
				address_book->count++; 
				iterator++;
			}
			
			chr = getc(address_book->fp); //previous char was \n
			free(ptr);

		}
	}
	else
		address_book->fp = fopen(DEFAULT_FILE, "w");


	//TESTING

	//ContactInfo test1 = *(address_book->list + 0); 
	//ContactInfo test2 = *(address_book->list + sizeof(ContactInfo));




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
