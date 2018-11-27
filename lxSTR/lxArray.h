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

#include "lxStr.h"

#ifndef _LXARRAY_H
#define _LXARRAY_H

#ifndef LXARRAY_ALLOC
#define LXARRAY_ALLOC 64
#endif


//#define X int
template <class X>
class lxArrayElement {
public:
    unsigned int raw;
    int index;
    lxStr *key;
    X *value;
};

template <class X>
class lxArray {

private:
    unsigned int n;
    unsigned int m;
    unsigned int vAlloc;
    lxArrayElement<X> *vArray;
    int id;

    int _Alloc(int z);
    int _ReAlloc(int z);
    int _Free();


public:

    lxArray();
    lxArray(lxArray &array);
    lxArray(const lxArray &array);

    ~lxArray();

    int Alloc(unsigned int z);

    int Add(X x);

    int Store(X x, unsigned int key);
    int Store(X x, lxStr key);


    void Clean();
    int Count();
    bool IsEmpty();

    int Key(unsigned int index);
    int Key(lxStr key);
    X Value(unsigned int index);
    X Value(lxStr key);

    lxArray & operator = (lxArray &array);
    lxArray & operator = (const lxArray &array);

    X operator [](unsigned int index);
    X operator [](const char *key);

};

#endif
