#include <bits/stdc++.h>
#include "user.hpp"
#include "train.hpp"
using namespace std;
user users[110] ;
train trains[110] ;
int main() {
    srand (time (0)) ;
    freopen("test.in", "w", stdout) ;
    string username, password, name, mailAddr ;
    int p ;
    cout << "clean" << endl ;
    cout << "add_user -c admin -u admin -p admin -n admin -m admin@admin.com -g 10" << endl ;
    cout << "login -u admin -p admin" << endl ;
    users[0] = user ("admin", "admin", "admin", "admin@admin.com", 10) ;
    int user_cnt = 100 ;
    for (int i = 1; i < user_cnt; i ++) {
        username.clear(); password.clear(); name.clear(); mailAddr.clear() ;
        for (int j = 0; j < 10; j ++) username += 'a' + rand() % 26 ;
        for (int j = 0; j < 10; j ++) password += 'a' + rand() % 26 ;
        for (int j = 0; j < 10; j ++) name += 'a' + rand() % 26 ;
        for (int j = 0; j < 10; j ++) mailAddr += 'a' + rand() % 26 ;
        p = rand() % 10 ;
        cout << "add_user -c admin -u " << username << " -p " << password << " -n " << name << " -m " << mailAddr << " -g " << p << endl ;
        cout << "login -u " << username << " -p " << password << endl ;
        users[i] = user (username.c_str(), password.c_str(), name.c_str(), mailAddr.c_str(), p) ;
    }

    for (int i = 1; i <= 200; i ++) {
        int op = rand() % 3 ;
        if (op == 0) {
            int x = rand() % 100, y = rand() % 100 ;
            cout << "query_profile -c " << users[x].username << " -u " << users[y].username << endl ;
        } else if (op == 1) {
            int x = rand() % 100, y = rand() % 100 ;
            cout << "modify_profile -c " << users[x].username << " -u " << users[y].username ;
            password.clear(); name.clear(); mailAddr.clear() ;
            for (int j = 0; j < 10; j ++) password += 'a' + rand() % 26 ;
            for (int j = 0; j < 10; j ++) name += 'a' + rand() % 26 ;
            for (int j = 0; j < 10; j ++) mailAddr += 'a' + rand() % 26 ;
            p = rand() % 10 ;
            cout << " -p " << password << " -n " << name << " -m " << mailAddr << " -g " << p << endl ;
            strcpy (users[y].password, password.c_str()) ;
            strcpy (users[y].name, name.c_str()) ;
            strcpy (users[y].mailAddr, mailAddr.c_str()) ;
            users[y].privilege = p ; 
        } else {
            int x = rand() % 100 ;
            cout << "logout -u " << users[x].username << endl ;
        }
    }

    printf("exit") ;
    return 0 ;
}