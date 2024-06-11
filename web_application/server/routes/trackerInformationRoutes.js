const express = require("express");
const TrackerInformation = require("../models/trackerInformation");

const router = express.Router();

// Endpoint to update tracker information
router.post("/trackerInformation", async (req, res) => {
  try {
    const { deviceID, temp, updateInterval, batteryStatus } = req.body;

    // Validate the request body
    if (
      !deviceID ||
      temp === undefined ||
      updateInterval === undefined ||
      batteryStatus === undefined
    ) {
      return res.status(400).json({ error: "Missing required fields" });
    }

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

// Endpoint to retrieve current information for a tracker
router.get("/trackerInformation/:devEUI", async (req, res) => {
  try {
    const tracker = await TrackerInformation.findOne({
      where: { devEUI: req.params.devEUI },
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
