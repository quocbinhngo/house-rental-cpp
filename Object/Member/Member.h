//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_MEMBER_H
#define TEST_MEMBER_H

#include "../User/User.h"
#include "../../Constant.h"
#include "../../Validation/Validation.h"
#include <iostream>
#include <vector>

class House;
class Review;
class Request;
class MemberOccupy;
class Date;

class Member: public User {
    int memberID;

    std::string firstName;
    std::string lastName;
    std::string phoneNumber;

    int creditPoints;

    House *ownedHouse;

    std::vector<Review*> memberReviewList;

    std::vector<Request*> memberRequestList;

    std::vector<MemberOccupy*> memberOccupyList;

public:
    void test();
    Member(int memberID, std::string username, std::string password, std::string firstName, std::string lastName,
           std::string phoneNumber, int creditPoints = INITIAL_CREDIT_POINTS);


    double getRatingScore();

    bool viewMemberInfo();

    bool listHouse(Date *startDate, Date *endDate, int consumingPointsPerDay, double minimumOccupierRating);

    bool unListHouse();

    bool viewRequestList();

    bool rejectTheRequest(int acceptedRequestID);

    bool acceptTheRequest(int acceptedRequestID);

    bool leaveHouse(int leaveOccupyHouseID);

    bool reviewHouse (House *occupyHouse, int scoreRating, std::string comment);

    bool viewUnratedOccupierList();

    bool reviewOccupier(int reviewedOccupierID, int score, std::string comment);

//    std::string toStr();

    bool createOwnedHouse(House *house);

    std::string getFullName();

    bool addRequestToMemberRequestList(Request *addedRequest);

    bool removeRequestFromMemberRequestList(Request *removedRequest);

    bool addOccupyToMemberOccupyList(MemberOccupy *addedOccupy);

    bool removeOccupyFromMemberOccupyList(MemberOccupy *leaveOccupy);

    bool viewMemberOccupyList();

    bool addReviewToMemberReviewList(Review *memberReview);

    bool minusCreditPoints(int points);

    bool addCreditPoints(int points);

    /*
     * View Member Review by admin
     */

    std::string viewMemberReview();

    /*
    Get current occupied house
    */

    ~Member();

    friend class System;

    friend class OutputStorageManager;
    friend class InputStorageManager;

    friend class Validation;
};


#endif //TEST_MEMBER_H
