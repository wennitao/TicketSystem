#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

#include "Database/B+Tree.hpp"
#include "time.hpp"
#include "user.hpp"
#include "train.hpp"
#include "ticket.hpp"
#include "main.h"

using namespace std;

class CommandHandler {
private:
    stringstream command ;
    char main_op[20], par_key[15][5], par_val[15][4010] ;
    int par_cnt = 1 ;

public:
    CommandHandler () {}
    CommandHandler (string op) {
        //cout << op << endl ;
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
            } else if (strcmp (main_op, "exit") == 0) {
                printf("bye\n") ;
                return ;
            } else if (strcmp (main_op, "add_train") == 0) {
                add_train () ;
                printf("0\n") ;
            } else if (strcmp (main_op, "release_train") == 0) {
                release_train() ;
                printf("0\n") ;
            } else if (strcmp (main_op, "query_train") == 0) {
                query_train() ;
            } else if (strcmp (main_op, "delete_train") == 0) {
                delete_train() ;
                printf("0\n") ;
            } else if (strcmp (main_op, "query_ticket") == 0) {
                query_ticket () ;
            } else if (strcmp (main_op, "query_transfer") == 0) {
                query_transfer () ;
            } else if (strcmp (main_op, "buy_ticket") == 0) {
                buy_ticket () ;
            } else if (strcmp (main_op, "query_order") == 0) {
                query_order() ;
            } else if (strcmp (main_op, "refund_ticket") == 0) {
                refund_ticket() ;
                printf("0\n") ;
            } else if (strcmp (main_op, "clean") == 0) {
                clean () ;
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

    void user_write (int pos, user &cur) {
        userio.seekp (pos, ios::beg) ;
        userio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    train train_read (int pos) {
        trainio.seekg (pos, ios::beg) ;
        train cur ;
        trainio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return cur ;
    }

    int train_write (train &cur) {
        trainio.seekp (0, ios::end) ;
        int pos = trainio.tellp() ;
        trainio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return pos ;
    }

    void train_write (int pos, train &cur) {
        trainio.seekp (pos, ios::beg) ;
        trainio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    ticket order_read (int pos) {
        orderio.seekg (pos, ios::beg) ;
        ticket cur ;
        orderio.read (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return cur ;
    }

    int order_write (ticket &cur) {
        orderio.seekp (0, ios::end) ;
        int pos = orderio.tellp() ;
        orderio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
        return pos ;
    }

    void order_write (int pos, ticket &cur) {
        orderio.seekp (pos, ios::beg) ;
        orderio.write (reinterpret_cast<char *>(&cur), sizeof (cur)) ;
    }

    void add_station (const char *_stationName) {
        stationio.seekg (0, ios::beg) ;
        int cnt ;
        stationio.read (reinterpret_cast<char *>(&cnt), sizeof cnt); cnt ++ ;
        stationio.seekp (0, ios::beg) ;
        stationio.write (reinterpret_cast<char *>(&cnt), sizeof cnt) ;

        char stationName[35] ;
        strcpy (stationName, _stationName) ;
        stationio.seekp (0, ios::end) ;
        stationio.write (reinterpret_cast<char *>(&stationName), sizeof (stationName)) ;
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
        user targ_user = user_read (pos[0]); int targ_pos = pos[0] ;
        pos.clear() ;
        curUsers.find (data (username, 0), pos) ;
        if (!pos.empty()) throw "already logged in" ;
        targ_user.login (password) ; 
        curUsers.insert (data (username, targ_pos)) ;
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
        curUsers.erase (data (username, pos[0])) ;
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
        if (cur_user.getPrivilege() <= targ_user.getPrivilege() && strcmp (cur_username, username) != 0) throw "no enough privilege" ;
        cout << targ_user << endl ;
    }

    void modify_profile () {
        if (par_cnt < 2 || par_cnt > 6) throw "command wrong format" ; 
        bool c = 0, u = 0 ;
        char *cur_username = nullptr, *username = nullptr, *password = nullptr, *name = nullptr, *mailAddr = nullptr; int p = -1 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'c') c = 1, cur_username = par_val[i] ;
            else if (par_key[i][1] == 'u') u = 1, username = par_val[i] ;
            else if (par_key[i][1] == 'p') password = par_val[i] ;
            else if (par_key[i][1] == 'n') name = par_val[i] ;
            else if (par_key[i][1] == 'm') mailAddr = par_val[i] ;
            else if (par_key[i][1] == 'g') {
                p = 0 ;
                int len = strlen (par_val[i]) ;
                for (int j = 0; j < len; j ++) p = p * 10 + par_val[i][j] - '0' ;
            } else {
                throw "command wrong format" ;
            }
        }
        if (!c || !u) throw "command wrong format" ;

        vector<int> pos ;
        curUsers.find (data (cur_username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;
        user cur_user = user_read (pos[0]) ;
        pos.clear() ;
        users.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not exists" ;
        user targ_user = user_read (pos[0]) ;
        if ((cur_user.getPrivilege() <= targ_user.getPrivilege() && strcmp (cur_username, username) != 0) || cur_user.getPrivilege() <= p) throw "privilege invalid" ;

        if (password) targ_user.updatePassword (password) ;
        if (name) targ_user.updateName (name) ;
        if (mailAddr) targ_user.updateMailAddress (mailAddr) ;
        if (p != -1) targ_user.updatePrivilege (p) ;
        user_write (pos[0], targ_user) ;
        cout << targ_user << endl ;
    }

    void add_train () {
        if (par_cnt != 10) throw "command wrong format" ;
        Time saleDate[3], startTime ;
        char stationName[110][30] = {0} ;
        char *trainID, type ;
        int stationNum = 0, seatNum = 0, prices[110] = {0}, travelTimes[110] = {0}, stopoverTimes[110] = {0};
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'i') trainID = par_val[i] ;
            else if (par_key[i][1] == 'n') {
                int len = strlen (par_val[i]) ;
                for (int j = 0; j < len; j ++) stationNum = stationNum * 10 + par_val[i][j] - '0' ;
            } else if (par_key[i][1] == 'm') {
                int len = strlen (par_val[i]) ;
                for (int j = 0; j < len; j ++) seatNum = seatNum * 10 + par_val[i][j] - '0' ;
            } else if (par_key[i][1] == 's') {
                int curid = 1, curlen = 0, cur = 0, len = strlen (par_val[i]) ;
                for (; cur < len; cur ++) {
                    if (par_val[i][cur] == '|') curid ++, curlen = 0 ;
                    else stationName[curid][curlen ++] = par_val[i][cur] ;
                }
            } else if (par_key[i][1] == 'p') {
                int curid = 1, cur = 0, len = strlen (par_val[i]) ;
                for (; cur < len; cur ++) {
                    if (par_val[i][cur] == '|') curid ++ ;
                    else prices[curid] = prices[curid] * 10 + par_val[i][cur] - '0' ;
                }
            } else if (par_key[i][1] == 'x') {
                startTime = Time ("00-00", par_val[i]) ;
            } else if (par_key[i][1] == 't') {
                int curid = 1, cur = 0, len = strlen (par_val[i]) ;
                for (; cur < len; cur ++) {
                    if (par_val[i][cur] == '|') curid ++ ;
                    else travelTimes[curid] = travelTimes[curid] * 10 + par_val[i][cur] - '0' ;
                }
            } else if (par_key[i][1] == 'o') {
                int curid = 1, cur = 0, len = strlen (par_val[i]) ;
                for (; cur < len; cur ++) {
                    if (par_val[i][cur] == '|') curid ++ ;
                    else stopoverTimes[curid] = stopoverTimes[curid] * 10 + par_val[i][cur] - '0' ;
                }
            } else if (par_key[i][1] == 'd') {
                char tmp[10] = {0} ;
                for (int j = 0; j < 5; j ++) tmp[j] = par_val[i][j] ;
                saleDate[1] = Time (tmp, "00:00") ;
                for (int j = 6; j < 11; j ++) tmp[j - 6] = par_val[i][j] ;
                saleDate[2] = Time (tmp, "00:00") ;
            } else if (par_key[i][1] == 'y') {
                type = par_val[i][0] ;
            }
        }
        saleDate[1].setTime (startTime); saleDate[2].setTime(startTime) ;

        train cur_train = train (trainID, stationName, startTime, saleDate, type, stationNum, seatNum, prices, travelTimes, stopoverTimes) ;
        vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (!pos.empty()) throw "train already exists" ;
        int write_pos = train_write (cur_train) ;
        trains.insert (data (trainID, write_pos)) ;

        for (int i = 1; i <= stationNum; i ++) {
            trainStations.insert (data (stationName[i], write_pos)) ;
        }

        for (int i = 1; i <= stationNum; i ++) {
            vector<int> pos ;
            stationNames.find (data (stationName[i], 0), pos) ;
            if (!pos.empty()) continue ;
            stationNames.insert (data (stationName[i], 0)) ;
            add_station (stationName[i]) ;
        }
    }

    void release_train () {
        if (par_cnt != 1 || par_key[1][1] != 'i') throw "command wrong format" ;
        char *trainID = par_val[1] ;
        vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not exists" ;
        train cur = train_read (pos[0]) ;
        cur.release() ;
        train_write (pos[0], cur) ;
    }

    void query_train () {
        if (par_cnt != 2) throw "command wrong format" ;
        char *trainID, *date ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'i') trainID = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
        }
        vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not exists" ;
        train cur = train_read (pos[0]) ;
        if (!cur.runningOnDate (date)) throw "train not runs on this date" ;
        cur.print (date) ;
    }

    void delete_train () {
        if (par_cnt != 1 || par_key[1][1] != 'i') throw "command wrong format" ;
        char *trainID = par_val[1] ;
        vector<int> pos ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        train cur_train = train_read (pos[0]) ;
        if (cur_train.getReleaseStatus()) throw "already released" ;
        trains.erase (data (trainID, pos[0])) ;

        int stationNum = cur_train.getStationNum() ;
        for (int i = 1; i <= stationNum; i ++) {
            trainStations.erase (data (cur_train.getStationName (i), pos[0])) ;
        }
    }

    static bool cmp_time (const ticket &a, const ticket &b) {
        return a.getTravellingTime() < b.getTravellingTime() ;
    }

    static bool cmp_cost (const ticket &a, const ticket &b) {
        return a.getPrice() < b.getPrice() ;
    }

    void query_ticket () {
        if (par_cnt < 3 || par_cnt > 4) throw "command wrong format" ;
        char *startStationName, *terminalStationName, *date ;
        bool priority = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 's') startStationName = par_val[i] ;
            else if (par_key[i][1] == 't') terminalStationName = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
            else priority = strcmp (par_val[i], "time") == 0 ? 0 : 1 ;
        }

        vector<int> start_pos, end_pos, train_pos ;
        trainStations.find (data (startStationName, 0), start_pos) ;
        trainStations.find (data (terminalStationName, 0), end_pos) ;
        //sort replace to diy_sort
        sort (start_pos.begin(), start_pos.end()) ;
        sort (end_pos.begin(), end_pos.end()) ;

        int start_id = 0, end_id = 0 ;
        while (start_id < start_pos.size() && end_id < end_pos.size()) {
            if (start_pos[start_id] < end_pos[end_id]) start_id ++ ;
            else if (start_pos[start_id] > end_pos[end_id]) end_id ++ ;
            else {
                train_pos.push_back (start_pos[start_id]) ;
                start_id ++; end_id ++ ;
            }
        }

        int ticket_cnt = 0 ;
        ticket *tickets = new ticket[train_pos.size()] ;
        for (int i = 0; i < train_pos.size(); i ++) {
            train cur_train = train_read (train_pos[i]) ;
            if (!cur_train.getReleaseStatus()) continue ;
            if (!cur_train.runningOnDate (date, startStationName)) continue ;
            if (!cur_train.runningFromTo (startStationName, terminalStationName)) continue ;
            Time trainStartTime = cur_train.getStartTime (date, startStationName) ;
            tickets[ticket_cnt ++] = ticket (cur_train.getTrainID(), startStationName, terminalStationName, 
            cur_train.leavingTime (trainStartTime, startStationName), 
            cur_train.arrivingTime (trainStartTime, terminalStationName), 
            cur_train.calPrice (startStationName, terminalStationName), 
            cur_train.calSeatNum (trainStartTime, startStationName, terminalStationName), 
            cur_train.getTravellingTime (startStationName, terminalStationName), 
            success) ;
        }

        if (priority) sort (tickets, tickets + ticket_cnt, cmp_cost) ;
        else sort (tickets, tickets + ticket_cnt, cmp_time) ;

        printf("%d\n", ticket_cnt) ;
        for (int i = 0; i < ticket_cnt; i ++)
            tickets[i].print() ;
        
        delete [] tickets ;
    }

    void query_transfer () {
        if (par_cnt < 3 || par_cnt > 4) throw "command wrong format" ;
        char *startStationName, *terminalStationName, *date ;
        bool priority = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 's') startStationName = par_val[i] ;
            else if (par_key[i][1] == 't') terminalStationName = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
            else priority = strcmp (par_val[i], "time") == 0 ? 0 : 1 ;
        }

        int stationCnt = 0 ;
        stationio.seekg (0, ios::beg) ;
        stationio.read (reinterpret_cast<char *>(&stationCnt), sizeof stationCnt) ;
        int cost = 1e9 ;
        ticket order_1, order_2 ;
        for (int i = 0; i < stationCnt; i ++) {
            char stationName[35] ;
            stationio.seekg (sizeof (int) + i * sizeof (stationName), ios::beg) ;
            stationio.read (reinterpret_cast<char *>(&stationName), sizeof stationName) ;

            if (strcmp (stationName, startStationName) == 0 || strcmp (stationName, terminalStationName) == 0) continue ;
            
            int cost1 = 1e9, cost2 = 1e9 ;
            ticket tmp_order_1, tmp_order_2 ;

            vector<int> pos ;
            trainStations.find (data (stationName, 0), pos) ;
            for (int i = 0; i < pos.size(); i ++) {
                train cur_train = train_read (pos[i]) ;
                if (!cur_train.getReleaseStatus()) continue ;
                if (!cur_train.runningOnDate (date, startStationName)) continue ;
                if (!cur_train.runningFromTo (startStationName, stationName)) continue ;
                Time trainStartTime = cur_train.getStartTime (date, startStationName) ;
                int cur_price = cur_train.calPrice (startStationName, stationName) ;
                int cur_travelling_time = cur_train.getTravellingTime (startStationName, stationName) ;
                if (priority == 0 && cur_travelling_time < cost1) {
                    cost1 = cur_travelling_time ;
                    tmp_order_1 = ticket (cur_train.getTrainID(), startStationName, stationName, 
                    cur_train.leavingTime (trainStartTime, startStationName), 
                    cur_train.arrivingTime (trainStartTime, stationName), 
                    cur_price, 
                    cur_train.calSeatNum (trainStartTime, startStationName, stationName), 
                    cur_travelling_time, 
                    success) ;
                }

                if (priority == 1 && cur_price < cost1) {
                    cost1 = cur_price ;
                    tmp_order_1 = ticket (cur_train.getTrainID(), startStationName, stationName, 
                    cur_train.leavingTime (trainStartTime, startStationName), 
                    cur_train.arrivingTime (trainStartTime, stationName), 
                    cur_price, 
                    cur_train.calSeatNum (trainStartTime, startStationName, stationName), 
                    cur_travelling_time, 
                    success) ;
                }
            }

            for (int i = 0; i < pos.size(); i ++) {
                train cur_train = train_read (pos[i]) ;
                if (!cur_train.getReleaseStatus()) continue ;
                if (!cur_train.runningOnDate (date, stationName)) continue ;
                if (!cur_train.runningFromTo (stationName, terminalStationName)) continue ;
                Time trainStartTime = cur_train.getStartTime (date, stationName) ;
                int cur_price = cur_train.calPrice (stationName, terminalStationName) ;
                int cur_travelling_time = cur_train.getTravellingTime (stationName, terminalStationName) ;
                if (priority == 0 && cur_travelling_time < cost2) {
                    cost2 = cur_travelling_time ;
                    tmp_order_2 = ticket (cur_train.getTrainID(), stationName, terminalStationName, 
                    cur_train.leavingTime (trainStartTime, stationName), 
                    cur_train.arrivingTime (trainStartTime, terminalStationName), 
                    cur_price, 
                    cur_train.calSeatNum (trainStartTime, stationName, terminalStationName), 
                    cur_travelling_time, 
                    success) ;
                }

                if (priority == 1 && cur_price < cost2) {
                    cost2 = cur_price ;
                    tmp_order_2 = ticket (cur_train.getTrainID(), stationName, terminalStationName, 
                    cur_train.leavingTime (trainStartTime, stationName), 
                    cur_train.arrivingTime (trainStartTime, terminalStationName), 
                    cur_price, 
                    cur_train.calSeatNum (trainStartTime, stationName, terminalStationName), 
                    cur_travelling_time, 
                    success) ;
                }
            }

            cout << cost1 << " " << cost2 << endl ;

            if (cost1 + cost2 < cost) {
                cost = cost1 + cost2 ;
                order_1 = tmp_order_1; order_2 = tmp_order_2 ;
            }
        }

        if (cost == 1e9) printf("0\n") ;
        else {
            order_1.print(); order_2.print() ;
        }
    }

    void buy_ticket () {
        if (par_cnt < 6 || par_cnt > 7) throw "command wrong format" ;
        char *username, *trainID, *startStationName, *terminalStationName, *date ;
        int ticketNum = 0 ;
        bool q = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'i') trainID = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
            else if (par_key[i][1] == 'f') startStationName = par_val[i] ;
            else if (par_key[i][1] == 't') terminalStationName = par_val[i] ;
            else if (par_key[i][1] == 'n') {
                int len = strlen (par_val[i]) ;
                for (int j = 0; j < len; j ++) ticketNum = ticketNum * 10 + par_val[i][j] - '0' ;
            }
            else if (par_key[i][1] == 'q') q = strcmp (par_val[i], "true") == 0 ? 1 : 0 ;
            else throw "command wrong format" ;
        }

        vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;
        user cur_user = user_read (pos[0]) ;

        pos.clear() ;
        trains.find (data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        int train_file_pos = pos[0] ;
        train cur_train = train_read (pos[0]) ;
        if (!cur_train.runningOnDate (date, startStationName)) throw "no trains run on this date" ;

        Time trainStartTime = cur_train.getStartTime (date, startStationName) ;

        int remainingSeatNum = cur_train.calSeatNum (trainStartTime, startStationName, terminalStationName) ;
        //cout << remainingSeatNum << " " << ticketNum << endl ;
        if (remainingSeatNum < ticketNum && !q) throw "no enough tickets" ;

        long long order_price = 1ll * ticketNum * cur_train.calPrice (startStationName, terminalStationName) ;
        ticket cur_order = ticket (trainID, startStationName, terminalStationName, 
        cur_train.leavingTime (trainStartTime, startStationName), 
        cur_train.arrivingTime (trainStartTime, terminalStationName), 
        cur_train.calPrice (startStationName, terminalStationName), 
        ticketNum, 
        cur_train.getTravellingTime (startStationName, terminalStationName), 
        success) ;

        if (remainingSeatNum < ticketNum) {
            cur_order.setStatus (pending) ;
            int write_pos = order_write (cur_order) ;
            orders.insert (data (username, write_pos)) ;
            pendingOrders.insert (data (trainID, write_pos)) ;
            printf("queue\n") ;
        } else {
            int write_pos = order_write (cur_order) ;
            orders.insert (data (username, write_pos)) ;
            cur_train.sellSeats (trainStartTime, startStationName, terminalStationName, ticketNum) ;
            printf("%lld\n", order_price) ;
            train_write (train_file_pos, cur_train) ;
        }
        
    }

    void query_order () {
        if (par_cnt != 1 || par_key[1][1] != 'u') throw "command wrong format" ;
        char *username = par_val[1] ;
        vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;

        pos.clear() ;
        orders.find (data (username, 0), pos) ;
        //if (pos.empty()) throw "orders not found" ;

        reverse (pos.begin(), pos.end()) ;

        cout << pos.size() << endl ;
        for (int i = 0; i < pos.size(); i ++) {
            int file_pos = pos[i] ;
            ticket cur = order_read (file_pos) ;
            cout << cur << endl ;
        }
    }

    void refund_ticket () {
        if (par_cnt < 1 || par_cnt > 2) throw "command wrong format" ;
        char *username ;
        int order_num = 1 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'n') {
                order_num = 0 ;
                int len = strlen (par_val[i]) ;
                for (int j = 0; j < len; j ++) order_num = order_num * 10 + par_val[i][j] - '0' ;
            }
        }

        vector<int> pos ;
        curUsers.find (data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;

        pos.clear() ;
        orders.find (data (username, 0), pos) ;
        if (pos.empty()) throw "no orders" ;
        if (pos.size() < order_num) throw "there is no nth order" ;
        reverse (pos.begin(), pos.end()) ;

        ticket cur_order = order_read (pos[order_num - 1]) ;
        if (cur_order.getStatus() != success) throw "can't refund" ;

        cur_order.setStatus (refunded) ;
        order_write (pos[order_num - 1], cur_order) ;

        pos.clear() ;
        const char *trainID = cur_order.getTrainID() ;
        trains.find (data (trainID, 0), pos) ;
        int train_file_pos = pos[0] ;
        train cur_train = train_read (train_file_pos) ;
        Time trainStartTime = cur_train.getStartTime (cur_order.getLeavingTime(), cur_order.getFromStation()) ;
        cur_train.addSeats (trainStartTime, cur_order.getFromStation(), cur_order.getToStation(), cur_order.getSeatNum()) ;
        train_write (train_file_pos, cur_train) ;

        pos.clear() ;
        pendingOrders.find (data (trainID, 0), pos) ;
        for (int i = 0; i < pos.size(); i ++) {
            ticket waiting_order = order_read (pos[i]) ;
            Time trainStartTime = cur_train.getStartTime (waiting_order.getLeavingTime(), waiting_order.getFromStation()) ;
            int remaining_seat_num = cur_train.calSeatNum (trainStartTime, waiting_order.getFromStation(), waiting_order.getToStation()) ;
            if (remaining_seat_num >= waiting_order.getSeatNum()) {
                waiting_order.setStatus (success) ;
                cur_train.sellSeats (trainStartTime, waiting_order.getFromStation(), waiting_order.getToStation(), waiting_order.getSeatNum()) ;
                pendingOrders.erase (data (trainID, pos[i])) ;
                order_write (pos[i], waiting_order) ;
            }
        }
        train_write (train_file_pos, cur_train) ;
    }

    void clean () {
        users.clear() ;
        trains.clear() ;
        trainStations.clear() ;
        curUsers.clear() ;
        orders.clear() ;
        pendingOrders.clear() ;
        stationNames.clear() ;

        userio.close() ;
        userio.open ("users.dat", ios::out | ios::binary) ;
        userio.close() ;
        userio.open ("users.dat", ios::in | ios::out | ios::binary) ;

        // ...
    }

} ;

#endif