#include "Course.h"
#include "Professor.h"
#include "PreconditionForListening.h"
#include "PreconditionPass.h"
#include "PreconditionMinimalGrade.h"
#include <iostream>


Course::~Course() noexcept
{
	if (!preconditions.empty())
	{
		preconditions.clear();
	}
}

void Course::readFromConsole(std::istream& is) noexcept 
{
	std::cout << "Enter course name: ";
	std::getline(is, courseName);

	std::cout << "Enter course acronym: ";
	is >> acronym;

	std::cout << "Enter professor username: "; 
	is >> professorUsername;

	std::ofstream professorCoursesFile(fs::current_path() / "Professors" / professorUsername / "Courses.csv", std::ios::app);
	professorCoursesFile << '\n' << courseName;
	professorCoursesFile.close();

	fs::path folderPath = fs::current_path() / "Courses" ;

	std::ofstream file(folderPath / "AvailableCourses.csv", std::ios::app);
	file << *this;
	file.close();

	folderPath = folderPath / courseName;

	fs::create_directory(folderPath);

	std::ofstream studentsWhoPassedFile(folderPath / "StudentsWhoPassed.csv", std::ios::out);
	studentsWhoPassedFile << "STUDENT USERNAME";
	studentsWhoPassedFile.close();

	std::ofstream attendingStudentsFile(folderPath / "AttendingStudents.csv", std::ios::out);
	attendingStudentsFile << "STUDENT USERNAME";
	attendingStudentsFile.close();

	std::ofstream studentsWhoListenedFile(folderPath / "StudentsWhoListened.csv", std::ios::out);
	studentsWhoListenedFile << "STUDENT USERNAME";
	studentsWhoListenedFile.close();

	std::ofstream courseApplicationFile(folderPath / "CourseApplications.csv", std::ios::out);
	courseApplicationFile << "STUDENT USERNAME";
	courseApplicationFile.close();

	std::cout << "Information about preconditions;" << std::endl;
	this->addPreconditions(folderPath);
	
}

void Course::readFromFile(std::ifstream& ifs) noexcept
{

		std::getline(ifs, acronym, ',');
		std::getline(ifs, courseName, ',');
		std::getline(ifs, professorUsername, '\n');
}

void Course::printToConsole(std::ostream& os) noexcept
{
	os << "Course acronym: " << acronym << std::endl;
	os << "Course name: " << courseName << std::endl;

	Professor professor;

	std::ifstream file(fs::current_path() / "Professors" / professorUsername / (professorUsername + ".csv"), std::ios::in); 

	file >> professor;
	file.close();

	os << "Professor of the cours: " << professor << std::endl;
}

void Course::printToFile(std::ofstream& ofs) noexcept
{
	ofs << '\n' << acronym << ',' << courseName << ',' << professorUsername;
}

std::string Course::getCourseName() noexcept
{
	return courseName;
}

std::string Course::getAcronym() noexcept
{
	return acronym;
}

bool Course::checkCourseInFile(std::ifstream& ifs, std::string courseNameForCheck) noexcept
{
	std::string dummyLine;
	std::getline(ifs, dummyLine, '\n');

	do
	{
		ifs >> *this;
		if (courseNameForCheck.compare(this->courseName) == 0)
		{
			return true;
		}

	} while (!ifs.eof()); 

	return false;
}

bool Course::readFromFolder(fs::path folderPath, std::string courseNameForRead) noexcept
{
	fs::path filePath = folderPath / "AvailableCourses.csv";

	std::ifstream availableCoursesFile(filePath, std::ios::in);

	if (fs::is_empty(filePath))
	{
		return false;
	}

	if (!this->checkCourseInFile(availableCoursesFile, courseNameForRead))
	{
		return false;
	}

	availableCoursesFile.close();

	filePath = folderPath / courseName / "StudentsWhoPassed.csv";

	std::ifstream studentsWhoPassedFile(filePath, std::ios::in);

	std::string dummyLine; 

	std::getline(studentsWhoPassedFile, dummyLine, '\n');

	while (!studentsWhoPassedFile.eof() && !fs::is_empty(filePath))
	{
		std::string studentUsername, gradeString;
		Grade studentGrade;

		std::getline(studentsWhoPassedFile, studentUsername, ',');
		std::getline(studentsWhoPassedFile, gradeString, '\n');
		studentGrade.grade = stoi(gradeString);

		studentsWhoPassed.insert({ studentUsername, studentGrade }); 
	}

	studentsWhoPassedFile.close();

	filePath = folderPath / courseName / "AttendingStudents.csv";

	std::ifstream attendingStudentsFile(filePath, std::ios::in);

	std::getline(attendingStudentsFile, dummyLine, '\n');

	while (!attendingStudentsFile.eof() && !fs::is_empty(filePath))
	{
		std::string studentUsername;
		std::getline(attendingStudentsFile, studentUsername, '\n');
		attendingStudents.push_back(studentUsername);
	}

	attendingStudentsFile.close();

	filePath = folderPath / courseName / "StudentsWhoListened.csv";

	std::ifstream studentsWhoListenedFile(filePath, std::ios::in);

	std::getline(studentsWhoListenedFile, dummyLine, '\n');

	while (!studentsWhoListenedFile.eof() && !fs::is_empty(filePath))
	{
		std::string studentUsername;
		std::getline(studentsWhoListenedFile, studentUsername, '\n');
		studentsWhoListened.push_back(studentUsername);
	}

	studentsWhoListenedFile.close(); 

	filePath = folderPath / courseName / "PreconditionForListening.csv";  

	std::ifstream preconditionForListeningFile(filePath, std::ios::in);

	if (preconditionForListeningFile.is_open())
	{
		PreconditionForListening preconditionForListening;
		preconditionForListeningFile >> preconditionForListening;
		preconditions.push_back(new PreconditionForListening(preconditionForListening));

		preconditionForListeningFile.close();
	}

	filePath = folderPath / courseName / "PreconditionPass.csv";  

	std::ifstream preconditionPassFile(filePath, std::ios::in);

	if (preconditionPassFile.is_open())
	{
		PreconditionPass preconditionPass;
		preconditionPassFile >> preconditionPass;
		preconditions.push_back(new PreconditionPass(preconditionPass));

		preconditionPassFile.close();
	}

	filePath = folderPath / courseName / "PreconditionMinimalGrade.csv";  

	std::ifstream preconditionMinimalGradeFile(filePath, std::ios::in);

	if (preconditionMinimalGradeFile.is_open())
	{
		PreconditionMinimalGrade preconditionMinimalGrade;
		preconditionMinimalGradeFile >> preconditionMinimalGrade;
		preconditions.push_back(new PreconditionMinimalGrade(preconditionMinimalGrade));

		preconditionMinimalGradeFile.close();
	}

	return true;
}

bool Course::checkStudentsWhoPassed(const std::string& studentUsername) noexcept
{
	for (auto iterator = studentsWhoPassed.begin(); iterator != studentsWhoPassed.end(); ++iterator)
	{
		if (iterator->first == studentUsername)
		{
			return true;
		}
	}
	return false;
}

bool Course::checkStudentsWhoPassedUsingGrade(const std::string& studentUsername, int minimalGrade) noexcept
{
	for (auto iterator = studentsWhoPassed.begin(); iterator != studentsWhoPassed.end(); ++iterator)
	{
		if (iterator->first == studentUsername)
		{
			if (iterator->second.grade >= minimalGrade)
			{
				return true;
			}

			return false; 
		}
	}
	return false;
}

bool Course::checkAttendingStudents(const std::string& studentUsername) noexcept
{
	for (auto iterator = attendingStudents.begin(); iterator != attendingStudents.end(); ++iterator)
	{
	
		if ((*iterator) == studentUsername)
		{
			return true;
		}
	}
	return false;
}

bool Course::checkStudentsWhoListened(const std::string& studentUsername) noexcept
{
	for (auto iterator = studentsWhoListened.begin(); iterator != studentsWhoListened.end(); ++iterator)
	{

		if ((*iterator) == studentUsername)
		{
			return true;
		}
	}
	return false;
}

bool Course::checkAllPreconditions(const std::string& studentUsername) noexcept
{
	for (int i = 0; i < preconditions.size(); ++i)
	{
		if (!preconditions[i]->check(studentUsername))
		{
			return false;
		}
	}

	return true;
}

void Course::addStudentsWhoPassed(const std::string& studentUsername, const Grade& studentGrade) noexcept
{
	studentsWhoPassed.insert({ studentUsername, studentGrade });

	fs::path courseFilePath = fs::current_path() / "Courses" / this->courseName / "StudentsWhoPassed.csv";

	std::ofstream studentsWhoPassedFile(courseFilePath, std::ios::app);
	studentsWhoPassedFile << '\n' << studentUsername << ',' << studentGrade.grade;
	studentsWhoPassedFile.close();

	fs::path studentFilePath = fs::current_path() / "Students" / studentUsername / "PassedCourses.csv";

	std::ofstream passedCoursesFile(studentFilePath, std::ios::app);
	passedCoursesFile << '\n' << courseName << ',' << studentGrade.grade;
	passedCoursesFile.close();

}

void Course::addAttendingStudents(const std::string& studentUsername) noexcept
{
	attendingStudents.push_back(studentUsername);
	fs::path filePath = fs::current_path() / "Courses" / this->courseName/ "AttendingStudents.csv";

	std::ofstream attendingStudentsFile(filePath, std::ios::app);
	attendingStudentsFile << '\n' << studentUsername;
	attendingStudentsFile.close();

	fs::path studentFilePath = fs::current_path() / "Students" / studentUsername / "CoursesInProgress.csv";

	std::ofstream coursesInProgressFile(studentFilePath, std::ios::app);
	coursesInProgressFile << '\n' << this->courseName << ',' << professorUsername;
	coursesInProgressFile.close();
}

void Course::addStudentsWhoListened(const std::string& studentUsername) noexcept
{
	studentsWhoListened.push_back(studentUsername);
	fs::path filePath = fs::current_path() / "Courses" / this->courseName / "StudentsWhoListened.csv";

	std::ofstream studentsWhoListenedFile(filePath, std::ios::app);
	studentsWhoListenedFile << '\n' << studentUsername;
	studentsWhoListenedFile.close();

	fs::path studentFilePath = fs::current_path() / "Students" / studentUsername / "ListenedCourses.csv";

	std::ofstream listenedCoursesFile(studentFilePath, std::ios::app);
	listenedCoursesFile << '\n' << this->courseName;
	listenedCoursesFile.close();

	fs::path studentCoursesInProgressPath = fs::current_path() / "Students" / studentUsername / "CoursesInProgress.csv";
	fs::path tmpPath = fs::current_path() / "Students" / studentUsername / "tmpFile.csv";

	std::ifstream studentFile(studentCoursesInProgressPath, std::ios::in);
	std::ofstream tmpFile(tmpPath, std::ios::out);

	std::string dummyLine, courseName, professor;
	std::getline(studentFile, dummyLine, '\n');

	tmpFile << dummyLine;

	while (!studentFile.eof())
	{
		std::getline(studentFile, courseName, ',');
		std::getline(studentFile, professor, '\n');

		if ( this->courseName != courseName)
		{
			tmpFile << '\n' << courseName << ',' << professor;
		}
	}

	tmpFile.close();
	studentFile.close();

	fs::remove(studentCoursesInProgressPath);
	fs::rename(tmpPath, studentCoursesInProgressPath);



}

void Course::addPreconditions(fs::path courseFolder) noexcept
{
	int choice = 0;

	do {
		std::cout << "[1] Precondition Listening, [2] Precondition Pass, [3] Precondition Minimal Grade, [0] KRAJ!" << std::endl;
		std::cout << "Select option: ";
		std::cin >> choice;

		if (choice == 1)
		{
			PreconditionForListening preconditionForListening;
			std::cin >> preconditionForListening;
			preconditions.push_back(new PreconditionForListening(preconditionForListening));

			std::ofstream precondtionForListeningFile(courseFolder / "PreconditionForListening.csv", std::ios::out);

			precondtionForListeningFile << preconditionForListening;

			precondtionForListeningFile.close();
		}
		else if (choice == 2)
		{
			PreconditionPass preconditionPass;
			std::cin >> preconditionPass;
			preconditions.push_back(new PreconditionPass(preconditionPass));

			std::ofstream precondtionPassFile(courseFolder / "PreconditionPass.csv", std::ios::out);

			precondtionPassFile << preconditionPass;

			precondtionPassFile.close();
		}
		else if (choice == 3) 
		{
			PreconditionMinimalGrade preconditionMinimalGrade;
			std::cin >> preconditionMinimalGrade;
			preconditions.push_back(new PreconditionMinimalGrade(preconditionMinimalGrade));

			std::ofstream PreconditionMinimalGradeFile(courseFolder / "PreconditionMinimalGrade.csv", std::ios::out);

			PreconditionMinimalGradeFile << preconditionMinimalGrade;

			PreconditionMinimalGradeFile.close();
		}
		else if (choice != 0)
		{
			std::cout << "Invalid type\n";
		}

	} while (choice != 0);
}

void unionCoursesOfAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept
{
	Course course1, course2;
	
	fs::path courseFolder = fs::current_path() / "Courses";

	course1.readFromFolder(courseFolder, courseName1);
	course2.readFromFolder(courseFolder, courseName2);

	std::cout << "Union Courses " << courseName1 << " and " << courseName2 << ":";

	for (int i = 0; i < course1.attendingStudents.size(); i++)
	{
		std::cout << "\n" << course1.attendingStudents[i];
	}

	for (int i = 0; i < course2.attendingStudents.size(); ++i)
	{
		if (!course1.checkAttendingStudents(course2.attendingStudents[i]))
		{
			std::cout << "\n" << course2.attendingStudents[i];
		}
	}
}

void crossSectionOfAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept
{
	Course course1, course2;

	fs::path courseFolder = fs::current_path() / "Courses";

	course1.readFromFolder(courseFolder, courseName1);
	course2.readFromFolder(courseFolder, courseName2);

	std::cout << "Cross section of courses " << courseName1 << " and " << courseName2 << ":";

	for (int i = 0; i < course1.attendingStudents.size(); ++i)
	{
		if (course2.checkAttendingStudents(course1.attendingStudents[i]))
		{
			std::cout << "\n" << course1.attendingStudents[i];
		}
	}
}

void unionCoursesOfStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept
{
	Course course1, course2;

	fs::path courseFolder = fs::current_path() / "Courses";

	course1.readFromFolder(courseFolder, courseName1);
	course2.readFromFolder(courseFolder, courseName2);

	std::cout << "Union Courses " << courseName1 << " and " << courseName2 << ":";

	for (auto iterator : course1.studentsWhoPassed)
	{
		std::cout << "\n" << iterator.first;
	}

	for (auto iterator : course2.studentsWhoPassed)
	{
		if (!course1.checkAttendingStudents(iterator.first))
		{
			std::cout << "\n" << iterator.first;
		}
	}
}

void crossSectionOfStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept
{
	Course course1, course2;

	fs::path courseFolder = fs::current_path() / "Courses";

	course1.readFromFolder(courseFolder, courseName1);
	course2.readFromFolder(courseFolder, courseName2);

	std::cout << "Cross section of courses " << courseName1 << " and " << courseName2 << ":";

	for (auto iterator : course1.studentsWhoPassed)
	{
		if (course2.checkAttendingStudents(iterator.first))
		{
			std::cout << "\n" << iterator.first;
		}
	}
}

void differenceBetweenAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept
{

	Course course1, course2;

	fs::path courseFolder = fs::current_path() / "Courses";

	course1.readFromFolder(courseFolder, courseName1);
	course2.readFromFolder(courseFolder, courseName2);

	std::cout << "Difference between attending students of courses " << courseName1 << " and " << courseName2 << ":";

	for (int i = 0; i < course1.attendingStudents.size(); ++i)
	{
		if (!course2.checkAttendingStudents(course1.attendingStudents[i]))
		{
			std::cout << "\n" << course1.attendingStudents[i];
		}
	}
}

void differenceBetweenStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept
{
	Course course1, course2;

	fs::path courseFolder = fs::current_path() / "Courses";

	course1.readFromFolder(courseFolder, courseName1);
	course2.readFromFolder(courseFolder, courseName2);

	std::cout << "Difference between attending students of courses " << courseName1 << " and " << courseName2 << ":";

	for (auto iterator : course1.studentsWhoPassed)
	{
		if (!course2.checkAttendingStudents(iterator.first))
		{
			std::cout << "\n" << iterator.first;
		}
	}
}
