//
// Created by ADMIN on 5/4/2022.
//

#ifndef TEST_OUTPUTSTORAGEMANAGER_H
#define TEST_OUTPUTSTORAGEMANAGER_H

#include <vector>
#include <utility>
#include "../../Object/House/House.h"
#include "../../Object/Member/Member.h"


class Member;
class House;
class System;

class OutputStorageManager {
    std::vector<Member*> outputStorageMemberList;
    std::vector<House*> outputStorageHouseList;

    void outputStorageLoadMemberListFromSystem(System *system);
    void outputStorageLoadHouseListFromSystem(System *system);

    //All output to file function
    void outputMemberListToFile();
    void outputHouseListToFile();
    void outputMemberOwnHouseToFile();
    void outputMemberListHouseToFile();
    void outputMemberRequestHouseToFile();
    void outputMemberOccupyHouseToFile();
    void outputMemberReviewHouseToFile();
    void outputMemberReviewOccupierToFile();
    void outputHouseUnratedOccupierToFile();

public:
    OutputStorageManager();

    void outputStorageToFileList();

    void outputStorageLoadDataFromSystem(System *system);
};




#endif //TEST_OUTPUTSTORAGEMANAGER_H
