import requests
import json
import os
from dotenv import load_dotenv

import platform

load_dotenv()  # Load variables from .env file
api_key = os.environ.get('GOOGLE_API_KEY')

def get_device_location(api_key):
    url = f'https://www.googleapis.com/geolocation/v1/geolocate?key={api_key}'

    payload = {
        #"homeMobileCountryCode":238,        # https://mcc-mnc.com/
        #"homeMobileNetworkCode":299,        # https://mcc-mnc.com/
        #"radioType":"gsm",
        #"carrier":"Vodafone",
        "considerIp": "true"    # consider IP address for location
    }

    headers = {
        "Content-Type": "application/json"      # the content being sent is in JSON format
    }

    try:
        # Make a POST request to the Geolocation API
        response = requests.post(url, data=json.dumps(payload), headers=headers)        

        if response.status_code == 200:
            result = response.json()
            location = result.get('location')
            accuracy = round(result.get('accuracy'), 3) # take only 3 decimals
            return location['lat'], location['lng'], accuracy
        else:
            print(f"Error: {response.status_code}, {response.text}")

    except Exception as e:
        print(f"An error occurred: {e}")

def showLocationOnMap(lat, log, acc):
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

if __name__ == "__main__":
    lat, log, acc = get_device_location(api_key)
    print("Latitude:", lat, "Longitude:", log, "Accuracy:", acc, "meters")

    showLocationOnMap(lat, log, acc)


