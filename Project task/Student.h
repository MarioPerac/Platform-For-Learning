#pragma once
#include "User.h"
#include <fstream>
#include <vector>

class Student : public User
{
public:
	virtual void showMenu() noexcept override;
	void courseApplication() noexcept;
	void showAvailableCourses() noexcept;
	void showCoursesInProgress() noexcept;
	void showListenedCourses() noexcept;
	void showPassedCourses() noexcept;
	void sendFriendRequest() noexcept;
	void acceptOrRejectFriendRequests() noexcept;
	void sendMessage() noexcept;
	void readMessages() noexcept;
	void filterMessages() noexcept;
private:
	bool checkFriendship(const std::string& username);
	bool checkProfessor(const std::string& professorUsername);

};