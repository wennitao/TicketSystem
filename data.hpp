#ifndef data_type
#define data_type

#include <cstdio>
#include <cstring>

#include "string.h"

struct data {
    string str; int pos ;
    data () {
        pos = -1 ;
    }
    data (const char *_str, int p) {
        str = string (_str) ;
        pos = p ;
    }
    data (const string _str, int p) {
        str = _str; pos = p ;
    }
    void print() const {
        printf("str:%s pos:%d\n", str, pos) ;
    }
    bool operator < (const data &a) const {
        if (str == a.str) return pos < a.pos ;
        return str < a.str ;
    }
    bool operator <= (const data &a) const {
        if (str == a.str) return pos <= a.pos ;
        return str < a.str ;
    }
    bool operator == (const data &a) const {
        return str == a.str && pos == a.pos ;
    }
} ;

#endif