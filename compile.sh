#!/bin/bash

g++ -m64 -g main.cpp -o main -I$GUROBI_HOME/include -L$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm
