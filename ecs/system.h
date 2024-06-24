//
// Created by max on 5/4/23.
//

#ifndef SCARECROW2D_SYSTEM_H
#define SCARECROW2D_SYSTEM_H

#include "sc_types.h"
#include "component.h"
#include "vec.h"

namespace sc
{
    struct heap_alloc;
}

namespace sc2d
{

    class system
    {
        using init_fn_ptr = void (*)();
        using update_fn_ptr = void (*)(f32);

    public:
        system(init_fn_ptr init, update_fn_ptr update, sc::heap_alloc& lalloc);
        void init();
        void update(f32 delta_time);
        void add_entity(entity e);
        void remove_entity(entity e);
        component_mask get_signarute();

    private:
        init_fn_ptr init_fn;
        update_fn_ptr update_fn;

        sc::vec<entity> regist;
        component_mask signature;
    };
}

#endif //SCARECROW2D_SYSTEM_H
