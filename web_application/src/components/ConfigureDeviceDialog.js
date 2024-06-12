import React, { useEffect, useState } from "react";
import Switch from "@mui/material/Switch";
import "./ConfigureDeviceDialog.css";

const ConfigureDeviceDialog = ({ tracker, onConfirm, onCancel }) => {
  const [hours, setHours] = useState("0");
  const [minutes, setMinutes] = useState("0");
  const [loraWANClass, setLorawanClass] = useState(tracker.loraWANClass || "1");
  const [wifiScanning, setWifiScanning] = useState(Boolean(tracker.wifiStatus) || false);
  const [gnssScanning, setGnssScanning] = useState(Boolean(tracker.gnssStatus) || false);

  const { deviceId } = tracker;
  const updateInterval = tracker.updateInterval === "-" ? 0 : (tracker.updateInterval || 0);

  useEffect(() => {
    convertUpdateInterval(updateInterval);
  }, [updateInterval]);

  const handleSave = () => {
    const updatedTracker = {
      ...tracker,
      updateInterval: convertHoursAndMinutesToMinutes(hours, minutes),
      loraWANClass,
      wifiStatus: wifiScanning,
      gnssStatus: gnssScanning,
    };
    onConfirm(updatedTracker);
  };

  const convertUpdateInterval = (interval) => {
    const hrs = Math.floor(interval / 60);
    const mins = interval % 60;
    setHours(hrs.toString());
    setMinutes(mins.toString());
  };

  const convertHoursAndMinutesToMinutes = (hrs, mins) => {
    const hoursAsNumber = parseInt(hrs, 10);
    const minutesAsNumber = parseInt(mins, 10);
    return isNaN(hoursAsNumber) || isNaN(minutesAsNumber) ? 0 : hoursAsNumber * 60 + minutesAsNumber;
  };

  const handleHoursChange = (e) => {
    let value = e.target.value.trim() === "" ? "0" : e.target.value;
    value = parseInt(value, 10);
    if (isNaN(value) || value < 0 || value > 8760) {
      value = parseInt(hours, 10); // Reset to previous value if invalid input
    }
    setHours(value.toString());
  };

  const handleMinutesChange = (e) => {
    let value = e.target.value.trim() === "" ? "0" : e.target.value;
    value = parseInt(value, 10);
    if (isNaN(value) || value < 0 || value > 525600) {
      value = parseInt(minutes, 10); // Reset to previous value if invalid input
    }
    setMinutes(value.toString());
  };

  const handleInputKeyDown = (e) => {
    if (e.key === "-") {
      e.preventDefault(); // Prevent typing "-"
    }
  };

  return (
    <div className="dialog-container">
      <div className="dialog">
        <div className="dialog-title">Configure Device</div>
        <div className="dialog-content">
          <div className="device-info">
            <p>
              <strong>Device ID:</strong> {deviceId}
            </p>
          </div>
          <div className="input-field">
            <label htmlFor="hours">Update Interval (Hours)</label>
            <input
              id="hours"
              type="number"
              min="0"
              max="8760"
              value={hours}
              onChange={handleHoursChange}
              onKeyDown={handleInputKeyDown}
            />
          </div>
          <div className="input-field">
            <label htmlFor="minutes">Update Interval (Minutes)</label>
            <input
              id="minutes"
              type="number"
              min="0"
              max="525600"
              value={minutes}
              onChange={handleMinutesChange}
              onKeyDown={handleInputKeyDown}
            />
          </div>
          <div className="input-field">
            <label htmlFor="lorawanClass">LoRaWAN Class</label>
            <select
              id="lorawanClass"
              value={loraWANClass}
              onChange={(e) => setLorawanClass(e.target.value)}
            >
              <option value="1">Class A</option>
              <option value="2">Class B</option>
              <option value="3">Class C</option>
            </select>
          </div>
          <div className="input-field-switch">
            <span className="label-text">WiFi Scanning</span>
            <div className="switch-container">
              <Switch
                checked={wifiScanning}
                onChange={(e) => setWifiScanning(e.target.checked)}
                color="primary"
              />
            </div>
          </div>
          <div className="input-field-switch">
            <span className="label-text">GNSS Scanning</span>
            <div className="switch-container">
              <Switch
                checked={gnssScanning}
                onChange={(e) => setGnssScanning(e.target.checked)}
                color="primary"
              />
            </div>
          </div>
        </div>
        <div className="dialog-actions">
          <button onClick={onCancel} className="button cancel-button">Cancel</button>
          <button onClick={handleSave} className="button primary-button">Save</button>
        </div>
      </div>
    </div>
  );
};

export default ConfigureDeviceDialog;