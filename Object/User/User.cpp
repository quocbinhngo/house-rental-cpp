//
// Created by ADMIN on 5/3/2022.
//

#include "User.h"

User::User(std::string username, std::string password) {
    this->username = std::move(username);
    this->password = std::move(password);
}