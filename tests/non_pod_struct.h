//
// Created by maksim.ruts on 2/10/2022.
//

#ifndef SC_BASE_NON_POD_STRUCT_H
#define SC_BASE_NON_POD_STRUCT_H

#include "doctest/doctest.h"
#include <cstdlib>
#include <algorithm>

struct NonPod
{

    NonPod()
        : val(88.8)
        , bytes((char*)malloc(256))

    {
        MESSAGE("default ctr");
    }

    NonPod(double dval)
        : val(dval)
        , bytes((char*)malloc(256))
    {
        MESSAGE("ctr");
    }

    NonPod(double dval, size_t size)
        : val(dval)
        , bytes((char*)malloc(size))
    {
        MESSAGE("ctr 2");
    }

    NonPod(const NonPod& o)
        : val(o.val)
        , bytes((char*)malloc(256))
    {
        MESSAGE("copy ctr");
        std::copy(o.bytes, o.bytes + 256, bytes);
    }

    NonPod& operator=(const NonPod& o)
    {
        MESSAGE("assignment operator");
        if(this != &o) {
            // free(bytes);
            // bytes = nullptr;

            bytes = (char*)malloc(256);
            val = o.val;
            std::copy(o.bytes, o.bytes + 256, bytes);
        }
        return *this;
    }

    //NonPod(NonPod&&) = default;

    ~NonPod()
    {
        MESSAGE("dstr");
        free(bytes);
        bytes = nullptr;
    }

    char* bytes;
    double val;
};

#endif //SC_BASE_NON_POD_STRUCT_H
