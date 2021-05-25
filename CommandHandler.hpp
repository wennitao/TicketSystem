#ifndef TicketSystem_CommandHandler
#define TicketSystem_CommandHandler

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>

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
    char main_op[110], par_key[15][35], par_val[15][10010] ;
    int par_cnt = 1 ;

public:
    CommandHandler () {
        memset (main_op, 0, sizeof main_op) ;
        memset (par_key, 0, sizeof par_key) ;
        memset (par_val, 0, sizeof par_val) ;
    }
    CommandHandler (string op) {
        memset (main_op, 0, sizeof main_op) ;
        memset (par_key, 0, sizeof par_key) ;
        memset (par_val, 0, sizeof par_val) ;

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
            if (strlen (main_op) == 0) return ;
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

        char stationName[110] ;
        strcpy (stationName, _stationName) ;
        stationio.seekp (0, ios::end) ;
        stationio.write (reinterpret_cast<char *>(&stationName), sizeof (stationName)) ;
        
        //printf("after add_station file io ok: %d\n", stationio.good()) ;
    }

    int strToInt (const char *str) {
        int len = strlen (str), res = 0 ;
        for (int i = 0; i < len; i ++)
            res = res * 10 + str[i] - '0' ;
        return res ;
    }

    void add_user () {
        const char *cur_username, *username, *password, *name, *mailAddr ;
        int p = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'c') cur_username = par_val[i] ;
            else if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'p') password = par_val[i] ;
            else if (par_key[i][1] == 'n') name = par_val[i] ;
            else if (par_key[i][1] == 'm') mailAddr = par_val[i] ;
            else if (par_key[i][1] == 'g') p = strToInt (par_val[i]) ;
        }
        if (users.empty()) {
            p = 10 ;
        } else {
            vector<int> pos ;
            users.find (my_data (username, 0), pos) ;
            if (!pos.empty()) throw "user already exists" ;
            curUsers.find (my_data (cur_username, 0), pos) ;
            if (pos.empty()) throw "current user not logged in" ;
            user cur_user = user_read (pos[0]) ;
            if (cur_user.getPrivilege() <= p) throw "no enough privilege" ;
        }
        add_user (username, password, name, mailAddr, p) ;
    }

    void add_user (const char *username, const char *password, const char *name, const char *mailAddr, int p) {
        user cur_user = user (username, password, name, mailAddr, p) ;
        int pos = user_write (cur_user) ;
        users.insert (my_data (username, pos)) ;
    }

    void login () {
        const char *username, *password ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'p') password = par_val[i] ;
        }
        login (username, password) ;
    }

    void login (const char *username, const char *password) {
        vector<int> pos ;
        users.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "user not found" ;
        user targ_user = user_read (pos[0]); int targ_pos = pos[0] ;
        pos.clear() ;
        curUsers.find (my_data (username, 0), pos) ;
        if (!pos.empty()) throw "already logged in" ;
        targ_user.login (password) ; 
        curUsers.insert (my_data (username, targ_pos)) ;
    }

    void logout () {
        char *username = par_val[1] ;
        logout (username) ;
    }

    void logout (const char *username) {
        vector<int> pos ;
        curUsers.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "not logged in" ;
        curUsers.erase (my_data (username, pos[0])) ;
    }

    void query_profile () {
        const char *cur_username, *username ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'c') cur_username = par_val[i] ;
            else if (par_key[i][1] == 'u') username = par_val[i] ;
        }
        vector<int> pos ;
        curUsers.find (my_data (cur_username, 0), pos) ;
        if (pos.empty()) throw "not logged in" ;
        user cur_user = user_read (pos[0]) ;
        pos.clear() ;
        users.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "user not exists" ;
        user targ_user = user_read (pos[0]) ;
        if (cur_user.getPrivilege() <= targ_user.getPrivilege() && strcmp (cur_username, username) != 0) throw "no enough privilege" ;
        cout << targ_user << endl ;
    }

    void modify_profile () {
        const char *cur_username = nullptr, *username = nullptr, *password = nullptr, *name = nullptr, *mailAddr = nullptr; int p = -1 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'c') cur_username = par_val[i] ;
            else if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'p') password = par_val[i] ;
            else if (par_key[i][1] == 'n') name = par_val[i] ;
            else if (par_key[i][1] == 'm') mailAddr = par_val[i] ;
            else if (par_key[i][1] == 'g') p = strToInt (par_val[i]) ;
        }

        vector<int> pos ;
        curUsers.find (my_data (cur_username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;
        user cur_user = user_read (pos[0]) ;
        pos.clear() ;
        users.find (my_data (username, 0), pos) ;
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
        Time saleDate[3], startTime ;
        char stationName[110][110], type ;
        const char *trainID ;
        int stationNum = 0, seatNum = 0, prices[110], travelTimes[110], stopoverTimes[110] ;
        memset (stationName, 0, sizeof stationName) ;
        memset (prices, 0, sizeof prices) ;
        memset (travelTimes, 0, sizeof travelTimes) ;
        memset (stopoverTimes, 0, sizeof stopoverTimes) ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'i') trainID = par_val[i] ;
            else if (par_key[i][1] == 'n') stationNum = strToInt (par_val[i]) ;
            else if (par_key[i][1] == 'm') seatNum = strToInt (par_val[i]) ;
            else if (par_key[i][1] == 's') {
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
                char tmp[110] = {0} ;
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
        trains.find (my_data (trainID, 0), pos) ;
        if (!pos.empty()) throw "train already exists" ;
        int write_pos = train_write (cur_train) ;
        trains.insert (my_data (trainID, write_pos)) ;

        for (int i = 1; i <= stationNum; i ++) {
            trainStations.insert (my_data (stationName[i], write_pos)) ;
        }

        for (int i = 1; i <= stationNum; i ++) {
            vector<int> pos ;
            stationNames.find (my_data (stationName[i], 0), pos) ;
            if (!pos.empty()) continue ;
            stationNames.insert (my_data (stationName[i], 0)) ;
            add_station (stationName[i]) ;
        }
    }

    void release_train () {
        const char *trainID = par_val[1] ;
        vector<int> pos ;
        trains.find (my_data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not exists" ;
        train cur = train_read (pos[0]) ;
        cur.release() ;
        train_write (pos[0], cur) ;
    }

    void query_train () {
        if (par_cnt != 2) throw "command wrong format" ;
        const char *trainID, *date ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'i') trainID = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
        }
        vector<int> pos ;
        trains.find (my_data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not exists" ;
        train cur = train_read (pos[0]) ;
        if (!cur.runningOnDate (date)) throw "train not runs on this date" ;
        cur.print (date) ;
    }

    void delete_train () {
        const char *trainID = par_val[1] ;
        vector<int> pos ;
        trains.find (my_data (trainID, 0), pos) ;
        if (pos.empty()) throw "train not found" ;
        train cur_train = train_read (pos[0]) ;
        if (cur_train.getReleaseStatus()) throw "already released" ;
        trains.erase (my_data (trainID, pos[0])) ;

        int stationNum = cur_train.getStationNum() ;
        for (int i = 1; i <= stationNum; i ++) {
            trainStations.erase (my_data (cur_train.getStationName (i), pos[0])) ;
        }
    }

    static bool cmp_time (const ticket &a, const ticket &b) {
        if (a.getTravellingTime() == b.getTravellingTime()) return strcmp (a.getTrainID(), b.getTrainID()) < 0 ;
        return a.getTravellingTime() < b.getTravellingTime() ;
    }

    static bool cmp_cost (const ticket &a, const ticket &b) {
        if (a.getPrice() == b.getPrice()) return strcmp (a.getTrainID(), b.getTrainID()) < 0 ;
        return a.getPrice() < b.getPrice() ;
    }

    void query_ticket () {
        const char *startStationName, *terminalStationName, *date ;
        bool priority = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 's') startStationName = par_val[i] ;
            else if (par_key[i][1] == 't') terminalStationName = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
            else priority = strcmp (par_val[i], "time") == 0 ? 0 : 1 ;
        }

        vector<int> start_pos, end_pos, train_pos ;
        trainStations.find (my_data (startStationName, 0), start_pos) ;
        trainStations.find (my_data (terminalStationName, 0), end_pos) ;
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

        if (train_pos.size() == 0) {
            printf("0\n"); return ;
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

            //cout << tickets[ticket_cnt - 1] << " travelling time: " << tickets[ticket_cnt - 1].getTravellingTime() << endl ; 
        }

        if (priority) sort (tickets, tickets + ticket_cnt, cmp_cost) ;
        else sort (tickets, tickets + ticket_cnt, cmp_time) ;

        printf("%d\n", ticket_cnt) ;
        for (int i = 0; i < ticket_cnt; i ++)
            tickets[i].print() ;
        
        delete [] tickets ;
    }

    void query_transfer () {
        const char *startStationName, *terminalStationName, *date ;
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
            char stationName[110] ;
            memset (stationName, 0, sizeof stationName) ;
            stationio.seekg (sizeof (int) + i * sizeof (stationName), ios::beg) ;
            stationio.read (reinterpret_cast<char *>(&stationName), sizeof stationName) ;

            if (strcmp (stationName, startStationName) == 0 || strcmp (stationName, terminalStationName) == 0) continue ;

            vector<int> pos ;
            trainStations.find (my_data (stationName, 0), pos) ;
            
            vector<int> train_1, train_2 ;
            for (int j = 0; j < pos.size(); j ++) {
                train cur_train = train_read (pos[j]) ;
                if (!cur_train.getReleaseStatus()) continue ;
                if (cur_train.runningFromTo (startStationName, stationName) && cur_train.runningOnDate (date, startStationName))
                    train_1.push_back (pos[j]) ;
                if (cur_train.runningFromTo (stationName, terminalStationName))
                    train_2.push_back (pos[j]) ;
            }

            //printf("中转站: %s %d %d\n", stationName, train_1.size(), train_2.size()) ;

            for (int j = 0; j < train_1.size(); j ++)
                for (int k = 0; k < train_2.size(); k ++) {
                    if (train_1[j] == train_2[k]) continue ;
                    train train1 = train_read (train_1[j]), train2 = train_read (train_2[k]) ;
                    Time train1_startTime = train1.getStartTime (date, startStationName) ;
                    Time train1_arrivingTime = train1.arrivingTime (train1_startTime, stationName) ;
                    //cout << "train1 start time: " << train1_startTime << " arriving time: " << train1_arrivingTime << endl ;
                    //cout << train2.runningAfterTime (train1_arrivingTime, stationName) << endl ;
                    if (!train2.runningAfterTime (train1_arrivingTime, stationName)) continue ;
                    Time train2_startTime = train2.getStartTimeAfterTime (train1_arrivingTime, stationName) ;
                    Time train2_arrivingTime = train2.arrivingTime (train2_startTime, terminalStationName) ;
                    int travellingTime = train2_arrivingTime.calTimeInterval (train1.leavingTime (train1_startTime, startStationName)) ;
                    //cout << train1_startTime << " " << train2_arrivingTime << " " << travellingTime << endl ;
                    int price1 = train1.calPrice (startStationName, stationName) ;
                    int price2 = train2.calPrice (stationName, terminalStationName) ;

                    if (priority == 0 && (travellingTime < cost || (travellingTime == cost && train1.getTravellingTime (startStationName, stationName) < order_1.getTravellingTime()))) {
                        cost = travellingTime ;
                        order_1 = ticket (train1.getTrainID(), startStationName, stationName, 
                        train1.leavingTime (train1_startTime, startStationName), 
                        train1_arrivingTime, 
                        price1, 
                        train1.calSeatNum (train1_startTime, startStationName, stationName), 
                        train1.getTravellingTime (startStationName, stationName), 
                        success) ;
                        order_2 = ticket (train2.getTrainID(), stationName, terminalStationName, 
                        train2.leavingTime (train2_startTime, stationName), 
                        train2_arrivingTime, 
                        price2, 
                        train2.calSeatNum (train2_startTime, stationName, terminalStationName), 
                        train2.getTravellingTime (stationName, terminalStationName), 
                        success) ;
                    }
                    
                    if (priority == 1 && (price1 + price2 < cost || (price1 + price2 == cost && train1.getTravellingTime (startStationName, stationName) < order_1.getTravellingTime()))) {
                        cost = price1 + price2 ;
                        order_1 = ticket (train1.getTrainID(), startStationName, stationName, 
                        train1.leavingTime (train1_startTime, startStationName), 
                        train1_arrivingTime, 
                        price1, 
                        train1.calSeatNum (train1_startTime, startStationName, stationName), 
                        train1.getTravellingTime (startStationName, stationName), 
                        success) ;
                        order_2 = ticket (train2.getTrainID(), stationName, terminalStationName, 
                        train2.leavingTime (train2_startTime, stationName), 
                        train2_arrivingTime, 
                        price2, 
                        train2.calSeatNum (train2_startTime, stationName, terminalStationName), 
                        train2.getTravellingTime (stationName, terminalStationName), 
                        success) ;
                    }
                }
        }

        if (cost == 1e9) printf("0\n") ;
        else {
            order_1.print(); order_2.print() ;
        }
    }

    void buy_ticket () {
        const char *username, *trainID, *startStationName, *terminalStationName, *date ;
        int ticketNum = 0 ;
        bool q = 0 ;
        for (int i = 1; i <= par_cnt; i ++) {
            if (par_key[i][1] == 'u') username = par_val[i] ;
            else if (par_key[i][1] == 'i') trainID = par_val[i] ;
            else if (par_key[i][1] == 'd') date = par_val[i] ;
            else if (par_key[i][1] == 'f') startStationName = par_val[i] ;
            else if (par_key[i][1] == 't') terminalStationName = par_val[i] ;
            else if (par_key[i][1] == 'n') ticketNum = strToInt (par_val[i]) ;
            else if (par_key[i][1] == 'q') q = strcmp (par_val[i], "true") == 0 ? 1 : 0 ;
        }

        vector<int> pos ;
        curUsers.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;
        user cur_user = user_read (pos[0]) ;

        pos.clear() ;
        trains.find (my_data (trainID, 0), pos) ;
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
            orders.insert (my_data (username, write_pos)) ;
            pendingOrders.insert (my_data (trainID, write_pos)) ;
            printf("queue\n") ;
            //printf("pending orders insert %s %d\n", trainID, write_pos) ;
        } else {
            int write_pos = order_write (cur_order) ;
            orders.insert (my_data (username, write_pos)) ;
            cur_train.sellSeats (trainStartTime, startStationName, terminalStationName, ticketNum) ;
            printf("%lld\n", order_price) ;
            train_write (train_file_pos, cur_train) ;
        }

        //cur_train.print (trainStartTime.getDateString()) ;
        
    }

    void query_order () {
        if (par_cnt != 1 || par_key[1][1] != 'u') throw "command wrong format" ;
        const char *username = par_val[1] ;
        vector<int> pos ;
        curUsers.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;

        pos.clear() ;
        orders.find (my_data (username, 0), pos) ;
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
        const char *username ;
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
        curUsers.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "user not logged in" ;

        pos.clear() ;
        orders.find (my_data (username, 0), pos) ;
        if (pos.empty()) throw "no orders" ;
        if (pos.size() < order_num) throw "there is no nth order" ;
        reverse (pos.begin(), pos.end()) ;

        ticket cur_order = order_read (pos[order_num - 1]) ;
        if (cur_order.getStatus() == refunded) throw "can't refund" ;
        
        bool is_cur_order_success = cur_order.getStatus() == success ;
        if (cur_order.getStatus() == pending)
            pendingOrders.erase (my_data (cur_order.getTrainID(), pos[order_num - 1])) ;
        cur_order.setStatus (refunded) ;
        order_write (pos[order_num - 1], cur_order) ;

        pos.clear() ;
        const char *trainID = cur_order.getTrainID() ;
        trains.find (my_data (trainID, 0), pos) ;
        int train_file_pos = pos[0] ;
        train cur_train = train_read (train_file_pos) ;
        if (is_cur_order_success) {
            Time trainStartTime = cur_train.getStartTime (cur_order.getLeavingTime(), cur_order.getFromStation()) ;
            cur_train.addSeats (trainStartTime, cur_order.getFromStation(), cur_order.getToStation(), cur_order.getSeatNum()) ;
            train_write (train_file_pos, cur_train) ;
        }

        pos.clear() ;
        pendingOrders.find (my_data (trainID, 0), pos) ;
        for (int i = 0; i < pos.size(); i ++) {
            ticket waiting_order = order_read (pos[i]) ;
            Time trainStartTime = cur_train.getStartTime (waiting_order.getLeavingTime(), waiting_order.getFromStation()) ;
            int remaining_seat_num = cur_train.calSeatNum (trainStartTime, waiting_order.getFromStation(), waiting_order.getToStation()) ;
            if (remaining_seat_num >= waiting_order.getSeatNum()) {
                waiting_order.setStatus (success) ;
                cur_train.sellSeats (trainStartTime, waiting_order.getFromStation(), waiting_order.getToStation(), waiting_order.getSeatNum()) ;
                pendingOrders.erase (my_data (trainID, pos[i])) ;
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
    }

} ;

#endif
