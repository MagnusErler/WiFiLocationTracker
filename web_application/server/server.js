const path = require('path');
const cors = require('cors');
const axios = require("axios");
const express = require('express');
const fs = require("fs");
const https = require("https");
const { Sequelize, Model, DataTypes } = require('sequelize');

const app = express();
const PORT = process.env.PORT || 3001;

app.use(cors());
app.use(express.json())
app.use(express.urlencoded({extended: true}))

const sequelize = new Sequelize({
  dialect: 'sqlite',
  storage: './geolocationdata.sqlite'
});

class GeolocationSolve extends Model {}
GeolocationSolve.init({
  deviceID: {
    type: DataTypes.STRING,
    allowNull: false,
  },
  geocode: {
    type: DataTypes.JSON,
    allowNull: false,
  },
  source: {
    type: DataTypes.STRING,
    allowNull: false,
  },
  accuracy: {
    type: DataTypes.FLOAT,
    allowNull: false,
  }
}, { sequelize, modelName: 'geolocationSolve' });

// Sync model with database
sequelize.sync();

// Helper function
function deg2rad(deg) {
  return deg * (Math.PI/180);
}

// Returns all pings for all devices
app.get('/api/geolocationSolves', async (req, res) => {
  try {
    const geolocationSolves = await GeolocationSolve.findAll();
    res.status(200).json(geolocationSolves);
  } catch (error) {
    console.error('Error querying database:', error.message);
    res.status(500).send('Internal server error');
  }
});

// Returns all pings for a specific device
app.get('/api/geolocationSolves/:deviceID', async (req, res) => {
  try {
    const geolocationSolvesWithDeviceID = await GeolocationSolve.findAll({ where: { deviceID: req.params.deviceID } });

    // Check if any data was found for the device ID
    if (geolocationSolvesWithDeviceID.length === 0) {
      // Device ID not found, return 404 status code
      console.log('Device ID not found:', req.params.deviceID);
      return res.status(404).json({ error: 'Device ID not found' });
    }

    // Device ID found, return data
    res.status(200).json(geolocationSolvesWithDeviceID);
  } catch (error) {
    console.error('Error querying database:', error.message);
    res.status(500).send('Internal server error');
  }
});

// Add a ping to the database
app.post('/api/geolocationSolves', async (req, res) => {
  try {
    // console.log(req.body);
    const { end_device_ids, location_solved } = req.body;
    
    // Extract device ID and geolocation data
    const deviceID = end_device_ids.dev_eui;
    const { location } = location_solved;
    const { latitude, longitude, accuracy, source } = location;
    
    // Format geocode
    const geocode = [latitude, longitude];

    // Check if there are existing pings for the device
    const existingPings = await GeolocationSolve.findAll({
      where: { deviceID },
      order: [['createdAt', 'DESC']], // Get the latest ping
      limit: 1
    });

    if (existingPings.length > 0) {
      console.log('Existing ping found"', existingPings[0].toJSON());
      const latestPing = existingPings[0];
      const latestGeocode = latestPing.geocode;

      const R = 6371;
      const dLat = deg2rad(latitude-latestGeocode[0]);
      const dLon = deg2rad(longitude-latestGeocode[1]);

      const a = Math.sin(dLat/2) * Math.sin(dLat/2) + Math.cos(deg2rad(latestGeocode[0])) * Math.cos(deg2rad(latitude)) * Math.sin(dLon/2) * Math.sin(dLon/2);
      const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
      const d = R * c * 1000; // Distance in meters

      console.log("Distance betweeen pings: " + d + " in meters");

      if (d < 15) { 
        // Update the latest ping instead of adding a new one
        console.log("Distance is less than 15 meters, updating latest ping instead of adding a new one")
        latestPing.update({ geocode, accuracy, source });
        console.log('Latest ping updated in database:', latestPing.toJSON());
        return res.status(204).json(latestPing);
      }
    }
      
    // Save geolocation solve to the database
    const geolocationSolve = await GeolocationSolve.create({ deviceID, geocode, accuracy, source});
    console.log('Geolocation solve added to database:', geolocationSolve.toJSON());
    res.status(201).json(geolocationSolve);
  } catch (error) {
    console.error('Error adding geolocation solve to database:', error.message);
    res.status(500).send('Internal server error');
  }
});

app.get("/api/getDeviceInfoFromJoinserver", async (req, res) => {
  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/list_devices";
    const url = `https://${h}${u}`;
    const cert_file = path.join(__dirname, "../credentials", `${a}.crt`);
    const key_file = path.join(__dirname, "../credentials", `${a}.key`);
    const trust_file = path.join(__dirname, "../credentials", `${a}.trust`);

    const response = await axios.get(url, {
      httpsAgent: new https.Agent({
        cert: fs.readFileSync(cert_file),
        key: fs.readFileSync(key_file),
        ca: fs.readFileSync(trust_file)
      })
    });

    res.status(200).json(response.data);
  } catch (error) {
    console.error("Failed to fetch device information:", error);
    res.status(500).json({ error: "Failed to fetch device information" });
  }
});

app.delete("/api/unclaimDeviceFromModemServices", async (req, res) => {
  console.log("Unclaiming device from Modem Services:", req.body);

  try {
    let { Token, deveuis } = req.body;

    if (!Token || !deveuis || !Array.isArray(deveuis) || deveuis.length === 0) {
      return res.status(400).json({
        error: 'Bad request',
        message: 'API token and a non-empty array of deveuis are required in the request body'
      });
    }

    // Format deveuis if necessary
    deveuis = deveuis.map(deveui => {
      // If deveui doesn't contain "-", add it
      if (!deveui.includes('-')) {
        return deveui.match(/.{1,2}/g).join('-');
      }
      return deveui;
    });

    const API_TOKEN = "AQEAMxvmHw4f5Y1zdgp8vM+9TjpolwNgkrojGr+bfkhT31oA7jF6";

    const response = await axios.post('https://mgs.loracloud.com/api/v1/device/remove', {
      deveuis
    }, {
      headers: {
        'Authorization': Token,
        'Content-Type': 'application/json'
      }
    });

    res.status(200).json(response.data);
  } catch (error) {
    console.error("Failed to remove devices:", error);

    let errorMessage;
    if (axios.isAxiosError(error)) {
      errorMessage = 'Failed to connect to server';
    } else {
      errorMessage = error.message;
    }

    return res.status(500).json({
      error: 'Request error',
      message: errorMessage
    });
  }
});

app.post("/api/claimDevicesOnJoinServer", async (req, res) => {
  // Check if the request body is empty or not an array
  if (!Array.isArray(req.body)) {
    return res.status(400).json({
      error: 'Bad request',
      message: 'Request body must be a non-empty array of devices'
    });
  }

  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/claim_devices";
    const url = `https://${h}${u}`;
    const cert_file = path.join(__dirname, "../credentials", `${a}.crt`);
    const key_file = path.join(__dirname, "../credentials", `${a}.key`);
    const trust_file = path.join(__dirname, "../credentials", `${a}.trust`);

    const devices = req.body.map(device => ({
      DevEUI: device.DevEUI,
      ChipEUI: device.ChipEUI || device.DevEUI, // Optional, use DevEUI if ChipEUI is not provided
      JoinEUI: device.JoinEUI, // Optional, but should be provided if not using default
      claim: device.Pin
    }));

    console.log("Devices to claim on LoRaCloud:", devices);

    const response = await axios.post(url, devices, {
      httpsAgent: new https.Agent({
        cert: fs.readFileSync(cert_file),
        key: fs.readFileSync(key_file),
        ca: fs.readFileSync(trust_file)
      }),
      headers: {
        'Content-Type': 'application/json'
      }
    });

    // console.log("Response:", response.data);

    if (!Array.isArray(response.data)) {
      console.error("Unexpected response format from server. All devices failed to claim.");
      return res.status(500).json({
        error: 'Server error',
        message: 'Unexpected response format from server. All devices failed to claim.'
      });
    }

    const failedDevices = response.data.filter(device => device.error);
    if (failedDevices.length > 0) {
      const errorMessages = failedDevices.map(device => device.error);

      if (errorMessages.includes('[IsYours] Device is already claimed by you')) {
        return res.status(409).json({
          error: 'Device claim conflict',
          message: "Failed to claim device(s): " + errorMessages.join(", ")
        });
      }

      if (errorMessages.includes('[BadClaim] Claim does not verify')) {
        return res.status(400).json({
          error: 'Bad request',
          message: "Failed to claim device(s): " + errorMessages.join(", ")
        });
      }

      if (errorMessages.includes('[UnknownEUI] ChipEUI is not provisioned on this server')) {
        return res.status(404).json({
          error: 'Device not found',
          message: "Failed to claim device(s): " + errorMessages.join(", ")
        });
      }

      // Handle other error scenarios
      return res.status(500).json({
        error: 'Server error',
        message: "Failed to claim device(s): " + errorMessages.join(", ")
      });
    }

    return res.status(200).json({ success: true, message: "Devices claimed successfully", data: response.data });

  } catch (error) {
    console.error("Failed to claim devices:", error);

    let errorMessage;
    if (axios.isAxiosError(error)) {
      errorMessage = 'Failed to connect to server';
    } else {
      errorMessage = error.message;
    }

    return res.status(500).json({
      error: 'Request error',
      message: errorMessage
    });
  }
});

app.delete("/api/unclaimDeviceOnJoinServer", async (req, res) => {
  // Check if the request body is empty or not an array
  if (!Array.isArray(req.body)) {
    return res.status(400).json({
      error: 'Bad request',
      message: 'Request body must be a non-empty array of devices'
    });
  }

  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/unclaim_devices";
    const url = `https://${h}${u}`;
    const cert_file = path.join(__dirname, "../credentials", `${a}.crt`);
    const key_file = path.join(__dirname, "../credentials", `${a}.key`);
    const trust_file = path.join(__dirname, "../credentials", `${a}.trust`);

    const devices = req.body.map(device => ({
      DevEUI: device.DevEUI
    }));

    console.log("Devices to unclaim from LoRaCloud:", devices);

    const response = await axios.post(url, devices, {
      httpsAgent: new https.Agent({
        cert: fs.readFileSync(cert_file),
        key: fs.readFileSync(key_file),
        ca: fs.readFileSync(trust_file)
      }),
      headers: {
        'Content-Type': 'application/json'
      }
    });

    // console.log("Response:", response.data);

    return res.status(200).json({
      message: "Devices unclaimed successfully",
      data: response.data
    });

  } catch (error) {
    console.error("Failed to unclaim devices:", error);

    let errorMessage;
    if (axios.isAxiosError(error)) {
      errorMessage = 'Failed to connect to server';
    } else {
      errorMessage = error.message;
    }

    return res.status(500).json({
      error: 'Request error',
      message: errorMessage
    });
  }
});

// Create device on TTN Identity server
app.post("/api/createDeviceOnTTNIDServer", async (req, res) => {
  try {
    const token = req.body.Token;
    const appId = req.body.AppID;
    const deviceId = req.body.deviceID;
    const devEui = req.body.devEUI;
    const joinEui = req.body.joinEUI;

    // console.log(req.body);

    // console.log("Token:", token);
    // console.log("App ID:", appId);
    // console.log("Device ID:", deviceId);
    // console.log("DevEUI:", devEui);
    // console.log("JoinEUI:", joinEui);

    // Check if all required parameters are provided
    if (!deviceId || !devEui || !joinEui || !appId || !token) {
      return res.status(400).json({ error: 'Device ID, DevEUI, JoinEUI, App ID and API token are required' });
    }

    // Construct the request data JSON object
    const requestData = {
      end_device: {
        ids: {
          device_id: deviceId,
          dev_eui: devEui,
          join_eui: joinEui
        },
        join_server_address: "eu1.cloud.thethings.network",
        network_server_address: "eu1.cloud.thethings.network",
        application_server_address: "eu1.cloud.thethings.network"
      },
      field_mask: {
        paths: [
          'join_server_address',
          'network_server_address',
          'application_server_address',
          'ids.dev_eui',
          'ids.join_eui'
        ]
      }
    };

    const response = await axios.post(`https://eu1.cloud.thethings.network/api/v3/applications/${appId}/devices`, requestData, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}` 
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to create device on TTN's identity server:", error);
    res.status(500).json({ error: 'Failed to create device on TTN' });
  }
});

// Create device on TTN Join server (does not work)
app.put("/api/createDeviceOnTTNJoinServer", async (req, res) => {
  try {
    const token = req.body.Token;
    const appId = req.body.AppID;
    const deviceId = req.body.deviceID;
    const devEui = req.body.devEUI;
    const joinEui = req.body.joinEUI;
  
    if(!deviceId || !devEui || !joinEui || !appId || !token) {
      return res.status(400).json({ error: 'Device ID, DevEUI, JoinEUI, App ID and API token are required' });
    }

    // Construct the request data JSON object
    const requestData = {
      end_device: {
        ids: {
          device_id: deviceId,
          dev_eui: devEui,
          join_eui: joinEui
        },
        network_server_address: "eu1.cloud.thethings.network",
        application_server_address: "eu1.cloud.thethings.network",
        root_keys: {
          app_key: {
            key: ""
          }
        }
      },
      field_mask: {
        paths: [
          "network_server_address",
          "application_server_address",
          "ids.device_id",
          "ids.dev_eui",
          "ids.join_eui",
          "root_keys.app_key.key"
        ]
      }
    };

    const response = await axios.put(`https://eu1.cloud.thethings.network/api/v3/js/applications/${appId}/devices/${deviceId}`, requestData, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to create device on TTN's join server:", error);
    res.status(500).json({ error: 'Failed to create device on TTN' });
  }
});

// Create device on TTN Network server
app.put("/api/createDeviceOnTTNNS", async (req, res) => {
  try {
    const token = req.body.Token;
    const appId = req.body.AppID;
    const deviceId = req.body.deviceID;
    const devEui = req.body.devEUI;
    const joinEui = req.body.joinEUI;
  
    if(!deviceId || !devEui || !joinEui || !appId || !token) {
      return res.status(400).json({ error: 'Device ID, DevEUI, JoinEUI, App ID and API token are required' });
    }

    // Construct the request data JSON object for NS
    const requestData = {
      end_device: {
        supports_join: true,
        lorawan_version: "1.0.3",
        ids: {
          device_id: deviceId,
          dev_eui: devEui,
          join_eui: joinEui
        },
        lorawan_phy_version: "1.0.3-a",
        frequency_plan_id: "EU_863_870_TTN"
      },
      field_mask: {
        paths: [
          "supports_join",
          "lorawan_version",
          "ids.device_id",
          "ids.dev_eui",
          "ids.join_eui",
          "lorawan_phy_version",
          "frequency_plan_id"
        ]
      }
    };

    const response = await axios.put(`https://eu1.cloud.thethings.network/api/v3/ns/applications/${appId}/devices/${deviceId}`, requestData, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to create device on Network Server:", error);
    res.status(500).json({ error: 'Failed to create device on Network Server' });
  }
});

// Create device on TTN Application server
app.put("/api/createDeviceOnTTNAS", async (req, res) => {
  try {
    const token = req.body.Token;
    const appId = req.body.AppID;
    const deviceId = req.body.deviceID;
    const devEui = req.body.devEUI;
    const joinEui = req.body.joinEUI;
  
    if(!deviceId || !devEui || !joinEui || !appId || !token) {
      return res.status(400).json({ error: 'Device ID, DevEUI, JoinEUI, App ID and API token are required' });
    }

    // Construct the request data JSON object for AS
    const requestData = {
      end_device: {
        ids: {
          device_id: deviceId,
          dev_eui: devEui,
          join_eui: joinEui
        },
        application_server_address: "eu1.cloud.thethings.network"
      },
      field_mask: {
        paths: [
          "ids.device_id",
          "ids.dev_eui",
          "ids.join_eui"
        ]
      }
    };

    const response = await axios.put(`https://eu1.cloud.thethings.network/api/v3/as/applications/${appId}/devices/${deviceId}`, requestData, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to create device on Application Server:", error);
    res.status(500).json({ error: 'Failed to create device on Application Server' });
  }
});

// Update device name on TTN
app.put("/api/updateDeviceNameOnTTN", async (req, res) => {
  try {
    const deviceId = req.body.deviceId.toLowerCase();
    const name = req.body.name;
    const token = req.body.Token;
    const appId = req.body.AppID;

    // console.log(req.body);
    // console.log("Device ID:", deviceId);
    // console.log("Name:", name);
    // console.log("Token:", token);
    // console.log("App ID:", appId);

    if (!deviceId || !name || !appId || !token) {
      return res.status(400).json({ error: 'Device ID, App ID, API token and name are required' });
    }

    const requestData = {
      end_device: {
        ids: {
          device_id: deviceId,
          application_ids: {
            application_id: appId
          }
        },
        name: name
      },
      field_mask: {
        paths: ["name"]
      }
    };
    
    const response = await axios.put(`https://eu1.cloud.thethings.network/api/v3/applications/${appId}/devices/eui-${deviceId}`, requestData, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to update device name on TTN:", error);
    res.status(500).json({ error: 'Failed to update device name on TTN' });
  }
});

// Delete device on TTN Network server
app.delete("/api/deleteDeviceOnTTNNS", async (req, res) => {
  try {
    const { Token, AppID, DeviceID } = req.body;

    if (!AppID || !DeviceID || !Token) {
      return res.status(400).json({ error: 'App ID, Device ID, and API token are required in the request body' });
    }

    const url = `https://eu1.cloud.thethings.network/api/v3/ns/applications/${AppID}/devices/${DeviceID}`;
    const response = await axios.delete(url, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${Token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to delete device on Network Server:", error);
    let errorMessage = 'Failed to delete device on Network Server';
    if (error.response && error.response.data && error.response.data.message) {
      errorMessage = error.response.data.message;
    }
    res.status(error.response ? error.response.status : 500).json({ error: errorMessage });
  }
});

// Delete device on TTN Application server
app.delete("/api/deleteDeviceOnTTNAS", async (req, res) => {
  try {
    const { Token, AppID, DeviceID } = req.body;

    if (!AppID || !DeviceID || !Token) {
      return res.status(400).json({ error: 'App ID, Device ID, and API token are required in the request body' });
    }

    const url = `https://eu1.cloud.thethings.network/api/v3/as/applications/${AppID}/devices/${DeviceID}`;
    const response = await axios.delete(url, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${Token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to delete device on Network Server:", error);
    let errorMessage = 'Failed to delete device on Network Server';
    if (error.response && error.response.data && error.response.data.message) {
      errorMessage = error.response.data.message;
    }
    res.status(error.response ? error.response.status : 500).json({ error: errorMessage });
  }
});

// Delete device on TTN (must be deleted on AS, JN, and NS first)
app.delete("/api/deleteDeviceOnTTN", async (req, res) => {
  try {
    const { Token, AppID, DeviceID } = req.body;

    if (!AppID || !DeviceID || !Token) {
      return res.status(400).json({ error: 'App ID, Device ID, and API token are required in the request body' });
    }

    const url = `https://eu1.cloud.thethings.network/api/v3/applications/${AppID}/devices/${DeviceID}`;
    const response = await axios.delete(url, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${Token}`
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to delete device on Network Server:", error);
    let errorMessage = 'Failed to delete device on Network Server';
    if (error.response && error.response.data && error.response.data.message) {
      errorMessage = error.response.data.message;
    }
    res.status(error.response ? error.response.status : 500).json({ error: errorMessage });
  }
});

// Start server
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});