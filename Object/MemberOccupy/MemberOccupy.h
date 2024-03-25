//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_MEMBEROCCUPY_H
#define TEST_MEMBEROCCUPY_H

class Date;
class House;

class MemberOccupy {
    Date *occupyFromDate;
    Date *occupyToDate;
    House *occupyHouse;

public:
    MemberOccupy(Date *occupyFromDate, Date *occupyToDate, House *occupyHouse);

    friend class Member;

    friend class System;

    friend class OutputStorageManager;
};


#endif //TEST_MEMBEROCCUPY_H
