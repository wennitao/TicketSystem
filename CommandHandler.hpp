#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

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

    void analyze () {
        command >> main_op ;
        while (command >> par_key[par_cnt] >> par_val[par_cnt]) par_cnt ++ ;
        par_cnt -- ;
    }

    void run () {
        analyze() ;
        if (strcmp (main_op, "add_user")) {

        } else if (strcmp (main_op, "login")) {

        } else if (strcmp (main_op, "logout")) {
            
        }
    }

} ;

#endif