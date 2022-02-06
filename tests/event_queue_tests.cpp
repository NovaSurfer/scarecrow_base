//
// Created by novasurfer on 1/13/22.
//

#include "doctest/doctest.h"
#include "../event_queue.h"
#include "../heap_alloc.h"
#include "../types.h"

using namespace sc;

enum class EventType
{
    BUTTON_PRESSED
};

struct ButtonPressedEvent
{
    u32 keycode;
    double test_data;
};

TEST_CASE("event-stream")
{

    heap_alloc halloc;
    event_queue events(halloc);

    ButtonPressedEvent ev {455, 5.6};
    ButtonPressedEvent ev2 {121, 3.5};

    events.write(EventType::BUTTON_PRESSED, ev);
    events.write(EventType::BUTTON_PRESSED, ev2);

    const size_t size = events.len();
    u32 read = 0;

    while(read < size) {
        const EventHeader* header = (EventHeader*)&events.get()[read];
        const char* data = (char*)&header[1];
        read += sizeof(*header) + header->size;

        switch(header->type) {
        case EventType::BUTTON_PRESSED: {
            const ButtonPressedEvent btnev = *(ButtonPressedEvent*)data;
            MESSAGE(btnev.keycode);
            MESSAGE(btnev.test_data);
        } break;
        default:
            MESSAGE("NO SUCH EVENT");
            break;
        }
    }
}
