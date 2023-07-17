#pragma once
#include "User.h"
#include <vector>

class Professor : public User
{
private:
	std::vector<std::string> courses;
public:
	virtual void showMenu() noexcept override;
	void courseApplications() noexcept;

	void addStudentsWhoPassed()noexcept;
	void addStudentsWhoListened()noexcept;
	void sendMessage() noexcept;
	void readMessages() noexcept; 
	void filterMessages() noexcept;
private:
	void readCoursesFromFile() noexcept;
	void readCourseAplications(std::ifstream& ifs, std::vector<std::string>& studentsWhoApplied) noexcept;
	bool acceptOrRejectApplications(const std::string& studentUsername) noexcept;
	bool checkStudentsOfAllCourses(const std::string& studentUsername) noexcept;
	void automaticMessage(std::ofstream& inbox, const std::string message) noexcept;
};
