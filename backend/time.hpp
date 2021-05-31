#ifndef TicketSystem_Time
#define TicketSystem_Time

#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

int days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} ;

class Time {
    
    friend ostream &operator << (ostream &out, const Time &_time) ;

private:
    int month, day, h, m ;

public:
    Time () {}
    Time (const char *_date, const char *_time) {
        month = (_date[0] - '0') * 10 + (_date[1] - '0') ;
        day = (_date[3] - '0') * 10 + (_date[4] - '0') ;
        h = (_time[0] - '0') * 10 + (_time[1] - '0') ;
        m = (_time[3] - '0') * 10 + (_time[4] - '0') ;
    }
    Time (const char *_date, pair<int, int> dayTime) {
        month = (_date[0] - '0') * 10 + (_date[1] - '0') ;
        day = (_date[3] - '0') * 10 + (_date[4] - '0') ;
        h = dayTime.first; m = dayTime.second ;
    }
    /*Time operator = (const Time &_time) {
        month = _time.month; day = _time.day ;
        h = _time.h; m = _time.m ;
    }*/
    Time operator + (const int min) {
        Time res = *this ;
        res.m += min ;
        res.h += res.m / 60; res.m %= 60 ;
        res.day += res.h / 24; res.h %= 24 ;
        while (res.day > days[res.month] && res.month < 12) {
            res.day -= days[res.month]; res.month ++ ;
        }
        if (res.day > days[res.month]) return Time ("12-31", "23:59") ;
        //if (res.day > days[res.month]) res.day -= days[res.month], res.month ++ ;
        return res ;
    }
    Time operator - (const int min) {
        Time res = *this ;
        res.m -= min ;
        res.h += res.m / 60; res.m %= 60 ;
        if (res.m < 0) res.m += 60, res.h -- ;
        res.day += res.h / 24; res.h %= 24 ;
        if (res.h < 0) res.h += 24, res.day -- ;
        while (res.day <= 0 && res.month > 1) {
            res.month --; res.day += days[res.month] ;
        }
        if (res.day <= 0) return Time ("01-01", "00:00") ;
        //if (res.day <= 0) res.month --, res.day += days[res.month] ;
        return res ;
    }
    int operator - (const Time &_time) const {
        int res = day - _time.day ;
        for (int i = _time.month; i < month; i ++) res += days[i] ;
        return res ;
    }
    bool operator <= (const Time &_time) const {
        if (month == _time.month && day == _time.day && h == _time.h) return m <= _time.m ;
        if (month == _time.month && day == _time.day) return h <= _time.h ;
        if (month == _time.month) return day <= _time.day ;
        return month <= _time.month ;
    }
    void setTime (const Time &_time) {
        h = _time.h; m = _time.m ;
    }
    pair<int, int> getDayTime () const {
        return make_pair (h, m) ;
    }

    int calTimeInterval (const Time &_time) {
        int day_interval = day - _time.day ;
        for (int i = _time.month; i < month; i ++) day_interval += days[i] ;
        int min = m - _time.m ;
        min += (h - _time.h) * 60 ;
        min += 24 * 60 * day_interval ;
        return min ;
    }

    const char* getDateString () const {
        static char date[10] = {0};
        date[0] = month / 10 + '0' ;
        date[1] = month % 10 + '0' ;
        date[2] = '-' ;
        date[3] = day / 10 + '0' ;
        date[4] = day % 10 + '0' ;
        date[5] = 0 ;
        return date ;
    }
} ;

ostream& operator << (ostream &out, const Time &_time) {
    out << setw (2) << setfill ('0') << _time.month << '-' << setw (2) << setfill ('0') << _time.day << ' ' << setw (2) << setfill ('0') << _time.h << ':' << setw (2) << setfill ('0') << _time.m ;
    return out ;
}

#endif