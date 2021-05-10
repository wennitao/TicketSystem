#ifndef TicketSystem_user
#define TicketSystem_user

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

class user {

    friend ostream& operator << (ostream &out, const user &_user) ;

private:
    char username[25], password[35], name[30], mailAddr[35] ;
    int privilege ;

public:
    user () {}

    user (const char *_username, const char *_password, const char *_name, const char *_mailAddr, int p) {
        strcpy (username, _username) ;
        strcpy (password, _password) ;
        strcpy (name, _name) ;
        strcpy (mailAddr, _mailAddr) ;
        privilege = p ;
    }

    int getPrivilege () {
        return privilege ;
    }

} ;

ostream& operator << (ostream &out, const user &_user) {
    out << "username:" << _user.username << " password:" << _user.password << " name:" << _user.name << " mailAddress:" << _user.mailAddr << " privilege:" << _user.privilege ;
    return out ;
}

#endif