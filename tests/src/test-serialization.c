#include <stdlib.h>
#include <stdio.h>
#include <cspecs/cspec.h>

context (test_serialization) {

    describe ("t_string serialization test") {

        it ("serialization-deserialization ") {
            should_bool(true) be equal to(true);
        } end

    } end

}