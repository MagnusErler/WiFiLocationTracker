const express = require("express");
const TrackerInformation = require("../models/trackerInformation");

const router = express.Router();

// Endpoint to update tracker information from uplink messages
router.post("/trackerInformation", async (req, res) => {
  try {
    //console.log("Received request to update tracker information:", req.body);
    const { end_device_ids, uplink_message } = req.body;
    const { f_port, decoded_payload } = uplink_message || {};
    
    // Check the f_port
    if (f_port !== 10 && f_port !== 2) {
      return res.status(400).json({ error: "Invalid f_port. Expected f_port 2 or 10." });
    }

    const deviceID = end_device_ids?.dev_eui;
    const bytes = decoded_payload?.bytes;

    let tracker; // Declare tracker variable outside

    if(f_port === 10) {
      if (!deviceID || !bytes || bytes.length < 6) {
        return res.status(400).json({ error: "Missing required fields or invalid byte array length." });
      }

      // Extract values from the bytes array for f_port 10
      let temperature = bytes[4] * 5;
      let updateInterval = bytes[0] + bytes[1];
      let batteryStatus = bytes[5] / 77;
      let wifiStatus = false;
      let gnssStatus = false;
      let loraWANClass = bytes[2];

      if (bytes[3] === 1) {
        wifiStatus = true;
      } else if (bytes[3] === 2) {
        gnssStatus = true;
      } else if (bytes[3] === 3) {
        wifiStatus = true;
        gnssStatus = true;
      }

      // Perform upsert operation for f_port 10
      [tracker, created] = await TrackerInformation.upsert(
        { deviceID, temperature, updateInterval, batteryStatus, wifiStatus, gnssStatus, loraWANClass },
        { returning: true }
      );
      
    } else if(f_port === 2) {
      if (!deviceID || !bytes || bytes.length < 4) {
        return res.status(400).json({ error: "Missing required fields or invalid byte array length." });
      }

      // Extract values from the bytes array for f_port 2
      let temperature = bytes[0] * 5;
      let batteryStatus = bytes[1] / 77;

      // Perform upsert operation for f_port 2
      [tracker, created] = await TrackerInformation.upsert(
        { deviceID, temperature, batteryStatus },
        { returning: true }
      );
    }

    // Send response based on existence check
    if (tracker) {
      if (created) {
        res.status(201).json(tracker);
      } else {
        res.status(200).json(tracker);
      }
    } else {
      res.status(400).json({ error: "Failed to update tracker information." });
    }
    
  } catch (error) {
    console.error("Error updating tracker information:", error.message);
    res.status(500).send("Internal server error");
  }
});

// Endpoint to update specific tracker information
router.put("/trackerInformation/:devEUI", async (req, res) => {
  try {
    const deviceID = req.params.devEUI;
    const { updateInterval, wifiStatus, gnssStatus, loraWANClass } = req.body;

    // Perform the upsert operation
    const [tracker, created] = await TrackerInformation.upsert(
      { deviceID, updateInterval, wifiStatus, gnssStatus, loraWANClass },
      {
        returning: true,
      }
    );

    // Send response based on existence check
    if (created) {
      res.status(201).json(tracker);
    } else {
      res.status(200).json(tracker);
    }
  } catch (error) {
    console.error("Error updating tracker information:", error.message);
    res.status(500).send("Internal server error");
  }
});

// Endpoint to retrieve current information for a tracker
router.get("/trackerInformation/:devEUI", async (req, res) => {
  try {
    const tracker = await TrackerInformation.findOne({
      where: { deviceID: req.params.devEUI },
    });

    if (!tracker) {
      return res.status(404).json({ error: "Tracker not found" });
    }

    res.status(200).json(tracker);
  } catch (error) {
    console.error("Error retrieving tracker information:", error.message);
    res.status(500).send("Internal server error");
  }
});

// Endpoint to delete specific tracker information
router.delete("/trackerInformation/:devEUI", async (req, res) => {
  const deviceID = req.params.devEUI;

  // Perform deletion operation
  await TrackerInformation.destroy({
    where: { deviceID },
  });
  res.status(200).json({ message: "Tracker information deleted successfully" });
});

module.exports = router;