#include "Student.h"
#include "Course.h"
#include <iostream>
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

void Student::showMenu() noexcept
{
	int choice;
	char buffer;

	do
	{
		std::cout << "[1] Review Available Courses\n";
		std::cout << "[2] Course Application\n";
		std::cout << "[3] Show Courses In Progress\n";
		std::cout << "[4] Show Listened Courses\n";
		std::cout << "[5] Show Passed Courses\n";
		std::cout << "[6] Send Friend Request\n";
		std::cout << "[7] Received Friend Requests\n";
		std::cout << "[8] Send Message\n";
		std::cout << "[9] Read Messages\n";
		std::cout << "[10] Filter Messages\n";
		std::cout << "[0] Log Out\n";
		std::cout << "Select option: ";
		std::cin >> choice;

		system("cls");
		std::cin.get(buffer);

		if (choice == 1)
		{
			this->showAvailableCourses();
		}
		else if (choice == 2)
		{
			this->courseApplication();
		}
		else if (choice == 3)
		{
			this->showCoursesInProgress();
		}
		else if (choice == 4)
		{
			this->showListenedCourses();
		}
		else if (choice == 5)
		{
			this->showPassedCourses();
		}
		else if (choice == 6)
		{
			this->sendFriendRequest();
		}
		else if (choice == 7)
		{
			this->acceptOrRejectFriendRequests();
		}
		else if (choice == 8)
		{
			this->sendMessage();
		}
		else if (choice == 9)
		{
			this->readMessages();
		}
		else if (choice == 10)
		{
			this->filterMessages();
		}
		else if (choice != 0)
		{
			std::cout << "Non-existent option!\n";
		}

	} while (choice != 0);
	
}

void Student::courseApplication() noexcept
{
	std::string courseName;
	std::cout << "Enter course name: ";
	std::getline(std::cin, courseName);

	fs::path coursesFolder = fs::current_path() / "Courses";
	Course course;

	course.readFromFolder(coursesFolder, courseName);

	if (!course.checkAllPreconditions(this->username))
	{
		std::cout << "The application cannot be submitted, because You have not fulfilled preconditions.";
		Sleep(2500);
		system("cls");

		return;
	}

	fs::path applicationPath = coursesFolder / courseName / "CourseApplications.csv";

	std::ofstream applicationFile(applicationPath, std::ios::app);

	if (applicationFile.is_open())
	{
		applicationFile << '\n' << username;
		applicationFile.close();

		std::cout << "The application has been sent.";
	}
	else
	{
		std::cout << "Unable to submit application.";
	}

	Sleep(1500);
	system("cls");

}

void Student::showAvailableCourses() noexcept
{
	Course availableCourse;

	std::ifstream coursesFile(fs::current_path() / "Courses" / "AvailableCourses.csv", std::ios::in);

	std::string dummyLine;
	std::getline(coursesFile, dummyLine, '\n');

	std::cout << "Available Courses:\n";
	int iterator = 1;

	while (!coursesFile.eof())
	{
		coursesFile >> availableCourse;
		std::cout << iterator << "." << " " << availableCourse.getCourseName() << '\n';
		++iterator;
	}

	Sleep(2500);
	system("cls");

}

void Student::showCoursesInProgress() noexcept
{
	std::ifstream coursesFile(fs::current_path() / "Students" / username / ("CoursesInProgress.csv"), std::ios::in);

	int iterator = 1;
	std::string dummyLine, courseName;
	std::getline(coursesFile, dummyLine);

	while (!coursesFile.eof())
	{
		std::getline(coursesFile, courseName, '\n');
		std::cout << iterator << "." << " " << courseName << '\n';
		++iterator;
	}

	coursesFile.close();

	Sleep(2500);
	system("cls");

}

void Student::showListenedCourses() noexcept
{
	std::ifstream coursesFile(fs::current_path() / "Students" / username / ("ListenedCourses.csv"), std::ios::in);

	int iterator = 1;
	std::string dummyLine, courseName;
	std::getline(coursesFile, dummyLine);

	while (!coursesFile.eof())
	{
		std::getline(coursesFile, courseName, '\n');
		std::cout << iterator << "." << " " << courseName << '\n';
		++iterator;
	}

	coursesFile.close();

	Sleep(2500);
	system("cls");

}

void Student::showPassedCourses() noexcept
{
	std::ifstream coursesFile(fs::current_path() / "Students" / username / ("PassedCourses.csv"), std::ios::in);

	int iterator = 1;
	
	std::string dummyLine, courseName, grade;
	std::getline(coursesFile, dummyLine);

	while (!coursesFile.eof())
	{
		std::getline(coursesFile, courseName, ',');
		std::getline(coursesFile, grade, '\n');
		std::cout << iterator << "." << " " << courseName << " " << "Grade(" << stoi(grade) << ")\n";
		++iterator;
	}

	coursesFile.close();

	Sleep(2500);
	system("cls");

}

void Student::sendFriendRequest() noexcept
{
	std::string friendUsername;
	std::cout << "Enter username: ";
	std::cin >> friendUsername;

	if (friendUsername == this->username)
	{
		std::cout << "Doesn't make sense.";

		Sleep(2000);
		system("cls");
		return;
	}

	fs::path friendRequestsFile(fs::current_path() / "Students" / friendUsername / "FriendRequests.csv");

	std::ofstream friendFile(friendRequestsFile, std::ios::app);

	if (friendFile.is_open())
	{
		friendFile << '\n' << this->username;
		std::cout << "Request sent.";

		friendFile.close();
	}
	else
	{
		std::cout << "There are no students with this username.";
	}

	Sleep(2000);
	system("cls");
}

void Student::acceptOrRejectFriendRequests() noexcept
{
	fs::path friendReqPath = fs::current_path() / "Students" / this->username / "FriendRequests.csv";

	std::ifstream friendReqFile(friendReqPath, std::ios::in);
	
	std::string dummyLine, friendUsername;
	std::getline(friendReqFile, dummyLine);

	std::cout << "FriendRequests;\n";

	while (!friendReqFile.eof())
	{
		std::getline(friendReqFile, friendUsername, '\n');

		int choice;
		std::cout << "Username student: " << friendUsername << '\n';
		std::cout << "[1] Accept or [0] Reject request: ";
		std::cin >> choice;

		if (choice == 1)
		{
			std::ofstream myFriendsFile(fs::current_path() / "Students" / this->username / "Friends.csv", std::ios::app);
			myFriendsFile << '\n' << friendUsername;
			myFriendsFile.close();

			std::ofstream friendFile(fs::current_path() / "Students" / friendUsername / "Friends.csv", std::ios::app);
			friendFile << '\n' << this->username;
			friendFile.close();
		}
	}

	friendReqFile.close();

	fs::remove(friendReqPath);
	std::ofstream newFriendReqFile(friendReqPath, std::ios::out);
	newFriendReqFile << "USERNAME";
	newFriendReqFile.close();

	system("cls");
}

void Student::sendMessage() noexcept
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

	if (this->checkFriendship(username))
	{
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);

		std::ofstream friendFile(fs::current_path() / "Students" / username / "Inbox" / (this->username + ".txt"), std::ios::out | std::ios::app);
		friendFile << "MESSAGE:" << '\n' << message << '\n';
		
		friendFile.close();

		std::cout << "Message sent.";
	}
	else if (this->checkProfessor(username))
	{
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);

		std::ofstream friendFile(fs::current_path() / "Professors" / username / "Inbox" / (this->username + ".txt"), std::ios::out | std::ios::app);
		friendFile << "MESSAGE:" << '\n' << message << '\n';

		friendFile.close();

		std::cout << "Message sent.";
	}
	else 
	{
		std::cout << "This person is not Your friend or professor.";
	}
	
	Sleep(2000);
	system("cls");
	return;
}

void Student::readMessages() noexcept
{
	fs::path inboxFolder = fs::current_path() / "Students" / this->username / "Inbox";
	std::string dummyLine, message;

	for (auto& messagePath : fs::directory_iterator(inboxFolder))
	{
		std::ifstream messageFile(messagePath.path(), std::ios::in);

		while (std::getline(messageFile, dummyLine, '\n') && std::getline(messageFile, message, '\n'))
		{
			std::cout << "User " << messagePath.path().stem().string() << " sent you: ";
			std::cout << message <<'\n';

			char next;
			std::cout << "Next messages[1]: ";
			std::cin >> next;
			system("cls");
		}
	}
}

void Student::filterMessages() noexcept
{
	std::string searchString;
	std::cout << "Enter search string: ";
	std::cin >> searchString;

	std::getchar();
	system("cls");

	fs::path inboxFolder = fs::current_path() / "Students" / this->username / "Inbox";
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

bool Student::checkFriendship(const std::string& username)
{
	std::ifstream friendsFile(fs::current_path() / "Students" / this->username / "Friends.csv");

	std::string dummyLine, friendUsername;
	std::getline(friendsFile, dummyLine);

	while (!friendsFile.eof())
	{
		std::getline(friendsFile, friendUsername);

		if (username == friendUsername)
		{
			friendsFile.close();
			return true;
		}
	}

	friendsFile.close();
	return false;
}

bool Student::checkProfessor(const std::string& professorUsername)
{
	std::string dummyLine, myProfessorUsername;

	std::ifstream studentCoursesFile(fs::current_path() / "Students" / this->username / ("CoursesInProgress.csv"), std::ios::in);
	std::getline(studentCoursesFile, dummyLine);


	while (!studentCoursesFile.eof())
	{
		std::getline(studentCoursesFile, dummyLine, ',');
		std::getline(studentCoursesFile, myProfessorUsername, '\n');

		if (myProfessorUsername == professorUsername)
		{
			return true;
		}
		
	}

	studentCoursesFile.close();
	return false;
}
