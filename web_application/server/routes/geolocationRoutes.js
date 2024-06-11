const express = require("express");
const GeolocationSolve = require('../models/geolocationSolve');
const { deg2rad } = require('../helpers');

const router = express.Router();

// Returns all pings for all devices
router.get("/geolocationSolves", async (req, res) => {
  try {
    const geolocationSolves = await GeolocationSolve.findAll();
    res.status(200).json(geolocationSolves);
  } catch (error) {
    console.error("Error querying database:", error.message);
    res.status(500).send("Internal server error");
  }
});

// Returns all pings for a specific device
router.get("/geolocationSolves/:deviceID", async (req, res) => {
  try {
    const geolocationSolvesWithDeviceID = await GeolocationSolve.findAll({
      where: { deviceID: req.params.deviceID },
    });

    if (geolocationSolvesWithDeviceID.length === 0) {
      console.log("Device ID not found:", req.params.deviceID);
      return res.status(404).json({ error: "Device ID not found" });
    }

    res.status(200).json(geolocationSolvesWithDeviceID);
  } catch (error) {
    console.error("Error querying database:", error.message);
    res.status(500).send("Internal server error");
  }
});

// Add a ping to the database
router.post("/geolocationSolves", async (req, res) => {
  try {
    const { end_device_ids, location_solved } = req.body;

    if (
      !end_device_ids ||
      !end_device_ids.dev_eui ||
      !location_solved ||
      !location_solved.location
    ) {
      return res.status(400).json({ error: "Invalid data" });
    }

    const { location } = location_solved;
    const { latitude, longitude, accuracy, source } = location;

    if (
      latitude === undefined ||
      longitude === undefined ||
      accuracy === undefined ||
      !source
    ) {
      return res.status(400).json({ error: "Incomplete location data" });
    }

    const deviceID = end_device_ids.dev_eui;
    const geocode = [latitude, longitude];

    const existingPings = await GeolocationSolve.findAll({
      where: { deviceID },
      order: [["createdAt", "DESC"]],
      limit: 1,
    });

    if (existingPings.length > 0) {
      const latestPing = existingPings[0];
      const latestGeocode = latestPing.geocode;

      const R = 6371;
      const dLat = deg2rad(latitude - latestGeocode[0]);
      const dLon = deg2rad(longitude - latestGeocode[1]);

      const a =
        Math.sin(dLat / 2) * Math.sin(dLat / 2) +
        Math.cos(deg2rad(latestGeocode[0])) *
          Math.cos(deg2rad(latitude)) *
          Math.sin(dLon / 2) *
          Math.sin(dLon / 2);
      const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
      const d = R * c * 1000; // Distance in meters

      if (d < 15) {
        await latestPing.update({ geocode, accuracy, source });
        return res.status(204).json(latestPing);
      }
    }

    const geolocationSolve = await GeolocationSolve.create({
      deviceID,
      geocode,
      accuracy,
      source,
    });
    res.status(201).json(geolocationSolve);
  } catch (error) {
    console.error("Error adding geolocation solve to database:", error.message);
    res.status(500).send("Internal server error");
  }
});

module.exports = router;