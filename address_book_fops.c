#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

void AddressBookInit(AddressBook *address_book)
{
	address_book->count = 0;
	address_book->list = (ContactInfo *)calloc(100, sizeof(ContactInfo));
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
		// if(address_book->fp != NULL)
		// {
		// 	int counter = 0;
		// }
	}
	else
		address_book->fp = fopen(DEFAULT_FILE, "w");

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	// address_book->fp = fopen(DEFAULT_FILE, "w"); 

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
