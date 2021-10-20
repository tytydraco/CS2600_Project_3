#ifndef ABK_MENUS_H
#define ABK_MENUS_H

#include "address_book.h"

#define	NONE							0
#define	NUM								1
#define	CHAR							2

#define WINDOW_SIZE						5

void menu_header(const char *str);
void main_menu(void);
Status menu(AddressBook *address_book);
Status save_prompt(AddressBook *address_book);
Status add(AddressBook *address_book, ContactInfo contact, int option);
Status search_contact(AddressBook *address_book);
Status edit_contact(AddressBook *address_book);
Status edit(AddressBook *address_book, int index);
Status delete_contact(AddressBook *address_book);
Status list_contacts(AddressBook *address_book, int index);

#endif
