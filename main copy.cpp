#include "TspSolver.hpp"
#include <iostream>
#include <iomanip>

int main()
{
    // testing
    auto start_date = std::chrono::system_clock::now();
    int num_days = 4;

    // // Read cost matrices and cities from CSV
    // auto [cities, cost_matrices] = TspSolver::readCostMatrices(
    //     "costs_by_date.csv", start_date, num_days, "Kochi");

    std::vector<std::string> cities = {"Kochi", "Paris", "Cologne"};
    std::vector<Matrix> cost_matrices = {
        {{0, 100, 200}, {100, 0, 150}, {200, 150, 0}},
        {{0, 120, 220}, {120, 0, 170}, {220, 170, 0}},
        {{0, 130, 230}, {130, 0, 180}, {230, 180, 0}},
        {{0, 140, 240}, {140, 0, 190}, {240, 190, 0}}
    };

    std::vector<int> stay_durations = {0, 2, 2};

    // Solve the problem
    auto result = TspSolver::solveTsp(cost_matrices, cities, stay_durations, start_date);

    // Print results
    std::cout << "Minimum total cost: " << std::fixed << std::setprecision(2)
              << result.total_cost << std::endl;

    // Print itinerary details
    // ... (implementation of itinerary printing)

    return 0;
}