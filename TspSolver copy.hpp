#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <chrono>
#include <tuple>

using Matrix = std::vector<std::vector<double>>;
using Date = std::chrono::system_clock::time_point;

class TspSolver
{
public:
    struct Result
    {
        double total_cost;               // Total cost of the journey
        std::vector<int> path;           // Path as city indices
        std::vector<int> days;           // Days when each city is visited
        std::vector<std::string> cities; // Path as city names
    };

    static Result solveTsp(
        const std::vector<Matrix> &cost_matrices,
        const std::vector<std::string> &cities,
        const std::vector<int> &stay_durations,
        const Date &start_date);

    // static std::tuple<std::vector<std::string>, std::vector<Matrix>>
    // readCostMatrices(
    //     const std::string &csv_path,
    //     const Date &start_date,
    //     int num_days,
    //     const std::string &start_city);

private:
    static bool isValidSchedule(int current_day, int days_needed, int num_dates);
    static double getCost(const std::vector<Matrix> &cost_matrices, int from_city, int to_city, int day);
};