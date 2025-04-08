#include "TspSolver.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <queue>

bool TspSolver::isValidSchedule(int current_day, int days_needed, int num_dates)
{
    return current_day + days_needed < num_dates;
}

double TspSolver::getCost(const std::vector<Matrix> &cost_matrices,
                          int from_city, int to_city, int day)
{
    return cost_matrices[day][from_city][to_city];
}

TspSolver::Result TspSolver::solveTsp(
    const std::vector<Matrix> &cost_matrices,
    const std::vector<std::string> &cities,
    const std::vector<int> &stay_durations,
    const Date &start_date)
{

    int n = cities.size();
    if (stay_durations.size() != n)
    {
        throw std::invalid_argument("Stay durations must match number of cities");
    }

    int num_dates = cost_matrices.size();

    // Define the state type for dynamic programming
    using State = std::tuple<std::vector<int>, int, int>;
    struct StateHash
    {
        size_t operator()(const State &s) const
        {
            const auto &[visited, current, day] = s;
            size_t hash = 0;
            for (int city : visited)
            {
                hash ^= std::hash<int>{}(city) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            hash ^= std::hash<int>{}(current) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<int>{}(day) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };

    std::map<State, std::tuple<double, std::vector<int>, std::vector<int>>> dp;

    // Initialize base cases
    for (int i = 1; i < n; i++)
    {
        if (isValidSchedule(0, stay_durations[0], num_dates))
        {
            std::vector<int> initial_path = {0, i};
            std::vector<int> initial_days = {0};
            dp[std::make_tuple(std::vector<int>{i}, i, 0)] =
                std::make_tuple(cost_matrices[0][0][i], initial_path, initial_days);
        }
    }


    // Main dynamic programming loop
    for (int visited_count = 2; visited_count <= n; visited_count++) {
        std::map<State, std::tuple<double, std::vector<int>, std::vector<int>>> next_dp;
        
        for (const auto& [state, value] : dp) {
            const auto& [visited, current, current_day] = state;
            const auto& [current_cost, current_path, current_days] = value;
            
            // Try visiting each unvisited city
            for (int next_city = 0; next_city < n; next_city++) {
                // Skip if city is already visited
                if (std::find(visited.begin(), visited.end(), next_city) != visited.end()) {
                    continue;
                }
                
                // Calculate next day after staying in current city
                int next_day = current_day + stay_durations[current];
                if (!isValidSchedule(next_day, stay_durations[next_city], num_dates)) {
                    continue;
                }
                
                // Calculate cost including stay duration
                double travel_cost = getCost(cost_matrices, current, next_city, next_day);
                double new_cost = current_cost + travel_cost;
                
                // Create new state
                std::vector<int> new_visited = visited;
                new_visited.push_back(next_city);
                State new_state = std::make_tuple(new_visited, next_city, next_day);
                
                // Update path and days
                std::vector<int> new_path = current_path;
                new_path.push_back(next_city);
                std::vector<int> new_days = current_days;
                new_days.push_back(next_day);
                
                // Update if better solution found
                auto it = next_dp.find(new_state);
                if (it == next_dp.end() || std::get<0>(it->second) > new_cost) {
                    next_dp[new_state] = std::make_tuple(new_cost, new_path, new_days);
                }
            }
        }
        
        dp = std::move(next_dp);
    }

    // Result collection
    Result result;
    result.total_cost = std::numeric_limits<double>::infinity();

    // Find best solution that returns to start city
    for (const auto& [state, value] : dp) {
        const auto& [visited, current, current_day] = state;
        const auto& [cost, path, days] = value;
        
        if (visited.size() == n - 1) {
            // Check if we can return to start city
            int final_day = current_day + stay_durations[current];
            if (isValidSchedule(final_day, 0, num_dates)) {
                double final_cost = cost + getCost(cost_matrices, current, 0, final_day);
                
                if (final_cost < result.total_cost) {
                    result.total_cost = final_cost;
                    result.path = path;
                    result.days = days;
                    // Add return to start
                    result.path.push_back(0);
                    result.days.push_back(final_day);
                }
            }
        }
    }

    // Convert indices to city names
    result.cities.reserve(result.path.size());
    for (int idx : result.path) {
        result.cities.push_back(cities[idx]);
    }

    return result;
}

// std::tuple<std::vector<std::string>, std::vector<Matrix>>
// TspSolver::readCostMatrices(
//     const std::string &csv_path,
//     const Date &start_date,
//     int num_days,
//     const std::string &start_city)
// {
    // std::vector<std::string> cities;
    // std::vector<Matrix> cost_matrices;

    // // Read CSV file
    // std::ifstream file(csv_path);
    // if (!file.is_open())
    // {
    //     throw std::runtime_error("Could not open file: " + csv_path);
    // }

    // // Read header to get cities
    // std::string header;
    // std::getline(file, header);
    // std::stringstream header_stream(header);
    // std::string cell;

    // // Skip first column (from city)
    // std::getline(header_stream, cell, ',');

    // // Read cities from header
    // while (std::getline(header_stream, cell, ',')) {
    //     cities.push_back(cell);
    // }

    // int n = cities.size();
    
    // // Find index of start city
    // auto start_idx = std::find(cities.begin(), cities.end(), start_city);
    // if (start_idx == cities.end()) {
    //     throw std::runtime_error("Start city not found in CSV");
    // }
    
    // // If start city is not first, reorder cities to put it first
    // if (start_idx != cities.begin()) {
    //     std::rotate(cities.begin(), start_idx, cities.end());
    // }

    // // Initialize cost matrices for each day
    // cost_matrices.resize(num_days);
    // for (int day = 0; day < num_days; day++) {
    //     cost_matrices[day].resize(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    // }

    // // Read cost data
    // std::string line;
    // while (std::getline(file, line)) {
    //     std::stringstream line_stream(line);
    //     std::string from_city;
    //     std::getline(line_stream, from_city, ',');
        
    //     // Find index of source city
    //     auto from_idx = std::find(cities.begin(), cities.end(), from_city);
    //     if (from_idx == cities.end()) continue;
    //     int i = std::distance(cities.begin(), from_idx);

    //     // Read costs to each destination
    //     int j = 0;
    //     while (std::getline(line_stream, cell, ',')) {
    //         if (cell.empty() || cell == "NA") {
    //             j++;
    //             continue;
    //         }

    //         double base_cost = std::stod(cell);
            
    //         // Apply cost variations for different days
    //         for (int day = 0; day < num_days; day++) {
    //             // Apply day-of-week pricing factor (example: weekend surcharge)
    //             Date current_date = start_date + day;
    //             double day_factor = (current_date.isWeekend()) ? 1.2 : 1.0;
                
    //             // Apply seasonal pricing factor (example: holiday season)
    //             double season_factor = current_date.getSeasonFactor();
                
    //             cost_matrices[day][i][j] = base_cost * day_factor * season_factor;
    //         }
    //         j++;
    //     }
    // }

    // return std::make_tuple(cities, cost_matrices);
// }