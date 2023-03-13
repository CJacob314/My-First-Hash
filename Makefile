.PHONY: compile run comprun
.DEFAULT_GOAL := comprun

comprun:
	make -s compile
	make -s run

compile:
	g++ -O0 -std=c++17 -o test *.cpp

run:
	./test