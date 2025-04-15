import os
import requests
import pandas as pd
from datetime import datetime, timedelta
from typing import Dict, List, Optional

api_key = "e06533d97cbb013256e08153c2622a383892f12b7f16af52757eb6cc4421cb73"

def evaluate_flight(flight: Dict, min_cost_flight: Optional[Dict] = None) -> bool:
    """
    Evaluates if a flight is better based on cost and duration criteria.
    Returns True if the flight should be selected.
    """
    if min_cost_flight is None:
        return True
        
    cost_increase = (flight['cost'] - min_cost_flight['cost']) / min_cost_flight['cost']
    duration_reduction = (min_cost_flight['total_duration'] - flight['total_duration']) / min_cost_flight['total_duration']
    
    return cost_increase <= 0.20 and duration_reduction >= 0.40

def process_daily_flights(flights: List[Dict]) -> Dict:
    """
    Process flights for a single day and return the best option.
    """
    if not flights:
        return None
        
    # Find minimum cost flight
    min_cost_flight = min(flights, key=lambda x: x['cost'])
    
    # Find flight with best duration-cost trade-off
    best_flight = min_cost_flight
    for flight in flights:
        if evaluate_flight(flight, min_cost_flight):
            if flight['total_duration'] < best_flight['total_duration']:
                best_flight = flight
    
    # Add flag if a different minimum cost flight exists
    best_flight['alt_min_cost_exists'] = (
        best_flight['cost'] > min_cost_flight['cost']
    )
    
    return best_flight

def fetch_flight_data(departure_id: str, arrival_id: str, 
                     start_date: datetime, end_date: datetime,
                     output_file: str = None) -> None:
    """
    Fetches flight data between two cities for a date range and saves to CSV.
    
    Args:
        departure_id (str): Airport code of departure city
        arrival_id (str): Airport code of arrival city
        start_date (datetime): Start date of range
        end_date (datetime): End date of range
        output_file (str): Name of output CSV file
    """

    data_dir = "Data"

    if output_file is None:
        # Create filename: DEP-ARR_YYYYMMDD-YYYYMMDD.csv
        output_file = os.path.join(
            data_dir, 
            f"{departure_id}-{arrival_id}_{start_date.strftime('%Y-%m-%d')}-{end_date.strftime('%Y-%m-%d')}.csv"
        )
    data: List[Dict] = []
    current_date = start_date
    
    while current_date <= end_date:
        try:
            response = requests.get(
                "https://serpapi.com/search",
                params={
                    "api_key": api_key,
                    "engine":"google_flights",
                    "hl": "en",
                    "gl": "in",
                    "type": 2,
                    "departure_id": departure_id,
                    "arrival_id": arrival_id,
                    "outbound_date": current_date.strftime("%Y-%m-%d"),
                    "travel_class": 1,
                    "adults": 5,
                    "currency": "INR"
                }
            )
            response.raise_for_status()
            flights_data = response.json()
            
            # Process both best_flights and other_flights
            daily_flights = []
            if 'best_flights' in flights_data:
                for flight in flights_data['best_flights']:
                    first_flight = flight['flights'][0]
                    last_flight = flight['flights'][-1]
                    
                    flight_data = {
                        'date': current_date.strftime("%Y-%m-%d"),
                        'from': first_flight['departure_airport']['id'],
                        'to': last_flight['arrival_airport']['id'],
                        'cost': flight['price'],
                        'total_duration': flight['total_duration']
                    }
                    daily_flights.append(flight_data)
            
            # Select best flight for the day
            if daily_flights:
                best_flight = process_daily_flights(daily_flights)
                if best_flight:
                    data.append(best_flight)
            
            current_date += timedelta(days=1)
            
        except requests.RequestException as e:
            print(f"Error fetching data for {current_date}: {e}")
            current_date += timedelta(days=1)
            continue
        
        except KeyError as e:
            print(f"Error processing data for {current_date}: {e}")
            current_date += timedelta(days=1)
            continue
    
    # Convert to DataFrame and save
    if data:
        df = pd.DataFrame(data)
        columns = ['date', 'from', 'to', 'cost', 'total_duration', 'alt_min_cost_exists']
        df = df.reindex(columns=columns)
        df.to_csv(output_file, index=False)
        print(f"Data saved to {output_file}")
    else:
        print("No data was collected")

def fetch_all_routes(cities: List[str], start_date: datetime, end_date: datetime) -> None:
    """
    Fetches flight data for all possible combinations of cities.
    
    Args:
        cities (List[str]): List of airport codes
        start_date (datetime): Start date of range
        end_date (datetime): End date of range
    """
    # Generate all possible pairs of cities
    for departure in cities:
        for arrival in cities:
            if departure != arrival:  # Skip same city pairs
                print(f"\nFetching flights from {departure} to {arrival}...")
                fetch_flight_data(departure, arrival, start_date, end_date)

# Example usage
if __name__ == "__main__":
    # List of cities (airport codes)
    cities = ["COK", "ZRH", "FRA"]
    
    # Date range
    start = datetime(2025, 5, 16)
    end = datetime(2025, 5, 23)
    
    fetch_all_routes(cities, start, end)