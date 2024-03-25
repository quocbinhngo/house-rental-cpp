
//
// Created by ADMIN on 5/7/2022.
//

#ifndef GROUP_PROJECT_ONE_FILE_HELPER_H
#define GROUP_PROJECT_ONE_FILE_HELPER_H

#include <iostream>
#include "../DataType/Date/Date.h"

int convertStringToInt(std::string &str);
double convertStringToDouble(std::string &str);
Date* convertStringToDate(std::string &str);
bool isNumber(std::string &str);
Date* getCurrentDate();
bool isDouble(std::string &str);
#endif //GROUP_PROJECT_ONE_FILE_HELPER_H
