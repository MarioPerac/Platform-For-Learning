#include "PreconditionPass.h"
#include "Course.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void PreconditionPass::printToConsole(std::ostream& os) noexcept
{
	os << "Precondition Pass contains courses: \n";

	for (int i = 0; i < courseNames.size(); i++)
	{
		os << i + 1 << "." << " " << courseNames[i] << '\n';
	}
}

void PreconditionPass::printToFile(std::ofstream& ofs) noexcept
{
	ofs << "COURSE NAME";

	for (int i = 0; i < courseNames.size(); i++)
	{
		ofs << '\n' << courseNames[i];
	}
}

void PreconditionPass::readFromConsole(std::istream& is) noexcept
{
	int numberCourses;

	std::cout << "Enter the courses number for precondition: ";
	is >> numberCourses;

	if (numberCourses < 1)
		return;

	courseNames.resize(numberCourses);

	system("cls");

	char buffer;
	is.get(buffer);

	for (int i = 0; i < courseNames.size(); i++)
	{
		std::cout << i + 1 << "." << " " << "Enter course name: ";
		std::getline(is, courseNames[i]);
	}

	system("cls");
}

void PreconditionPass::readFromFile(std::ifstream& ifs) noexcept
{
	std::string name, dummyLine;
	std::getline(ifs, dummyLine, '\n');

	while (!ifs.eof())
	{
		std::getline(ifs, name, '\n');
		courseNames.emplace_back(name);
	}
}

bool PreconditionPass::check(const std::string& studentUsername) noexcept
{
	for (int i = 0; i < courseNames.size(); i++)
	{
		fs::path folderPath = fs::current_path() / "Courses";

		Course course;
		course.readFromFolder(folderPath, courseNames[i]);

		if (!course.checkStudentsWhoPassed(studentUsername))
		{
			return false;
		}
	}
	return true;
}
