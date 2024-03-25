//
// Created by ADMIN on 5/3/2022.
//

#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>


#include "Member.h"
#include "../Review/Review.h"
#include "../House/House.h"
#include "../Request/Request.h"
#include "../HouseOccupy/HouseOccupy.h"
#include "../MemberOccupy/MemberOccupy.h"
#include "../../Helper/Helper.h"

//Constructor
Member::Member(int memberID, std::string username, std::string password, std::string firstName, std::string lastName,
               std::string phoneNumber, int creditPoints)
        : User(std::move(username), std::move(password)) {
    this->memberID = memberID;
    this->firstName = std::move(firstName);
    this->lastName = std::move(lastName);
    this->phoneNumber = std::move(phoneNumber);

    this->creditPoints = creditPoints;

    this->ownedHouse = nullptr;

    memberReviewList.clear();

}

void Member::test() {
    std::cout << memberID << " " << ownedHouse->houseID << std::endl;
}

//Get the member’s fullname
std::string Member::getFullName() {
    return firstName + " " + lastName;
}

//Get the average rating score of the member
double Member::getRatingScore() {

    // If no people rate for the member
    if (memberReviewList.empty()) {
        return 0;
    }

    double res = 0;

    // Loop over the reviewList to find the score
    for (auto &review: memberReviewList) {
        res += review->ratingScore;
    }

    // Get the average score
    double ratingScore = (double) res / (double) memberReviewList.size();

    // Round that number
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << ratingScore;
    ss >> ratingScore;

    return ratingScore;
}

//View the member’s information
bool Member::viewMemberInfo() {
    std::cout << "\n- Username: " << username << "\n";
    std::cout << "- Fullname: " << getFullName() << "\n";
    std::cout << "- Phone number: " << phoneNumber << "\n";
    std::cout << "- Credit Points: " << creditPoints << "\n";
    std::cout << "- Rating Score: " << getRatingScore() << "\n";

    if (ownedHouse != nullptr) {
        std::cout << "- House Address: " << ownedHouse->getHouseAddress() << "\n";
    }
    std::cout << "\n";
    return true;
}

//List the house
bool Member::listHouse(Date *startDate, Date *endDate, int consumingPointsPerDay, double minimumOccupierRating) {
    if (ownedHouse->isListed) {
        return false;
    }

    ownedHouse->isListed = true;
    ownedHouse->availableFromDate = startDate;
    ownedHouse->availableToDate = endDate;
    ownedHouse->consumingPointsPerDay = consumingPointsPerDay;
    ownedHouse->minOccupierRating = minimumOccupierRating;

    return true;
}

//Unlist the house
bool Member::unListHouse() {
    if (!ownedHouse->isListed || !ownedHouse->houseRequestList.empty()) {
        return false;
    }

    ownedHouse->isListed = false;
    ownedHouse->availableFromDate = nullptr;
    ownedHouse->availableToDate = nullptr;
    ownedHouse->consumingPointsPerDay = 0;
    ownedHouse->minOccupierRating = 0;

    ownedHouse->houseRequestList.clear();

    return true;
}

//Assign the house to a member
bool Member::createOwnedHouse(House *house) {
    if (house == nullptr) {
        return false;
    }

    ownedHouse = house;
    house->houseOwner = this;
    return true;
}

//View the request list sent to the house
bool Member::viewRequestList() {
    if (ownedHouse->houseRequestList.empty()) {
        std::cout << "\nYour house does not have any request\n\n";
        return false;
    }

    std::cout << "The request list for your house:\n\n";
    for (int i = 0; i < ownedHouse->houseRequestList.size(); i++) {
        auto request = ownedHouse->houseRequestList[i];
        std::cout << "--> " << i+1 << ". " << request->requestFromDate->toStr() << " to "
                  << request->requestToDate->toStr() << ": "
                  << "(Member: " << request->requestedByMember->getFullName()
                  << ", Rating: " << request->requestedByMember->getRatingScore() << ")\n";
    }

    return true;
}

//Reject the request in the request list
bool Member::rejectTheRequest(int acceptedRequestID) {
    auto acceptedRequest = ownedHouse->houseRequestList[acceptedRequestID];
    for (auto &request: ownedHouse->houseRequestList) {

        // Check the current request is the accepted request
        if (request == acceptedRequest) {
            continue;
        }

        // If the current request is not overlapped with the accepted request
        if (*(request->requestToDate) - *(acceptedRequest->requestFromDate) < 0
            || *(acceptedRequest->requestToDate) - *(request->requestFromDate) < 0) {
            continue;
        }

        // Remove the rejected request from the requestList of the member
        request->requestedByMember->removeRequestFromMemberRequestList(request);

        // Remove the rejected request from the requestList of the house
        ownedHouse->removeRequestFromHouseRequestList(request);
    }

    return true;
}

//Accept the request in the request list
bool Member::acceptTheRequest(int acceptedRequestID) {
    //check acceptedRequestID is larger than or equal the houseRequestList size
    if (acceptedRequestID >= ownedHouse->houseRequestList.size()) {
        return false;
    }

    auto acceptedRequest = ownedHouse->houseRequestList[acceptedRequestID];

    // Get the parameter for occupy object
    auto occupyStartDate = acceptedRequest->requestFromDate;
    auto occupyEndDate = acceptedRequest->requestToDate;
    auto occupyMember = acceptedRequest->requestedByMember;

    // Check whether the accepted member have enough credit points
    int requiredCreditPoints = (*occupyEndDate - *occupyStartDate) * ownedHouse->consumingPointsPerDay;
    if (occupyMember->creditPoints < requiredCreditPoints) {
        // Remove the request from that member request list
        occupyMember->removeRequestFromMemberRequestList(acceptedRequest);

        // Remove the accepted request from the requestList
        ownedHouse->removeRequestFromHouseRequestList(acceptedRequest);

        return false;
    }

    // assign  to object
    auto *houseOccupy = new HouseOccupy(occupyStartDate, occupyEndDate, occupyMember);
    auto *memberOccupy = new MemberOccupy(occupyStartDate, occupyEndDate, ownedHouse);

    // push to houseOccupyList
    ownedHouse->houseOccupyList.push_back(houseOccupy);

    // Push the current occupy to the occupy list
    occupyMember->addOccupyToMemberOccupyList(memberOccupy);

    // Reject other requests if their time is overlapped
    rejectTheRequest(acceptedRequestID);

    // Remove the request from that member request list
    occupyMember->removeRequestFromMemberRequestList(acceptedRequest);

    // Remove the accepted request from the requestList
    ownedHouse->removeRequestFromHouseRequestList(acceptedRequest);



    // Minus credit point for occupier and add creditPoints for owner
    occupyMember->minusCreditPoints(requiredCreditPoints);
    this->addCreditPoints(requiredCreditPoints);

    return true;
}

//Leave the occupy house
bool Member::leaveHouse(int leaveOccupyHouseID) {

    // Check leaveOccupyHouseID is out of bound
    if (leaveOccupyHouseID >= memberOccupyList.size()) {
        return false;
    }

    auto leaveOccupy = memberOccupyList[leaveOccupyHouseID];
    auto leaveOccupyFromDate = leaveOccupy->occupyFromDate;
    auto leaveOccupyToDate = leaveOccupy->occupyToDate;
    auto leaveOccupyHouse = leaveOccupy->occupyHouse;

//    leaveOccupyHouse->removeOccupyFromHouseOccupyList(leaveOccupyFromDate, leaveOccupyToDate, this);

    // Create new class for noting unrated occupier
    auto *unratedOccupier = new HouseOccupy(leaveOccupyFromDate, leaveOccupyToDate, this);
    leaveOccupyHouse->addOccupierToUnratedOccupierList(unratedOccupier);
    removeOccupyFromMemberOccupyList(leaveOccupy);

    return true;
}

//Review the occupied house
bool Member::reviewHouse(House *occupyHouse, int scoreRating, std::string comment) {
    auto *houseReview = new Review(scoreRating, std::move(comment), this);
    occupyHouse->addReviewToHouseReviewList(houseReview);

    return true;
}

//View the unrated occupier list of the house
bool Member::viewUnratedOccupierList() {
    if (ownedHouse->unratedOccupierList.empty()) {
        std::cout << "There is no unrated occupier\n\n";
        return false;
    }

    std::cout << "The list of unrated occupier:\n\n";
    int i = 1;
    for (auto &unratedOccupy: ownedHouse->unratedOccupierList) {
        auto occupyFromDate = unratedOccupy->occupyFromDate;
        auto occupyToDate = unratedOccupy->occupyToDate;
        auto occupiedByMember = unratedOccupy->occupiedByMember;

        std::cout << "-->\t" << i << ". " << occupyFromDate->toStr() << " " << occupyToDate->toStr() <<  ": " << occupiedByMember->getFullName() << "\n";
        i++;
    }

    return true;
}

//Review a particular occupier
bool Member::reviewOccupier(int reviewedOccupierID, int score, std::string comment) {
    if (reviewedOccupierID >= ownedHouse->unratedOccupierList.size()) {
        return false;
    }

    auto ratedOccupy = ownedHouse->unratedOccupierList[reviewedOccupierID];

    auto ratedOccupier = ratedOccupy->occupiedByMember;

    auto *memberReview = new Review(score, std::move(comment), this);

    ratedOccupier->addReviewToMemberReviewList(memberReview);

    ownedHouse->removeOccupierFromUnratedOccupierList(ratedOccupy);

    return true;
}

//Add a request to member request list
bool Member::addRequestToMemberRequestList(Request *addedRequest) {
    memberRequestList.push_back(addedRequest);
    return true;
}

//Remove the request from member request list.
bool Member::removeRequestFromMemberRequestList(Request *removedRequest) {
    memberRequestList.erase(std::find(memberRequestList.begin(), memberRequestList.end(), removedRequest));
    return true;
}

//Add an occupy session to the member’s occupy list
bool Member::addOccupyToMemberOccupyList(MemberOccupy *addedOccupy) {
    memberOccupyList.push_back(addedOccupy);
    return true;
}

//Remove the occupy session of the member
bool Member::removeOccupyFromMemberOccupyList(MemberOccupy *leaveOccupy) {
    memberOccupyList.erase(std::find(memberOccupyList.begin(), memberOccupyList.end(), leaveOccupy));
    return true;
}

//View all the occupy list of a member
bool Member::viewMemberOccupyList() {

    if (memberOccupyList.empty()) {
        std::cout << "There is no house you are occupying\n";
        return false;
    }

    std::cout << "The list of houses you are occuying:\n\n";
    for (int i = 0; i < memberOccupyList.size(); i++) {
        auto memberOccupy = memberOccupyList[i];

        // Get the occupyFromDate, occupyToDate, occupyHouse
        auto occupyFromDate = memberOccupy->occupyFromDate;
        auto occupyToDate = memberOccupy->occupyToDate;
        auto occupyHouse = memberOccupy->occupyHouse;

        // Print the occupy
        std::cout << "--> " << i + 1 << ".\t" << occupyFromDate->toStr() << " to " << occupyToDate->toStr() << ": " << occupyHouse->getHouseAddress() << "\n";
    }
    return true;
}

//Add a review to member’s review list
bool Member::addReviewToMemberReviewList(Review *memberReview) {
    memberReviewList.push_back(memberReview);
    return true;
}

//Minus credit points from the member account
bool Member::minusCreditPoints(int points) {
    if (points > creditPoints) {
        return false;
    }


    creditPoints -= points;
    return true;
}

//Adding credit points to the member account
bool Member::addCreditPoints(int points) {
    creditPoints += points;
    return false;
}

/*
 * View Member Review by admin
 */

//Viewing the review of a member when they are an occupier
std::string Member::viewMemberReview() {
    std::stringstream ss;
    if (memberReviewList.empty()) {
        return "There is no review for this member.\n";
    }

    ss << "The list of reviews for occupier ";
    for (int i = 0; i < memberReviewList.size(); i++) {
        auto review = memberReviewList[i];
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

//Destructor
Member::~Member() {
    // Destructor for all request
    for (auto &request: memberRequestList) {
        delete request;
    }

    // Destructor for all review
    for (auto &review: memberReviewList) {
        delete review;
    }

    // Destructor for all occupySession
    for (auto &occupySession: memberOccupyList) {
        delete occupySession;
    }
}