//
// Created by cuong on 17/11/2023.
//

#ifndef CTSP_INSTANCE_H
#define CTSP_INSTANCE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>

class Instance {
public:
    int num_v;
    int period;
    int max_diff;
    std::string weight_type;
    std::vector<std::vector<int>> demand;
    std::vector<std::vector<int>> travel_time;
    std::vector<std::pair<int, int>> serve_time_bound;
    std::vector<int> customer_k_min;

    explicit Instance(const std::string& file_path);

    static std::vector <std::string> SplitStringWithDelimiter(const std::string& s, const std::string& delimiter);
};
#endif //CTSP_INSTANCE_H
