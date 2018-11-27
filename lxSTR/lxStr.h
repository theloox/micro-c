/***************************************************************************
 *   2004-2006 by Axel Gonzalez                                            *
 *   loox@e-shell.net                                                      *
 *                                                                         *
 *   This software is in the public domain. Permission to use, copy,       *
 *   modify, and distribute this software and its documentation for any    *
 *   purpose and without fee is hereby granted, without any conditions or  *
 *   restrictions. This software is provided "as is" without express or    *
 *   implied warranty.                                                     *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <time.h>

#ifndef _LXSTR_H
#define _LXSTR_H

#ifndef LXSTR_MAX_LENGHT
#define LXSTR_MAX_LENGHT 10480000
#endif

#ifndef LXSTR_ALLOC
#define LXSTR_ALLOC 128
#endif


class lxStr {
private:
    char *vStr;
    int vLen;
    int vAlloc;

    void Alloc(int n);
    void ReAlloc(int n);
    void Free();


public:
    lxStr();
    lxStr(lxStr &str);
    lxStr(const lxStr &str);
    lxStr(const char *str);
    lxStr(char c);
    ~lxStr();


    lxStr & Store(lxStr &str);
    lxStr & Store(const char *str);
    lxStr & Store(const char *str, size_t n);
    lxStr & Store(int n);
    lxStr & Store(double n);

    void Clear();


    //char *xstr(lxStr str);
    int printf(lxStr str, ...);

    size_t Len();
    size_t Length();

    //case insensitive comparisson
    bool IsEqual(lxStr &str);
    bool IsEqual(const char *str);
    bool StartsWith(const char *str);

    lxStr lower();
    lxStr upper();

    void trim();
    void ltrim();
    void rtrim();

    void strip_tags();
    void strip_sql();

    void strcat(lxStr &str);
    void strcat(char c);
    void strcat(const char *str,int n=LXSTR_MAX_LENGHT);

    lxStr substr(int start,int lenght = 0);

    void replace(const char search, lxStr &replace);
    void replace(const char search, const char *replace);
    void replace(lxStr &search, lxStr &replace);
    void replace(lxStr &search, const char *replace);
    void replace(const char *search, lxStr &replace);
    void replace(const char *search, const char *replace);

    int strpos(lxStr &tok, int start = 0);
    int strpos(const char *tok, int start = 0);
    int strpos(const char tok, int start = 0);

    int rstrpos(lxStr &tok, int start = 0);
    int rstrpos(const char *tok, int start = 0);
    int rstrpos(const char tok, int start = 0);

    lxStr strtokl(lxStr &tok);
    lxStr strtokl(const char *tok);
    lxStr strtokr(lxStr &tok);
    lxStr strtokr(const char *tok);
    lxStr rstrtokl(lxStr &tok);
    lxStr rstrtokl(const char *tok);
    lxStr rstrtokr(lxStr &tok);
    lxStr rstrtokr(const char *tok);

    int Int();
    double Double();


    lxStr & operator = (lxStr &str);
    lxStr & operator = (const char *str);
    lxStr & operator = (int n);
    lxStr & operator = (double n);

    lxStr operator + (lxStr &str);
    lxStr operator + (const char *str);
    lxStr operator += (lxStr &str);
    lxStr operator += (const char *str);


    //this are case insensitive!
    //for case sensitive use IsEqual()

    bool operator == (lxStr str);
    bool operator == (const char *str);
    bool operator != (lxStr str);
    bool operator != (const char *str);

    bool operator > (lxStr str);
    bool operator > (const char *str);

    bool operator < (lxStr str);
    bool operator < (const char *str);

    operator char *();

    void ReadFile(lxStr file);

};


//auxiliar functions
int strnlen(const char *s,int n=LXSTR_MAX_LENGHT);
inline bool samechar(char s, char d);


#endif

