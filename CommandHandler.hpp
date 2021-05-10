#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

#include "Database/B+Tree.hpp"
#include "user.hpp"

using namespace std;

class CommandHandler {
private:
    stringstream command ;
    char main_op[20], par_key[15][5], par_val[15][35] ;
    int par_cnt = 1 ;

public:
    CommandHandler () {}
    CommandHandler (string op) {
        command = stringstream (op) ;
    }

    void add_user () ;

    void analyze () {
        command >> main_op ;
        while (command >> par_key[par_cnt] >> par_val[par_cnt]) par_cnt ++ ;
        par_cnt -- ;
    }

    void run () {
        analyze() ;
        if (strcmp (main_op, "add_user")) {
            add_user () ;
        } else if (strcmp (main_op, "login")) {

        } else if (strcmp (main_op, "logout")) {

        }
    }

    void add_user () {
        if (par_cnt != 6) throw "command wrong format" ;
        char *cur_username, *username, *password, *name, *mailAddr ;
        int p = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'c') cur_username = par_val[i] ;
            else if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'p') password = par_val[i] ;
            else if (par_key[i][1] == 'n') name = par_val[i] ;
            else if (par_key[i][1] == 'm') mailAddr = par_val[i] ;
            else if (par_key[i][1] == 'g') {
                int len = strlen (par_val[i]) ;
                for (int j = 0; j < len; j ++) p = p * 10 + par_val[i][j] - '0' ;
            } else {
                throw "command wrong format" ;
            }
        }
        user cur_user = user (username, password, name, mailAddr, p) ;

    }

} ;

#endif