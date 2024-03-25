//
// Created by ADMIN on 5/3/2022.
//

#include "MemberOccupy.h"

MemberOccupy::MemberOccupy(Date *occupyFromDate, Date *occupyToDate, House *occupyHouse) {
    this->occupyFromDate = occupyFromDate;
    this->occupyToDate = occupyToDate;
    this->occupyHouse = occupyHouse;
}