import itertools

def traveling_salesman(cost_matrix, cities):
    """
    Solve the Asymmetric Traveling Salesman Problem using dynamic programming.
    
    Args:
        cost_matrix: 2D list where cost_matrix[i][j] represents the cost to travel from city i to city j
        cities: List of city names corresponding to the indices in cost_matrix
    
    Returns:
        tuple: (minimum_cost, optimal_route, city_names_route)
    """
    n = len(cost_matrix)
    if len(cities) != n:
        raise ValueError("Number of cities must match the size of the cost matrix")
    
    # Verify matrix is asymmetric
    for i in range(n):
        for j in range(n):
            if i != j and cost_matrix[i][j] == cost_matrix[j][i]:
                print(f"Warning: Cost from {cities[i]} to {cities[j]} equals cost from {cities[j]} to {cities[i]}")
        
    all_sets = []
    for r in range(n):
        all_sets.extend(itertools.combinations(range(1, n), r))

    dp = {}

    # Initialize base cases
    for i in range(1, n):
        dp[(tuple([i]), i)] = (cost_matrix[0][i], [0, i])

    # Main dynamic programming loop
    for size in range(2, n):
        for S in itertools.combinations(range(1, n), size):
            for last in S:
                prev = tuple(x for x in S if x != last)
                min_cost = float('inf')
                min_path = []
                
                for prev_last in prev:
                    if (prev, prev_last) in dp:
                        # Use directional cost from prev_last to last
                        cost = dp[(prev, prev_last)][0] + cost_matrix[prev_last][last]
                        if cost < min_cost:
                            min_cost = cost
                            min_path = dp[(prev, prev_last)][1] + [last]
                
                if min_path:
                    dp[(S, last)] = (min_cost, min_path)

    # Find the optimal solution
    min_cost = float('inf')
    min_path = []
    S = tuple(range(1, n))
    
    for last in S:
        if (S, last) in dp:
            # Use directional cost from last city back to start
            cost = dp[(S, last)][0] + cost_matrix[last][0]
            if cost < min_cost:
                min_cost = cost
                min_path = dp[(S, last)][1] + [0]

    city_names_route = [cities[i] for i in min_path]
    
    return min_cost, min_path, city_names_route

# Example usage:
if __name__ == "__main__":
    # Example asymmetric cost matrix and city names
    cities = ["Kochi", "Paris", "Cologne", "Zurich", "Rome"]
    cost_matrix = [
        [0,     25481,  42484,  35005, 25035],  # Costs from Kochi
        [28738,  0,     13867,  11611, 6022],   # Costs from Paris
        [54545,  11989, 0,      5074,  5262],   # Costs from Cologne
        [36057,  9002,  8038,   0,     4110],   # Costs from Zurich
        [31297,  3948,  5263,   4840,  0]       # Costs from Rome
    ]

    # Solve the ATSP
    total_cost, route_indices, route_cities = traveling_salesman(cost_matrix, cities)
    
    print(f"Minimum cost: {total_cost}")
    print(f"Optimal route (indices): {' -> '.join(str(i) for i in route_indices)}")
    print(f"Optimal route (cities): {' -> '.join(route_cities)}")
    
    # Print the individual flight costs
    print("\nFlight costs breakdown:")
    for i in range(len(route_indices)-1):
        from_city = route_cities[i]
        to_city = route_cities[i+1]
        cost = cost_matrix[route_indices[i]][route_indices[i+1]]
        print(f"{from_city} -> {to_city}: {cost}")