#pragma once
#include "Precondition.h"
#include "Grade.h"
#include <map>

class PreconditionMinimalGrade : virtual public Precondition
{
private:
	std::map<std::string, Grade> courses;
protected:
	virtual void printToConsole(std::ostream& os) noexcept override;
	virtual void printToFile(std::ofstream& ofs) noexcept override;

	virtual void readFromConsole(std::istream& is) noexcept override;
	virtual void readFromFile(std::ifstream& ifs) noexcept override;
public:
	virtual bool check(const std::string& studentUsername) noexcept override;
};
