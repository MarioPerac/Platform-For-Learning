#pragma once
#include <fstream>

class IPrintable 
{
protected:
	virtual void printToConsole(std::ostream& os) noexcept = 0;
	virtual void printToFile(std::ofstream& ofs) noexcept = 0;
public:
	friend std::ostream& operator<<(std::ostream& os, IPrintable& object) noexcept;
	friend std::ofstream& operator<<(std::ofstream& ofs, IPrintable& object) noexcept;
};