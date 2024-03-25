//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_HOUSEOCCUPY_H
#define TEST_HOUSEOCCUPY_H

class Date;
class Member;

class HouseOccupy {
    Date *occupyFromDate;
    Date *occupyToDate;
    Member *occupiedByMember;

public:
    HouseOccupy(Date *occupyFromDate, Date *occupyToDate, Member *occupiedByMember);

    friend class System;

    friend class Member;

    friend class House;

    friend class OutputStorageManager;
};


#endif //TEST_HOUSEOCCUPY_H
