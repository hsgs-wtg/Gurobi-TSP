#include <iostream>

#include "gurobi_c++.h"

int main(int argc, char** argv){
	try {
		// Create the main model
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);


	}
	catch (GRBException e) {
		std::cout << "Error " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...) {
		std::cout << "An error has occured" << std::endl;
	}
}
