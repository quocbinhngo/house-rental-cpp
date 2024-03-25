//
// Created by ADMIN on 5/3/2022.
//

#include "HouseOccupy.h"

HouseOccupy::HouseOccupy(Date *occupyFromDate, Date *occupyToDate, Member *occupiedByMember) {
    this->occupyFromDate = occupyFromDate;
    this->occupyToDate = occupyToDate;
    this->occupiedByMember = occupiedByMember;
}


