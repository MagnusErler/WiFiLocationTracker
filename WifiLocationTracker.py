import requests
import json
import os
from dotenv import load_dotenv

import platform

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
            return location['lat'], location['lng'], accuracy
        else:
            print(f"Error: {response.status_code}, {response.text}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    lat, log, acc = get_device_location(api_key)
    print("Latitude:", lat, "Longitude:", log, "Accuracy:", acc, "meters")

    # open a html file in browser to see the location on map
    with open('location.html', 'w') as f:
        f.write(f"<h1>Latitude: {lat}</h1>")
        f.write(f"<h1>Longitude: {log}</h1>")
        f.write(f"<h1>Accuracy: {acc} meters</h1>")
        f.write(f"<iframe width='100%' height='500' src='https://maps.google.com/maps?q={lat},{log}&output=embed'></iframe>")

    # Use the appropriate command based on the operating system
    if platform.system() == 'Darwin':  # macOS
        os.system('open location.html')
    elif platform.system() == 'Windows':
        os.system('start location.html')
    elif platform.system() == 'Linux':
        os.system('xdg-open location.html')
    else:
        print("Unsupported operating system")


