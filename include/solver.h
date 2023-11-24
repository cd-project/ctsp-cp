//
// Created by cuong on 17/11/2023.
//

#ifndef CTSP_SOLVER_H
#define CTSP_SOLVER_H

#include "instance.h"
#include "ortools/base/logging.h"
#include "ortools/sat/cp_model.h"
#include "ortools/sat/cp_model_solver.h"
#include "ortools/util/sorted_interval_list.h"
#include <memory>
class Solver {
public:
    std::shared_ptr<Instance> instance;
    explicit Solver(std::shared_ptr<Instance> &inst) {
        instance = inst;
    }

    void Solve();

};
#endif //CTSP_SOLVER_H
