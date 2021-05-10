#ifndef TrainSystem_train
#define TrainSystem_train

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

class train {
private:
    char trainID[25], stations[110][35], startTime[10], saleDate[10], type ;
    int stationNum, seatNum, prices[110], travelTimes[110], stopoverTimes[110] ;

public:
    train () {}
    train (const char *_trainID, const char **_stations, const char *_startTime, const char *_saleDate, 
    const char _type, int _stationNum, int _seatNum, int *_prices, int *_travelTimes, int *_stopoverTimes) {
        stationNum = _stationNum; seatNum = _seatNum; type = _type ;
        strcpy (trainID, _trainID) ;
        for (int i = 1; i <= stationNum; i ++) strcpy (stations[i], _stations[i]) ;
        strcpy (startTime, _startTime) ;
        strcpy (saleDate, _saleDate) ;
        for (int i = 1; i < stationNum; i ++) prices[i] = _prices[i] ;
        for (int i = 1; i < stationNum; i ++) travelTimes[i] = _travelTimes[i] ;
        for (int i = 1; i <= stationNum - 2; i ++) stopoverTimes[i] = _stopoverTimes[i] ;
    }

    
} ;

#endif