#ifndef TicketSystem_user
#define TicketSystem_user

#include <iostream>
#include "string.h"

class user {

    friend std::ostream& operator << (std::ostream &out, const user &_user) ;

private:
    string username, password, name, mailAddr ;
    int privilege ;

public:
    user () {}
    user (const string &_username, const string &_password, const string &_name, const string &_mailAddr, const int _privilege) {
        username = _username; password = _password ;
        name = _name; mailAddr = _mailAddr ;
        privilege = _privilege ;
    }

    int getPrivilege () const {
        return privilege ;
    }

    void login (const string &_password) const {
        if (password != _password) throw "wrong password" ;
    }

    void modifyPassword (const string &_password) {
        password = _password ;
    }

    void modifyName (const string &_name) {
        name = _name ;
    }

    void modifyMailAddress (const string &_mailAddr) {
        mailAddr = _mailAddr ;
    }

    void modifyPrivilege (const int _privilege) {
        privilege = _privilege ;
    }
} ;

std::ostream& operator << (std::ostream &out, const user &_user) {
    out << _user.username << " " << _user.name << " " << _user.mailAddr << " " << _user.privilege ;
    return out ;
}

#endif