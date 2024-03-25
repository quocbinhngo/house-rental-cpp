//
// Created by ADMIN on 5/3/2022.
//

#include "Review.h"

Review::Review(int ratingScore, std::string comment, Member *reviewedByMember) {
    this->ratingScore = ratingScore;
    this->comment = std::move(comment);
    this->reviewedByMember = reviewedByMember;
}