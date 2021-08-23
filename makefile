all: clean
	make -C shared
	make -C server
	make -C user

clean:
	make clean -C shared
	make clean -C server
	make clean -C user