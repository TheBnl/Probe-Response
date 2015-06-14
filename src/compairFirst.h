//
//  compairFirst.h
//  probeResponseNew
//
//  Created by Bram de Leeuw on 13-06-15.
//
//

#ifndef _PR_COMPAIR_FIRST
#define _PR_COMPAIR_FIRST

#include "prVictim.h"

/**
 * Compair function
 * compairs a given value with the first item of a pair
 */
struct CompareFirst
{
    CompareFirst(string val) : val_(val) {}
    bool operator()(const std::pair<string,string>& elem) const {
        return val_ == elem.first;
    }
private:
    string val_;
};


/**
 * Compair function
 * compairs a given value with the first item of a pair
 */
struct VictimExists
{
    VictimExists(string val) : val_(val) {}
    bool operator()(const std::pair<string,prVictim>& elem) const {
        return val_ == elem.first;
    }
private:
    string val_;
};

#endif
