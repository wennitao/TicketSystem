#ifndef TrainSystem_Date
#define TrainSystem_Date

#include <iostream>
#include <iomanip>

using namespace std;

class Date {

    friend ostream & operator << (ostream &out, const Date &_date) ;

private:
    int m, d ;

public:
    Date () {}
    Date (const char *_date) {
        m = (_date[0] - '0') * 10 + (_date[1] - '0') ;
        d = (_date[3] - '0') * 10 + (_date[4] - '0') ;
    }
    Date operator = (const Date &_date) {
        m = _date.m; d = _date.d ;
    }
    bool operator <= (const Date &_date) const {
        if (m == _date.m) return d <= _date.d ;
        return m <= _date.m ;
    }
} ;

ostream& operator << (ostream &out, const Date &_date) {
    out << setw (2) << setfill ('0') << _date.m << '-' << _date.d ;
}

#endif