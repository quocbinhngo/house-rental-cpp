//
// Created by ADMIN on 5/3/2022.
//
#include "../../Helper/Helper.h"
#include "House.h"
#include "../Review/Review.h"
#include "../../DataType/Date/Date.h"
#include "../HouseOccupy/HouseOccupy.h"
#include "../Member/Member.h"
#include "../../Constant.h"

#include <sstream>
#include <algorithm>

class Member;

House::House(int houseID, std::string houseNumber, std::string street, std::string district, int cityID, std::string houseDescription) {
    this->houseID = houseID;
    this->houseNumber = std::move(houseNumber);
    this->street = std::move(street);
    this->district = std::move(district);
    this->cityID = cityID;
    this->houseDescription = std::move(houseDescription);
    this->isListed = false;
    this->availableFromDate = nullptr;
    this->availableToDate = nullptr;
    this->consumingPointsPerDay = 0;
    this->minOccupierRating = 0;
}

// get the name of the house's city
std::string House::getCityName() {
    return CITY_NAME_LIST[cityID];
}

//get house's address
std::string House::getHouseAddress() {
    std::stringstream ss;
    ss << houseNumber << " " << street << ", District: " << district << ", City: " << getCityName();
    return ss.str();
}

//get the average rating of the house
double House::getRatingScore() {
    // If no people rate for the house

    if (houseReviewList.empty()) {
        return 0;
    }

    double res = 0;

    // Loop over the reviewList to find the score
    for (auto &review: houseReviewList) {
        res += review->ratingScore;
    }

    // Get the average score
    double ratingScore = (double) res / (double) houseReviewList.size();

    // Round that number
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << ratingScore;
    ss >> ratingScore;

    return ratingScore;
}

//Push the request made by member to the request list of the house.
bool House::addRequestToHouseRequestList(Request *request) {
    houseRequestList.push_back(request);
    return true;
}

//Removing a request from house’s request list.
bool House::removeRequestFromHouseRequestList(Request *request) {
    houseRequestList.erase(std::find(houseRequestList.begin(), houseRequestList.end(), request));
    return true;
}

//Removing an occupy session from the list of occupy sessions of the house.
//bool House::removeOccupyFromHouseOccupyList(Date *occupyFromDate, Date *occupyToDate, Member *occupiedByMember) {
////    houseOccupyList.erase(std::find_if(houseOccupyList.begin(), houseOccupyList.end(), [&](HouseOccupy *houseOccupy) {
////        return (*(houseOccupy->occupyFromDate) - *occupyFromDate == 0) && (*(houseOccupy->occupyToDate) - *occupyToDate == 0) && (houseOccupy->occupiedByMember == occupiedByMember);
////    }));
//
//    // Create new class for noting unrated occupier
//    auto *unratedOccupier = new HouseOccupy(occupyFromDate, occupyToDate, occupiedByMember);
//    addOccupierToUnratedOccupierList(unratedOccupier);
//
//    return true;
//}

//Adding a review to the house’s review list
bool House::addReviewToHouseReviewList(Review *houseReview) {
    houseReviewList.push_back(houseReview);
    return true;
}

//Adding the info of an occupy session to the unrated occupier list
bool House::addOccupierToUnratedOccupierList(HouseOccupy *houseOccupier) {
    unratedOccupierList.push_back(houseOccupier);
    return true;
}

//Removing an occupier from an unrated occupier list
bool House::removeOccupierFromUnratedOccupierList(HouseOccupy *houseOccupier) {
    unratedOccupierList.erase(std::find(unratedOccupierList.begin(), unratedOccupierList.end(), houseOccupier));
    return true;
}

/*
 * Function for viewing house info by admin
 */
//View house information
std::string House::viewHouseInfo() {
    std::stringstream ss;
    ss << "Address: " << getHouseAddress()
       << "\t--> Owner: " << houseOwner->getFullName() << " <--"
       << " \nDescription: " << houseDescription
       << " \nRating: " << getRatingScore();

    if (!isListed) {
        return ss.str();
    }

    ss << "\nAvailable from Date: " << availableFromDate->toStr()
       << " to Date: " << availableToDate->toStr()
       << "\nConsuming Points Per Day: " << consumingPointsPerDay
       << "\nMinimum Occupier Rating Score: " << minOccupierRating;

    return ss.str();
}

/*
 * Function for viewing review by admin
 */

//View the house’s review.
std::string House::viewHouseReview() {
    std::stringstream ss;
    if (houseReviewList.empty()){
        return "There is no review for this house";
    }

    for (int i = 0; i < houseReviewList.size(); i++) {
        auto review = houseReviewList[i];
        auto comment = review->comment;
        auto ratingScore = review->ratingScore;
        auto reviewedByMember = review->reviewedByMember;
        ss << "\n\n***************************************"
           << "\n Reviewed by Member:\t" << reviewedByMember->getFullName() << "     "
           << "\n***************************************"
           << "\n--> Rating Score: " << ratingScore << " <--"
           << "\nComment: " << comment
           << "\n\n";
    }

    return ss.str();
}

/*
 * View House info by guest
 */

//View the house information guest
std::string House::viewHouseInfoByGuest() {
    std::stringstream ss;
    ss << "Address: " << getHouseAddress()
       << "\t--> Owner: " << houseOwner->getFullName() << " <--"
       << "\n\nDescription: " << houseDescription
       << " \n\n-*- Rating Score: " << getRatingScore() << " -*-\n";
    return ss.str();
}

//Destructor
House::~House() {
    for (auto &request: houseReviewList) {
        delete request;
    }

    // Destructor for all review
    for (auto &review: houseReviewList) {
        delete review;
    }

    // Destructor for all occupySession
    for (auto &occupySession: houseOccupyList) {
        delete occupySession;
    }

    // Delete all the date pointers
    delete availableFromDate;
    delete availableToDate;
}

