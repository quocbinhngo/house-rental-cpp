//
// Created by ADMIN on 5/9/2022.
//

#include "Admin.h"

#include <utility>

Admin::Admin(std::string username, std::string password)
    : User(std::move(username), std::move(password)) {
}
