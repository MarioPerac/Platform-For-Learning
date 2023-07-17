#include "Professor.h"
#include "Course.h"
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <algorithm>

namespace fs = std::filesystem;

void Professor::showMenu() noexcept
{
	int choice;

	do
	{
		std::cout << "[1] Student applications, on Your courses\n";
		std::cout << "[2] Record the student who passed\n";
		std::cout << "[3] Record the student who listened\n";
		std::cout << "[4] Send Message\n";
		std::cout << "[5] Read Messages\n";
		std::cout << "[6] Filter Messages\n";
		std::cout << "[0] Log Out\n";
		std::cout << "Select option: ";
		std::cin >> choice;

		system("cls");
		std::getchar();

		if (choice == 1)
		{
			this->readCoursesFromFile(); 
			this->courseApplications();
		}
		else if (choice == 2)
		{
			this->readCoursesFromFile(); 
			this->addStudentsWhoPassed();
			
		}
		else if (choice == 3)
		{
			this->readCoursesFromFile(); 
			this->addStudentsWhoListened();
		}
		else if (choice == 4)
		{
			this->readCoursesFromFile(); 
			this->sendMessage();
		}
		else if (choice == 5)
		{
			this->readMessages();
		}
		else if (choice == 6)
		{
			this->filterMessages();
		}
		else if (choice != 0)
		{
			std::cout << "Non-existent option!\n";
		}

	} while (choice != 0);
}

void Professor::courseApplications() noexcept
{
	std::vector<std::string> studentsWhoApplied;

	for (int i = 0; i < courses.size(); ++i)
	{
		std::cout << "Course" << " " << courses[i] << ";\n";
		Course course;
		
		fs::path coursesFolder = fs::current_path() / "Courses";

		course.readFromFolder(coursesFolder, courses[i]);

		std::ifstream courseFile(coursesFolder / courses[i] /"CourseApplications.csv", std::ios::in);

		this->readCourseAplications(courseFile, studentsWhoApplied);

		if (studentsWhoApplied.size() == 0)
		{
			std::cout << "No applications currently.";
			Sleep(1500);
			system("cls");
			continue;
		}

		for (int j = 0; j < studentsWhoApplied.size(); j++)
		{
			std::ofstream studentInbox(fs::current_path() / "Students" / studentsWhoApplied[j] / "Inbox" / (this->username + ".txt"), std::ios::app);

			if (this->acceptOrRejectApplications(studentsWhoApplied[j]))
			{
				course.addAttendingStudents(studentsWhoApplied[j]); 
				std::string message = "Welcome to the course " + courses[i] + "!";
				this->automaticMessage(studentInbox,message);
			}
			else
			{
				std::string message = "Your application for the course " + courses[i] + " has not been accepted!";
				this->automaticMessage(studentInbox, message);
			}
			studentInbox.close();
		}
		courseFile.close();

		fs::remove(coursesFolder / courses[i] / "CourseApplications.csv");

		std::ofstream courseApplicationsFile(coursesFolder / courses[i] / "CourseApplications.csv");
		courseApplicationsFile << "STUDENT USERNAME";
		courseApplicationsFile.close();

		Sleep(1500);
		system("cls");
	}
}

void Professor::addStudentsWhoPassed() noexcept
{
	std::string courseName, studentUsername;
	Grade studentGrade;
	std::cout << "Enter course name: ";
	std::getline(std::cin, courseName);

	
	if (std::find(courses.begin(), courses.end(), courseName) == courses.end())
	{
		std::cout << "You are not professor of the course " << courseName << "!";
		Sleep(2500);
		system("cls");
		return;
	}

	Course course;
	fs::path coursesFolder = fs::current_path() / "Courses";
	course.readFromFolder(coursesFolder, courseName);

	std::cout << "Enter student username: ";
	std::cin >> studentUsername;
	std::cout << "Enter student grade: ";
	std::cin >> studentGrade.grade;

	course.addStudentsWhoPassed(studentUsername, studentGrade);
	
	system("cls");

}

void Professor::addStudentsWhoListened() noexcept
{
	std::string courseName, studentUsername;
	Grade studentGrade;
	std::cout << "Enter course name: ";
	std::getline(std::cin, courseName);


	if (std::find(courses.begin(), courses.end(), courseName) == courses.end())
	{
		std::cout << "You are not professor of the course " << courseName << "!";
		Sleep(2500);
		system("cls");
		return;
	}

	Course course;
	fs::path coursesFolder = fs::current_path() / "Courses";
	course.readFromFolder(coursesFolder, courseName);

	std::cout << "Enter student username: ";
	std::cin >> studentUsername;

	course.addStudentsWhoListened(studentUsername);

	system("cls");
}

void Professor::sendMessage() noexcept
{
	std::string username;
	std::cout << "Enter username: ";
	std::cin >> username;

	std::getchar();

	if (username == this->username)
	{
		std::cout << "Doesn't make sense.";

		Sleep(2000);
		system("cls");
		return;
	}

	if (this->checkStudentsOfAllCourses(username))
	{
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);

		std::ofstream studentFile(fs::current_path() / "Students" / username / "Inbox" / (this->username + ".txt"), std::ios::out | std::ios::app);
		studentFile << "MESSAGE:" << '\n' << message << '\n';

		studentFile.close();

		std::cout << "Message sent.";
	}
	else
	{
		std::cout << "This person is not your student.";
	}

	Sleep(2000);
	system("cls");
	return; 

}

void Professor::readMessages() noexcept
{
	fs::path inboxFolder = fs::current_path() / "Professors" / this->username / "Inbox";
	std::string dummyLine, message;

	for (auto& messagePath : fs::directory_iterator(inboxFolder))
	{
		std::ifstream messageFile(messagePath.path(), std::ios::in);

		while (std::getline(messageFile, dummyLine, '\n') && std::getline(messageFile, message, '\n'))
		{
			std::cout << "User " << messagePath.path().stem().string() << " sent you: ";
			std::cout << message << '\n';

			char next;
			std::cout << "Next messages[1]: ";
			std::cin >> next;
			system("cls");
		}
	}
}

void Professor::filterMessages() noexcept
{
	std::string searchString;
	std::cout << "Enter search string: ";
	std::cin >> searchString;

	std::getchar();
	system("cls");

	fs::path inboxFolder = fs::current_path() / "Professors" / this->username / "Inbox";
	std::string dummyLine, message;

	for (auto& messagePath : fs::directory_iterator(inboxFolder))
	{

		if (messagePath.path().stem().string() == searchString)
		{
			std::ifstream messageFile(messagePath.path(), std::ios::in);

			while (std::getline(messageFile, dummyLine, '\n') && std::getline(messageFile, message, '\n'))
			{
				std::cout << "User " << messagePath.path().stem().string() << " sent you: ";
				std::cout << message << '\n';

				char next;
				std::cout << "Next messages[1]: ";
				std::cin >> next;
				system("cls");
			}
		}
		
	}
}

void Professor::readCoursesFromFile() noexcept
{
	std::ifstream coursesFile(fs::current_path() / "Professors" / username / "Courses.csv", std::ios::in);

	std::string dummyLine, courseName;
	std::getline(coursesFile, dummyLine);

	while (!coursesFile.eof())
	{
		std::getline(coursesFile, courseName, '\n');
		courses.push_back(courseName);
	}
	coursesFile.close();
}

void Professor::readCourseAplications(std::ifstream& ifs, std::vector<std::string>& studentsWhoApplied) noexcept
{
	std::string dummyLine, studentUsername;
	std::getline(ifs, dummyLine);

	while (!ifs.eof())
	{
		std::getline(ifs, studentUsername, '\n');
		studentsWhoApplied.push_back(studentUsername);
	}
}

bool Professor::acceptOrRejectApplications(const std::string& studentUsername) noexcept
{
	int choice;
	std::cout << "Student usernme: " << studentUsername << '\n';
	std::cout << "[1] Accept or [0] Reject application: ";
	std::cin >> choice;

	if (choice == 1)
	{
		return true;
	}
	else if (choice == 0)
	{
		return false;
	}
	else
	{
		return false;
	}

}

bool Professor::checkStudentsOfAllCourses(const std::string& studentUsername) noexcept 
{

	fs::path coursesFolder = fs::current_path() / "Courses";
	Course course;

	for (int i = 0; i < courses.size(); ++i)
	{
		course.readFromFolder(coursesFolder, courses[i]);

		if (course.checkAttendingStudents(studentUsername))
		{
			return true;
		}
	}
	
	return false;
}

void Professor::automaticMessage(std::ofstream& inbox, const std::string message) noexcept
{
	inbox << "MESSAGE:" << '\n' << message << '\n';
}
