#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

#include "Database/B+Tree.hpp"
#include "user.hpp"
#include "main.h"

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
        try {
            analyze() ;
            if (strcmp (main_op, "add_user") == 0) {
                add_user () ;
                printf("0\n") ;
            } else if (strcmp (main_op, "login") == 0) {
                login () ;
                printf("0\n") ;
            } else if (strcmp (main_op, "logout") == 0) {
                logout () ;
                printf("0\n") ;
            } else if (strcmp (main_op, "query_profile") == 0) {
                query_profile() ;
            } else if (strcmp (main_op, "modify_profile") == 0) {
                modify_profile() ;
                printf("0\n") ;
            }
            
        } catch (...) {
            printf("-1\n") ;
        }
    }

    user user_read (int pos) {
        userio.seekg (pos, ios::beg) ;
        user cur ;
        userio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return cur ;
    }

    int user_write (user &cur) {
        userio.seekp (0, ios::end) ;
        int pos = userio.tellp() ;
        userio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return pos ;
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
        if (users.empty()) {
            p = 10 ;
        } else {
            vector<int> pos ;
            users.find (data (username, 0), pos) ;
            if (!pos.empty()) throw "user already exists" ;
            curUsers.find (data (cur_username, 0), pos) ;
            if (pos.empty()) throw "current user not logged in" ;
            user cur_user = user_read (pos[0]) ;
            if (cur_user.getPrivilege() <= p) throw "no enough privilege" ;
        }
        add_user (username, password, name, mailAddr, p) ;
    }

    void add_user (const char *username, const char *password, const char *name, const char *mailAddr, int p) {
        user cur_user = user (username, password, name, mailAddr, p) ;
        int pos = user_write (cur_user) ;
        users.insert (data (username, pos)) ;
    }

    void login () {
        if (par_cnt != 2) throw "command wrong format" ;
        char *username, *password ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'p') password = par_val[i] ;
            else throw "command wrong format" ;
        }
        login (username, password) ;
    }

    void login (const char *username, const char *password) {
        vector<int> pos ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not found" ;
        user targ_user = user_read (pos[0]) ;
        pos.clear() ;
        curUsers.find (data (username, 0), pos) ;
        if (!pos.empty()) throw "already logged in" ;
        targ_user.login (password) ; 
        curUsers.insert (data (username, 0)) ;
    }

    void logout () {
        if (par_cnt != 1) throw "command wrong format" ;
        if (par_key[1][1] != 'u') throw "command wrong format" ;
        char *username = par_val[1] ;
        logout (username) ;
    }

    void logout (char *username) {
        vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "not logged in" ;
        curUsers.erase (data (username, 0)) ;
    }

    void query_profile () {
        if (par_cnt != 2) throw "command wrong format" ;
        char *cur_username, *username ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'c') cur_username = par_val[i] ;
            else if (par_key[i][1] == 'u') username = par_val[i] ;
            else throw "command wrong format" ;
        }
        vector<int> pos ;
        curUsers.find (data (cur_username, 0), pos) ;
        if (pos.empty()) throw "not logged in" ;
        user cur_user = user_read (pos[0]) ;
        pos.clear() ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not exists" ;
        user targ_user = user_read (pos[0]) ;
        if (cur_user.getPrivilege() < targ_user.getPrivilege()) throw "no enough privilege" ;
        cout << targ_user << endl ;
    }

    void modify_profile () {

    }

} ;

#endif