#include <cstdio>
#include <string>
#include "main.h"
#include "CommandHandler.hpp"
#include "Database/B+Tree.hpp"
using namespace std;

BPlusTree users ("users_B+Tree.dat") ;
BPlusTree curUsers ("curUsers.dat") ;

fstream userio ;

void init () {
    fstream in ("users.dat", ios::in | ios::binary) ;
    if (!in.is_open()) {
        fstream out ("users.dat", ios::out | ios::binary) ;
        out.close() ;
        userio.open ("users.dat", ios::in | ios::out | ios::binary) ;
        userio.seekp (0, ios::end) ;
    }
}

int main() {
    init() ;
    string op ;
    while (getline (cin, op)) {
        CommandHandler ch (op) ;
        try {
            ch.run() ;
            printf("0\n") ;
        } catch (...) {
            printf("-1\n") ;
        }
    }
    userio.close() ;
    return 0 ;
}