//
// Created by 罗皓天 on 2021/5/11.
//

#ifndef TICKETSYSTEM_DATA_H
#define TICKETSYSTEM_DATA_H
#include <iostream>
#include <cstring>
#define datasize 200

using namespace std;
struct data{
    char str[datasize];
    int pos;
    data(){
        str[0] = '\0';
//        memset(str , 0 ,sizeof str)
        pos = -1;
    }
    data(const char * str_ , int pos_){
        strcpy(str , str_);
        pos = pos_;
    }
    void print() const{
        std::cout << "str: " << str << "pos: " <<  pos << endl;
    }
    bool operator < (const data & other) const{
        if (strcmp(str , other.str) == 0)return pos < other.pos;
        return strcmp(str , other.str) < 0;
    }
    bool operator == (const data & other) const {
        return strcmp(str , other.str) == 0 && pos == other.pos;
    }
    bool operator <= (const data & other) const {
        if (strcmp(str , other.str) == 0)return pos <= other.pos;
        return strcmp(str , other.str) <= 0 ;
    }
};
#endif //TICKETSYSTEM_DATA_H
