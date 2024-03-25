//
// Created by ADMIN on 5/3/2022.
//

#ifndef TEST_SYSTEM_H
#define TEST_SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <string>
#include <ctype.h>
#include "../Validation/Validation.h"


class House;
class Member;
class Admin;
class Date;
class OutputStorageManager;
class Validation;

class System {

public:

    std::vector<House*> systemHouseList;
    std::vector<Member*> systemMemberList;
    std::vector<House*> memberSuitableHouseList;
    std::vector<Admin*> systemAdminList;
    Member* currentMember;
    Validation validate;

    System();

    void setCurrentMember(Member *member);

    void clearScreen();

    void highlighter();

    void addMember(Member *member);

    void addHouse(House *house);

    void addAdmin(Admin *admin);

    bool memberLogin();

    bool memberSearchSuitableHouseList(Date *startDate, Date *endDate, int cityID);

    bool isSuitableHouse(Date *startDate, Date *endDate, int cityID, House *house);

    bool memberSendRequest(Date *startDate, Date *endDate, int houseID);

    bool memberSeeRequestList();

    bool memberAcceptRequest(int requestID);

    bool memberViewOccupyList();

    bool leaveHouse(int leaveOccupyHouseID);

    bool memberViewUnratedOccupierList();

    bool memberRateOccupier(int occupierID);

    bool memberRateHouse(House* ratedHouse);

    void mainMenu();

    void adminMenu();

    bool adminLogin();

    void adminViewMemberList();

    void adminViewHouseList();

    void memberMenu();

    void memberLoginMenu();

    bool memberRegister();

    void memberListMenu();

    bool memberListHouse();

    bool memberUnlistHouse();

    void memberSearchSuitableHouseMenu();

    void memberSuitableHouseMenu(Date *sD, Date *eD, int cityID);

    void memberViewHouseRequestList(int houseID);

    bool memberViewHouseReviewList(int houseID, Date *sD, Date *eD);

    void memberUnratedOccupierRatingMenu(int member);

    void memberViewOccupiedHouse();

    bool memberLeaveReview(House *leaveHouse);;

    bool memberEnterHouseInfo();

    void guestMenu();

    void guestViewHouseList();

    void memberViewUnratedOccupierListMenu();

    void memberViewHouseRequestListMenu();

    void suitableHouseMenu(int choice, Date *sD, Date *eD, int cityID);

    void memberViewOccupierReviewList(int choice);

    int choiceFunc(int a, int b);

    void systemExit();

    friend class InputStorageManager;

    friend class OutputStorageManager;

};


#endif //TEST_SYSTEM_H
