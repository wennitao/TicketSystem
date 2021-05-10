#include <cstdio>
#include <string>
#include "main.h"
#include "CommandHandler.hpp"
#include "Database/B+Tree.hpp"
using namespace std;

BPlusTree users ("users_B+Tree.dat") ;
BPlusTree curUsers ("curUsers.dat") ;

fstream userio ("users.dat", ios::in | ios::out | ios::binary) ;

int main() {
    string op ;
    while (getline (cin, op)) {
        CommandHandler ch (op) ;
        ch.run() ;
    }
    userio.close() ;
    return 0 ;
}