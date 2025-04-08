#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>

struct Result
{
    double minCost;
    std::vector<int> path;
    std::vector<std::string> cityNames;
};

Result solveTSP(const std::vector<std::vector<double>> &costMatrix, const std::vector<std::string> &cityNames)
{
    const int n = costMatrix.size();
    std::vector<int> cities;
    for (int i = 0; i < n; i++)
    {
        cities.push_back(i);
    }

    Result result;
    result.minCost = std::numeric_limits<double>::infinity();
    result.cityNames = cityNames;

    do
    {
        std::cout << "Current permutation: ";
        for(int i : cities) std::cout << i << " ";
        std::cout << "\n";
        double currentCost = 0;
        for (int i = 0; i < n - 1; i++)
        {
            currentCost += costMatrix[cities[i]][cities[i + 1]];
        }
        currentCost += costMatrix[cities[n - 1]][cities[0]];
        if (currentCost < result.minCost)
        {
            result.minCost = currentCost;
            result.path = cities;
        }
    } while (std::next_permutation(cities.begin() + 1, cities.end()));

    return result;
}

int main()
{
    // City names
    std::vector<std::string> cityNames = {"Kochi", "Cologne", "Paris", "Rome"};

    // Sample cost matrix for 3 cities
    std::vector<std::vector<double>> costMatrix = {
        {0, 15, 20, 10}, // Cost from Kochi to others
        {15, 0, 1, 2}, // Cost from Cologne to others
        {20, 1, 0, 3}, // Cost from Paris to others
        {10, 2, 3, 0}  // Cost from Rome to others
    };

    Result result = solveTSP(costMatrix, cityNames);

    // Print results
    std::cout << "Minimum cost: " << result.minCost << std::endl;
    std::cout << "Optimal path: ";
    for (int i = 0; i < result.path.size(); i++)
    {
        std::cout << result.cityNames[result.path[i]];
        if (i < result.path.size() - 1)
        {
            std::cout << " -> ";
        }
    }
    std::cout << " -> " << result.cityNames[result.path[0]] << std::endl; // Return to start

    return 0;
}