#include "./Validation.h"
#include "./../Helper/Helper.h"
#include "./../DataType/Date/Date.h"
#include "./../Object/House/House.h"
#include "./../Object/Member/Member.h"
#include "./../Object/HouseOccupy/HouseOccupy.h"
#include "./../Object/HouseOccupy/HouseOccupy.h"
#include "./../Object/MemberOccupy/MemberOccupy.h"
#include "./../Object/Request/Request.h"
#include "./../Object/Review/Review.h"
#include "./../System/System.h"
#include "./../Constant.h"
#include <ctype.h>
#include <sstream>


bool Validation::password(std::string password) {
    if (password.length() < 6) {
        std::cout << "Password is too short. Please enter a longer password.\n";
        return false;
    }
    for (int i = 0; i < password.size(); i++){
        if (password[i] == ' '){
            std::cout << "Password cannot contain space. Try again.\n";
            return false;
        }
    }
    return true;
}

bool Validation::username(std::string username, System *sys) {
    if (username.length() < 6) {
        std::cout << "Username is must be more than 6 characters. Try another.\n";
        return false;
    }
    for (int i = 0; i < username.size(); i++){
        if (username[i] == ' '){
            std::cout << "Username cannot contain a space\n";
            return false;
        }
    }
    for (Member *mem: sys->systemMemberList) {
        if (mem->username == username) {
            std::cout << "Username is taken. Try another.\n";
            return false;
        }
    }
    return true;
}


bool Validation::phoneNumber(std::string phoneNum, System *sys) {
    if (!isNumber(phoneNum)) {
        std::cout << "Please enter a valid phone number. Try again.\n";
        return false;
    } else if (phoneNum.length() != 10) {
        std::cout << "Please enter a valid phone number. Try again.\n";
        return false;
    }

    for (Member *mem: sys->systemMemberList) {
        if (mem->phoneNumber == phoneNum) {
            std::cout << "Phone number is taken. Please try another\n";
            return false;
        }
    }
    return true;
}

//Date Validation::getCurrentDate(){
//    time_t t = std::time(nullptr);
//    tm tm = *std::localtime(&t);
//
//    std::stringstream ss;
//    ss << std::put_time(&tm, "%d/%m/%Y");
//    std::string str = ss.str();
//    return convertStringToDate(str);
//}

bool Validation::date(std::string date) {
    if (date.length() != 10) {
        std::cout << "Please enter a valid date. Try again.\n";
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (date[i] != '/') {
                std::cout << "Please enter a valid date. Try again.\n";
                return false;
            }
        } else if (!std::isdigit(date[i])) {
            std::cout << "Please enter a valid date. Try again.\n";
            return false;
        }
    }
    Date *temp = convertStringToDate(date);

    if (temp->day > DAY_PER_MONTH[temp->month - 1]){
        std::cout << "Please enter a valid date. Try again\n";
        return false;
    }

    if (*temp < *getCurrentDate()){
        std::cout << "Please enter a date in the future. Try again.\n";
        return false;
    }


    return true;
}

bool Validation::listDate(std::string startDate, std::string endDate) {
    Date *sD = convertStringToDate(startDate), *eD = convertStringToDate(endDate);
    if (*eD < *sD){
        std::cout << "End date must be after start date. Try again\n";
        return false;
    }
    return true;
}

bool Validation::name(std::string name){
    for (int i = 0; i < name.length(); i++){
        if (!std::isalpha(name[i]) && name[i] != ' '){
            return false;
        }
    }
    return true;
}



