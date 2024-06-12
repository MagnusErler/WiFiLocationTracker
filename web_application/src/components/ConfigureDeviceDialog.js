import React, { useState } from "react";
import Switch from "@mui/material/Switch";
import "./ConfigureDeviceDialog.css";

const ConfigureDeviceDialog = ({
  deviceId,
  trackerInformation,
  handleClose,
}) => {
  const [hours, setHours] = useState("");
  const [minutes, setMinutes] = useState("");
  const [lorawanClass, setLorawanClass] = useState("A");
  const [wifiScanning, setWifiScanning] = useState(false);
  const [gnssScanning, setGnssScanning] = useState(false);

  const handleSave = () => {
    handleClose();
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
              value={hours}
              onChange={(e) => setHours(e.target.value)}
            />
          </div>
          <div className="input-field">
            <label htmlFor="minutes">Update Interval (Minutes)</label>
            <input
              id="minutes"
              type="number"
              value={minutes}
              onChange={(e) => setMinutes(e.target.value)}
            />
          </div>
          <div className="input-field">
            <label htmlFor="lorawanClass">LoRaWAN Class</label>
            <select
              id="lorawanClass"
              value={lorawanClass}
              onChange={(e) => setLorawanClass(e.target.value)}
            >
              <option value="A">Class A</option>
              <option value="B">Class B</option>
              <option value="C">Class C</option>
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
          <button onClick={handleClose}>Cancel</button>
          <button onClick={handleSave} className="primary-button">
            Save
          </button>
        </div>
      </div>
    </div>
  );
};

export default ConfigureDeviceDialog;