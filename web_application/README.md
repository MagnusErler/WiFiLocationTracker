# How to run

1. Install NPM and node.js (Ubuntu: [https://www.digitalocean.com/community/tutorials/how-to-install-node-js-on-ubuntu-20-04](https://www.digitalocean.com/community/tutorials/how-to-install-node-js-on-ubuntu-20-04)

   ```
   sudo apt update
   sudo apt install nodejs
   sudo apt install npm
   ```
2. Create account and application on [The Things Network](https://eu1.cloud.thethings.network/console/)
3. Generate an API token on [The Things Network](https://eu1.cloud.thethings.network/console/)
4. Create .env file in base folder and add API token and application ID:

   ```
   #.env
   REACT_APP_TTN_API_KEY="YOUR_API_KEY"
   REACT_APP_TTN_APP_ID="YOUR_APPLICATION_ID"
   ```
5. Create account and application on [loracloud.com](loracloud.com)
6. Download credentials and unpack to credentials folder in base folder from [https://www.loracloud.com/portal/join_service/account_admin_credentials](https://www.loracloud.com/portal/join_service/account_admin_credentials)
7. Generate API token on [loracloud.com](https://www.loracloud.com/portal/modem_services/tokens) and add to [TNN Integration: LoRa Cloud](https://eu1.cloud.thethings.network/console/applications/dtu-master-thesis/integrations/lora-cloud)
8. Run `npm install` in base folder
9. Run `npm install` in server folder
10. Create user on [ngrok.com](ngrok.com) and download application
11. Start ngrok and run `ngrok.exe http 3001`
12. Copy forwarding address
13. Add webhook under intergrations on TTN with Webhook format: JSON Base URL: forwarding address from ngrok Filter event data: up.location_solved, up.location_solved.location, end_device_ids.dev_eui Enabled event types: Location solved (api/geolocationSolves)
14. Open another terminal and run `node server.js` in server folder
15. Open another run `npm start` in base folder
16. If your browser doesn't open automatically, navigate to localhost:3000. You're can now add your LR1110 device, and start seeing it's location, if it has TTN network coverage, and can detect either WiFi access points or GNSS satellites (GPS and Baidu).

# Future work

Might implement as EXPO app https://github.com/Dean177/expo-leaflet

# API Endpoint Documentation

This document outlines the API endpoints available in the application.

## Base URL

The base URL for all endpoints is `localhost:3001/api`.

## Endpoints

### 1. Get all Geolocation Solves

- **URL:** `/geolocationSolves`
- **Method:** `GET`
- **Description:** Retrieves all geolocation solves stored in the database.
- **Response:**
  - `200 OK` if successful, returning an array of geolocation solves.
  - `500 Internal Server Error` if there's an issue querying the database.

### 2. Get Geolocation Solves by Device ID

- **URL:** `/geolocationSolves/:deviceID`
- **Method:** `GET`
- **Description:** Retrieves all geolocation solves for a specific device ID.
- **Parameters:**
  - `deviceID` (string): The unique identifier of the device.
- **Response:**
  - `200 OK` if successful, returning an array of geolocation solves.
  - `404 Not Found` if the device ID is not found.
  - `500 Internal Server Error` if there's an issue querying the database.

### 3. Add a Geolocation Solve

- **URL:** `/geolocationSolves`
- **Method:** `POST`
- **Description:** Adds a new geolocation solve to the database.
- **Request Body:**
  - `end_device_ids` (object): Object containing the device's IDs.
  - `location_solved` (object): Object containing the solved location data.
- **Response:**
  - `201 Created` if successful, returning the added geolocation solve.
  - `500 Internal Server Error` if there's an issue adding the solve to the database.

### 4. Get Device Information from Join Server

- **URL:** `/getDeviceInfoFromJoinserver`
- **Method:** `GET`
- **Description:** Fetches device information from the Join server.
- **Response:**
  - `200 OK` if successful, returning device information.
  - `500 Internal Server Error` if there's an issue fetching device information.

### 5. Claim Devices on Join Server

- **URL:** `/claimDevicesOnJoinServer`
- **Method:** `POST`
- **Description:** Claims devices on the Join server.
- **Request Body:**
  - Array of devices to claim.
- **Response:**
  - `200 OK` if successful, returning the claimed devices.
  - `400 Bad Request` if the request body is invalid.
  - `404 Not Found`, `409 Conflict`, or `500 Internal Server Error` for different error scenarios.

### 6. Unclaim Device on Join Server

- **URL:** `/unclaimDeviceOnJoinServer`
- **Method:** `DELETE`
- **Description:** Unclaims devices on the Join server.
- **Request Body:**
  - Array of devices to unclaim.
- **Response:**
  - `200 OK` if successful, returning the unclaimed devices.
  - `400 Bad Request` if the request body is invalid.
  - `500 Internal Server Error` if there's an issue unclaiming devices.

### 7. Create Device on TTN Identity Server

- **URL:** `/createDeviceOnTTNIDServer`
- **Method:** `POST`
- **Description:** Creates a device on TTN's identity server.
- **Request Body:**
  - Token, AppID, deviceID, DevEUI, JoinEUI.
- **Response:**
  - `200 OK` if successful, returning device creation response.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue creating the device.

### 8. Update Device Name on TTN

- **URL:** `/updateDeviceNameOnTTN`
- **Method:** `PUT`
- **Description:** Updates the name of a device on TTN.
- **Request Body:**
  - Device ID, name, Token, AppID.
- **Response:**
  - `200 OK` if successful, returning the updated device.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue updating the device name.

### 9. Delete Device on TTN

- **URL:** `/deleteDeviceOnTTN`
- **Method:** `DELETE`
- **Description:** Deletes a device on TTN.
- **Request Body:**
  - Token, AppID, DeviceID.
- **Response:**
  - `200 OK` if successful, returning device deletion response.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue deleting the device.

### 10. Create Device on TTN Network Server

- **URL:** `/createDeviceOnTTNNS`
- **Method:** `PUT`
- **Description:** Creates a device on TTN's network server.
- **Request Body:**
  - Token, AppID, deviceID, DevEUI, JoinEUI.
- **Response:**
  - `200 OK` if successful, returning device creation response.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue creating the device.

### 11. Create Device on TTN Application Server

- **URL:** `/createDeviceOnTTNAS`
- **Method:** `PUT`
- **Description:** Creates a device on TTN's application server.
- **Request Body:**
  - Token, AppID, deviceID, DevEUI, JoinEUI.
- **Response:**
  - `200 OK` if successful, returning device creation response.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue creating the device.

### 12. Delete Device on TTN Network Server

- **URL:** `/deleteDeviceOnTTNNS`
- **Method:** `DELETE`
- **Description:** Deletes a device on TTN's network server.
- **Request Body:**
  - Token, AppID, DeviceID.
- **Response:**
  - `200 OK` if successful, returning device deletion response.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue deleting the device.

### 13. Delete Device on TTN Application Server

- **URL:** `/deleteDeviceOnTTNAS`
- **Method:** `DELETE`
- **Description:** Deletes a device on TTN's application server.
- **Request Body:**
  - Token, AppID, DeviceID.
- **Response:**
  - `200 OK` if successful, returning device deletion response.
  - `400 Bad Request` if request parameters are missing.
  - `500 Internal Server Error` if there's an issue deleting the device.
