#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include "main.h"
#include "CommandHandler.hpp"
#include "Database/B+Tree.hpp"
using namespace std;

BPlusTree users ("users_B+Tree.dat") ;
BPlusTree trains ("trains_B+Tree.dat") ;
BPlusTree trainStations ("trainStations_B+Tree.dat") ;
BPlusTree curUsers ("curUsers.dat") ;
BPlusTree orders ("orders_B+Tree.dat") ;
BPlusTree pendingOrders ("pendingOrders.dat") ;
BPlusTree stationNames ("stationNames_B+Tree.dat") ;

fstream userio, trainio, orderio, stationio ;

void init () {
    fstream in ("users.dat", ios::in | ios::binary) ;
    if (!in.is_open()) {
        fstream out ("users.dat", ios::out | ios::binary) ;
        out.close() ;
        userio.open ("users.dat", ios::in | ios::out | ios::binary) ;
        userio.seekp (0, ios::end) ;
    }
    in.close() ;
    if (!userio.is_open()) userio.open ("users.dat", ios::in | ios::out | ios::binary) ;

    in.open ("trains.dat", ios::in | ios::binary) ;
    if (!in.is_open()) {
        fstream out ("trains.dat", ios::out | ios::binary) ;
        out.close() ;
        trainio.open ("trains.dat", ios::in | ios::out | ios::binary) ;
        trainio.seekp (0, ios::end) ;
    }
    in.close() ;
    if (!trainio.is_open()) trainio.open ("trains.dat", ios::in | ios::out | ios::binary) ;

    in.open ("orders.dat", ios::in | ios::binary) ;
    if (!in.is_open()) {
        fstream out ("orders.dat", ios::out | ios::binary) ;
        out.close() ;
        orderio.open ("orders.dat", ios::in | ios::out | ios::binary) ;
        orderio.seekp (0, ios::end) ;
    }
    in.close() ;
    if (!orderio.is_open()) orderio.open ("orders.dat", ios::in | ios::out | ios::binary) ;

    in.open ("stationNames.dat", ios::in | ios::binary) ;
    if (!in.is_open()) {
        fstream out ("stationNames.dat", ios::out | ios::binary) ;
        out.close() ;
        stationio.open ("stationNames.dat", ios::in | ios::out | ios::binary) ;
        stationio.seekp (0, ios::end) ;
        int x = 0 ;
        stationio.write (reinterpret_cast<char *>(&x), sizeof x) ;
    }
    in.close() ;
    if (!stationio.is_open()) stationio.open ("stationNames.dat", ios::in | ios::out | ios::binary) ;
}

int main() {
    init() ;
    string op ;
    while (getline (cin, op)) {
        CommandHandler ch (op) ;
        ch.run () ;
    }
    userio.close() ;
    trainio.close() ;
    orderio.close() ;
    stationio.close() ;
    curUsers.clear() ;
    return 0 ;
}