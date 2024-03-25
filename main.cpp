#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "./Helper//Helper.h"
#include "./DataType/Date/Date.h"
#include "./Object/User/User.h"
#include "./Object/Request/Request.h"
#include "./Object/Review/Review.h"
#include "./Object/HouseOccupy/HouseOccupy.h"
#include "./Object/MemberOccupy/MemberOccupy.h"
#include "./Object/House/House.h"
#include "./Object/Member/Member.h"
#include "./System/System.h"
#include "StorageManager/OutputStorageManager/OutputStorageManager.h"
#include "StorageManager/InputStorageManager/InputStorageManager.h"
#include "./Object/Admin/Admin.h"
using std::cout;


class User;
class Request;
class Review;
class HouseOccupy;
class MemberOccupy;
class House;
class Member;
class System;


int main() {
    cout << "\n\nEEET2482/COSC2082 ASSIGNMENT\n"
         << "Instructor: Mr. Linh Tran"
         << "\tGroup: 9\n"
         << "1.\ts3927469, Ngo Quoc Binh\n"
         << "2.\ts3926822, Hoang Nguyen Anh\n"
         << "3.\ts3926377,Doan Tran Thien Phuc\n"
         << "4.\ts3883025, Tran Gia Hoang Long\n";


    InputStorageManager inputStorageManager;
    System system;

    inputStorageManager.inputStorageFromFileList();
    inputStorageManager.inputStorageLoadDataToSystem(&system);

    system.mainMenu();

//    auto m1 = system.systemMemberList[0];
//    auto h1 = system.systemHouseList[0];
//    auto m2 = system.systemMemberList[1];
//    auto h2 = system.systemHouseList[1];
//
//
////     View m1 info
//    m1->viewMemberInfo();
//    m2->viewMemberInfo();
//
////    system.setCurrentMember(m2);
////    system.memberSearchSuitableHouseList(new Date(4, 6, 2022), new Date(5, 6, 2022), 1);
//
//    m1->viewRequestList();
//
//
//
//    std::cout << "End\n";


//     //Init the member
//    Member m1(1, "binh", "0312", "Binh", "Ngo", "021516");
//    Member m2(2, "long", "0312", "Long", "Tran", "34324");
//    Member m3(3, "ah", "3232", "Anh", "Hoang", "432432");
//    Member m4(4, "pd", "234", "Phuc", "Doan", "432242");
//
//    // Init the house
//    House h1(1, "31", "Lang", "9", 2, "nha dep");
//    House h2(2, "43", "Long", "1", 1, "nha dep");
//    House h3(3, "43", "Long", "41", 0, "nha dep");
//    House h4(4, "43", "Long", "23", 1, "nha dep");
//    House h5(5, "43", "Long", "51", 2, "nha dep");
//    House h6(6, "43", "Long", "61", 2, "nha dep");
//////
////
//    // m1 List house
//    m1.createOwnedHouse(&h1);
//    m2.createOwnedHouse(&h2);
//    m3.createOwnedHouse(&h3);
//    m4.createOwnedHouse(&h4);
//
//    m1.listHouse(new Date(1, 2, 2022), new Date(1, 9, 2022), 20, 0);
////
////    // Set the data for system
////    System system;
////
////    // Add member to systemMemberList
//    for (Member *member: {&m1, &m2, &m3, &m4}) {
//        system.addMember(member);
//    }
//
//    // Add house to systemHouseList
//    for (House *house: {&h1, &h2, &h3, &h4, &h5, &h6}) {
//        system.addHouse(house);
//    }

//
//    // Set current member
//    system.setCurrentMember(&m2);
//
//    // Search suitable house list
//    system.memberSearchSuitableHouseList(new Date(3, 2, 2022), new Date(14, 2, 2022), 2);
//
//    // Send request the house list
//    system.memberSendRequest(new Date(3, 2, 2022), new Date(14, 2, 2022), 0);
//
//    // Member m3 request similar to member m2
//    system.setCurrentMember(&m3);
//    system.memberSearchSuitableHouseList(new Date(5, 2, 2022), new Date(16, 2, 2022), 2);
//    system.memberSendRequest(new Date(5, 2, 2022), new Date(16, 2, 2022), 0);
//
//    // Member m4 request similar to member m2
//    system.setCurrentMember(&m4);
//    system.memberSearchSuitableHouseList(new Date(5, 3, 2022), new Date(16, 3, 2022), 2);
//    system.memberSendRequest(new Date(5, 3, 2022), new Date(16, 3, 2022), 0);
//
//    // Member m5 request another very different date from m4 and m2
//    system.setCurrentMember(&m4);
//    system.memberSearchSuitableHouseList(new Date(1, 5, 2022), new Date(21, 5, 2022), 2);
//    system.memberSendRequest(new Date(1, 5, 2022), new Date(21, 5, 2022), 0);
//
//    // Change to the member m1 for viewing request
//    system.setCurrentMember(&m1);
////    system.memberViewHouseRequestList();
//
//    // Accept the request and see all request after accept
//    system.memberAcceptRequest(1);
//    system.memberAcceptRequest(0);
////    system.memberViewHouseRequestList();
//
//    // The member m3 leave house
//    system.setCurrentMember(&m3);
//    system.memberViewOccupyList();
//
//    system.leaveHouse(0);
//
//    // CHeck the house rating
//    // system.memberSearchSuitableHouseList(new Date(5, 3, 2022), new Date(16, 3, 2022), 2);
//
//    // The member m2 leave house
//    system.setCurrentMember(&m2);
//    system.memberViewOccupyList();
//    system.leaveHouse(0);
//
//    system.memberSearchSuitableHouseList(new Date(1, 2, 2022), new Date(2, 2, 2022), 2);
//
//    // The member m1 view all unrated occupier
//    system.setCurrentMember(&m1);
//    system.memberViewUnratedOccupierList();
//
//    system.memberRateOccupier(0);
//
//    system.setCurrentMember(&m3);
//    m3.viewMemberInfo();
//
//    OutputStorageManager outputStorageManager(&system);
//    outputStorageManager.outputStorageToFileList();
//
//
//    std::cout << getCurrentDate()->toStr();
//    system.mainMenu();

    return 0;
}



