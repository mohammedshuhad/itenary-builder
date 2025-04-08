#include "TspSolver.hpp"
#include <iostream>
#include <iomanip>

int main() {
    //testing
    auto start_date = std::chrono::system_clock::now();
    int num_days = 4;
    
    // Read cost matrices and cities from CSV
    auto [cities, cost_matrices] = TspSolver::readCostMatrices(
        "costs_by_date.csv", start_date, num_days, "Kochi"
    );
    
    std::vector<int> stay_durations = {0, 2, 2};
    
    // Solve the problem
    auto result = TspSolver::solveTsp(cost_matrices, cities, stay_durations, start_date);
    
    // Print results
    std::cout << "Minimum total cost: " << std::fixed << std::setprecision(2) 
              << result.min_cost << std::endl;
    std::cout << "\nItinerary:" << std::endl;
    
    if (result.route_cities.empty() || result.dates.empty()) {
        std::cout << "No valid itinerary found!" << std::endl;
        return 1;
    }
    
    // Print itinerary details
    // ... (implementation of itinerary printing)
    
    return 0;
}