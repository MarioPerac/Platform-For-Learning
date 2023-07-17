#include <iostream> 
#include "User.h"

int main()
{
	User* user = nullptr;
	int choice = 0;

	do
	{
		std::cout << "WELCOME!" << "\n\n";
		std::cout << "[1] Log In\n";
		std::cout << "[0] Close the system\n";
		std::cout << "Select option: ";

		std::cin >> choice;
		std::getchar();

		system("cls");

		if (choice == 1)
		{
			
			if (login(&user))
			{
				user->showMenu();
				delete user;
			}
			
		}

	} while (choice != 0);
	

	return 0;
}