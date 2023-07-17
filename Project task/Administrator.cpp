#include "Administrator.h"
#include "Student.h"
#include "Professor.h"
#include "Course.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <filesystem>

namespace fs = std::filesystem;

void Administrator::studentRegistration() noexcept
{
	Student student;
	
	std::cout << "Student registration;\n";
	
	std::cin >> student;


	system("cls");

	if (student.getUsername().empty() || student.getPassword().empty() || student.getFirstName().empty() || student.getLastName().empty())
	{
		std::cout << "Student registration failed.";
	}
	else
	{
		fs::path folderPath = fs::current_path() / "Students" / student.getUsername();

		fs::create_directory(folderPath);

		std::ofstream file(folderPath / (student.getUsername() + ".csv"), std::ios::out);
		
		file << student;

		file.close();

		std::ofstream passedCourseFile(folderPath / "PassedCourses.csv", std::ios::out);
		passedCourseFile << "COURSE NAME,GRADE";
		passedCourseFile.close();

		std::ofstream coursesInProgressFile(folderPath / "CoursesInProgress.csv", std::ios::out);
		coursesInProgressFile << "COURSE NAME, PROFESSOR USERNAME";
		coursesInProgressFile.close();

		std::ofstream listenedCoursesFile(folderPath / "ListenedCourses.csv", std::ios::out);
		listenedCoursesFile << "COURSE NAME";
		listenedCoursesFile.close();

		fs::create_directory(folderPath / "Inbox");

		std::ofstream friendRequestsFile(folderPath / "FriendRequests.csv", std::ios::out);
		friendRequestsFile << "USERNAME";
		friendRequestsFile.close();

		std::ofstream friendsFile(folderPath / "Friends.csv", std::ios::out);
		friendsFile << "USERNAME";
		friendsFile.close();


		std::cout << "Successful student registration.";
	}

	Sleep(1500);
	system("cls");
	
}

void Administrator::professorRegistration() noexcept
{
	Professor professor;

	std::cout << "Professor registration;\n";

	std::cin >> professor;

	system("cls");

	if (professor.getUsername().empty() || professor.getPassword().empty() || professor.getFirstName().empty() || professor.getLastName().empty())
	{
		std::cout << "Professor registration failed.";
	}
	else
	{
		fs::path folderPath = fs::current_path() / "Professors" / professor.getUsername();
		
		fs::create_directory(folderPath);

		std::ofstream file(folderPath / (professor.getUsername() + ".csv"), std::ios::out);

		file << professor;

		file.close();

		fs::create_directory(folderPath / "Inbox");

		std::ofstream courseFile(folderPath / "Courses.csv", std::ios::out);
		courseFile << "COURSE NAME";
		courseFile.close();

		std::cout << "Successful professor registration.";
	}

	Sleep(1500);
	system("cls");
}

void Administrator::removeStudent() noexcept
{
	std::string username;

	std::cout << "Enter username: ";
	std::cin >> username;
	
	system("cls");

	fs::path folderPath = fs::current_path() / "Students" / username;

	if (fs::remove_all(folderPath))
		std::cout << "Student deleted successfully!";
	else
		std::cout << "Error occurred, student has not been deleted!";

	Sleep(1500);
	system("cls");
}

void Administrator::removeProfessor() noexcept
{
	std::string username;

	std::cout << "Enter username: ";
	std::cin >> username;

	system("cls");

	fs::path folderPath = fs::current_path() / "Professors" / username;

	if (fs::remove_all(folderPath))
		std::cout << "Professor deleted successfully!";
	else
		std::cout << "Error occurred, professor has not been deleted!";

	Sleep(1500);
	system("cls");
}

void Administrator::createCourse() noexcept 
{
	Course course;
	std::cin >> course;

	system("cls");
}

void Administrator::removeCours() noexcept
{
	std::string courseName, dummyLine;
	Course course;

	std::cout << "Enter course name: ";
	std::cin >> courseName;

	system("cls");

	fs::path folderPath = fs::current_path() / "Courses" / courseName;

	if (fs::remove_all(folderPath))
	{
		std::ofstream tmpFile(fs::current_path() / "Courses" / "TmpFile.csv", std::ios::out);
		tmpFile << "ACRONYM,COURSE NAME,PROFESSOR USERNAME";
		
		std::ifstream availableCoursesFile(fs::current_path() / "Courses" / "AvailableCourses.csv", std::ios::in);
		std::getline(availableCoursesFile, dummyLine, '\n');

		while (!availableCoursesFile.eof())
		{
			availableCoursesFile >> course;

			if (course.getCourseName() != courseName)
			{
				tmpFile << course;
			}
		}

		tmpFile.close();
		availableCoursesFile.close();

		fs::remove(fs::current_path() / "Courses" / "AvailableCourses.csv");
		fs::rename(fs::current_path() / "Courses" / "TmpFile.csv", fs::current_path() / "Courses" / "AvailableCourses.csv");
		
		std::cout << "Course deleted successfully!";
	}
	else
		std::cout << "Error occurred, course has not been deleted!";

	Sleep(1500);
	system("cls");
}

void Administrator::shortestPathBetweenUsers() noexcept
{
	Graph<std::string> graph;

	fs::path folderPath = fs::current_path() / "Students";

	int iterator = 0;
	std::string dummyLine, username;

	for (auto& user : fs::directory_iterator(folderPath))
	{
		graph.add(iterator, user.path().stem().string());
		++iterator;
	}

	for (auto& user : fs::directory_iterator(folderPath))
	{
		std::ifstream friendsFile(user.path() / "Friends.csv", std::ios::in);
		std::getline(friendsFile, dummyLine, '\n');

		int id1 = graph.getId(user.path().stem().string());

		while (!friendsFile.eof())
		{
			std::getline(friendsFile, username, '\n');
			int id2 = graph.getId(username);

			graph.connect(id1, id2);		
		}

	}

	std::cout << "Graph nodes:\n";
	std::cout << graph;
	std::cout << std::endl;

	int id1, id2;
	std::cout << "Enter id1: "; std::cin >> id1;
	std::cout << "Enter id2: "; std::cin >> id2;
	

	graph.findShortestPath(id1, id2);

	Sleep(2000);
	system("cls");
}

void Administrator::compareCourses() noexcept
{
	int choice;
	do
	{
		std::cout << "[1] Union Courses Of Attending Students\n";
		std::cout << "[2] Cross Section Of Attending Students\n";
		std::cout << "[3] Union Courses Of Students Who Passed\n";
		std::cout << "[4] Cross Section Of Students Who Passed\n";
		std::cout << "[5] Difference Between Attending Students\n";
		std::cout << "[6] Difference Between Students Who Passed\n";
		std::cout << "[0] Return\n";
		std::cout << "Select option: ";
		std::cin >> choice;

		system("cls");
		std::getchar();

		if (choice == 1)
		{
			std::string course1, course2;
			std::cout << "Enter name course1: ";
			std::getline(std::cin, course1, '\n');

			std::cout << "Enter name course2: ";
			std::getline(std::cin, course2, '\n');

			unionCoursesOfAttendingStudents(course1, course2);
		}
		else if (choice == 2)
		{
			std::string course1, course2;
			std::cout << "Enter name course1: ";
			std::getline(std::cin, course1, '\n');

			std::cout << "Enter name course2: ";
			std::getline(std::cin, course2, '\n');

			crossSectionOfAttendingStudents(course1, course2);
		}
		else if (choice == 3)
		{
			std::string course1, course2;
			std::cout << "Enter name course1: ";
			std::getline(std::cin, course1, '\n');

			std::cout << "Enter name course2: ";
			std::getline(std::cin, course2, '\n');

			unionCoursesOfStudentsWhoPassed(course1, course2);
		}
		else if (choice == 4)
		{
			std::string course1, course2;
			std::cout << "Enter name course1: ";
			std::getline(std::cin, course1, '\n');

			std::cout << "Enter name course2: ";
			std::getline(std::cin, course2, '\n');

			crossSectionOfStudentsWhoPassed(course1, course2);
		}
		else if (choice == 5)
		{
			std::string course1, course2;
			std::cout << "Enter name course1: ";
			std::getline(std::cin, course1, '\n');

			std::cout << "Enter name course2: ";
			std::getline(std::cin, course2, '\n');

			differenceBetweenAttendingStudents(course1, course2);
		}
		else if (choice == 6)
		{
			std::string course1, course2;
			std::cout << "Enter name course1: ";
			std::getline(std::cin, course1, '\n');

			std::cout << "Enter name course2: ";
			std::getline(std::cin, course2, '\n');

			differenceBetweenStudentsWhoPassed(course1, course2);
		}
		else if (choice != 0)
		{
			std::cout << "Non-existent option!\n";
		}

		Sleep(1500);
		system("cls");

	} while (choice != 0);
}

void Administrator::modifyStudentAccount() noexcept
{
	Student student;
	std::cin >> student;

	std::ofstream studentFile(fs::current_path() / "Students" / student.getUsername() /(student.getUsername() + ".csv"), std::ios::out);
	studentFile << student;
	system("cls");
}

void Administrator::modifyProfessorAccount() noexcept
{
	Professor professor;
	std::cin >> professor;

	std::ofstream studentFile(fs::current_path() / "Professor" / professor.getUsername() / (professor.getUsername() + ".csv"), std::ios::out);
	studentFile << professor;
	system("cls");
}

void Administrator::showMenu() noexcept
{
	int choice;

	do
	{
		std::cout << "[1] Student Registration\n";
		std::cout << "[2] Professor Registration\n";
		std::cout << "[3] Remove Student\n";
		std::cout << "[4] Remove Professor\n";
		std::cout << "[5] Create Course\n";
		std::cout << "[6] Remove Course\n";
		std::cout << "[7] Find Shortest Path Between Friends\n";
		std::cout << "[8] Compare courses\n";
		std::cout << "[9] Modify Student Account\n";
		std::cout << "[10] Modify Professor Account\n";
		std::cout << "[0] Log Out\n";
		std::cout << "Select option: ";
		std::cin >> choice;

		system("cls");
		std::getchar();

		if (choice == 1)
		{
			this->studentRegistration();
		}
		else if (choice == 2)
		{
			this->professorRegistration();
		}
		else if (choice == 3)
		{
			this->removeStudent();
		}
		else if (choice == 4)
		{
			this->removeProfessor();
		}
		else if (choice == 5)
		{
			this->createCourse();
		}
		else if (choice == 6)
		{
			this->removeCours();
		}
		else if (choice == 7)
		{
			this->shortestPathBetweenUsers();
		}
		else if (choice == 8)
		{
			this->compareCourses();
		}
		else if (choice == 9)
		{
			this->modifyStudentAccount();
		}
		else if (choice == 10)
		{
			this->modifyProfessorAccount();
		}
		else if (choice != 0)
		{
			std::cout << "Non-existent option!\n";
		}

	} while (choice != 0);
}
