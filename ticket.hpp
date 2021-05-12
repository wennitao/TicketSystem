#ifndef TrainSystem_Ticket
#define TrainSystem_Ticket

#include <iostream>
#include <cstdio>
#include <cstring>

#include "time.hpp"

using namespace std;

enum Status {success, pending, refunded} ;

class ticket {

private:
    char trainID[25], from[35], to[35] ;
    Time leaving_time, arriving_time ;
    int price, seat ;
    Status status ;

public:
    ticket () {}
    ticket (const char *_trainID, const char *_from, const char *_to, const Time &_leaving_time, const Time &_arriving_time, const int price, const int seat, const Status _status) {
        strcpy (trainID, _trainID) ;
        strcpy (from, _from) ;
        strcpy (to, _to) ;
        
    }

} ;



#endif