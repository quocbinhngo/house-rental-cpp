//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_DATE_H
#define TEST_DATE_H

#include <iostream>

class Date {
    int day;
    int month;
    int year;

    // Find the number of leap year from 0/0/0
    int getCountLeapYear();

    // Find the number of date from 0/0/0
    int getCountDate();

public:
    Date();
    Date(int day, int month, int year);

    // Operator for compare two dates
    bool operator<(Date &otherDate);

    // Operator for finding the difference between two dates
    int operator-(Date &otherDate);

    // Function for printing the date
    std::string toStr();

    friend class System;

    friend class OutputStorageManager;

    friend class Validation;
};


#endif //TEST_DATE_H
