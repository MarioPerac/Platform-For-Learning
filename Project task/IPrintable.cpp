#include "IPrintable.h"

std::ostream& operator<<(std::ostream& os, IPrintable& object) noexcept
{
	object.printToConsole(os);
	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, IPrintable& object) noexcept
{
	object.printToFile(ofs);
	return ofs;
}
