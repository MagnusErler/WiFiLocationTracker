const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const path = require('path');
const { Sequelize, Model, DataTypes } = require('sequelize');
const axios = require("axios");
const fs = require("fs");
const https = require("https");

const app = express();
const PORT = process.env.PORT || 3001;

app.use(cors());

// Create Sequelize instance
const sequelize = new Sequelize({
  dialect: 'sqlite',
  storage: './geolocationdata.sqlite'
});

// Define GeolocationSolve model
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
}, { sequelize, modelName: 'geolocationSolve' });

// Sync model with database
sequelize.sync();

// Middleware for parsing request body
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('api/geolocationSolves', async (req, res) => {
  try {
    const geolocationSolves = await GeolocationSolve.findAll();
    res.json(geolocationSolves);
  } catch (error) {
    console.error('Error querying database:', error.message);
  }
});

app.get('api/geolocationSolves/:deviceID', async (req, res) => {
  try {
    const geolocationSolvesWithDeviceID = await GeolocationSolve.findAll({where: {deviceID: req.params.deviceID}});
    res.json(geolocationSolvesWithDeviceID);
  } catch (error) {
    console.error('Error querying database:', error.message);
  }
});

app.post('api/geolocationSolves', async (req, res) => {
  try {
    console.log(req.body);
    const { end_device_ids, location_solved } = req.body;
    
    // Extract device ID and geolocation data
    const deviceID = end_device_ids.dev_eui;
    const { location } = location_solved;
    const { latitude, longitude, accuracy, source } = location;
    
    // Format geocode
    const geocode = [latitude, longitude];
      
    // Save geolocation solve to the database
    const geolocationSolve = await GeolocationSolve.create({ deviceID, geocode, accuracy, source});
    
    res.json(geolocationSolve);
    console.log('Geolocation solve added to database:', geolocationSolve.toJSON());
  } catch (error) {
    console.error('Error querying database:', error.message);
    res.status(500).json({ error: 'Internal server error' });
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

    res.json(response.data);
  } catch (error) {
    console.error("Failed to fetch device information:", error);
    res.status(500).json({ error: "Failed to fetch device information" });
  }
});

// TODO: Test if join actually works, and how does the double error handling work?
app.post("/api/claimDevicesOnJoinServer", async (req, res) => {
  console.log("Claiming devices...")
  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/claim_devices";
    const url = `https://${h}${u}`;
    const cert_file = path.join(__dirname, "../credentials", `${a}.crt`);
    const key_file = path.join(__dirname, "../credentials", `${a}.key`);
    const trust_file = path.join(__dirname, "../credentials", `${a}.trust`);

    const devices = req.body;

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

    if (Array.isArray(response.data)) {
      const failedDevices = response.data.filter(device => device.error);
      if (failedDevices.length > 0) {
        throw new Error();
      }
    } else {
      throw new Error();
    }

    res.json(response.data);
    console.log("Devices claimed successfully:", response.data);
  } catch (error) {
    console.error("Failed to claim devices:", error);
    res.status(500).json({ error: error.message });
  }
});

// TODO: Check of this endpoint works
app.get("/api/unclaimDeviceFromJoinServer", async (req, res) => {
  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/unclaim_devices";
    const url = `https://${h}${u}`;
    const cert_file = path.join(__dirname, "../credentials", `${a}.crt`);
    const key_file = path.join(__dirname, "../credentials", `${a}.key`);
    const trust_file = path.join(__dirname, "../credentials", `${a}.trust`);

    const deviceEUIs = req.body;

    const response = await axios.get(url, deviceEUIs, {
      httpsAgent: new https.Agent({
        cert: fs.readFileSync(cert_file),
        key: fs.readFileSync(key_file),
        ca: fs.readFileSync(trust_file)
      }),
      headers: {
        'Content-Type': 'application/json'
      }
    });

    res.json(response.data);
  } catch (error) {
    console.error("Failed to fetch device information:", error);
    res.status(500).json({ error: "Failed to fetch device information" });
  }
});

// Helper function to make HTTP requests
async function makeRequest(method, url, data, headers) {
  try {
    const response = await axios({ method, url, data, headers });
    return response.data;
  } catch (error) {
    console.error('Error:', error.response.data);
    throw new Error('An error occurred while making the request.');
  }
}

// TODO: test if this endpoint works
app.post('/api/createDeviceOnTTN', async (req, res) => {
  try {
    const { device_id, dev_eui, join_eui } = req.body;

    // Step 1: Create device on identity server
    const payloadIdServer = {
      end_device: {
        ids: {
          device_id,
          dev_eui,
          join_eui
        },
        join_server_address: "js.loracloud.com:7009",
        network_server_address: "eu1.cloud.thethings.network",
        application_server_address: "eu1.cloud.thethings.network"
      },
      field_mask: {
        paths: [
          "join_server_address",
          "network_server_address",
          "application_server_address",
          "ids.dev_eui",
          "ids.join_eui"
        ]
      }
    };

    const createDeviceResponse = await axios.post('https://thethings.example.com/api/v3/applications/my-test-app/devices', payloadIdServer, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': '${token}'
      }
    });

    // Check if step 1 was successful
    if (createDeviceResponse.status !== 200) {
      throw new Error(`Failed to create device on TTN: ${createDeviceResponse.statusText}`);
    }

    // Step 2: Create device on network server
    const payloadNwServer = {
      end_device: {
        supports_join: true,
        lorawan_version: "1.0.3",
        ids: {
          device_id,
          dev_eui,
          join_eui
        },
        lorawan_phy_version: "1.0.3-b",
        frequency_plan_id: "EU_863_870_TTN"
      },
    };

    const updateNetworkResponse = await axios.post('https://thethings.example.com/api/v3/applications/my-test-app/devices', payloadNwServer, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': '${token}'
      }
    });

    // Check if step 2 was successful
    if (updateNetworkResponse.status !== 200) {
      throw new Error(`Failed to update device on the Network Server: ${updateNetworkResponse.statusText}`);
    }

    // Step 3: Update the device on the Application Server
    const payloadAppServer = {
      end_device: {
        ids: {
          device_id,
          dev_eui,
          join_eui
        }
      },
      field_mask: {
        paths: [
          "ids.device_id",
          "ids.dev_eui",
          "ids.join_eui"
        ]
      }
    };

    const updateAppResponse = await axios.put(`https://thethings.example.com/api/v3/as/applications/my-test-app/devices/${device_id}`, payloadAppServer, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': token
      }
    });

    res.json(updateAppResponse.data);
  } catch (error) {
    console.error('Error creating device:', error);
    let errorMessage = 'An error occurred while creating the device.';
    if (error.response && error.response.data) {
      errorMessage = error.response.data.error || errorMessage;
    }
    res.status(500).json({ error: errorMessage });
  }
});

app.post('/api/createDeviceOnTTN', async (req, res) => {
  try {
    const { device_id, dev_eui, join_eui } = req.body;
    
    // 
    const payload = {
      end_device: {
        ids: {
          device_id,
          dev_eui,
          join_eui
        },
        join_server_address: "js.loracloud.com:7009",
        network_server_address: "eu1.cloud.thethings.network",
        application_server_address: "eu1.cloud.thethings.network"
      },
      field_mask: {
        paths: [
          "join_server_address",
          "network_server_address",
          "application_server_address",
          "ids.dev_eui",
          "ids.join_eui"
        ]
      }
    };
    
    const response = await axios.post('https://eu1.cloud.thethings.network/api/v3/applications/my-test-app/devices', payload, {
      headers: {
        'Content-Type': 'application/json',
        'Authorization': '${token}'
      }
    });
    





    res.json(response.data);
  } catch (error) {
    console.error('Error creating device:', error.response.data);
    res.status(500).json({ error: 'An error occurred while creating the device.' });
  }
});

// Start server
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});