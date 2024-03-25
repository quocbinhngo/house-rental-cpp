//
// Created by ADMIN on 5/3/2022.
//

#include "Request.h"

Request::Request(Date *requestFromDate, Date *requestToDate, Member *requestedByMember) {
    this->requestFromDate = requestFromDate;
    this->requestToDate = requestToDate;
    this->requestedByMember = requestedByMember;
}