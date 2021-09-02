#include "utils/utils.h"
#include <cspecs/cspec.h>

context (test_buffer) {

    describe ("buffer packing tests") {

        t_buffer* buffer;

        before {
            buffer = buffer_create();
        } end

        after {
            buffer_destroy(buffer);
        } end

        it ("packing-unpacking a string is correct") {
            buffer_pack_string(buffer, "Test string :)");
            char* result = buffer_unpack_string(buffer);
            should_string(result) be equal to("Test string :)");
        } end

        it ("packing-unpacking a chat room is correct") {
            buffer_pack_chat_room(buffer, 0, "th3g3ntl3man");
            t_chat_room* chatRoom = (t_chat_room*) buffer_unpack_chat_room(buffer);
            should_int(chatRoom->roomID) be equal to(0);
            should_string(chatRoom->roomName) be equal to("th3g3ntl3man");
        } end

    } end
}
