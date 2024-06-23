# How to run

1. Install NPM and node.js 
<pre>
<code><b><u>Ubuntu</u></b>  
  sudo apt update
  sudo apt install nodejs
  sudo apt install npm
<b><u>Windows</u></b>
  <a>https://nodejs.org/en/download/prebuilt-installer</a></code>
</pre>
2. Create an account and application on [The Things Network](https://eu1.cloud.thethings.network/console/)
3. Generate an API token on [The Things Network](https://eu1.cloud.thethings.network/console/)
4. Create an account and application on [loracloud.com](loracloud.com)
5. Download TLS Credentials from [https://www.loracloud.com/portal/join_service/keys_and_credentials](https://www.loracloud.com/portal/join_service/keys_and_credentials), create credentials folder in base folder and unpack credentials to this. If your credentials aren't called acct-d13, rename them all
6. Generate API token on [loracloud.com](https://www.loracloud.com/portal/modem_services/tokens) and add to [TNN Integration: LoRa Cloud](https://eu1.cloud.thethings.network/console/)
7. Create a `.env` file in base folder and add the following code:
```
  REACT_APP_TTN_API_KEY=YOUR_THE_THINGS_NETWORK_API_KEY
  REACT_APP_TTN_APP_ID=YOUR_THE_THINGS_NETWORK_APPLICATION_ID
  REACT_APP_LORACLOUD_API_KEY=YOUR_LORACLOUD_API_KEY
  REACT_APP_USE_CLERK=false
```
8. Run `npm install` in base folder
9. Run `npm install` in server folder
10. Create user on [ngrok.com](ngrok.com) and download application
11. Start ngrok and run `ngrok.exe http 3001` (Ubuntu: `ngrok http 3001`)
12. Copy forwarding address
13. Add webhook under intergrations on [TTN](https://eu1.cloud.thethings.network/console/) with Webhook format: JSON Base URL: forwarding address from ngrok <br>Filter event data: *up.location_solved*, *up.location_solved.location*, *end_device_ids.dev_eui* <br>Enabled event types: *Location solved (api/geolocationSolves)*
14. Open a terminal and run `node server.js` in the server folder
15. Open another run `npm start` in the base folder
16. If your browser doesn't open automatically, navigate to localhost:3000. You can now add your LR1110 device, and start seeing it's location, if it has TTN network coverage, and can detect either Wi-Fi access points or GNSS satellites (GPS and BeiDou).

## OPTIONAL (limit user access)
If you're planning on hosting the server online, and not just running it locally, page access can be controlled using Clerk [clerk.com](clerk.com).
Create an account, create an API token for React, change *USE_CLERK* to *true* in the .env file and add the following:
```
  REACT_APP_CLERK=YOUR_CLERK_API_KEY
```

# Future work

Might implement as EXPO app https://github.com/Dean177/expo-leaflet

# API Endpoint Documentation

This document outlines the API endpoints available in the application.

## Base URL

The base URL for all endpoints is `localhost:3001/api`.

## Geolocation Solves Endpoints

### 1. Get all Geolocation Solves
- **URL:** `/geolocationSolves`
- **Method:** `GET`
- **Description:** Retrieves all geolocation solves stored in the database.
- **Response:**
  - `200 OK` Successfully retrieved geolocation solves.
    - Description: Successfully retrieved geolocation solves.
    - Content: Array of geolocation solve objects.
  - `500 Internal Server Error` Server error occurred while processing the request.

### 2. Get Geolocation Solves by Device ID
- **URL:** `/geolocationSolves/:deviceID`
- **Method:** `GET`
- **Description:** Retrieves all geolocation solves for a specific device ID.
- **Parameters:**
  - `deviceID` (string): The unique identifier of the device.
- **Response:**
  - `200 OK`  
    - Description: Successfully retrieved geolocation solves.
    - Content: Array of geolocation solve objects.
  - `404 Not Found`
    - Description: Device ID not found in the database.
    - Content: Error message indicating device ID not found.
  - `500 Internal Server Error` Server error occurred while processing the request.

### 3. Add a Geolocation Solve
- **URL:** `/geolocationSolves`
- **Method:** `POST`
- **Description:** Adds a new geolocation solve to the database.
- **Request Body:**
  - `end_device_ids` (object): Object containing the device's IDs.
  - `location_solved` (object): Object containing the solved location data with latitude (number), longitude (number), accuracy (number) and source (string).
- **Response:**
  - `201 Created`
    - Description: Successfully created a new geolocation solve.
    - Content: Created geolocation solve object.
  - `204 No Content`
    - Description: Updated existing geolocation solve due to proximity.
    - Content: Updated geolocation solve object.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.


## Tracker Information Endpoints
### 1. Create/Update Tracker Information
- **URL:** `/trackerInformation`
- **Method:** `POST`
- **Description:** Updates tracker information from uplink messages.
- **Request Body:**
  - `end_device_ids` (object):
    - `dev_eui` (string): Device EUI identifier.
    - `uplink_message` (object):
      - `f_port` (number): Port number of the uplink message.
      - `decoded_payload` (object): Decoded payload data.
      - `rx_metadata` (array): Array of received metadata objects.
- **Response:**
  - `201 Created`
    - Description: Successfully created tracker information.
    - Content: Created tracker information object.
  - `200 OK`
    - Description: Successfully updated existing tracker information.
    - Content: Updated tracker information object.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 2. Update Specific Tracker Information
- **URL:** `/trackerInformation/:devEUI`
- **Method:** `PUT`
- **Description:** Updates specific tracker information identified by devEUI.
- **Parameters:** `devEUI` (path parameter): Device EUI identifier.
- **Request Body:**
  Optional fields to update:
  - `updateInterval` (number): Update interval of the tracker.
  - `wifiStatus` (boolean): Wi-Fi status of the tracker.
  - `gnssStatus` (boolean): GNSS status of the tracker.
  - `loraWANClass` (number): LoRaWAN class of the tracker.
- **Response:**
  - `201 Created`
    - Description: Successfully updated tracker information.
    - Content: Updated tracker information object.
  - `200 Created`
    - Description: Successfully updated existing tracker information.
    - Content: Updated tracker information object.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 3. Retrieve Specific Tracker Information
- **URL:** `/trackerInformation/:devEUI`
- **Method:** `GET`
- **Description:** Retrieves current information for a tracker identified by devEUI.
- **Parameters:** `devEUI` (path parameter): Device EUI identifier.
- **Response:**
  - `200 OK`
    - Description: Successfully retrieved tracker information.
    - Content: Tracker information object.
  - `404 Not Found` Tracker with the specified devEUI not found.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 4. Delete Specific Tracker Information
- **URL:** `/trackerInformation/:devEUI`
- **Method:** `DELETE`
- **Description:** Deletes specific tracker information identified by devEUI.
- **Parameters:** `devEUI` (path parameter): Device EUI identifier.
- **Response:**
  - `200 OK`
    - Description: Successfully deleted tracker information.
    - Content: Success message indicating tracker information was deleted.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

## Tracker Management Endpoints
### 1. Claim Devices on Semtechs Join Server
- **URL:** `/claimDevicesOnJoinServer`
- **Method:** `POST`
- **Description:** Claims device(s) on the Join server.
- **Request Body:**
  - Array of devices to claim.
    - `DevEUI` (string): Device EUI identifier.
    - `ChipEUI` (string, optional): Chip EUI identifier (defaults to DevEUI if not provided).
    - `JoinEUI` (string): Join EUI identifier.
    - `Pin` (string): PIN code for claiming the device.
- **Response:**
  - `200 OK`
    - Description: Successfully claimed device(s) on the Join Server.
    -Content: Object containing success message and claimed devices data.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `409 Conflict` Device claim conflict, device already claimed by the user.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 2. Get Device Information from Join Server
- **URL:** `/getDeviceInfoFromJoinserver`
- **Method:** `GET`
- **Description:** Fetches device information from the Join server.
- **Response:**
  - `200 OK`
    - Description: Success message indicating device information retrieval was successful.
    - Content: Device information object.
  - `500 Internal Server Error` Server error occurred while processing the request.

### 3. Unclaim Device(s) on Join Server
- **URL:** `/unclaimDeviceOnJoinServer`
- **Method:** `DELETE`
- **Description:** Unclaims devices on the Join server.
- **Request Body:**
  - Array of devices to unclaim.
    - `DevEUI` (string): Device EUI identifier.
- **Response:**
  - `200 OK` Successfully unclaimed device(s) on the Join Server.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` Server error occurred while processing the request.

### 4. Unclaim Device(s) from Modem & Geolocation services
- **URL:** `/unclaimDeviceFromModemServices`
- **Method:** `POST`
- **Description:** Unclaims device(s) from Modem Services.
- **Request Body:**
  - Token (string): API token for authentication.
  - deveuis (array): Array of device EUIs to unclaim.
- **Response:**
  - `200 OK` Successfully unclaimed device(s) from Modem Services.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` Server error occurred while processing the request.

### 5. Create Device on TTN Identity Server
- **URL:** `/createDeviceOnTTNIDServer`
- **Method:** `POST`
- **Description:** Creates a device on TTN's identity server.
- **Request Body:**
  - `Token` (string): API token for authentication.
  - `AppID` (string): Application ID.
  - `deviceID` (string): Device ID.
  - `devEUI` (string): Device EUI.
  - `joinEUI` (string): Join EUI.
- **Response:**
  - `200 Created`
    - Description: Successfully created device on TTN ID Server.
    - Content: Response data confirming device creation.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 6. Create Device on TTN Network Server
- **URL:** `/createDeviceOnTTNNS`
- **Method:** `PUT`
- **Description:** Creates a device on TTN's network server.
- **Request Body:**
  - `Token` (string): API token for authentication.
  - `AppID` (string): Application ID.
  - `deviceID` (string): Device ID.
  - `devEUI` (string): Device EUI.
  - `joinEUI` (string): Join EUI.
- **Response:**
  - `200 Created`
    - Description: Successfully created device on TTN ID Server.
    - Content: Response data confirming device creation.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 7. Create Device on TTN Application Server
- **URL:** `/createDeviceOnTTNAS`
- **Method:** `PUT`
- **Description:** Creates a device on TTN's application server.
- **Request Body:**
  - `Token` (string): API token for authentication.
  - `AppID` (string): Application ID.
  - `deviceID` (string): Device ID.
  - `devEUI` (string): Device EUI.
  - `joinEUI` (string): Join EUI.
- **Response:**
  - `200 Created`
    - Description: Successfully created device on TTN AS Server.
    - Content: Response data confirming device creation.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 8. Update Device Name on TTN
- **URL:** `/updateDeviceNameOnTTN`
- **Method:** `PUT`
- **Description:** Updates the name of a device on TTN.
- **Request Body:**
deviceId (string): Device ID.
name (string): New device name.
Token (string): API token.
AppID (string): Application ID.
- **Response:**
  - `200 Created` Succesfully updated device name in TTN ID server
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 9. Delete Device on TTN Network Server
- **URL:** `/deleteDeviceOnTTNNS`
- **Method:** `DELETE`
- **Description:** Deletes a device on TTN's network server.
- **Request Body:**
Token (string): API token.
AppID (string): Application ID.
DeviceID (string): Device ID.
- **Response:**
200 OK: Response data from the TTN Network server.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 10. Delete Device on TTN Application Server
- **URL:** `/deleteDeviceOnTTNAS`
- **Method:** `DELETE`
- **Description:** Deletes a device on TTN's application server.
- **Request Body:**
Token (string): API token.
AppID (string): Application ID.
DeviceID (string): Device ID.
- **Response:**
200 OK: Response data from the TTN Application server.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 11. Delete Device on TTN
- **URL:** `/deleteDeviceOnTTN`
- **Method:** `DELETE`
- **Description:** Deletes a device on TTN.
- **Request Body:**
Token (string): API token.
AppID (string): Application ID.
DeviceID (string): Device ID.
- **Response:**
Response data from TTN.
  - `400 Bad Request` Invalid or incomplete data provided in the request body.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.