#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <iomanip>
#include <numeric>
#include <fstream>
#include <filesystem>
#include <sstream>

struct Result
{
    double minCost;
    std::vector<int> path;
    std::vector<int> travelDays; // Days when travel occurs
    std::vector<std::string> cityNames;
};

Result solveTSP(const std::vector<std::vector<std::vector<double>>> &costMatrix,
                const std::vector<std::string> &cityNames,
                const std::vector<int> &stayDurations)
{
    const int numCities = cityNames.size();
    const int numDays = costMatrix.size();

    // Calculate total trip duration needed
    int totalDuration = std::accumulate(stayDurations.begin(), stayDurations.end(), 0) + 1;

    std::vector<int> cities;
    for (int i = 0; i < numCities; i++)
    {
        cities.push_back(i);
    }

    Result result;
    result.minCost = std::numeric_limits<double>::infinity();
    result.cityNames = cityNames;

    do
    {
        double currentCost = 0;
        std::vector<int> travelDays;
        bool validPath = true;
        int currentDay = 0;

        // Try starting on different days
        for (int startDay = 0; startDay <= numDays - totalDuration ; startDay++)
        {
            currentDay = startDay;
            currentCost = 0;
            travelDays.clear();
            validPath = true;

            // Try each city transition
            for (int i = 0; i < numCities - 1 && validPath; i++)
            {
                // Stay in current city for specified duration
                currentDay += stayDurations[i];

                if (currentDay >= numDays)
                {
                    validPath = false;
                    break;
                }

                // Add travel cost to next city
                currentCost += costMatrix[currentDay][cities[i]][cities[i + 1]];
                travelDays.push_back(currentDay);
            }

            // Handle last city stay and return to start
            if (validPath && currentDay + stayDurations[numCities - 1] < numDays)
            {
                currentDay += stayDurations[numCities - 1];
                currentCost += costMatrix[currentDay][cities[numCities - 1]][cities[0]];
                travelDays.push_back(currentDay);

                if (currentCost < result.minCost)
                {
                    result.minCost = currentCost;
                    result.path = cities;
                    result.travelDays = travelDays;
                }
            }
        }
    } while (std::next_permutation(cities.begin() + 1, cities.end()));

    return result;
}

std::vector<std::vector<std::vector<double>>> buildCostMatrix(
    const std::vector<std::string>& cityNames,
    const std::string& dataDir = "Data") {
    
    int numCities = cityNames.size();
    int numDays = 0;
    std::vector<std::vector<std::vector<double>>> costMatrix;
    
    // First, determine number of days by checking any single route
    std::string firstFile = dataDir + "/" + cityNames[0] + "-" + cityNames[1] + "_*.csv";
    for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
        if (entry.path().string().find(cityNames[0] + "-" + cityNames[1]) != std::string::npos) {
            std::ifstream file(entry.path());
            std::string line;
            // Skip header
            std::getline(file, line);
            while (std::getline(file, line)) {
                numDays++;
            }
            break;
        }
    }
    
    // Initialize cost matrix
    costMatrix.resize(numDays, 
        std::vector<std::vector<double>>(numCities, 
            std::vector<double>(numCities, 0.0)));
    
    // Read each route's CSV file
    for (int from = 0; from < numCities; from++) {
        for (int to = 0; to < numCities; to++) {
            if (from == to) continue;
            
            std::string filename = dataDir + "/" + 
                                 cityNames[from] + "-" + 
                                 cityNames[to] + "_*.csv";
                                 
            for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
                if (entry.path().string().find(cityNames[from] + "-" + cityNames[to]) != std::string::npos) {
                    std::ifstream file(entry.path());
                    std::string line;
                    // Skip header
                    std::getline(file, line);
                    
                    int day = 0;
                    while (std::getline(file, line)) {
                        std::stringstream ss(line);
                        std::string cell;
                        std::vector<std::string> values;
                        
                        while (std::getline(ss, cell, ',')) {
                            values.push_back(cell);
                        }
                        
                        // Assuming cost is in the 4th column (index 3)
                        costMatrix[day][from][to] = std::stod(values[3]);
                        day++;
                    }
                    break;
                }
            }
        }
    }
    
    return costMatrix;
}

void printCostMatrix(const std::vector<std::vector<std::vector<double>>> &costMatrix,
                     const std::vector<std::string> &cityNames)
{
    std::cout << "\nCost Matrix for " << costMatrix.size() << " days:\n";

    for (int day = 0; day < costMatrix.size(); day++)
    {
        std::cout << "\nDay " << day << ":\n";
        std::cout << "    ";
        for (const auto &city : cityNames)
        {
            std::cout << city << " ";
        }
        std::cout << "\n";

        for (int from = 0; from < cityNames.size(); from++)
        {
            std::cout << cityNames[from] << " ";
            for (int to = 0; to < cityNames.size(); to++)
            {
                std::cout << costMatrix[day][from][to] << " ";
            }
            std::cout << "\n";
        }
    }
}

int main()
{
     //std::vector<std::string> cityNames = {"COK", "ZRH", "FRA"};
     //std::vector<int> stayDurations = { 0, 2, 1 };
     //std::vector<std::vector<std::vector<double>>> costMatrix = {
     //    {{0, 10, 20},
     //     {10, 0, 2},
     //     {20, 2, 0}},
     //    {{0, 10, 20},
     //     {10, 0, 2},
     //     {20, 2, 0}},
     //    {{0, 10, 20},
     //     {10, 0, 2},
     //     {20, 2, 0}},
     //    {{0, 4, 8},
     //     {4, 0, 2},
     //     {8, 2, 0}},
     //    {{0, 5, 9},
     //     {5, 0, 2},
     //     {9, 2, 0}} };

    std::vector<std::string> cityNames = {"COK", "ZRH", "FRA"};
    std::vector<int> stayDurations = {0, 2, 2};
    auto costMatrix = buildCostMatrix(cityNames);
    
    // printCostMatrix(costMatrix, cityNames);

    Result result = solveTSP(costMatrix, cityNames, stayDurations);

    // Print results
    std::cout << "Minimum total cost: " << std::fixed << std::setprecision(2)
              << result.minCost << std::endl;
    std::cout << "\nOptimal route with stays:\n";

    // Print intermediate cities and stays
    for (int i = 0; i < result.path.size() - 1; i++) {
        std::cout << "Day " << result.travelDays[i] << ": Travel from "
                 << result.cityNames[result.path[i]] << " to "
                 << result.cityNames[result.path[i + 1]] << "\n";
        std::cout << "Stay at " << result.cityNames[result.path[i + 1]]
                 << " for " << stayDurations[i + 1] << " days\n\n";
    }

    // Print final return
    int last = result.path.size() - 1;
    std::cout << "Day " << result.travelDays[last] << ": Return to "
              << result.cityNames[result.path[0]] << "\n";

    return 0;
}