//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_USER_H
#define TEST_USER_H

#include <iostream>
#include "./../../System/System.h"

class User {
protected:
    std::string username;
    std::string password;

public:
    User(std::string username, std::string password);

    friend class System;

    friend class OutputStorageManager;

    friend class Validation;
};


#endif //TEST_USER_H
