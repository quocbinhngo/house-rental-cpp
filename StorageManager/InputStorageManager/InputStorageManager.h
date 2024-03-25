//
// Created by ADMIN on 5/4/2022.
//

#ifndef TEST_INPUTSTORAGEMANAGER_H
#define TEST_INPUTSTORAGEMANAGER_H

#include <map>
#include <utility>
#include <vector>

class Member;
class House;
class System;
class Admin;

class InputStorageManager {
    std::map<int, Member*> inputStorageMemberList;
    std::map<int, House*> inputStorageHouseList;
    std::vector<Admin*> inputStorageAdminList;

    void inputStorageLoadHouseListToSystem(System *system);
    void inputStorageLoadMemberListToSystem(System *system);
    void inputStorageLoadAdminListToSystem(System *system);

    void inputMemberListFromFile();
    void inputHouseListFromFile();
    void inputAdminListFromFile();

    void inputMemberOwnHouseFromFile();
    void inputMemberListHouseFromFile();
    void inputMemberRequestHouseFromFile();
    void inputMemberOccupyHouseFromFile();
    void inputMemberReviewHouseFromFile();
    void inputMemberReviewOccupierFromFile();
    void inputHouseUnratedOccupierFromFile();

public:
    InputStorageManager();

    void inputStorageFromFileList();

    void inputStorageLoadDataToSystem(System *system);
};


#endif //TEST_INPUTSTORAGEMANAGER_H