#include <cstdio>
#include <string>
#include "main.h"
#include "CommandHandler.hpp"
#include "Database/B+Tree.hpp"
using namespace std;

BPlusTree curUsers ("curUsers.dat") ;

int main() {
    string op ;
    while (getline (cin, op)) {
        CommandHandler ch (op) ;
        ch.run() ;
    }
    return 0 ;
}