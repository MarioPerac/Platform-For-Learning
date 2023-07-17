#pragma once
#include "IReadable.h"
#include "IPrintable.h"
#include "Precondition.h"
#include "Grade.h"
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

class Course : virtual public IReadable, virtual public IPrintable
{
protected:
	std::string courseName;
	std::string acronym; 
	std::string professorUsername;
	std::map<std::string, Grade>  studentsWhoPassed; 
	std::vector<std::string> attendingStudents;
	std::vector<std::string> studentsWhoListened;
	std::vector<Precondition *> preconditions;

	virtual void readFromConsole(std::istream& is) noexcept override;
	virtual void readFromFile(std::ifstream& ifs) noexcept override;

	virtual void printToConsole(std::ostream& os) noexcept override;
	virtual void printToFile(std::ofstream& ofs) noexcept override;
public:
	~Course() noexcept;

	std::string getCourseName() noexcept; 
	std::string getAcronym() noexcept;

	bool checkCourseInFile(std::ifstream& ifs, std::string courseNameForCheck) noexcept;

	bool readFromFolder(fs::path folderPath, std::string courseName) noexcept;

	bool checkStudentsWhoPassed(const std::string& studentUsername) noexcept;
	bool checkStudentsWhoPassedUsingGrade(const std::string& studentUsername, int minimalGrade) noexcept;
	bool checkAttendingStudents(const std::string& studentUsername) noexcept;
	bool checkStudentsWhoListened(const std::string& studentUsername) noexcept;

	bool checkAllPreconditions(const std::string& studentUsername) noexcept;

	void addStudentsWhoPassed(const std::string& studentUsername,const Grade& grade) noexcept;
	void addAttendingStudents(const std::string& studentUsername)noexcept;
	void addStudentsWhoListened(const std::string& studentUsername)noexcept;

	friend void unionCoursesOfAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept;
	friend void crossSectionOfAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept;

	friend void unionCoursesOfStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept;
	friend void crossSectionOfStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept;

	friend void differenceBetweenAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept;
	friend void differenceBetweenStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept;

private:
	void addPreconditions(fs::path courseFolder) noexcept;
};

void unionCoursesOfAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept;
void crossSectionOfAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept;

void unionCoursesOfStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept;
void crossSectionOfStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept;

void differenceBetweenAttendingStudents(const std::string& courseName1, const std::string& courseName2) noexcept;
void differenceBetweenStudentsWhoPassed(const std::string& courseName1, const std::string& courseName2) noexcept;