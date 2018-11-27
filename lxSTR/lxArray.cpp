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


#include "lxArray.h"

template <class X>
lxArray<X>::lxArray()
{
    this->n = 0;
    this->m = 0;

    this->vAlloc = 0;
    this->vArray = NULL;
}


template <class X>
lxArray<X>::lxArray(lxArray &array)
{
    unsigned int i;

    this->n = array.n;
    this->m = array.m;

    this->vAlloc = 0;
    this->vArray = NULL;

    if (!Alloc(array.n))
        return;

    for (i = 0; i < array.n; i++)
    {
        this->vArray[i].raw = array.vArray[i].raw;
        this->vArray[i].index = array.vArray[i].index;

        if (array.vArray[i].key != NULL)
            this->vArray[i].key = new lxStr(*array.vArray[i].key);

        if (array.vArray[i].value != NULL)
            this->vArray[i].value = new X(*array.vArray[i].value);
    }
}

template <class X>
lxArray<X>::lxArray(const lxArray &array)
{
    unsigned int i;

    this->n = array.n;
    this->m = array.m;

    this->vAlloc = 0;
    this->vArray = NULL;

    if (!Alloc(array.n))
        return;

    for (i = 0; i < array.n; i++)
    {
        this->vArray[i].raw = array.vArray[i].raw;
        this->vArray[i].index = array.vArray[i].index;

        if (array.vArray[i].key != NULL)
            this->vArray[i].key = new lxStr(*array.vArray[i].key);

        if (array.vArray[i].value != NULL)
            this->vArray[i].value = new X(*array.vArray[i].value);
    }
}

template <class X>
lxArray<X>::~lxArray()
{
    Clean();
}

template <class X>
int lxArray<X>::_Alloc(int z)
{
    unsigned int i;


    if (z <= 0)
        return(0);

    if (vAlloc != 0)
        return(_ReAlloc(z));

    vAlloc = (z % LXARRAY_ALLOC ? z + (LXARRAY_ALLOC - (z % LXARRAY_ALLOC)) : z);

    vArray = (lxArrayElement<X> *)calloc(vAlloc, sizeof(lxArrayElement<X>));

    if (vArray == NULL)
        return(0);

    for (i = 0; i < vAlloc; i++)
    {
        vArray[i].raw = i;
        vArray[i].index = -1;
        vArray[i].key = NULL;
        vArray[i].value = NULL;
    }

    return(n);
}

template <class X>
int lxArray<X>::_ReAlloc(int z)
{
    int y;
    unsigned int i;
    unsigned int t;


    y = n + z;

    t = (y % LXARRAY_ALLOC ? y + (LXARRAY_ALLOC - (y % LXARRAY_ALLOC)) : y);


    if (t <= vAlloc)
        return(0);
    else
        vAlloc = t;

    vArray = (lxArrayElement<X> *)realloc(vArray, vAlloc * sizeof(lxArrayElement<X>));

    if (vArray == NULL)
        return(0);

    for (i = y - z; i < vAlloc; i++)
    {
        vArray[i].raw = i;
        vArray[i].index = -1;
        vArray[i].key = NULL;
        vArray[i].value = NULL;
    }

    return(z);
}


template <class X>
int lxArray<X>::Add(X x)
{
    Alloc(1);

    vArray[n].index = m;
    vArray[n].value = new X(x);

    n++;
    m++;

    return(1);
}


template <class X>
int lxArray<X>::Alloc(unsigned int z)
{
    return(_Alloc(z));
}


template <class X>
void lxArray<X>::Clean()
{
    unsigned int i;

    for (i = 0; i < n; i++) {
        if (vArray[i].key != NULL)
            delete(vArray[i].key);
        if (vArray[i].value != NULL)
            delete(vArray[i].value);
    }

    if (vAlloc != 0) {
        vAlloc = 0;
        free(vArray);
    }

    n = 0;
    m = 0;
}


template <class X>
int lxArray<X>::Count()
{
    return(this->n);
}


template <class X>
bool lxArray<X>::IsEmpty()
{
    return(this->Count() ? true : false);
}


template <class X>
int lxArray<X>::Store(X x, unsigned int key)
{
    int k;


    k = Key(key);

    if (k < 0)
        return(0);

    delete(vArray[k].value);
    vArray[k].value = new X(x);

    return(1);
}


template <class X>
int lxArray<X>::Store(X x, lxStr key)
{
    int k;


    k = Key(key);

    if (k < 0)
        return(0);

    delete(vArray[k].value);
    vArray[k].value = new X(x);

    return(1);
}


//operatorz
template <class X>
lxArray<X> & lxArray<X>::operator = (lxArray &array)
{
    int i;


    this->Clean();

    this->n = array.n;
    this->m = array.m;

    _Alloc(m);

    for(i = 0; i < m; i++) {
        this->vArray[i].raw = array.vArray[i].raw;
        this->vArray[i].index = array.vArray[i].index;

        if (array.vArray[i].key != NULL)
            this->vArray[i].key = new lxStr(*array.vArray[i].key);

        if (array.vArray[i].value != NULL)
            this->vArray[i].value = new X(*array.vArray[i].value);

    }

    return(*this);
}


template <class X>
lxArray<X> & lxArray<X>::operator = (const lxArray &array)
{
    int i;
    int z;


    this->Clean();

    z = array.n;

    _Alloc(z);

    this->n = array.n;
    this->m = array.m;


    for(i = 0; i < z; i++) {
        this->vArray[i].raw = array.vArray[i].raw;
        this->vArray[i].index = array.vArray[i].index;

        if (array.vArray[i].key != NULL)
            this->vArray[i].key = new lxStr(*array.vArray[i].key);

        if (array.vArray[i].value != NULL)
            this->vArray[i].value = new X(*array.vArray[i].value);
    }

    return(*this);
}


template <class X>
int lxArray<X>::Key(unsigned int index)
{
    int i;


    if (index < 0)
        return(-1);

    if (index > m)
        return(-1);

    if (vArray[index].raw == vArray[index].index)
        return (vArray[index].raw);


    // FIXME This can be optimized... binary search ?
    for (i = m; i < n; i++) {
        if (vArray[i].index == index)
            return (i);
    }

    return(-1);
}


template <class X>
int lxArray<X>::Key(lxStr key)
{
    int i;


    // FIXME This can be optimized... binary search ?
    for (i = 0; i < n; i++) {

        if (vArray[i].key == NULL)
            continue;

        if (vArray[i].key->Len() == 0)
            continue;

        if (*vArray[i].key == key)
            return (vArray[i].raw);
    }

    return(-1);
}

template <class X>
X lxArray<X>::Value(unsigned int index)
{
    int i;
    X ret;

    if (Count() == 0)
        return(ret);

    if (index < 0)
        return(ret);

    if (index > m)
        return(ret);

    if (vArray[index].raw == vArray[index].index)
        return (*vArray[index].value);

    // FIXME This can be optimized... binary search ?
    for (i = m; i < n; i++) {
        if (vArray[i].index == index)
            return (*vArray[i].value);
    }

    return(ret);
}


template <class X>
X lxArray<X>::Value(lxStr key)
{
    int i;
    X ret;


    // FIXME This can be optimized... binary search ?
    for (i = 0; i < n; i++) {

        if (vArray[i].key == NULL)
            continue;

        if (vArray[i].key->Len() == 0)
            continue;

        if (*vArray[i].key == key)
            return (*vArray[i].value);
    }

    return(ret);
}


template <class X>
X lxArray<X>::operator [](unsigned int index)
{
    return(Value(index));
}


template <class X>
X lxArray<X>::operator [](const char *key)
{
    return(Value(key));
}


#if 0
lxArray lxArray::QSort()
{
    lxArray ret;

    ret = *this;

    quicksort(ret, 0, n - 1, n);

    return(ret);
}


void lxArray::quicksort(lxArray &array, int low, int high, int order)
{
    int bottom, top;
    lxStr pivot;
    lxStr temp;

    if (high > low)
    {
        pivot = array[high];
        bottom = low - 1;
        top = high;

        while(1)
        {
            while((array[++bottom] < pivot) && (bottom < order));
            while((array[--top] > pivot) && (top > 0));

            if (bottom >= top) break;

            temp = array[bottom];
            array.Store(array[top], bottom);
            array.Store(temp, top);
        }

        temp = array[bottom];
        array.Store(array[high], bottom);
        array.Store(temp, high);

        quicksort(array, low, bottom - 1, order);
        quicksort(array, bottom + 1, high, order);
    }
}
#endif
