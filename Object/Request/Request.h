//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_REQUEST_H
#define TEST_REQUEST_H

#include "../../DataType/Date/Date.h"

class Member;
class Date;

class Request {
    Date* requestFromDate;
    Date* requestToDate;
    Member* requestedByMember;

public:
    Request(Date *requestFromDate, Date *requestToDate, Member *requestedByMember);

    friend class System;

    friend class Member;

    friend class OutputStorageManager;
};


#endif //TEST_REQUEST_H
