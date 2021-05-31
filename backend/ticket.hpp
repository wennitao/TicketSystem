#ifndef TrainSystem_Ticket
#define TrainSystem_Ticket

#include <iostream>
#include <cstdio>
#include <cstring>

#include "time.hpp"

using namespace std;

enum Status {success, pending, refunded} ;
char status_str[3][15] = {"[success]", "[pending]", "[refunded]"} ;

class ticket {

    friend ostream& operator << (ostream &out, const ticket &_ticket) ;

private:
    char trainID[110], from[110], to[110] ;
    Time leaving_time, arriving_time ;
    long long price ;
    int seat_num, travellingTime ;
    Status status ;

public:
    ticket () {
        memset (trainID, 0, sizeof trainID) ;
        memset (from, 0, sizeof from) ;
        memset (to, 0, sizeof to) ;
    }
    ticket (const char *_trainID, const char *_from, const char *_to, const Time &_leaving_time, const Time &_arriving_time, const long long _price, const int _seat_num, const int _travellingTime, const Status _status) {
        memset (trainID, 0, sizeof trainID) ;
        memset (from, 0, sizeof from) ;
        memset (to, 0, sizeof to) ;
        strcpy (trainID, _trainID) ;
        strcpy (from, _from) ;
        strcpy (to, _to) ;
        leaving_time = _leaving_time; arriving_time = _arriving_time ;
        price = _price; seat_num = _seat_num; travellingTime = _travellingTime ;
        status = _status ;
    }

    void print () {
        cout << trainID << " " << from << " " << leaving_time << " -> " << to << " " << arriving_time << " " << price << " " << seat_num << endl ; 
    }
    
    Time getLeavingTime () const {
        return leaving_time ;
    }

    int getTravellingTime () const {
        return travellingTime ;
    }

    const char* getTrainID () const {
        return trainID ;
    }

    const char* getFromStation() const {
        return from ;
    }

    const char* getToStation() const {
        return to ;
    }

    int getPrice() const {
        return price ;
    }

    int getSeatNum () const {
        return seat_num ;
    }

    Status getStatus () const {
        return status ;
    }

    void setStatus (Status st) {
        status = st ;
    }

} ;

ostream& operator << (ostream &out, const ticket &_ticket) {
    out << status_str[_ticket.status] << " " << _ticket.trainID << " " << _ticket.from << " " << _ticket.leaving_time << " -> " << _ticket.to << " " << _ticket.arriving_time << " " << _ticket.price << " " << _ticket.seat_num ; 
    return out ;
}

#endif