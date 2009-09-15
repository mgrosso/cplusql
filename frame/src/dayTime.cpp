#ident "file_id $Id: dayTime.cpp,v 1.2 2004/01/05 20:21:30 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      dayTime.cpp
// Author:    Lance Ball
// Created:   Fri Feb 11 16:43:02 EST 2000 on casper.int.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: dayTime.cpp,v 1.2 2004/01/05 20:21:30 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

//#include <time.h>
#include "dayTime.h"

namespace pk_frame
{
#define SECONDS_IN_A_DAY 86400

    // TODO:  construct a dayTime object for NOW!
    dayTime::dayTime()
    {
    }

    dayTime::dayTime(const day_key_t &day, const time_key_t &time)
    {
        day_key_     = day;
        time_key_    = time;
        julian_time_ = dayTime::toJulian(day, time);
    }

    dayTime::dayTime(const dayTime& other)
    {
        this->day_key_     = other.day_key_;
        this->time_key_    = other.time_key_;
        this->julian_time_ = other.julian_time_;
    }

    dayTime::dayTime(const julian_time_t& jtime)
    {
        this->day_key_     = dayTime::getDayKey(jtime);
        this->time_key_    = dayTime::getTimeKey(jtime);
        this->julian_time_ = jtime;
    }

    dayTime&
    dayTime::operator=(const dayTime& rhs)
    {
        if (this != &rhs)
        {
            this->day_key_     = rhs.day_key_;
            this->time_key_    = rhs.time_key_;
            this->julian_time_ = rhs.julian_time_;
        }
        return *this;
    }

    dayTime
    dayTime::operator+(const dayTime& rhs) const
    {
        julian_time_t result = this->julian_time_+rhs.julian_time_;
        return dayTime(dayTime::getDayKey(result), dayTime::getTimeKey(result));
    }

    dayTime
    dayTime::operator-(const dayTime& rhs) const
    {
        julian_time_t result = this->julian_time_-rhs.julian_time_;
        return dayTime(dayTime::getDayKey(result), dayTime::getTimeKey(result));
    }

    dayTime&
    dayTime::operator+=(const dayTime& rhs) 
    {
        this->julian_time_ += rhs.julian_time_;
        this->day_key_      = dayTime::getDayKey(julian_time_);
        this->time_key_     = dayTime::getTimeKey(julian_time_);
        return *this;
    }

    dayTime&
    dayTime::operator-=(const dayTime& rhs) 
    {
        this->julian_time_ -= rhs.julian_time_;
        this->day_key_      = dayTime::getDayKey(julian_time_);
        this->time_key_     = dayTime::getTimeKey(julian_time_);
        return *this;
    }

    bool
    dayTime::operator>(const dayTime& rhs) const
    {
        return (this->julian_time_ > rhs.julian_time_);
    }

    bool
    dayTime::operator<(const dayTime& rhs) const
    {
        return (this->julian_time_ < rhs.julian_time_);
    }

    bool
    dayTime::operator==(const dayTime& rhs) const
    {
        return (this->julian_time_ == rhs.julian_time_);
    }

    day_key_t
    dayTime::getDayKey(const julian_time_t& julian)
    {
        return julian/SECONDS_IN_A_DAY;
    }

    time_key_t
    dayTime::getTimeKey(const julian_time_t& julian)
    {
        return julian%SECONDS_IN_A_DAY;
    }

    julian_time_t
    dayTime::toJulian(const day_key_t &day, const time_key_t &t)
    {
        return ((day*SECONDS_IN_A_DAY)+t);
    }
}
