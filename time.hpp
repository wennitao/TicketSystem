#ifndef TicketSystem_Time
#define TicketSystem_Time

#include <iostream>
#include <iomanip>

using namespace std;

class Time {
    
    friend ostream &operator << (ostream &out, const Time &_time) ;

private:
    int h, m ;

public:
    Time () {}
    Time (const char *_time) {
        h = (_time[0] - '0') * 10 + (_time[1] - '0') ;
        m = (_time[3] - '0') * 10 + (_time[4] - '0') ;
    }
    Time operator = (const Time &_time) {
        h = _time.h; m = _time.m ;
    }
    Time operator + (const int min) {
        m += min ;
        h += m / 60; m %= 60 ;
    }
} ;

ostream& operator << (ostream &out, const Time &_time) {
    out << setw (2) << setfill ('0') << _time.h << ':' << _time.m ;
}

#endif