#include <vector>
#include <sstream>

#include "gurobi_c++.h"

std::vector<std::vector<GRBVar>> chosen_matrix(GRBModel &model, std::vector<std::vector<int>> &matrix){
    int n = matrix.size();
    std::vector<std::vector<GRBVar>> x(n, std::vector<GRBVar> (n));
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            x[i][j] = model.addVar(0, 1, matrix[i][j], GRB_BINARY);
        }
    }
    return x;
}

std::vector<int> get_mapping(std::vector<std::vector<GRBVar>> &b){
    int n = b.size();
	std::vector<int> mapping(n);
	for (int i=0; i<n; i++){
		int value = -1;
		for (int j=0; j<n; j++){
			if (std::round(b[i][j].get(GRB_DoubleAttr_X))){
				value = j;
				break;
			}
		}
		mapping[i] = value;
	}
    return mapping;
}

std::vector<int> get_min_cycle(std::vector<int> &mapping){
    int n = mapping.size();
    unsigned long long unused = (1ULL << n) - 1;
	int min_length = 1000;
	std::vector<int> result;
	for (int i=0; i<n; i++){
		if (!(unused & (1ULL<<i)))
			continue;
		std::vector<int> cycle = {i};
        unused ^= 1ULL << i;
		for (int ptr = mapping[i]; ptr != i; ptr = mapping[ptr]){
			cycle.push_back(ptr);
            unused ^= 1ULL << ptr;
        }
		if (min_length > cycle.size()){
            min_length = cycle.size();
            result = std::move(cycle);
        }
	}
    return result;
}

void constraint_12(GRBModel &model, std::vector<std::vector<GRBVar>> &b){
    int n = b.size();
    for (int i=0; i<n; i++){
        GRBLinExpr row = 0, col = 0;
        for (int j=0; j<n; j++){
            if (i == j) continue;
            row += b[i][j];
            col += b[j][i];
        }
        model.addConstr(row == 1);
        model.addConstr(col == 1);
    }
}

void constraint_3(GRBModel &model, std::vector<std::vector<GRBVar>> &b, std::vector<int> &cyc){
    int n = cyc.size();
    GRBLinExpr cycle = 0, cycle_rev = 0;
    for (int i=0; i<cyc.size(); i++){
        cycle += b[cyc[i]][cyc[(i+1)%n]];
        cycle_rev += b[cyc[(i+1)%n]][cyc[i]];
    }
    model.addConstr(cycle <= cyc.size()-1);
    model.addConstr(cycle_rev <= cyc.size()-1);
}
