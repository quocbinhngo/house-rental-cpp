//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_REVIEW_H
#define TEST_REVIEW_H

#include <iostream>

class Member;

class Review {
private:
    int ratingScore;
    std::string comment;
    Member* reviewedByMember;

public:
    Review(int ratingScore, std::string comment, Member* reviewedByMember);

    friend class System;

    friend class Member;

    friend class House;

    friend class OutputStorageManager;
};


#endif //TEST_REVIEW_H
