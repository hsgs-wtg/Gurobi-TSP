CPL				= g++
MAIN			= src/main.cpp

build: $(MAIN)
	$(CPL) -m64 -g $(MAIN) -o main -I$(GUROBI_HOME)/include -L$(GUROBI_HOME)/lib -lgurobi_c++ -lgurobi100 -lm

run: main
	./main

test: build run
