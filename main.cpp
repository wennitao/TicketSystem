#include <iostream>
#include <fstream>
#include <cstring>

#include "main.h"
#include "CommandHandler.hpp"

Database users ("users_B+Tree.dat") ;
Database curUsers ("curUsers.dat") ;

std::fstream userio ;

void init () {
    std::fstream in ("users.dat", std::ios::in | std::ios::binary) ;
    if (!in.is_open()) {
        std::fstream out ("users.dat", std::ios::out | std::ios::binary) ;
        out.close() ;
        userio.open ("users.dat", std::ios::in | std::ios::out | std::ios::binary) ;
        userio.seekp (0, std::ios::end) ;
    }
    in.close() ;
    if (!userio.is_open()) userio.open ("users.dat", std::ios::in | std::ios::out | std::ios::binary) ;
}

int main() {
    init () ;
    std::string op ;
    while (getline (std::cin, op)) {
        if (op == std::string ("exit")) {
            printf("bye\n"); break ;
        }
        CommandHandler cmd = CommandHandler (op) ;
        cmd.run () ;
    }

    userio.close() ;
    curUsers.clear() ;
    return 0 ;
}