// ----------------------------------------------------------------------
//
// File:      dayTime.h
// Author:    Lance Ball
// Created:   Tue Feb  8 11:37:45 EST 2000 on casper.int.primaryknowledge.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2000 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: dayTime.h,v 1.1 2000/03/28 18:51:12 lball Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DAYTIME_H
#define DAYTIME_H 1

namespace pk_frame
{
    typedef long julian_time_t;
    typedef int  day_key_t;
    typedef int  time_key_t;


    class dayTime 
    {
        public:
        dayTime(const dayTime & );
        dayTime(const julian_time_t & );
        dayTime(const day_key_t &, const time_key_t& );
        dayTime& operator=(const dayTime & );

        dayTime  operator+(const dayTime & ) const;
        dayTime  operator-(const dayTime & ) const;

        dayTime& operator+=(const dayTime & );
        dayTime& operator-=(const dayTime & );

        bool operator>(const dayTime & ) const;
        bool operator<(const dayTime & ) const;
        bool operator==(const dayTime & ) const;

        julian_time_t julianValue() const { return julian_time_; }
        time_key_t timeKeyValue() const { return time_key_; }
        day_key_t dayKeyValue() const { return day_key_; }

        static julian_time_t toJulian(const day_key_t &, const time_key_t &);
        static time_key_t getTimeKey(const julian_time_t &);
        static day_key_t getDayKey(const julian_time_t &);

        private:    
        // The default constructs a dayTime object for NOW
        // TODO implement this!
        dayTime(); 
        julian_time_t julian_time_;
        time_key_t time_key_;
        day_key_t day_key_;
    };
}

#endif /* DAYTIME_H */

