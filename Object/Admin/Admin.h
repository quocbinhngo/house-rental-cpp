//
// Created by ADMIN on 5/9/2022.
//

#ifndef GROUP_PROJECT_ONE_FILE_ADMIN_H
#define GROUP_PROJECT_ONE_FILE_ADMIN_H

#include "../User/User.h"

#include <iostream>

class User;

class Admin: public User {
public:
    Admin(std::string username, std::string password);

    friend class System;

    friend class InputStorageManager;

    friend class OutputStorageManager;
};


#endif //GROUP_PROJECT_ONE_FILE_ADMIN_H
