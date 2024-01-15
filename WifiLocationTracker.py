import requests
import json
import os
from dotenv import load_dotenv

load_dotenv()  # Load variables from .env file
api_key = os.environ.get('GOOGLE_API_KEY')

def get_device_location(api_key):
    # Google Geolocation API endpoint
    url = f'https://www.googleapis.com/geolocation/v1/geolocate?key={api_key}'

    # Payload with the "considerIp" parameter
    payload = {
        "considerIp": "true"
    }

    # Headers
    headers = {
        "Content-Type": "application/json"
    }

    try:
        # Make a POST request to the Geolocation API
        response = requests.post(url, data=json.dumps(payload), headers=headers)

        # Check if the request was successful (status code 200)
        if response.status_code == 200:
            result = response.json()
            location = result.get('location')
            accuracy = result.get('accuracy')
            print(f"Latitude: {location['lat']}, Longitude: {location['lng']}, Accuracy: {accuracy} meters")
        else:
            # Print error details for unsuccessful requests
            print(f"Error: {response.status_code}, {response.text}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    get_device_location(api_key)
