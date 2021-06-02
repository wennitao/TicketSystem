#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "main.h"
#include "Bpt_and_database.h"
#include "data.hpp"
#include "string.h"
#include "vector.h"

#include "user.hpp"

class CommandHandler {
private:
    std::string op ;
    char argument[30][10010] ;
    int key_cnt = 1 ;

public:
    CommandHandler (const std::string _op) {
        op = _op ;
    }

    void analyze () {
        int cur_len = 0 ;
        for (int i = 0; i < op.length(); i ++) {
            if (op[i] == ' ') {
                argument[key_cnt][cur_len ++] = 0; key_cnt ++; cur_len = 0 ;
            } else {
                argument[key_cnt][cur_len ++] = op[i] ;
            }
        }
        argument[key_cnt][cur_len ++] = 0 ;
    }

    user user_read (int pos) {
        userio.seekg (pos, std::ios::beg) ;
        user cur ;
        userio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return cur ;
    }

    int user_write (user &cur) {
        userio.seekp (0, std::ios::end) ;
        int pos = userio.tellp() ;
        userio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return pos ;
    }

    void user_write (int pos, user &cur) {
        userio.seekp (pos, std::ios::beg) ;
        userio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    void run () {
        try {
            analyze () ;
            if (strcmp (argument[1], "add_user") == 0) {
                add_user () ;
            } else if (strcmp (argument[1], "login") == 0) {
                login () ;
            } else if (strcmp (argument[1], "logout") == 0) {
                logout () ;
            } else if (strcmp (argument[1], "query_profile") == 0) {
                query_profile () ;
            } else if (strcmp (argument[1], "modify_profile") == 0) {
                modify_profile () ;
            }
        } catch (...) {
            printf("-1\n") ;
        }
    }

    void add_user () {
        string cur_username, username, password, name, mailAddr ;
        int privilege ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'c') cur_username = string (argument[i + 1]) ;
            else if (argument[i][1] == 'u') username = string (argument[i + 1]) ;
            else if (argument[i][1] == 'p') password = string (argument[i + 1]) ;
            else if (argument[i][1] == 'n') name = string (argument[i + 1]) ;
            else if (argument[i][1] == 'm') mailAddr = string (argument[i + 1]) ;
            else if (argument[i][1] == 'g') privilege = string (argument[i + 1]).toInt() ;
        }

        if (users.empty()) {
            privilege = 10 ;
        } else {
            std::vector<int> pos ;
            curUsers.find (data (cur_username, 0), pos) ;
            if (pos.empty()) throw "cur user not logged in" ;

            user cur_user = user_read (pos[0]) ;
            if (cur_user.getPrivilege() <= privilege) throw "invalid privilege" ;
        }

        std::vector<int> pos ;
        users.find (data (username, 0), pos) ;
        if (!pos.empty()) throw "user already exists" ;
        user new_user = user (username, password, name, mailAddr, privilege) ;
        int write_pos = user_write (new_user) ;
        users.insert (data (username, write_pos)) ;
        printf ("0\n") ;
    }

    void login () {
        string username, password ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = string (argument[i + 1]) ;
            else if (argument[i][1] == 'p') password = string (argument[i + 1]) ;
        }

        std::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (!pos.empty()) throw "already logged in" ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "username not exists" ;

        int user_file_pos = pos[0] ;
        user cur_user = user_read (user_file_pos) ;
        cur_user.login (password) ;
        curUsers.insert (data (username, user_file_pos)) ;
        printf ("0\n") ;
    }

    void logout () {
        string username ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'u') username = string (argument[i + 1]) ;
        }
        std::vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "cur user not logged in" ;
        int user_file_pos = pos[0] ;
        curUsers.erase (data (username, user_file_pos)) ;
        printf ("0\n") ;
    }

    void query_profile () {
        string cur_username, username ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'c') cur_username = string (argument[i + 1]) ;
            else if (argument[i][1] == 'u') username = string (argument[i + 1]) ;
        }

        std::vector<int> pos ;
        curUsers.find (data (cur_username, 0), pos) ;
        if (pos.empty()) throw "cur user not logged in" ;
        int cur_user_file_pos = pos[0] ;
        user cur_user = user_read (cur_user_file_pos) ;

        pos.clear() ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not found" ;
        int query_user_file_pos = pos[0] ;
        user query_user = user_read (query_user_file_pos) ;
        if (!(cur_user.getPrivilege() > query_user.getPrivilege() || cur_username == username)) throw "invalid privilege" ;
        
        std::cout << query_user << std::endl ;
    }

    void modify_profile () {
        string cur_username, username, password, name, mailAddr ;
        int privilege = -1 ;
        for (int i = 2; i <= key_cnt; i += 2) {
            if (argument[i][1] == 'c') cur_username = string (argument[i + 1]) ;
            else if (argument[i][1] == 'u') username = string (argument[i + 1]) ;
            else if (argument[i][1] == 'p') password = string (argument[i + 1]) ;
            else if (argument[i][1] == 'n') name = string (argument[i + 1]) ;
            else if (argument[i][1] == 'm') mailAddr = string (argument[i + 1]) ;
            else if (argument[i][1] == 'g') privilege = string (argument[i + 1]).toInt() ;
        }

        std::vector<int> pos ;
        curUsers.find (data (cur_username, 0), pos) ;
        if (pos.empty()) throw "user enot logged in" ;
        int cur_user_file_pos = pos[0] ;
        user cur_user = user_read (cur_user_file_pos) ;
        
        pos.clear() ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not found" ;
        int modify_user_file_pos = pos[0] ;
        user modify_user = user_read (modify_user_file_pos) ;

        if (!(cur_user.getPrivilege() > modify_user.getPrivilege() || cur_username == username)) throw "invalid privilege" ;
        if (privilege != -1 && privilege >= cur_user.getPrivilege()) throw "invalid privilege" ;

        if (!password.empty()) modify_user.modifyPassword (password) ;
        if (!name.empty()) modify_user.modifyName (name) ;
        if (!mailAddr.empty()) modify_user.modifyMailAddress (mailAddr) ;
        if (privilege != -1) modify_user.modifyPrivilege (privilege) ;
        user_write (modify_user_file_pos, modify_user) ;

        std::cout << modify_user << std::endl ;
    }



} ;

#endif