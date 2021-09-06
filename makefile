all: clean
    make -C shared
    make -C server
    make -C user
    make -C tests

clean:
    make clean -C shared
    make clean -C server
    make clean -C user
    make clean -C tests

test: all
    valgrind --leak-check=full ./tests/bin/tests.out
