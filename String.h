//
// Created by 罗皓天 on 2021/6/1.
//

#ifndef CODE_ST_RING_H
#define CODE_ST_RING_H

#include <cstring>

class string {
private:
    char str[110];
    int len = 0;//记录字母个数，有一个记一个
public:
    string(const char *ch){
        int i = 0;
        while (ch[i] != '\0'){
            str[i] = ch[i];
            i++;
        }
        len = i;
    }
    string(){
        memset(str , 0 , sizeof str);
        len = 0;
    }
    string(const string &ot){
        for (int i = 0; i < ot.len; ++i) {
            str[i] = ot.str[i];
        }
        len = ot.len;
    }
    string& operator=(const string &ot){
        if (this == &ot){
            return *this;
        }
        for (int i = 0; i < len; ++i) {
            str[i] = '\0';
        }
        len = ot.len;
        strcpy(str , ot.str);
        str[len] = '\0';
        return *this;
    }
    bool operator<(string &ot){
        return strcmp(this->str , ot.str) < 0;
    }
    bool operator<=(string &ot){
        return strcmp(this->str , ot.str) <= 0;
    }
    bool operator==(string &ot){
        return strcmp(this->str , ot.str) == 0;
    }
    bool operator>(string &ot){
        return strcmp(this->str , ot.str) > 0;
    }
    bool operator>=(string &ot){
        return strcmp(this->str , ot.str) >= 0;
    }
};
#endif //CODE_ST_RING_H
