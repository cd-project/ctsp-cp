//
// Created by cuong on 17/11/2023.
//

#include "../include/solver.h"


void Solver::Solve() {
    using namespace operations_research::sat;
    using namespace operations_research;

    auto n = instance->num_v;
    auto c = instance->travel_time;
    auto p = instance->period;
    auto dm = instance->demand;
    auto T = instance->max_diff;
    SatParameters sat;
    sat.set_log_search_progress(true);
    sat.set_max_time_in_seconds(3600);

    CpModelBuilder cp;
    LinearExpr obj;
    std::vector<std::vector<std::vector<BoolVar>>> x(p, std::vector<std::vector<BoolVar>>(n, std::vector<BoolVar>(n)));
    std::vector<std::vector<IntervalVar>> itv(p, std::vector<IntervalVar>(n));
    // creation of x
    for (int k = 0; k < p; k++) {
        for (int i = 0; i < n; i++) {
            if (dm[i][k] == 1) {
                for (int j = 0; j < n; j++) {
                    if (dm[j][k] == 1 && (i != j)) {
                        x[k][i][j] = cp.NewBoolVar();
                        std::cout << "created x for " << k << " " << i << " " << j << std::endl;
                        obj += x[k][i][j] * c[i][j];
                    }
                }
            }
        }
    }
    for (int k = 0; k < p; k++) {
        auto cc = cp.AddCircuitConstraint();
        std::cout << "circuit created for day " << k << std::endl;
        for (int i = 0; i < n; i++) {
            if (dm[i][k] == 1) {
                for (int j = 0; j < n; j++) {
                    if (dm[j][k] == 1 && (i != j)) {
                        cc.AddArc(j, i, x[k][i][j]);
                    }
                }
            }
        }
    }

    // creation of the interval
    auto t_domain = Domain({0, 9999999});
    for (int k = 0; k < p; k++) {
        for (int i = 0; i < n; i++) {
            if (dm[i][k] == 1) {
                IntVar start = cp.NewIntVar(t_domain);
                IntVar dur = cp.NewIntVar({instance->serve_time_bound[i].first, instance->serve_time_bound[i].second});
                IntVar end = cp.NewIntVar(t_domain);
                if (i == 0) {
                    cp.AddEquality(start, 0);
                }
                itv[k][i] = cp.NewIntervalVar(start, dur, end);
            }
        }
    }
//
    for (int k = 0; k < p; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dm[j][k] == 1 && dm[i][k] == 1 && (i != j)) {
                    if (j != 0) {
                        cp.AddEquality(itv[k][j].StartExpr(),itv[k][i].EndExpr()).OnlyEnforceIf(x[k][i][j]);
                        cp.AddEquality(itv[k][i].SizeExpr(), c[i][j]).OnlyEnforceIf(x[k][i][j]);
                    }
                }
            }
        }
    }

//  Time consistency constraint
    for (int i = 1; i < n; i++) {
        auto min_k = instance->customer_k_min[i];
        if (min_k == 0) {
            continue;
        }
        for (int k = min_k+1; k < p; k++) {
            if (dm[i][k] == 1) {
                cp.AddLessOrEqual(itv[min_k][i].StartExpr() - T, itv[k][i].StartExpr());
                cp.AddLessOrEqual(itv[k][i].StartExpr(), itv[min_k][i].StartExpr() + T);
            }
        }
    }
    

    cp.Minimize(obj);

    Model model;
    model.Add(NewSatParameters(sat));

    const CpSolverResponse response = SolveCpModel(cp.Build(), &model);

    if (response.status() == CpSolverStatus::OPTIMAL ||
        response.status() == CpSolverStatus::FEASIBLE) {
        LOG(INFO)
                        << "Best bound: " << response.best_objective_bound() << "; Best objective: "
                        << response.objective_value();

        for (int k = 0; k < p; k++) {
            std::cout << "Day " << k << std::endl;
            auto cost = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dm[i][k] == 1 && dm[j][k] == 1 && (i != j)) {
                        if (SolutionBooleanValue(response, x[k][i][j])) {
                            std::cout << i << " " << j << std::endl;
                            cost += c[i][j];
//                            std::cout << "start of " << i << " is: "<< SolutionIntegerValue(response, itv[k][i].StartExpr()) << std::endl;
//                            std::cout << "dur of " << i << " is: "<< SolutionIntegerValue(response, itv[k][i].SizeExpr()) << std::endl;
//                            std::cout << "end of " << i << " is: "<< SolutionIntegerValue(response, itv[k][i].EndExpr()) << std::endl;
//
//                            std::cout << "start of " << j << " is: "<< SolutionIntegerValue(response, itv[k][j].StartExpr()) << std::endl;
//                            std::cout << "dur of " << j << " is: "<< SolutionIntegerValue(response, itv[k][j].SizeExpr()) << std::endl;
//                            std::cout << "end of " << j << " is: "<< SolutionIntegerValue(response, itv[k][j].EndExpr()) << std::endl;


//                            std::cout << "serving time of " << i << std:: endl;/
//                            std::cout << SolutionIntegerValue(response, itv[k][i].StartExpr()) << ", " <<  SolutionIntegerValue(response, itv[k][i].SizeExpr()) << ", " << SolutionIntegerValue(response, itv[k][i].EndExpr()) << std::endl;
                        }
                    }
                }
            }
            std::cout << "This day's cost is: " << cost << std::endl;
        }
    }
}

    


