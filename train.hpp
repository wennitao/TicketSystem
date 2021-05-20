#ifndef TrainSystem_train
#define TrainSystem_train

#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>

#include "time.hpp"

using namespace std;

class train {
private:
    Time saleDate[3], startTime ;
    char trainID[25], stations[110][35], type ;
    int stationNum, seatNum, prices[110], travelTimes[110], stopoverTimes[110], seat[110][110] ;
    bool released = 0 ;

public:
    train () {}
    train (const char *_trainID, const char _stations[110][30], const Time _startTime, const Time _saleDate[3], 
    const char _type, int _stationNum, int _seatNum, int *_prices, int *_travelTimes, int *_stopoverTimes) {
        stationNum = _stationNum; seatNum = _seatNum; type = _type ;
        strcpy (trainID, _trainID) ;
        for (int i = 1; i <= stationNum; i ++) strcpy (stations[i], _stations[i]) ;
        startTime = _startTime ;
        saleDate[1] = _saleDate[1]; saleDate[2] = _saleDate[2] ;
        for (int i = 1; i < stationNum; i ++) prices[i] = _prices[i] ;
        for (int i = 1; i < stationNum; i ++) travelTimes[i] = _travelTimes[i] ;
        stopoverTimes[1] = 0 ;
        for (int i = 2; i <= stationNum - 1; i ++) stopoverTimes[i] = _stopoverTimes[i - 1] ;
        released = 0 ;
        int days = _saleDate[2] - _saleDate[1] ;
        for (int i = 0; i <= days; i ++)
            for (int j = 1; j <= stationNum; j ++)
                seat[i][j] = seatNum ;
    }

    bool getReleaseStatus () const {
        return released ;
    }

    void release () {
        if (released) throw "already released" ;
        released = 1 ;
    }

    const char* getTrainID () const {
        return trainID ;
    }

    int getStationNum () const {
        return stationNum ;
    }

    const char* getStationName (int id) const {
        return stations[id] ;
    }

    bool runningOnDate (const char *_date) {
        return saleDate[1] <= Time (_date, "23:59") && Time (_date, "00:00") <= saleDate[2] ;
    }

    bool runningOnDate (const char *_date, const char *stationName) {
        Time start_time = getStartTime (_date, stationName) ;
        return saleDate[1] <= start_time && start_time <= saleDate[2] ;
    }

    bool runningFromTo (const char *from, const char *to) {
        bool flag1 = 0, flag2 = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], from) == 0) flag1 = 1 ;
            if (strcmp (stations[i], to) == 0) flag2 = 1 ;
            if (flag2 > flag1) return 0 ;
        }
        return flag1 && flag2 ;
    }

    void print (const char *date) {
        cout << trainID << " " << type << endl ;
        Time cur_time = Time (date, "00:00") ;
        cur_time.setTime (startTime) ;
        int cur_price = 0 ;
        int days = cur_time - saleDate[1] ;
        for (int i = 1; i <= stationNum; i ++) {
            cout << stations[i] << " " ;
            if (i == 1) {
                cout << "xx-xx xx:xx -> " << cur_time << " 0" << " " << seat[days][i] << endl ;
                cur_time = cur_time + travelTimes[i] ; 
                cur_price += prices[i] ;
            } else if (i == stationNum) {
                cout << cur_time << " -> xx-xx xx:xx " << cur_price << " x" << endl ;
            } else {
                cout << cur_time << " -> " ;
                cur_time = cur_time + stopoverTimes[i] ;
                cout << cur_time << " " << cur_price << " " << seat[days][i] << endl ;
                cur_time = cur_time + travelTimes[i] ;
                cur_price += prices[i] ;
            }
        }
    }

    Time getStartTime (const char *date, const char *station) {
        Time tmp = saleDate[1] ;
        tmp.setTime (startTime) ;
        int min = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], station) == 0) break ;
            tmp = tmp + travelTimes[i] + stopoverTimes[i] ;
            min = min + travelTimes[i] + stopoverTimes[i] ;
        }
        Time res = Time (date, tmp.getDayTime()) ;
        res = res - min ;
        return res ;
    }

    Time leavingTime (const Time &trainStartTime, const char *station) {
        Time res = trainStartTime ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], station) == 0) return res + stopoverTimes[i] ;
            res = res + travelTimes[i] + stopoverTimes[i] ;
        }
        throw "station not found" ;
    }

    Time arrivingTime (const Time &trainStartTime, const char *station) {
        Time res = trainStartTime ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], station) == 0) return res ;
            res = res + travelTimes[i] + stopoverTimes[i] ;
        }
        throw "station not found" ;
    }

    int getTravellingTime (const char *fromStation, const char *toStation) {
        int travellingTime = 0 ;
        bool flag = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], toStation) == 0) break ;
            if (strcmp (stations[i], fromStation) == 0) flag = 1 ;
            if (flag) travellingTime += travelTimes[i] ;
        }
        return travellingTime ;
    }

    int calPrice (const char *from, const char *to) {
        int price = 0; bool flag = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], to) == 0) return price ;
            if (strcmp (stations[i], from) == 0) flag = 1 ;
            if (flag) price += prices[i] ;
        }
        throw "station not found" ;
    }

    int calSeatNum (const Time &_date, const char *from, const char *to) {
        int days = _date - saleDate[1] ;
        int seatNum = 1e9; bool flag = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], to) == 0) return seatNum ;
            if (strcmp (stations[i], from) == 0) flag = 1 ;
            if (flag) seatNum = min (seatNum, seat[days][i]) ;
        }
        throw "station not found" ;
    }

    void sellSeats (const Time &_date, const char *from, const char *to, int sellSeatNum) {
        int days = _date - saleDate[1] ;
        bool flag = 0 ; 
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], from) == 0) flag = 1 ;
            if (strcmp (stations[i], to) == 0) break ;
            if (flag) seat[days][i] -= sellSeatNum ;
        }
    }

    void addSeats (const Time &_date, const char *from, const char *to, int sellSeatNum) {
        int days = _date - saleDate[1] ;
        bool flag = 0 ; 
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], from) == 0) flag = 1 ;
            if (strcmp (stations[i], to) == 0) break ;
            if (flag) seat[days][i] += sellSeatNum ;
        }
    }
} ;

#endif