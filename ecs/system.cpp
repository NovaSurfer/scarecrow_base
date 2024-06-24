//
// Created by max on 5/25/23.
//

#include "system.h"
#include "heap_alloc.h"
#include "entity.h"

namespace sc2d
{

    system::system(init_fn_ptr init, update_fn_ptr update, sc::heap_alloc& halloc)
        : init_fn {init}
        , update_fn {update}
        , regist{halloc}
    { }

    void system::init()
    {
        init_fn();
    }

    void system::update(f32 delta_time)
    {
        update_fn(delta_time);
    }

    void system::add_entity(entity e)
    {
        regist.push(e);
    }

    void system::remove_entity(entity e)
    {
        for(size_t i = 0; i < regist.len(); ++i) {
            if(e.id == regist[i].id) {
                size_t last = regist.len() - 1;
                regist[last].id ^= regist[i].id;
                regist[i].id ^= regist[last].id;
                regist[last].id ^= regist[i].id;
                regist.pop_back();
            }
        }
    }

    component_mask system::get_signarute()
    {
        return signature;
    }
}
