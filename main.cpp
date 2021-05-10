#include <bits/stdc++.h>
#include "CommandHandler.hpp"
using namespace std;
int main() {
    string op ;
    while (getline (cin, op)) {
        CommandHandler ch (op) ;
        ch.run() ;
    }
    return 0 ;
}