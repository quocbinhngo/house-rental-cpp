
#ifndef GROUP_PROJECT_ONE_FILE_VALIDATION_H
#define GROUP_PROJECT_ONE_FILE_VALIDATION_H

#include <iostream>
#include <ctype.h>

class House;
class Review;
class Request;
class MemberOccupy;
class Date;
class System;

class Validation {
public:
    bool password(std::string password);

    bool username(std::string username, System *sys);

    bool phoneNumber(std::string phoneNumber, System *sys);

    bool date(std::string date);

    bool listDate (std::string startDate, std::string endDate);

    bool name(std::string name);

    friend class System;

//    Date getCurrentDate();
};
#endif //GROUP_PROJECT_ONE_FILE_VALIDATION_H