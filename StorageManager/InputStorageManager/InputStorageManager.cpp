#include <fstream>
#include <sstream>

#include "../Path.h"
#include "../../System/System.h"
#include "../../Object/Admin/Admin.h"
#include "../../Object/Member/Member.h"
#include "../../Object/House/House.h"
#include "../../Object/MemberOccupy/MemberOccupy.h"
#include "../../DataType/Date/Date.h"
#include "../../Object/Request/Request.h"
#include "../../Object/Review/Review.h"
#include "../../Object/HouseOccupy/HouseOccupy.h"
#include "../../Helper/Helper.h"
#include "InputStorageManager.h"


InputStorageManager::InputStorageManager() {
    // Clear all data from two lists
    inputStorageMemberList.clear();
    inputStorageHouseList.clear();
}

void InputStorageManager::inputStorageLoadHouseListToSystem(System *system) {
    // Push the house object to the system's house list
    for (auto &house: inputStorageHouseList) {
        system->systemHouseList.push_back(house.second);
    }
}

void InputStorageManager::inputStorageLoadMemberListToSystem(System *system) {
    system->systemMemberList.clear();

    // Add member to the system's member list
    for (auto &member: inputStorageMemberList) {
        system->addMember(member.second);
    }
}

void InputStorageManager::inputStorageLoadAdminListToSystem(System *system) {
    system->systemAdminList.clear();

    // Add admin to the system's admin list
    for (auto &admin: inputStorageAdminList) {
        system->addAdmin(admin);
    }
}

void InputStorageManager::inputMemberListFromFile() {
    std::ifstream is {MEMBER_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {

        // ss1 for splitting comma, ss2 for storage data
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        int memberID = convertStringToInt(wordList[0]);
        std::string username = wordList[1];
        std::string password = wordList[2];
        std::string firstName = wordList[3];
        std::string lastName = wordList[4];
        std::string phoneNumber = wordList[5];
        int creditPoints = convertStringToInt(wordList[6]);

        // Declare the member account
        auto *member = new Member(memberID, username, password, firstName, lastName, phoneNumber, creditPoints);
        inputStorageMemberList[memberID] = member;
    }
}

void InputStorageManager::inputHouseListFromFile() {
    std::ifstream is {HOUSE_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << HOUSE_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {

//        std::cout << line << "\n";
        // ss1 for splitting comma
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        int houseID = convertStringToInt(wordList[0]);
        std::string houseNumber = wordList[1];
        std::string street = wordList[2];
        std::string district = wordList[3];
        int cityID = convertStringToInt(wordList[4]);
        std::string houseDescription = wordList[5];

        // Declare the member account
        auto *house = new House(houseID, houseNumber, street, district, cityID, houseDescription);
        inputStorageHouseList[houseID] = house;
    }
}

void InputStorageManager::inputMemberOwnHouseFromFile() {
    std::ifstream is {MEMBER_OWN_HOUSE_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OWN_HOUSE_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {

//        std::cout << line << "\n";
        // ss1 for splitting comma
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        int memberID = convertStringToInt(wordList[0]);
        int houseID = convertStringToInt(wordList[1]);

        auto member = inputStorageMemberList[memberID];
        auto house = inputStorageHouseList[houseID];

        member->createOwnedHouse(house);
    }
}

void InputStorageManager::inputAdminListFromFile() {
    std::ifstream is {ADMIN_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << ADMIN_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {

//        std::cout << line << "\n";
        // ss1 for splitting comma
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        // Declare all variable
        std::string username = wordList[0];
        std::string password = wordList[1];

        auto *admin = new Admin(username, password);
        inputStorageAdminList.push_back(admin);
    }
}

void InputStorageManager::inputMemberListHouseFromFile() {
    std::ifstream is {MEMBER_LIST_HOUSE_PATH};

    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OWN_HOUSE_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1 {line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        Date *availableFromDate = convertStringToDate(wordList[0]);
        Date *availableToDate = convertStringToDate(wordList[1]);
        int consumingPointsPerDay = convertStringToInt(wordList[2]);
        double minOccupierRatingScore = convertStringToDouble(wordList[3]);
        int ownerMemberID = convertStringToInt(wordList[4]);
//        int houseID = convertStringToInt(wordList[5]);

        // Get the owner and their house
        auto ownerMember = inputStorageMemberList[ownerMemberID];
//        auto house = inputStorageHouseList[houseID];

        // List the house
        ownerMember->listHouse(availableFromDate, availableToDate, consumingPointsPerDay, minOccupierRatingScore);
    }
}

void InputStorageManager::inputMemberRequestHouseFromFile()
{
    std::ifstream is {MEMBER_REQUEST_HOUSE_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REQUEST_HOUSE_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        Date *requestFromDate = convertStringToDate(wordList[0]);
        Date *requestToDate = convertStringToDate(wordList[1]);
        int memberID = convertStringToInt(wordList[2]);
        int houseID = convertStringToInt(wordList[3]);

        //get member
        auto member = inputStorageMemberList[memberID];
        auto house = inputStorageHouseList[houseID];

        //store data to request

        auto *request = new Request(requestFromDate, requestToDate, member);
        member->memberRequestList.push_back(request);
        house->houseRequestList.push_back(request);
    }
}

void InputStorageManager::inputMemberOccupyHouseFromFile() {
    std::ifstream is{MEMBER_OCCUPY_HOUSE_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_OCCUPY_HOUSE_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        Date *occupyFromDate = convertStringToDate(wordList[0]);
        Date *occupyToDate = convertStringToDate(wordList[1]);
        int memberID = convertStringToInt(wordList[2]);
        int houseID = convertStringToInt(wordList[3]);

        //get member and house
        auto member = inputStorageMemberList[memberID];
        auto house = inputStorageHouseList[houseID];

        //store data to request
        auto *memberOccupy = new MemberOccupy(occupyFromDate, occupyToDate, house);
        member->memberOccupyList.push_back(memberOccupy);

        auto *houseOccupy = new HouseOccupy(occupyFromDate, occupyToDate, member);
        house->houseOccupyList.push_back(houseOccupy);
    }
}

void InputStorageManager::inputMemberReviewHouseFromFile()
{
    std::ifstream is{MEMBER_REVIEW_HOUSE_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_HOUSE_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};

        if (line.empty()) {
            return;
        }

        std::string word;
        std::vector<std::string> wordList;

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        int scoreRating = convertStringToInt(wordList[0]);
        std::string comment = wordList[1];
        int memberID = convertStringToInt(wordList[2]);
        int houseID = convertStringToInt(wordList[3]);

        //get member and house
        auto member = inputStorageMemberList[memberID];
        auto house = inputStorageHouseList[houseID];

        auto *houseReview = new Review(scoreRating, comment, member);
        house->houseReviewList.push_back(houseReview);

    }

}

void InputStorageManager::inputMemberReviewOccupierFromFile() {
    std::ifstream is{MEMBER_REVIEW_OCCUPIER_PATH};

    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << MEMBER_REVIEW_OCCUPIER_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        int scoreRating = convertStringToInt(wordList[0]);
        std::string comment = wordList[1];
        int memberID = convertStringToInt(wordList[2]);
        int occupierID = convertStringToInt(wordList[3]);

        //get member and house
        auto occupier = inputStorageMemberList[occupierID];
        auto owner = inputStorageMemberList[memberID];

        auto* reviewOccupier = new Review (scoreRating, comment, owner);
        occupier->memberReviewList.push_back(reviewOccupier);
    }
}

void InputStorageManager::inputHouseUnratedOccupierFromFile()
{
    std::ifstream is {HOUSE_UNRATED_OCCUPIER_PATH};
    //check open file
    if (!is.is_open()) {
        std::cerr << "Cannot open " << HOUSE_UNRATED_OCCUPIER_PATH << " for input\n";
        return;
    }

    // Each line of data
    std::string line;

    // Loop through every line
    while (std::getline(is, line)) {
        std::stringstream ss1{line};
        std::string word;
        std::vector<std::string> wordList;

        if (line.empty()) {
            return;
        }

        while (std::getline(ss1, word, ',')) {
            wordList.push_back(word);
        }

        Date *occupyFromDate = convertStringToDate(wordList[0]);
        Date *occupyToDate = convertStringToDate(wordList[1]);

        int houseID = convertStringToInt(wordList[2]);
        int occupierID = convertStringToInt(wordList[3]);


        auto member = inputStorageMemberList[occupierID];
        auto house = inputStorageHouseList[houseID];

        auto *unratedOccupier = new HouseOccupy(occupyFromDate, occupyToDate,member);
        house->unratedOccupierList.push_back(unratedOccupier);
    }
}

void InputStorageManager::inputStorageFromFileList() {
    inputMemberListFromFile();
    inputHouseListFromFile();
    inputAdminListFromFile();
    inputMemberOwnHouseFromFile();
    inputMemberListHouseFromFile();
    inputMemberRequestHouseFromFile();
    inputMemberOccupyHouseFromFile();
    inputMemberReviewHouseFromFile();
    inputMemberReviewOccupierFromFile();
    inputHouseUnratedOccupierFromFile();
}

void InputStorageManager::inputStorageLoadDataToSystem(System *system) {
    inputStorageLoadMemberListToSystem(system);
    inputStorageLoadHouseListToSystem(system);
    inputStorageLoadAdminListToSystem(system);
}


