#include "PreconditionMinimalGrade.h"
#include "Course.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void PreconditionMinimalGrade::printToConsole(std::ostream& os) noexcept
{
	os << "Precondition Minimal Grade contains courses: \n";

	for (auto iterator : courses)
	{
		os << "Course name: " << iterator.first << " " << "Minimal grade: " << iterator.second.grade << '\n';
	}
}

void PreconditionMinimalGrade::printToFile(std::ofstream& ofs) noexcept
{
	ofs << "COURSE NAME,MINIMAL GRADE";

	for (auto iterator : courses)
	{
		ofs << '\n' << iterator.first << ',' << iterator.second.grade;
	}
}

void PreconditionMinimalGrade::readFromConsole(std::istream& is) noexcept
{
	int numberCourses;

	std::cout << "Enter the courses number for precondition: ";
	is >> numberCourses;

	if (numberCourses < 1)
		return;

	system("cls");

	for (int i = 0; i < numberCourses; i++)
	{
		std::string courseName;
		Grade minimalGrade;

		char buffer;
		is.get(buffer);

		std::cout << i + 1 << "." << " " << "Enter course name: ";
		std::getline(is, courseName);

		std::cout << "Enter minimal grade: ";
		is >> minimalGrade.grade;
	
		courses.insert({ courseName, minimalGrade});
	}

	system("cls");
}

void PreconditionMinimalGrade::readFromFile(std::ifstream& ifs) noexcept
{
	std::string name, grade, dummyLine;
	

	std::getline(ifs, dummyLine, '\n');

	while (!ifs.eof())
	{
		std::getline(ifs, name, ',');
		std::getline(ifs, grade, '\n');

		Grade minimalGrade;
		minimalGrade.grade = stoi(grade);
		courses.insert({ name, minimalGrade });
	}
}

bool PreconditionMinimalGrade::check(const std::string& studentUsername) noexcept
{
	for (auto iterator : courses)
	{
		fs::path folderPath = fs::current_path() / "Courses";

		Course course;
		course.readFromFolder(folderPath, iterator.first);

		if (!course.checkStudentsWhoPassedUsingGrade(studentUsername, iterator.second.grade))
		{
			return false;
		}
	}
	return true;
}
