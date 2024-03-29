//
// Created by novasurfer on 1/13/22.
//

#ifndef SC_BASE_TEST_EVENT_QUEUE_H
#define SC_BASE_TEST_EVENT_QUEUE_H

#include "sc_types.h"
#include "heap_alloc.h"
#include "vec.h"

enum class EventType;

namespace sc
{
    struct EventHeader
    {
        EventType type;
        u32 size;
    };

    /**
     * [event_1_enum] [event_1_data] [event_2_enum] [event_2_data]
     */
    class event_queue
    {
    public:
        constexpr event_queue(allocator& alloc)
            : stream(alloc)
        { }

        template <typename T>
        void write(EventType type, T& event);

        constexpr size_t len() const;

        constexpr const vec<char>& get() const;

    private:
        vec<char> stream;

        void write(EventType type, u32 size, void* event);
    };
}

#include "event_queue.inl"

#endif //SC_BASE_TEST_EVENT_QUEUE_H
