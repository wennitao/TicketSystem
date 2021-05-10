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

    void login (const char *_password) {
        if (strcmp (password, _password) != 0) throw "wrong password" ;
    }

    void updatePassword (const char *_password) {
        strcpy (password, _password) ;
    }

    void updateName (const char *_name) {
        strcpy (name, _name) ;
    }

    void updateMailAddress (const char *_mailAddr) {
        strcpy (mailAddr, _mailAddr) ;
    }

    void updatePrivilege (int p) {
        privilege = p ;
    }

} ;

ostream& operator << (ostream &out, const user &_user) {
    out << _user.username << " " << _user.name << " " << _user.mailAddr << " " << _user.privilege ;
    return out ;
}

#endif