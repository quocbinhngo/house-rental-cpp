//
// Created by ADMIN on 5/3/2022.
//

#include "System.h"
#include "./../DataType/Date/Date.h"
#include "./../Object/House/House.h"
#include "./../Object/Member/Member.h"
#include "./../Object/Admin/Admin.h"
#include "./../Object/HouseOccupy/HouseOccupy.h"
#include "./../Object/MemberOccupy/MemberOccupy.h"
#include "./../Object/Request/Request.h"
#include "./../Object/Review/Review.h"
#include "./../Validation/Validation.h"
#include "./../Helper/Helper.h"
#include "../StorageManager/InputStorageManager/InputStorageManager.h"
#include "../StorageManager/OutputStorageManager/OutputStorageManager.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

using std::cout;

//Clear system info when initialized
System::System() {
    this->currentMember = nullptr;
    this->systemHouseList.clear();
    this->systemMemberList.clear();
    this->memberSuitableHouseList.clear();
}

//Clear screen
void System::clearScreen(){
    std::cout << "Loading..." << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000) );
    for (int  n = 0; n < 10; n++)
        cout << "\n\n\n\n\n\n\n\n\n\n" ;
}

//Function to highlight
void System::highlighter() {
    int n;
    cout << "\n";
    for(n = 0; n < 9; n++){
        cout << "-------------";
    }
}

//Set current member when logging in
void System::setCurrentMember(Member *member) {
    currentMember = member;
    memberSuitableHouseList.clear();
}

//Add member when registering
void System::addMember(Member *member) {
    systemMemberList.push_back(member);
}

//Add house after entering house info
void System::addHouse(House *house) {
    systemHouseList.push_back(house);
}

//Add new admin
void System::addAdmin(Admin *admin) {
    systemAdminList.push_back(admin);
}

//Function to show suitable house list
bool System::memberSearchSuitableHouseList(Date *startDate, Date *endDate, int cityID) {
    memberSuitableHouseList.clear();

    // If that member already send another request for this house and that request is overlapped with the current request
    for (auto &request: currentMember->memberRequestList) {

        // If two time frame are not overlapped
        if (*request->requestToDate < *startDate || *endDate < *request->requestFromDate) {
            continue;
        }

        std::cout << "\nYou have sent another request overlapped with this request\n\n";
        return false;
    }

    // Find the suitable house
    for (auto &house: systemHouseList) {

        // If the house is suitable
        if (isSuitableHouse(startDate, endDate, cityID - 1, house)) {
            memberSuitableHouseList.push_back(house);
        }
    }



    // If there is no suitable house
    if (memberSuitableHouseList.empty()) {
        std::cout << "\nThere is no house suitable for you\n\n";
        return false;
    }

    // Print the suitable house list for user
    std::cout << "\nThe suitable house list:\n\n";
    for (int i = 0; i < memberSuitableHouseList.size(); i++) {
        std::cout << "--> " << i + 1 << ". ";
        std::cout << "Address: " << memberSuitableHouseList[i]->getHouseAddress() << ", Rating: " << memberSuitableHouseList[i]->getRatingScore() << "\n";
    }
    cout << "\n\n";

    return true;
}

//Function to check if the house is suitable to the member
bool System::isSuitableHouse(Date *startDate, Date *endDate, int cityID, House *house) {

    double currentMemberRatingScore = currentMember->getRatingScore();

    // If the house is not listed
    if (!house->isListed) {
        return false;
    }

    // If the house is owned by currentMember
    if (house == currentMember->ownedHouse) {
        return false;
    }

    // If the member do not have minimum required score
    if (currentMemberRatingScore < house->minOccupierRating) {
        return false;
    }

    // If the house is not available from the start and end date
//    int num1 = *startDate - *(house->availableFromDate), num2 = *house->availableToDate - *endDate;
    if (*startDate - *(house->availableFromDate) < 0 || *house->availableToDate - *endDate < 0) {
        return false;
    }

    // If the user do not have enough credit Points
    int differenceBetweenStartAndEndDate = *endDate - *startDate;
    if (differenceBetweenStartAndEndDate * house->consumingPointsPerDay > currentMember->creditPoints) {
        return false;
    }

    // If the house is in different city
    if (cityID != house->cityID) {
        return false;
    }

    // Loop over the occupy status of the house for checking
    for (auto &occupy: house->houseOccupyList) {

        // If the startDate and endDate is not occupied by another
        if ((startDate < occupy->occupyFromDate && endDate < occupy->occupyToDate)
            || (startDate > occupy->occupyFromDate && endDate > occupy->occupyToDate)) {
            continue;
        }

        return false;
    }

    

    return true;
}

//Function to view house reviews
bool System::memberViewHouseReviewList(int houseID, Date *sD, Date *eD) {
    if (houseID >= memberSuitableHouseList.size()) {
        return false;
    }

    auto house = memberSuitableHouseList[houseID];
    std::cout << house->viewHouseReview();
//    if (house->houseReviewList.empty()) {
//        std::cout << "There is no reviews for the house " << house->getHouseAddress() << "\n";
//    } else {
//        std::cout << "The review for the house " << house->getHouseAddress() << "\n";
//        for (int i = 0; i < house->houseReviewList.size(); i++) {
//            auto houseReview = house->houseReviewList[i];
//
//            auto ratingScore = houseReview->ratingScore;
//            auto comment = houseReview->comment;
//            auto reviewedByMember = houseReview->reviewedByMember;
//
//            std::cout << i << ". " << "Reviewed by: " << reviewedByMember->getFullName() << "\n";
//            std::cout << "\t-Rating: " << ratingScore << "\n\t-Comment: " << comment << "\n";
//        }

    return true;
}

//Function to send request to the house owner
bool System::memberSendRequest(Date *startDate, Date *endDate, int houseID) {
    if (houseID >= memberSuitableHouseList.size()) {
        return false;
    }

    for (auto &memberRequest: currentMember->memberRequestList) {
        if (*(memberRequest->requestToDate) < *startDate || *endDate < *(memberRequest->requestFromDate)) {
            continue;
        }

        return false;
    }

    // Create an object for the request
    auto *request = new Request(startDate, endDate, currentMember);

    // Add the request to the request list of the house
    memberSuitableHouseList[houseID]->addRequestToHouseRequestList(request);

    // Add the request to the request list of the member
    currentMember->addRequestToMemberRequestList(request);

    return true;
}

//Function to accept house request
bool System::memberAcceptRequest(int requestID) {

    // Current member accept the request
    currentMember->acceptTheRequest(requestID);

    return true;
}

//Function to view occupy list
bool System::memberViewOccupyList() {

    currentMember->viewMemberOccupyList();
    return true;
}

//Function for current member to leave house
bool System::leaveHouse(int leaveOccupyHouseID) {
    currentMember->leaveHouse(leaveOccupyHouseID);
    return true;
}

//Function to view unrated occupiers
bool System::memberViewUnratedOccupierList() {
    return currentMember->viewUnratedOccupierList();
}

//Function to rate occupier
bool System::memberRateOccupier(int occupierID) {
    int rating;
    std::string comment;
    std::cout << "\nRate this member from -10 to 10\n";
    rating = choiceFunc(-10,10);
    std::cout << "\nEnter a comment about this member: ";
    std::cin.ignore();
    std::getline(std::cin, comment);
    return currentMember->reviewOccupier(occupierID, rating, comment);
}

//Function to rate house
bool System::memberRateHouse(House *ratedHouse) {
    int rating;
    std::string comment;
    std::cout << "\n---Rate this house from -10 to 10---\n";
    rating = choiceFunc(-10,10);
    std::cout << "\nEnter a comment about this house: ";
    std::cin.ignore();
    std::getline(std::cin, comment);
    return currentMember->reviewHouse(ratedHouse, rating, comment);
}

//Function for entering choice
int System::choiceFunc(int a, int b) {
    std::string temp;
    int tempInt;
    int err;

    do{
        err = 0;
        std::cout << "\nEnter your choice (Number only): ";
        std::cin >> temp;
        if (!isNumber(temp)){
            std::cout << "Error. Please try again!\n";
            err++;
            continue;
        }
        tempInt = convertStringToInt(temp);
        if (tempInt < a || tempInt > b){
            std::cout << "Error. Please try again!\n";
            err++;
            continue;
        }
    } while (err);

    return tempInt;
}



//main menu
void System::mainMenu() {
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    cout << "\t--> MAIN MENU <--\n\n"
         << "\t--> 1. Guest\n"
         << "\t--> 2. Admin\n"
         << "\t--> 3. Member\n"
         << "\t--> 4. Exit\n\n";
    switch (choiceFunc(1, 4)){
        case 1:
            guestMenu();
            break;

        case 2:
            if(adminLogin()){
                std::cout << "Login successful\n";
                // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                adminMenu();
            } else {
                std::cout << "Login failed\n";
                clearScreen();
//                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                mainMenu();
            }
            break;

        case 3:
            memberLoginMenu();
            break;

        case 4:
            systemExit();
            break;
    }
}
//end main




//guest (feature 2)
void System::guestMenu() {
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\t--> GUEST MENU <--\n\n"
              << "--> 1.\tView houses list\n"
              << "--> 2.\tRegister for an account\n"
              << "--> 3.\tBack to main menu\n";
    switch (choiceFunc(1, 3)){
        case 1:
            //Print house list
            guestViewHouseList();
            break;
        case 2:
            clearScreen();
            //Register new member
            if(memberRegister()){
                std::cout << "Registration successful\n";
                // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                memberLoginMenu();
            }
            break;
        case 3:
            //Back to main menu
            clearScreen();
            mainMenu();
            break;
    }
}

void System::guestViewHouseList(){
    int choice;
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\n\t\t--> HOUSE LIST <-- \n"
              << "ID    Address \n\n";
    //Print house list
    for (House *hou : systemHouseList){
        std::cout << std::setw(6) << std::left << std::to_string(hou->houseID) + ".";
        std::cout << hou->getHouseAddress() << "\n";
    }
    std::cout << "\n" << systemHouseList.size()+1 << ". Back to guest menu";
    choice = choiceFunc(1, systemHouseList.size()+1);
    if (choice == systemHouseList.size()+1){
        //Go back to guest menu
        guestMenu();
    } else {
        //View house info
        highlighter();
        cout << "\nYou have chosen house number " << choice;
        std::cout << "\n" << systemHouseList[choice - 1]->viewHouseInfoByGuest();
        std::cout << "\n1. Back to house list\n";
        switch(choiceFunc(1, 1)){
            case 1:
                guestViewHouseList();
                break;
        }
    }
}
//end guest




//admin (feature 4)
bool System::adminLogin() {
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::string username, password;
    //Enter admin username and password
    std::cout << "\t--> ADMIN LOGIN <--\n\n"
              << "Username: ";
    std::cin.ignore();
    std::getline(std::cin, username);
    std::cout << "\nPassword: ";
    std::getline(std::cin, password);

    for (auto &admin: systemAdminList) {

        // Check whether the password and username is valid
        if (username != admin->username || password != admin->password) {
            continue;
        }

        return true;
    }

    return false;
}

void System::adminMenu() {
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\t--> ADMIN MENU <--\n\n"
              << "--> 1.\tMember List \n"
              << "--> 2.\tHouse List \n"
              << "--> 3.\tLog Out";
    switch (choiceFunc(1, 3)){
        case 1:
            //View member list
            clearScreen();
            adminViewMemberList();
            break;

        case 2:
            //View house list
            adminViewHouseList();
            break;

        case 3:
            //Back to main menu
            clearScreen();
            mainMenu();
            break;
    }
}

void System::adminViewMemberList() {
    int choice;
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\n\t--> MEMBER LIST <-- \n\n"
              << "ID    Name  \n";
    //Print member list
    for (Member *mem : systemMemberList){
        std::cout << std::setw(6) << std::left << (std::to_string(mem->memberID) + ".");
        std::cout << mem->getFullName() << "\n";
    }
    std::cout << systemMemberList.size()+1 << ".\tBack to admin menu\n";
    choice = choiceFunc(1, systemMemberList.size()+1);
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    if (choice == systemMemberList.size()+1){
        //Back to admin menu
        adminMenu();
    } else {
        //View member info
        systemMemberList[choice - 1]->viewMemberInfo();
        std::cout << "1. View member reviews\n"
                  << "2. Back to member list\n";
        switch(choiceFunc(1, 2)){
            case 1:
                //View member reviews
                std::cout << systemMemberList[choice - 1]->viewMemberReview();
                std::cout << "1. Back to member list\n";
                choiceFunc(1, 1);
                //Back to member list
                adminViewMemberList();
                break;
            case 2:
                //Back to member list
                adminViewMemberList();
                break;
        }

    }
}

void System::adminViewHouseList() {
    int choice;
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\n\t\t--> HOUSE LIST <-- \n"
              << "ID    Address \n\n";
    //Print house list
    for (House *hou : systemHouseList){
        std::cout << std::setw(6) << std::left << std::to_string(hou->houseID) + ".";
        std::cout << hou->getHouseAddress() << "\n";
    }
    std::cout << systemHouseList.size()+1 << ". \tBack to Admin Menu\n";
    choice = choiceFunc(1, systemHouseList.size()+1);
    if (choice == systemHouseList.size()+1){
        adminMenu();
    } else {
        clearScreen();
        //Print house info
        cout << "\n\t\t\t\t\t\t\t***************************************************";
        cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
        cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
        std::cout << "\n" << systemHouseList[choice - 1]->viewHouseInfo() << "\n\n";
        std::cout << "--> 1.\tView reviews\n"
                  << "--> 2.\tBack to house list\n";
        switch(choiceFunc(1, 2)){
            case 1:
                //House review
                std::cout << systemHouseList[choice - 1]->viewHouseReview();
                std::cout << "1. Back to house list";
                choiceFunc(1, 1);
                //Back to house list
                adminViewHouseList();
                break;
            case 2:
                //Back to house list
                adminViewHouseList();
                break;
        }
    }
}
//end admin




//member
    //member login (feature 3)
void System::memberLoginMenu() {
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\t--> MEMBER LOGIN MENU <--\n\n"
              << "--> 1.\tLogin\n"
              << "--> 2.\tBack to main menu\n";
    switch(choiceFunc(1, 2)){
        case 1:
            //Login to a member account
            if(memberLogin()){
                std::cout << "Login successful\n";
                memberMenu();
            } else {
                std::cout << "Wrong username or password\n";
                // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                mainMenu();
            } 
            break;
        case 2:
            clearScreen();
            if(memberRegister()){
                std::cout << "Registration successful\n";
//                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                memberLoginMenu();
            }
            break;

        case 3:
            clearScreen();
            mainMenu();
            break;
    }
}

bool System::memberLogin() {
    std::string username, password;
    //Enter member account info
    std::cout << "\n\t--> MEMBER LOGIN <--\n\n"
              << "Username: ";
    std::cin >> username;
    std::cout << "\nPassword: ";
    std::cin >> password;
    //Check if username and password is correct
    for (Member *mem : systemMemberList){
        if (username == mem->username && password == mem->password){
            setCurrentMember(mem);
            return true;
        }
    }
    return false;
}
    //end login


    //member register (feature 1)
bool System::memberRegister() {
    std::string username, password, firstName, lastName, phoneNumber;
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    //Enter member information
    std::cout << "\t--> CUSTOMER REGISTRATION <--\n\n";
    do {
        std::cout << "Enter a username: ";
        std::cin.ignore();
        std::getline(std::cin, username);
    } while (!validate.username(username, this));

    do {
        std::cout << "\nEnter a password: ";
        std::getline(std::cin, password);
    } while (!validate.password(password));

    do {
        std::cout << "\nEnter your first name: ";
        std::getline(std::cin, firstName);
    } while (!validate.name(firstName));

    do {
        std::cout << "\nEnter your last name: ";
        std::getline(std::cin, lastName);
    } while (!validate.name(lastName));

    do {
        std::cout << "\nEnter your phone number: ";
        std::getline(std::cin, phoneNumber);
    } while (!validate.phoneNumber(phoneNumber , this));

    auto *newMember = new Member(systemMemberList.size() + 1, username, password, firstName, lastName, phoneNumber);
    //Add member to member list
    addMember(newMember);
    return true;
}
    //end register


    //menu for member
void System::memberMenu(){
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    std::cout << "\t--> MEMBER MENU <--\n\n"
              << "--> 1.\tView account detail\n"
              << "--> 2.\tList menu\n"
              << "--> 3.\tSearch for available house\n"
              << "--> 4.\tView house request list\n"
              << "--> 5.\tView unrated occupier list\n"
              << "--> 6.\tView currently occupied house\n"
              << "--> 7.\tLog out\n";
    switch(choiceFunc(1,7)){
        case 1:
            //View member info
            currentMember->viewMemberInfo();
            std::cout << "1. Back to member menu";
            choiceFunc(1,1);
            memberMenu();
            break;

        case 2:
            //Go to member list
            memberListMenu();
            break;

        case 3:
            //Go to search suitable house
            memberSearchSuitableHouseMenu();
            break;

        case 4:
            //View house requests
            memberViewHouseRequestListMenu();
            break;

        case 5:
            //View unrated occupier
            memberViewUnratedOccupierListMenu();
            break;

        case 6:
            //View occupied house
            memberViewOccupiedHouse();
            break;

        case 7:
            //Log out and go back to main menu
            setCurrentMember(nullptr);
            clearScreen();
            mainMenu();
            break;
    }
}
    //end menu


    //list menu (feature 5)
void System::memberListMenu(){
    cout << "\n\t\t\t\t\t\t\t***************************************************";
    cout << "\n\t\t\t\t\t\t\t   *    VACATION HOUSE EXCHANGE APPLICATION    *";
    cout << "\n\t\t\t\t\t\t\t***************************************************\n\n";
    cout << "\t--> LIST MENU <--\n\n";
    //Check if member has an owned house
    if (currentMember->ownedHouse != nullptr){
        //Check if member's house is listed
        if (currentMember->ownedHouse->isListed) {
            //Print house info
            std::cout << "Currently listed house:\n"
                      << currentMember->ownedHouse->viewHouseInfo() << "\n";
            if (currentMember->ownedHouse->houseRequestList.empty()) {
                std::cout << "\n--> 1.\tUnlist house\n"
                          << "--> 2.\tBack to member menu\n";
                switch (choiceFunc(1, 2)) {
                    case 1:
                        //Unlist house
                        memberUnlistHouse();
                        std::cout << "1. Back to list menu\n";
                        choiceFunc(1, 1);
                        memberListMenu();
                        break;
                    case 2:
                        //Back to member menu
                        memberMenu();
                        break;
                }
            } else {
                std::cout << "1. Back to member menu\n";
                int choice = choiceFunc(1, 1);
                memberMenu();
            }
        } else {
            //List house if house isn't listed
            std::cout << "Your house isn't listed\n\n"
                      << "--> 1. List your house\n"
                      << "--> 2. Back to member menu\n";
            switch (choiceFunc(1, 2)){
                case 1:
                    memberListHouse();
                    std::cout << "\nSuccessfully listed house\n\n";

//                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


                    memberListMenu();
                    break;
                case 2:
                    memberMenu();
                    break;
            }
        }
    } else {
        //Enter house info if member has no owned house
        std::cout << "Please enter your HOUSE INFO \n\n";
        memberEnterHouseInfo();
        memberListMenu();
    }
}

bool System::memberEnterHouseInfo() {
    std::string number, street, district, houseDescrip;
    int cityID;
    //Enter house info
    std::cout << "Enter the house address: \n"
              << " - House Number: ";
    std::cin.ignore();
    std::getline(std::cin, number);
    std::cout << "- Street: ";
    std::getline(std::cin, street);
    std::cout << "- District: ";
    std::getline(std::cin, district);
    std::cout << "- House Description: ";
    std::getline(std::cin, houseDescrip);

    std::cout << "\nChoose the city your house is in: \n\n"
              << "--> 1.\tHanoi\n"
              << "--> 2.\tSaigon\n"
              << "--> 3.\tDanang\n";
    cityID = choiceFunc(1, 3) - 1;
    auto *newHouse = new House(systemHouseList.size() + 1, number, street, district, cityID, houseDescrip);
    //Add house to house list
    addHouse(newHouse);
    //Set owned house to the newly added house
    currentMember->createOwnedHouse(newHouse);
    return true;
}

        //list house (feature 5.1)
bool System::memberListHouse() {
    std::string startDate, endDate, conPoints, minOccRating;
    //Enter listing info (with validation)
    std::cout << "\nEnter listing information: \n";
    int conPointsNum, minOccRatingNum;

    do{
        do {
            std::cout << " - Start date: ";
            std::cin >> startDate;
        } while (!validate.date(startDate));
        do {
            std::cout << "- End date: ";
            std::cin >> endDate;
        } while (!validate.date(endDate));
    } while (!validate.listDate(startDate, endDate));

    do {
        std::cout << "- Consuming points per day: ";
        std::cin >> conPoints;
        if (!isNumber(conPoints)){
            std::cout << "Not a number. Try again.\n";
        }
    } while (!isNumber(conPoints));
    conPointsNum = convertStringToInt(conPoints);

    do {
        std::cout << "- Minimum Occupier Rating: ";
        std::cin >> minOccRating;
        if (!isDouble(minOccRating)){
            std::cout << "\nNot a number. Try again.\n";
        }
    } while (!isDouble(minOccRating));
    minOccRatingNum = convertStringToDouble(minOccRating);

    Date *sD = convertStringToDate(startDate), *eD = convertStringToDate(endDate);
    //Add listing info to data
    currentMember->listHouse(sD, eD, conPointsNum, minOccRatingNum);
    return true;
}

        //unlist house (feature 5.2)
bool System::memberUnlistHouse() {
    if(currentMember->unListHouse()){
        std::cout << "\nSuccessfully unlist house\n\n";
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return true;
    } else {
        std::cout << "Failed to unlist house. Please accept member request before unlisting the house.\n";
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return false;
    }
}
    //end list menu


    //search suitable house (feature 6, 7)
void System::memberSearchSuitableHouseMenu(){
    std::string start, end;
    int cityID;
    //Enter search information
    std::cout << "\nSearch for a suitable house: \n\n";
    do {
        do {
            std::cout << "Enter start date (dd/mm/yyyy): ";
            std::cin >> start;
        } while (!validate.date(start));

        do {
            std::cout << "\n\nEnter end date (dd/mm/yyyy): ";
            std::cin >> end;
        } while (!validate.date(end));
    } while (!validate.listDate(start, end));

    std::cout << "\nChoose the city you want to search for: \n\n"
              << "--> 1. Hanoi\n"
              << "--> 2. Saigon\n"
              << "--> 3. Danang\n";
    cityID = choiceFunc(1, 3);
    Date *startDate = convertStringToDate(start), *endDate = convertStringToDate(end);
    //Go to suitable house list
    memberSuitableHouseMenu(startDate, endDate, cityID);
}

void System::memberSuitableHouseMenu(Date *sD, Date *eD, int cityID) {
    //Clear the suitable house vector
    memberSuitableHouseList.clear();
    //Show suitable houses
    memberSearchSuitableHouseList(sD, eD, cityID);
    std::cout << memberSuitableHouseList.size() + 1 << ". Back to member menu\n";
    int choice = choiceFunc(1, memberSuitableHouseList.size() + 1);
    if (choice == (memberSuitableHouseList.size()+1)){
        //Back to member menu
        memberMenu();
    } else {
        //View house info
        std::cout << memberSuitableHouseList[choice - 1]->viewHouseInfo();
        std::cout << "\n\n--> 1.\tRequest house\n\n"
                  << "--> 2.\tView house reviews\n\n"
                  << "--> 3.\tBack to house list\n";
        switch (choiceFunc(1,3)){
            case 1:
                //Send house request to the owner of the house and go back to member menu
                memberSendRequest(sD, eD, choice - 1);
                std::cout << "\n\tRequest sent\n";
//                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                memberMenu();
                break;
            case 2:
                //View house reviews
                memberViewHouseReviewList(choice - 1, sD, eD);
                std::cout << "\n\n--> 1.\tRequest house\n\n"
                          << "--> 2.\tBack to house list\n";
                switch (choiceFunc(1, 2)) {
                    case 1:
                        //Send house request to the owner of the house
                        memberSendRequest(sD, eD, choice - 1);
                        std::cout << "\tRequest sent\n";
//                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        memberMenu();
                        break;
                    case 2:
                        //Back to suitable house list
                        memberSuitableHouseMenu(sD, eD, cityID);
                        break;
                }
                break;
            case 3:
                //Back to suitable house list
                memberSuitableHouseMenu(sD, eD, cityID);
                break;

//                switch(choiceFunc(1, 2)){
//                    case 1:
//                        std::cout << systemHouseList[choice - 1]->viewHouseReview();
//                        std::cout << "1. Back to house list";
//                        choiceFunc(1, 1);
//                        adminViewHouseList();
//                        break;
//                    case 2:
//                        adminViewHouseList();
//                        break;
//                }
        }
    }
}

void System::suitableHouseMenu(int choice, Date *sD, Date *eD, int cityID) {
    std::cout << memberSuitableHouseList[choice - 1]->viewHouseInfo() << "\n";
    std::cout << "1. View reviews\n"
              << "2. Request house\n"
              << "3. Back to house list\n";
    switch (choiceFunc(1,3)){
        case 1:
            std::cout << memberSuitableHouseList[choice - 1]->viewHouseReview();
            std::cout << "1. Back to house menu\n";
            choiceFunc(1, 1);
            suitableHouseMenu(choice, sD, eD, cityID);
        case 2:
            if(memberSendRequest(sD, eD, memberSuitableHouseList[choice-1]->houseID)){
                std::cout << "Request sent\n";
            } else {
                std::cout << "Failed to send request\n";
            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            memberMenu();
            break;
        case 3:
            memberSuitableHouseMenu(sD, eD, cityID);
            break;
    }
}
    //end search suitable house


    //view house request (feature 8, 9, 10)
void System::memberViewHouseRequestListMenu() {
    //Check if member has an owned house
    if (currentMember->ownedHouse == nullptr){
        std::cout << "You haven't entered your house info\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
        //Check if owned house is listed
    } else if (!currentMember->ownedHouse->isListed){
        std::cout << "You haven't listed a house\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }

    std::cout << "\nChoose a request for view more detail: \n\n";

    //View house request list
    if (currentMember->viewRequestList()) {
        std::cout << "\n" << currentMember->ownedHouse->houseRequestList.size() + 1 << ". Back to member menu\n\n";
        int choice = choiceFunc(1, currentMember->ownedHouse->houseRequestList.size() + 1);
        if (choice == currentMember->ownedHouse->houseRequestList.size() + 1) {
            //Back to member menu
            memberMenu();
        } else {
            std::cout << "\n\n-->\t1. Accept the request\n"
                      << "-->\t2. View the reviews of this member\n"
                      << "-->\t3. Back to menu\n";
            switch (choiceFunc(1, 3)) {
                case 1: {
                    //Accept house request
                    if (currentMember->acceptTheRequest(choice - 1)) {
                        std::cout << "Accepting Request successfully\n";
                    } else {
                        std::cout << "Failed to accept request\n";
                    }
//                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    memberMenu();
                    break;
                }
                case 2: {
                    //View member reviews
                    memberViewOccupierReviewList(choice);
                    break;
                }
                case 3: {
                    //Back to member menu
                    memberMenu();
                    break;
                }
            }

        }
    } else {
        //Back to member menu
        std::cout << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    }
}

//             if (currentMember->acceptTheRequest(choice - 1)) {
//                 std::cout << "\nAccepting Request successfully\n";
//                 std::cout << "\n1. Back to member menu\n\n";
//                 choiceFunc(1,1);
//                 memberMenu();
//             } else {
//                 std::cout << "\nFailed to accept request\n";
//                 std::cout << "\n1. Back to member menu\n\n";
//                 choiceFunc(1,1);
//                 memberMenu();
// >>>>>>> origin/long_ui
//             }
        
    

    //end view house request


    //view unrated occupier (feature 12)
void System::memberViewUnratedOccupierListMenu() {
    //Check if member has an owned house
    if (currentMember->ownedHouse == nullptr){
        std::cout << "\nYou don't have an owned house.\n\n"
                  << "1. Back to member menu\n";
        choiceFunc(1,1);
        memberMenu();
    }
    //Unrated occupier list
    std::cout << "\nUnrated Occupier List: \n\n";
    currentMember->viewUnratedOccupierList();
    std::cout << "\n" << currentMember->ownedHouse->unratedOccupierList.size()+1 << ". Back to member menu\n";
    int choice = choiceFunc(1, currentMember->ownedHouse->unratedOccupierList.size() + 1);
    if (choice == currentMember->ownedHouse->unratedOccupierList.size() + 1){
        //Back to member menu
        memberMenu();
    } else {
        //View unrated occupier info
        memberUnratedOccupierRatingMenu(choice - 1);
    }
}

void System::memberUnratedOccupierRatingMenu(int member){
    currentMember->ownedHouse->unratedOccupierList[member]->occupiedByMember->viewMemberInfo();
    std::cout << "-->\t1. Rate this member\n"
              << "-->\t2. Back to unrated occupier list\n";
    switch (choiceFunc(1, 2)){
        case 1:
            //Rate unrated occupier
            if (memberRateOccupier(member)){
                std::cout << "\nSuccessfully rated member\n";
            } else {
                std::cout << "\nRating failed\n";
            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            memberViewUnratedOccupierListMenu();
            break;
        case 2:
            //Back to unrated occupier
            memberViewUnratedOccupierListMenu();
            break;
    }

}
    //end view unrated occupier


    //view currently occupied house (leave house)
void System::memberViewOccupiedHouse() {
    //Check if current member is occupying any house
    if (currentMember->memberOccupyList.empty()) {
        std::cout << "\nYou are not occupying any house\n\n"
                  << "1. Back to member menu\n";
        choiceFunc(1, 1);
        memberMenu();
    } else {
        //View occupy list
        currentMember->viewMemberOccupyList();
        std::cout << "--> "<< currentMember->memberOccupyList.size() + 1 << ". Back to member menu\n";
        int choice = choiceFunc(1, currentMember->memberOccupyList.size() + 1);
        if (choice == currentMember->memberOccupyList.size() + 1) {
            //Back to member menu
            memberMenu();
        } else {
            //Check occupying info
            auto occupyHouse = currentMember->memberOccupyList[choice - 1]->occupyHouse;
            std::cout << "\n" << occupyHouse->viewHouseInfo() << "\n"
                      << "\n-->\t1. Leave house\n"
                      << "\n-->\t2. Back to member menu\n";
            switch (choiceFunc(1, 2)){
                case 1:{
                    //Leave a house review
                    if(memberLeaveReview(occupyHouse)){
                        std::cout << "House reviewed\n";
                    }
                    //Leave house
                    if(currentMember->leaveHouse(choice - 1)){
                        std::cout << "\nLeft house successfully\n"
                                  << "Thank you for using our service\n"
                                  << "SEE YOU AGAIN!!!\n\n";
                    } else {
                        std::cout << "\nProcess unsuccessful\n";
                    }
                    std::cout << "-->\t1. Back to member menu\n";
                    choiceFunc(1, 1);
                    memberMenu();
                    break;
                }
                case 2:{
                    //Back to member menu
                    memberMenu();
                    break;
                }
            }
//                memberMenu();
        }
    }
}    //end view currently occupied house


    //leave review (feature 11)
    bool System::memberLeaveReview(House *leaveHouse) {
        std::cout << "\nWould you like to leave a review?\n\n"
                  << "-->\t1. Yes\n"
                  << "-->\t2. No\n";
        switch(choiceFunc(1, 2)){
            case 1:
                //Rate house
                if(memberRateHouse(leaveHouse)){
                    return true;
                } else {
                    std::cout << "\nFailed to leave a review\n";
                    return false;
                }
                break;
            case 2:
                return false;
                break;
        }
        return true;
    }

    //Save data to files when exiting the program;
void System::systemExit() {
    auto *outputStorageManager = new OutputStorageManager();
    outputStorageManager->outputStorageLoadDataFromSystem(this);
    outputStorageManager->outputStorageToFileList();
    std::exit;
}

void System::memberViewOccupierReviewList(int choice) {
    auto houseRequestList = currentMember->ownedHouse->houseRequestList;
    auto occupier = houseRequestList[choice - 1]->requestedByMember;
    //Occupier reviews
    std::cout << occupier->viewMemberReview();

    std::cout << "\n\n--> 1.\tAccept the request\n\n"
              << "--> 2.\tBack to menu list\n";

    switch (choiceFunc(1, 2)) {
        case 1: {
            //Accept house request
            if (currentMember->acceptTheRequest(choice - 1)) {
                std::cout << "Accepting Request successfully\n";
            } else {
                std::cout << "Failed to accept request\n";
            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            memberMenu();
            break;
        }
        case 2: {
            //Back to member menu
            memberMenu();
            break;
        }
    }
}


//end leave review


//end member