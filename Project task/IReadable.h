#pragma once
#include <fstream>

class IReadable
{
protected:
	virtual void readFromConsole(std::istream& is) noexcept = 0;
	virtual void readFromFile(std::ifstream& ifs) noexcept = 0;
public:
	friend std::istream& operator>>(std::istream& is, IReadable& object) noexcept;
	friend std::ifstream& operator>>(std::ifstream& ifs, IReadable& object) noexcept;
};
