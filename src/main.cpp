#include <iostream>
#include "../include/instance.h"
#include "../include/solver.h"
#include <memory>
int main() {
    std::string file_path = "/home/cuong/CLionProjects/CTSP/ctsp_formatted_instances/eil76_p3_f90_lL.contsp";
    auto instance = std::make_shared<Instance>(file_path);
    auto solver = std::make_shared<Solver>(instance);

    solver->Solve();

}
