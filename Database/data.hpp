#ifndef data_type
#define data_type

#include <cstdio>
#include <cstring>

struct my_data {
    char str[110]; int pos ;
    my_data () {
        memset (str, 0, sizeof str) ;
        pos = -1 ;
    }
    my_data (const char *_str, int p) {
        memset (str, 0, sizeof str) ;
        strcpy (str, _str) ;
        pos = p ;
    }
    void print() const {
        printf("str:%s pos:%d\n", str, pos) ;
    }
    bool operator < (const my_data &a) const {
        if (strcmp (str, a.str) == 0) return pos < a.pos ;
        return strcmp (str, a.str) < 0 ;
    }
    bool operator <= (const my_data &a) const {
        if (strcmp (str, a.str) == 0) return pos <= a.pos ;
        return strcmp (str, a.str) < 0 ;
    }
    bool operator == (const my_data &a) const {
        return strcmp (str, a.str) == 0 && pos == a.pos ;
    }
} ;

#endif