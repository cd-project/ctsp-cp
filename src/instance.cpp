//
// Created by cuong on 17/11/2023.
//
#include "../include/instance.h"
std::vector<std::string> Instance::SplitStringWithDelimiter(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> returnValue;
    std::string::size_type start = 0;
    std::string::size_type end = s.find(delimiter);

    while(end != std::string::npos) {
        returnValue.push_back(s.substr(start, end-start));
        start = end + 1;
        end = s.find(delimiter, start);
    }

    returnValue.push_back(s.substr(start));
    return returnValue;
}

std::string TrimSpace(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return ""; // String contains only spaces
    }

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
Instance::Instance(const std::string& file_path) {
    std::string line;
    std::ifstream data(file_path);
    int n;

    getline(data, line);
    num_v = n = stoi(TrimSpace(line));
    std::cout << num_v << std::endl;

    getline(data, line);
    period = stoi(TrimSpace(line));
    std::cout << period << std::endl;

    getline(data, line);
    max_diff = stoi(TrimSpace(line));
    std::cout << max_diff << std::endl;

    getline(data, line);
    weight_type = TrimSpace(line);
    std::cout << weight_type << std::endl;

    getline(data, line);
    if (weight_type == "EUC_2D") {
        std::vector<double> xV(n), yV(n);
        std::istringstream iss(line);
        double id, x, y;
        iss >> id >> x >> y;
        xV[0] = x;
        yV[0] = y;
        for (int i = 1; i < n; i++) {
            data >> id >> x >> y;
            xV[i] = x;
            yV[i] = y;
        }
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else {
                    auto dx = xV[i] - xV[j];
                    auto dy = yV[i] - yV[j];
                    dist[i][j] = int((sqrt(dx * dx + dy * dy) + 0.5));
                }
            }
        }
        travel_time = dist;
    } else if (weight_type == "ATT") {
        std::vector<double> xV(n), yV(n);
        std::istringstream iss(line);
        double id, x, y;
        iss >> id >> x >> y;
        xV[0] = x;
        yV[0] = y;
        for (int i = 1; i < n; i++) {
            data >> id >> x >> y;
            xV[i] = x;
            yV[i] = y;
        }
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else {
                    auto dx = xV[i] - xV[j];
                    auto dy = yV[i] - yV[j];
                    dist[i][j] = ceil(sqrt((dx * dx + dy * dy)/10.0));
                }
            }
        }
        travel_time = dist;

    } else if (weight_type == "LOWER_ROW") {
        std::istringstream iss(line);
        std::vector<int> nums;
        int number;
        while(iss >> number) {
            nums.push_back(number);
        }
        auto ns = nums.size();
        for (int i = 1; i <= n*(n+1)/2-n-ns; i++) {
            data >> number;
            nums.push_back(number);
        }
        int dem = 0;
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else {
                    dist[i][j] = dist[j][i] = nums[dem];
                    dem++;
                }
            }
        }
        travel_time = dist;

    } else if (weight_type == "UPPER_ROW") {
        std::istringstream iss(line);
        std::vector<int> nums;
        int number;
        while(iss >> number) {
            nums.push_back(number);
        }
        auto ns = nums.size();
        for (int i = 1; i <= n*(n+1)/2-n-ns; i++) {
            data >> number;
            nums.push_back(number);
        }
        int dem = 0;
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else {
                    dist[i][j] = dist[j][i] = nums[dem];
                    dem++;
                }
            }
        }
        travel_time = dist;

    } else if (weight_type == "FULL_MATRIX") {
        std::istringstream iss(line);
        std::vector<int> nums;
        int number;
        while(iss >> number) {
            nums.push_back(number);
        }
        auto ns = nums.size();
        for (int i = 1; i <= n*n-ns; i++) {
            data >> number;
            nums.push_back(number);
        }
        int dem = 0;
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dist[i][j] = nums[dem];
                dem++;
            }
        }
        travel_time = dist;

    } else if (weight_type == "GEO") {
        const double PI = 3.141592, RRR = 6378.388;
        std::vector<double> xV(n), yV(n);
        std::istringstream iss(line);
        double id, x, y;
        iss >> id >> x >> y;
        xV[0] = x;
        yV[0] = y;
        for (int i = 1; i < n; i++) {
            data >> id >> x >> y;
            xV[i] = x;
            yV[i] = y;
        }
        std::vector<double> lat(n, 0);
        std::vector<double> lon(n, 0);
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        for (int i = 0; i < n; i++){
            double deg, min;
            deg = (int)(xV[i]);
            min = xV[i] - deg;
            lat[i] = PI * (deg + 5.0 * min / 3.0 ) / 180.0;
            deg = (int)(yV[i]);
            min = yV[i] - deg;
            lon[i] = PI * (deg + 5.0 * min / 3.0 ) / 180.0;
        }
        for (int i  = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                double q1 = cos( lon[i] - lon[j]);
                double q2 = cos( lat[i] - lat[j]);
                double q3 = cos( lat[i] + lat[j]);
                if(i != j){
                    dist[i][j] = (int)  ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
                }
            }
        }
        travel_time = dist;

    } else if (weight_type == "LOWER_DIAG_ROW") {
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        std::vector<int> nums;
        int num;
        std::istringstream iss(line);
        while(iss >> num) {
            nums.push_back(num);
        }
        while (getline(data, line)) {
            if (TrimSpace(line) != "DISPLAY_DATA_SECTION") {
                std::istringstream iss2(line);
                while(iss2 >> num) {
                    nums.push_back(num);
                }
            } else {
            }
        }
        int numIdx = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                dist[i][j] = dist[j][i] = nums[numIdx];
                numIdx++;
            }
        }
        travel_time = dist;

    } else if (weight_type == "UPPER_DIAG_ROW") {
        std::vector<std::vector<int>> dist(n, std::vector<int>(n));
        std::vector<int> nums;
        int num;
        std::istringstream iss(line);
        while(iss >> num) {
            nums.push_back(num);
        }
        while (getline(data, line)) {
            if (TrimSpace(line) != "DISPLAY_DATA_SECTION") {
                std::istringstream iss2(line);
                while(iss2 >> num) {
                    nums.push_back(num);
                }
            } else {
            }
        }
        int numIdx = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                dist[i][j] = dist[j][i] = nums[numIdx];
                numIdx++;
            }
        }
        travel_time = dist;
    }

    // demand section
    int dm;
    for (int i = 0; i < n; i++) {
        std::vector<int> d(period);
        for (int j = 0; j < period+1; j++) {
            data >> dm;
            if (j == 0) {
                continue;
            } else {
                d[j-1] = dm;
            }
        }
        demand.push_back(d);
    }

    std::cout << "Printing demand info:" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < demand[i].size(); j++) {
            std::cout << demand[i][j] << " ";
        }
        std::cout << std::endl;
    }
    customer_k_min.resize(n);
    for (int i = 1; i < n; i++) {
        auto k_min = 0;
        for (int k = 0; k < period; k++) {
            if (demand[i][k] == 1) {
                k_min = k;
                break;
            }
        }
        customer_k_min[i] = k_min;
    }

    for (int i = 1; i < n; i++) {
        std::cout << "cus k min of " << i << ": " << customer_k_min[i] << std::endl;
    }
    serve_time_bound.resize(n);
    for (int i = 0; i < n; i++) {
        std::pair<int, int> minmax;
        auto max = std::numeric_limits<int>::min();
        auto min = std::numeric_limits<int>::max();
        for (int j = 0; j < n; j++) {
            if (i != j) {
                if (travel_time[i][j] < min) {
                    min = travel_time[i][j];
                }
                if (travel_time[i][j] > max) {
                    max = travel_time[i][j];
                }
            }
        }
        minmax.first = min;
        minmax.second = max;
        std::cout << "min, max of " << i << " is: {" << min << ", " << max << "}" << std::endl;
        serve_time_bound[i] = minmax;
    }
}

