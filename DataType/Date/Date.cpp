//
// Created by ADMIN on 5/3/2022.
//

#include "../../Constant.h"
#include "Date.h"
#include <sstream>


Date::Date(int day, int month, int year) {
    this->day = day;
    this->month = month;
    this->year = year;
}

int Date::getCountLeapYear() {
    int curYear = year;

    // If the current year needs to be considered for the count of leap year
    if (month < 2) {
        curYear--;
    }

    // Leap year is multiple of 4, multiple of 400 but not multiple of 100 => inclusive and exclusive
    return curYear / 4 - curYear / 100 + curYear / 400;
}

int Date::getCountDate() {

    // Init the result by calculate year and day
    int result = year * 365 + day;

    // Add months in given day
    for (int i = 0; i < month - 1; i++) {
        result += DAY_PER_MONTH[i];
    }

    // Add one day per leap year
    result += getCountLeapYear();

//    std::cout << result << "\n";
    return result;
}

bool Date::operator<(Date &otherDate) {
    return getCountDate() - otherDate.getCountDate() <= 0;
}

int Date::operator-(Date &otherDate) {
    // Get the difference between two dates
    return getCountDate() - otherDate.getCountDate();
}

std::string Date::toStr() {
    std::stringstream ss;
    ss << day << "/" << month << "/" << year;
    return ss.str();
}