#ifndef TicketSystem_Time
#define TicketSystem_Time

#include <iostream>
#include <iomanip>

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
    /*Time operator = (const Time &_time) {
        month = _time.month; day = _time.day ;
        h = _time.h; m = _time.m ;
    }*/
    Time operator + (const int min) {
        Time res = *this ;
        res.m += min ;
        res.h += res.m / 60; res.m %= 60 ;
        res.day += res.h / 24; res.h %= 24 ;
        if (res.day > days[res.month]) res.day -= days[res.month], res.month ++ ;
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
        h = _time.h; m = _time.h ;
    }
} ;

ostream& operator << (ostream &out, const Time &_time) {
    out << setw (2) << setfill ('0') << _time.month << '-' << _time.day << ' ' << _time.h << ':' << _time.m ;
    return out ;
}

#endif