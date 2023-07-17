#include "User.h"
#include "Student.h"
#include "Professor.h"
#include "Administrator.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <filesystem>

namespace fs = std::filesystem;

std::string User::getUsername() noexcept
{
	return username;
}

std::string User::getPassword() noexcept
{
	return password;
}

std::string User::getFirstName() noexcept
{
	return firstName;
}

std::string User::getLastName() noexcept
{
	return this->lastName;
}

void User::encryptionPassword(std::string& password) noexcept
{
	
	int character;

	do
	{
		character = _getch();

		if(character == 8  && !password.empty())
		{ 
			std::cout << "\b \b";
			password.pop_back();
		}
		else if(character != 13 && character != 8)
		{
			password.push_back(character);
			std::cout << '*';
		}

	} while (character != 13);

	std::cout << std::endl;
}

void User::readFromConsole(std::istream& is) noexcept
{
	std::cout << "Enter username: ";
	is >> username;

	std::cout << "Enter password: ";
	encryptionPassword(password);

	std::cout << "Enter first name: ";
	is >> firstName;

	std::cout << "Enter last name: ";
	is >> lastName;
}

void User::readFromFile(std::ifstream& ifs) noexcept
{
	std::string dummyLine;
	std::getline(ifs, dummyLine, '\n');
	std::getline(ifs, username, ',');
	std::getline(ifs, password, ',');
	std::getline(ifs, firstName, ',');
	std::getline(ifs, lastName, '\n');
}

void User::printToConsole(std::ostream& os) noexcept
{
	os << firstName << " " << lastName;
}

void User::printToFile(std::ofstream& ofs) noexcept
{
	ofs << "USERNAME,PASSWORD,FIRST NAME,LAST NAME\n";
	ofs << username << ',' << password << ',' << firstName << ',' << lastName;
}

bool login(User** user) noexcept
{
	std::string username, password;
	std::cout << "LOGIN FORM\n";

	std::cout << "Enter username: ";
	std::cin >> username; 

	std::cout << "Enter password: ";
	(*user)->encryptionPassword(password);

	system("cls");

	std::ifstream file;

	file.open(fs::current_path() / "Students" / username / (username + ".csv"), std::ios::in);

	if (file.is_open())
	{
		*user = new Student;
		User* student = *user;

		file >> *student;
		file.close();

		if (student->username == username && student->password == password)
		{

			std::cout << "Successful login!";
			Sleep(1500);

			system("cls");

			return true;
		}

		std::cout << "Unsuccessful login!";
		Sleep(1500);

		system("cls");

		return false;

	}

	file.open(fs::current_path() / "Professors" / username / (username + ".csv"), std::ios::in);

	if (file.is_open())
	{
		*user = new Professor;
		User* professor = *user;
		
		file >> *professor;
		file.close();

		if (professor->username == username && professor->password == password)
		{
			std::cout << "Successful login!";
			Sleep(1500);

			system("cls");

			return true;
		}

		std::cout << "Unsuccessful login!";
		Sleep(1500);

		system("cls");

		return false;
	}

	file.open(fs::current_path() / "Administrator" / (username + ".csv"), std::ios::in);

	if (file.is_open())
	{
		*user = new Administrator;
		User* admin = *user;

		file >> *admin;
		file.close();

		if (admin->username == username && admin->password == password)
		{
			std::cout << "Successful login!";
			Sleep(1500);

			system("cls");

			return true;
		}

		std::cout << "Unsuccessful login!";
		Sleep(1500);

		system("cls");

		return false;
	}


	std::cout << "Unsuccessful login!";
	Sleep(1500);

	system("cls");

	return false;
}

