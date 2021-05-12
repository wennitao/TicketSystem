#include <cstdio>
#include <string>
#include "main.h"
#include "CommandHandler.hpp"
#include "Database/B+Tree.hpp"
using namespace std;

BPlusTree users ("users_B+Tree.dat") ;
BPlusTree trains ("trains_B+Tree.dat") ;
BPlusTree trainStations ("trainStations_B+Tree.dat") ;
BPlusTree curUsers ("curUsers.dat") ;

fstream userio, trainio ;

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
    system ("rm -rf curUsers.dat") ;
    return 0 ;
}