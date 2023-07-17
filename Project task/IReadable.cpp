#include "IReadable.h"

std::istream& operator>>(std::istream& is, IReadable& object) noexcept
{
	object.readFromConsole(is);
	return is;
}

std::ifstream& operator>>(std::ifstream& ifs, IReadable& object) noexcept
{
	object.readFromFile(ifs);
	return ifs;
}
