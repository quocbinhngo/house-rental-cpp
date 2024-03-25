//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_HOUSE_H
#define TEST_HOUSE_H

#include <iostream>
#include <vector>

class Date;
class Review;
class Request;
class HouseOccupy;
class Member;

class House {
    int houseID;
    std::string houseNumber;
    std::string street;
    std::string district;
    int cityID;
    std::string houseDescription;

    Member* houseOwner;

    bool isListed;
    Date *availableFromDate;
    Date *availableToDate;
    int consumingPointsPerDay;
    double minOccupierRating;

    std::vector<Review*> houseReviewList;

    std::vector<Request*> houseRequestList;

    std::vector<HouseOccupy*> houseOccupyList;

    std::vector<HouseOccupy*> unratedOccupierList;

public:
    House(int houseID, std::string houseNumber, std::string street, std::string district, int cityID, std::string houseDescription);

    std::string getCityName();

    std::string getHouseAddress();

    double getRatingScore();

    bool addRequestToHouseRequestList(Request *request);

    bool removeRequestFromHouseRequestList(Request *request);

//    bool removeOccupyFromHouseOccupyList(Date *occupyFromDate, Date *occupyToDate, Member *occupiedByMember);

    bool addReviewToHouseReviewList(Review *houseReview);

    bool addOccupierToUnratedOccupierList(HouseOccupy *houseOccupier);

    bool removeOccupierFromUnratedOccupierList(HouseOccupy *houseOccupier);

    std::string viewHouseInfo();


    std::string viewHouseReview();

    std::string viewHouseInfoByGuest();

    ~House();

    friend class Member;

    friend class System;

    friend class InputStorageManager;

    friend class OutputStorageManager;
};


#endif //TEST_HOUSE_H
