import pandas as pd
from datetime import datetime, timedelta

def generate_sample_data(cities, start_date, num_days, base_costs):
    data = []
    
    for day in range(num_days):
        current_date = start_date + timedelta(days=day)
        # Simple multiplier: day 0 = 1x, day 1 = 2x, day 2 = 3x, day 3 = 4x
        multiplier = day + 1
        
        for i, from_city in enumerate(cities):
            for j, to_city in enumerate(cities):
                if i != j:  # Skip same city pairs
                    cost = base_costs[i][j] * multiplier
                    data.append({
                        'date': current_date,
                        'from': from_city,
                        'to': to_city,
                        'cost': round(cost, 2)
                    })
    
    df = pd.DataFrame(data)
    df.to_csv('costs_by_date.csv', index=False)
    df.to_csv('vs/costs_by_date.csv', index=False)
    print("Sample data generated in 'costs_by_date.csv'")
    print(f"First few rows of generated data:\n{df.head()}")

if __name__ == "__main__":
    cities = ["Kochi", "Paris", "Cologne"]  # Reduced to 3 cities for simplicity
    start_date = datetime(2025, 4, 7)
    num_days = 4  # Only 4 days of data
    
    # Simplified base costs matrix (3x3)
    base_costs = [
        [0, 100, 200],
        [100, 0, 150],
        [200, 150, 0]
    ]
    
    generate_sample_data(cities, start_date, num_days, base_costs)