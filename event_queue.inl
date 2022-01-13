//
// Created by novasurfer on 1/13/22.
//

#include "event_queue.h"

namespace sc
{

    template<typename T>
    void event_queue::write(EventType type, const T& event)
    {
        write(type, sizeof(T), &event);
    }

    void event_queue::write(EventType type, u32 size, const void* event)
    {
        constexpr static size_t header_size = sizeof(EventHeader);
        static size_t event_idx;
        const size_t offset = header_size + size;

        EventHeader event_header;
        event_header.type = type;
        event_header.size = size;

        stream.insert_at((char*)&event_header, event_idx, header_size);
        stream.insert_at((char*)event, event_idx + header_size, size);

        event_idx += offset;
    }

    inline constexpr size_t event_queue::len() const
    {
        return stream.len();
    }

    inline constexpr const vec<char>& event_queue::get() const
    {
        return stream;
    }
}
