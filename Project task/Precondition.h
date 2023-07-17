#pragma once
#include "Student.h"
#include "IPrintable.h"
#include "IReadable.h"

class Precondition : virtual public IReadable, virtual public IPrintable
{
protected:
	virtual void printToConsole(std::ostream& os) noexcept = 0;
	virtual void printToFile(std::ofstream& ofs) noexcept = 0;

	virtual void readFromConsole(std::istream& is) noexcept = 0;
	virtual void readFromFile(std::ifstream& ifs) noexcept = 0;
public:
	virtual bool check(const std::string& studentUsername) noexcept = 0;
};
