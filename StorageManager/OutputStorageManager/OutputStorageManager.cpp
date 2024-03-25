//
// Created by ADMIN on 5/4/2022.
//

#include <fstream>
#include <sstream>

#include "../Path.h"
#include "../../System/System.h"
#include "../../Object/Member/Member.h"
#include "../../Object/House/House.h"
#include "../../Object/MemberOccupy/MemberOccupy.h"
#include "../../DataType/Date/Date.h"
#include "../../Object/Request/Request.h"
#include "../../Object/Review/Review.h"
#include "../../Object/HouseOccupy/HouseOccupy.h"
#include "OutputStorageManager.h"

OutputStorageManager::OutputStorageManager() {
    this->outputStorageHouseList.clear();
    this->outputStorageMemberList.clear();
}

void OutputStorageManager::outputStorageLoadMemberListFromSystem(System *system) {
    for (auto &member: system->systemMemberList) {
        outputStorageMemberList.push_back(member);
    }
}

void OutputStorageManager::outputStorageLoadHouseListFromSystem(System *system) {
    for (auto &house: system->systemHouseList) {
        outputStorageHouseList.push_back(house);
    }
}



// Output data to Storage/Member.csv
void OutputStorageManager::outputMemberListToFile() {
    std::ofstream os {MEMBER_PATH};
    //check file open
    if (!os) {
        std::cerr << "Cannot open " << MEMBER_PATH << " for output\n";
        return;
    }
    //print to file following the structure in StorageStructure folder
    for (auto &member: outputStorageMemberList) {
        os << member->memberID << ","
           << member->username << ","
           << member->password << ","
           << member->firstName << ","
           << member->lastName << ","
           << member->phoneNumber << ","
           << member->creditPoints << "\n";
    }
    //close file
    os.close();
}

// Output House List to Storage/House.csv
void OutputStorageManager::outputHouseListToFile() {
    std::ofstream os {HOUSE_PATH};

    if (!os) {
        std::cerr << "Cannot open " << HOUSE_PATH << " for output\n";
        return;
    }

    for (auto &house: outputStorageHouseList) {
        os << house->houseID << ","
           << house->houseNumber << ","
           << house->street << ","
           << house->district << ","
           << house->cityID << ","
           << house->houseDescription << "\n";
    }

    os.close();
}

// Output member own house  to Storage/MemberOwnHouse.csv
void OutputStorageManager::outputMemberOwnHouseToFile() {
    std::ofstream os {MEMBER_OWN_HOUSE_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_OWN_HOUSE_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        // Check member has house or not
        if (member->ownedHouse) {
            os << member->memberID << ","
               << member->ownedHouse->houseID << "\n";// bỏ houseID thì chạy vẫn ra địa chỉ, giữ houseID thì mất mẹ hết đ hiểu
        }
    }

    os.close();

}

//Output data to Storage/MemberListHouse.csv
void OutputStorageManager::outputMemberListHouseToFile() {
    std::ofstream os {MEMBER_LIST_HOUSE_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_LIST_HOUSE_PATH << " for output\n";
        return;
    }

    // Loop through the list of member
    for (auto &member: outputStorageMemberList) {
        // If member does not have a house
        if (!member->ownedHouse) {
            continue;
        }

        // If the house is not listed
        House* house = member->ownedHouse;

        if (!house->isListed) {
            continue;
        }

        // Output the info for listing house
        os << house->availableFromDate->toStr() << ","
           << house->availableToDate->toStr() << ","
           << house->consumingPointsPerDay << ","
           << house->minOccupierRating << ","
           << member->memberID << ","
           << house->houseID << "\n";
    }

    os.close();

}

//Output member request to Storage/MemberRequestHouse.csv
void OutputStorageManager::outputMemberRequestHouseToFile() {
    std::ofstream os {MEMBER_REQUEST_HOUSE_PATH};

    if (!os) {
        std::cerr << "Cannot open " << MEMBER_REQUEST_HOUSE_PATH << " for output\n";
        return;
    }

    for (auto &house: outputStorageHouseList) {
        for (auto &memberRequest: house->houseRequestList) {
            os << memberRequest->requestFromDate->toStr() << ","
               << memberRequest->requestToDate->toStr() << ","
               << memberRequest->requestedByMember->memberID << ","
               << house->houseID << "\n";
        }
    }

    os.close();
}

//Output data member occupy house to Storage/MemberOccupyHouse.csv
void OutputStorageManager::outputMemberOccupyHouseToFile() {
    std::ofstream os {MEMBER_OCCUPY_HOUSE_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OCCUPY_HOUSE_PATH << " for output\n";
        return;
    }

    for (auto member: outputStorageMemberList) {
        for (auto memberOccupy: member->memberOccupyList) {
            os << memberOccupy->occupyFromDate->toStr() << ","
               << memberOccupy->occupyToDate->toStr() << ","
               << member->memberID << ","
               << memberOccupy->occupyHouse->houseID << "\n";
        }
    }

    os.close();
}

//Output member review the house to Storage/MemberReviewHouse.csv
void OutputStorageManager::outputMemberReviewHouseToFile() {
    std::ofstream os {MEMBER_REVIEW_HOUSE_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_HOUSE_PATH << " for output\n";
        return;
    }

    for (auto &house: outputStorageHouseList) {
        for (auto &memberReview: house->houseReviewList) {
            std::stringstream ss;
            ss << memberReview->ratingScore << ","
               << memberReview->comment <<  ","
              << memberReview->reviewedByMember->memberID << ","
               << house->houseID <<"\n";
            std::string line = ss.str();
            os << line;
        }
    }

    os.close();
}

//Output HouseOwner review Occupier to data file /Storage/MemberReviewOccupier.csv
void OutputStorageManager::outputMemberReviewOccupierToFile()
{
    std::ofstream os {MEMBER_REVIEW_OCCUPIER_PATH};

    if (!os.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_OCCUPIER_PATH << " for output\n";
        return;
    }

    for (auto &member: outputStorageMemberList) {
        for (auto &memberReview: member->memberReviewList) {
            std::stringstream ss;
            ss << memberReview->ratingScore << "," //score rating
               << memberReview->comment << ","     //comment
               << memberReview->reviewedByMember->memberID << "," //memberID
               << member->memberID << "\n";          //occupierID
            std::string line = ss.str();
            os << line;
        }
    }

    os.close();
}

//Output occupier id and house info that unrated the house after rent. /Storage/HouseUnratedOccupier.csv
void OutputStorageManager::outputHouseUnratedOccupierToFile()
{
    std::ofstream os {HOUSE_UNRATED_OCCUPIER_PATH};

    for (auto &house: outputStorageHouseList) {
        for (auto &unratedOccupier: house->unratedOccupierList) {
            os << unratedOccupier->occupyFromDate->toStr() << "," //scorerating
            << unratedOccupier->occupyToDate->toStr() << ","     //comment
            << house->houseID << ","           //houseID
            << unratedOccupier->occupiedByMember->memberID <<"\n"; //occupierID
        }
    }

    os.close();
}

void OutputStorageManager::outputStorageLoadDataFromSystem(System *system) {
    outputStorageLoadMemberListFromSystem(system);
    outputStorageLoadHouseListFromSystem(system);
}


void OutputStorageManager::outputStorageToFileList() {
    outputMemberListToFile();
    outputHouseListToFile();
    outputMemberOwnHouseToFile();
    outputMemberListHouseToFile();
    outputMemberRequestHouseToFile();
    outputMemberOccupyHouseToFile();
    outputMemberReviewHouseToFile();
    outputMemberReviewOccupierToFile();
    outputHouseUnratedOccupierToFile();
}


