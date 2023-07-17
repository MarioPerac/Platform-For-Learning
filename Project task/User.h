#pragma once
#include "IReadable.h"
#include "IPrintable.h"
#include <string>

class User : virtual public IReadable, virtual public IPrintable
{
protected:
	std::string username;
	std::string password;
	std::string firstName;
	std::string lastName;
public:
	std::string getUsername() noexcept;
	std::string getPassword() noexcept;
	std::string getFirstName() noexcept;
	std::string getLastName() noexcept;

	virtual void showMenu() noexcept = 0;

	friend bool login(User** user) noexcept;
protected:
	void encryptionPassword(std::string& password) noexcept;

	virtual void readFromConsole(std::istream& is) noexcept override;
	virtual void readFromFile(std::ifstream& ifs) noexcept override;
	virtual void printToConsole(std::ostream& os) noexcept override;
	virtual void printToFile(std::ofstream& ofs) noexcept override;
};

