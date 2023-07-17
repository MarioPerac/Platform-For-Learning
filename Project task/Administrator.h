#pragma once
#include "User.h"

class Administrator : public User 
{
public:
	void studentRegistration() noexcept;
	void professorRegistration() noexcept;
	void removeStudent() noexcept;
	void removeProfessor() noexcept;
	void createCourse() noexcept;
	void removeCours() noexcept;
	void shortestPathBetweenUsers() noexcept;
	void compareCourses() noexcept;
	void modifyStudentAccount() noexcept;
	void modifyProfessorAccount() noexcept;

	virtual void showMenu() noexcept override; 
};
