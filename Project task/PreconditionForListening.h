#pragma once
#include "Precondition.h"
#include <vector>

class PreconditionForListening : virtual public Precondition
{
private:
	std::vector<std::string> courseNames;
protected:
	virtual void printToConsole(std::ostream& os) noexcept override;
	virtual void printToFile(std::ofstream& ofs) noexcept override;

	virtual void readFromConsole(std::istream& is) noexcept override;
	virtual void readFromFile(std::ifstream& ifs) noexcept override;
public:
	virtual bool check(const std::string& studentUsername) noexcept override;
};