//
// Created by novasurfer on 9/3/21.
//

#ifndef SC_BASE_TEST_SHARED_TEST_CPP_H
#define SC_BASE_TEST_SHARED_TEST_CPP_H


#include "../shared.h"
#include "doctest/doctest.h"

TEST_CASE("shared-ptr-operations")
{
    SUBCASE("default constructor")
    {
        MESSAGE(sizeof(sc::shared<int*>));
    }

    SUBCASE("make-and-reset-shared")
    {
        /*
        auto shared_1 = sc::make_shared<int>(47);
        auto shared_2 = shared_1;

        {
            const auto& shared_3 = shared_1;

            MESSAGE(shared_1.use_count());
            MESSAGE(shared_3.use_count());

        }

        shared_2.reset();
        */
    }
}

#endif //SC_BASE_TEST_SHARED_TEST_CPP_H
