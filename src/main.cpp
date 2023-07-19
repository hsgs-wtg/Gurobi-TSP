#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cassert>

#include "gurobi_c++.h"

#include "constraints.cpp"

std::vector<std::vector<int>> get_data(int n){
	std::ifstream input("data/" + std::to_string(n) + ".txt");
	std::vector<std::vector<int>> a(n, std::vector<int> (n));
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++)
			input >> a[i][j];
		a[i][i] = 1;
	}
	input.close();
	return a;
}

bool single_iteration(GRBModel &model, std::vector<std::vector<GRBVar>> &b){
	int n = b.size();
	model.optimize();
	std::cout << "    Objective: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
	std::vector<int> mapping = get_mapping(b);
	std::vector<int> min_cycle = get_min_cycle(mapping);
	std::cout << "    Minimum cycle length: " << min_cycle.size() << std::endl;
	if (min_cycle.size() == n) return true;
	constraint_3(model, b, min_cycle);
	return false;
}

int main(){
	try {
		// Create the main model
		GRBEnv env = GRBEnv();
		env.set("OutputFlag", "0");
		GRBModel model = GRBModel(env);
		std::cout << "Starting" << std::endl;
		std::vector<std::vector<int>> a = get_data(58);
		std::vector<std::vector<GRBVar>> b = chosen_matrix(model, a);
		constraint_12(model, b);
		for (int i=1; true; i++){
			std::cout << "Iteration No." << i << ": " << std::endl;
			bool done = single_iteration(model, b);
			if (done){
				std::cout << "Done" << std::endl;
				break;
			}
		}
		assert(model.get(GRB_IntAttr_Status) == GRB_OPTIMAL);
		std::cout << "RESULT:\n";
		std::cout << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
		std::vector<int> mapping = get_mapping(b);
		for (int f: mapping)
			std::cout << f << " ";
		std::cout << "\n";
	}
	catch (GRBException e) {
		std::cout << "Error " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...) {
		std::cout << "An error has occured" << std::endl;
	}
}
