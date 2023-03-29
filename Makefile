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
	g++ ${COMPILE_FLAGS} -O0 -Wall -std=gnu++2a -o test *.cpp include/SHA256/*.cpp

run:
	./test

runi:
	./test -i

windows_compile:
	C:\cygwin64\bin\g++.exe -O0 -std=gnu++2a -o test.exe *.cpp include/SHA256/*.cpp