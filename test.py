import os
import json
import requests

SERPAPI_API_KEY = "e06533d97cbb013256e08153c2622a383892f12b7f16af52757eb6cc4421cb73"

api_key = SERPAPI_API_KEY

params = {
    "api_key": api_key,
    "engine": "google_flights",
    "departure_id": "COK",
    "arrival_id": "ZRH",
    "type": 2,
    "outbound_date": "2025-04-17",
}

search = requests.get("https://serpapi.com/search", params=params)
results = search.json()
print(json.dumps())