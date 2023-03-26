.PHONY: compile run comprun
.DEFAULT_GOAL := comprun

compruni:
	make -s compile
	make -s runi

comprun:
	make -s compile
	make -s run

# Use COMPILE_FLAGS to add things like -g for debugging
compile:
	g++ ${COMPILE_FLAGS} -O0 -std=c++17 -o test *.cpp include/SHA256/*.cpp

run:
	./test

runi:
	./test -i