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
    if (f_port !== 15) {
      return res.status(400).json({ error: "Invalid f_port. Expected f_port 15." });
    }

    const deviceID = end_device_ids?.dev_eui;
    const bytes = decoded_payload?.bytes;

    // Validate the request body and extracted fields
    if (!deviceID || !bytes || bytes.length < 4) {
      return res.status(400).json({ error: "Missing required fields or invalid byte array length." });
    }

    // Extract the values from the bytes array
    const temp = bytes[0] * 5;
    const batteryStatus = bytes[1] / 70;
    const updateInterval = bytes[2];

    // Check if the tracker already exists
    const existingTracker = await TrackerInformation.findOne({
      where: { deviceID },
    });

    // Perform the upsert operation
    const [tracker, created] = await TrackerInformation.upsert(
      { deviceID, temp, updateInterval, batteryStatus },
      {
        returning: true,
      }
    );

    // Send response based on existence check
    if (existingTracker) {
      res.status(200).json(tracker);
    } else {
      res.status(201).json(tracker);
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

module.exports = router;
