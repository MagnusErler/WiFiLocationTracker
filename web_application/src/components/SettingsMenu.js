import React, { useState, useEffect } from "react";
import "./SettingsMenu.css";
import Switch from '@mui/material/Switch';

const SettingsMenu = ({ isOpen, handleClose, trackerInformation, handleShowLocationSwitch, handleShowMovement }) => {
  const [showCurrentLocationIds, setShowCurrentLocationIds] = useState(trackerInformation.map(tracker => tracker.deviceId));
  const [showMovementIds, setShowMovementIds] = useState([]);

  const handleLocationSwitch = (id) => {
    const newShowCurrentLocationIds = showCurrentLocationIds.includes(id)
      ? showCurrentLocationIds.filter((checkedId) => checkedId !== id)
      : [...showCurrentLocationIds, id];
    setShowCurrentLocationIds(newShowCurrentLocationIds);
    handleShowLocationSwitch(newShowCurrentLocationIds);
  };
  
  const handleMovementSwitch = (id) => {
    const newShowMovementIds = showMovementIds.includes(id)
      ? showMovementIds.filter((checkedId) => checkedId !== id)
      : [...showMovementIds, id];
    setShowMovementIds(newShowMovementIds);
    handleShowMovement(newShowMovementIds);
  };

  return (
    <div className={`modal ${isOpen ? "show" : ""}`}>
      <div className="modal-content">
        <span className="close" onClick={handleClose}>&times;</span>
        <table>
          <thead>
            <tr>
              <th className="id-column">ID</th>
              <th className="name-column">Name</th>
              <th className="battery-status-column">Battery status</th>
              <th className="temperature-status-column">Degree</th>
              <th className="update-interval-column">Update interval</th>
              <th className="view-location-column">View location</th>
              <th className="view-movement-column">View movement</th>
            </tr>
          </thead>
          <tbody>
            {trackerInformation.map((tracker, index) => (
              <tr key={tracker.deviceId} style={{ backgroundColor: index % 2 === 0 ? "#ffffff" : "#f9f9f9" }}>
                <td className="id-column">{tracker.deviceId}</td>
                <td className="name-column">{tracker.name}</td>
                <td className="battery-status-column">{tracker.batteryStatus}</td>
                <td className="temperature-status-column">{tracker.temperature}</td>
                <td className="update-interval-column">{tracker.updateInterval}</td>
                <td className="view-location-column">
                  <Switch
                    color="warning"
                    checked={showCurrentLocationIds.includes(tracker.deviceId)}
                    onChange={() => handleLocationSwitch(tracker.deviceId)}
                  />
                </td>
                <td className="view-movement-column">
                  <Switch
                    color="warning"
                    checked={showMovementIds.includes(tracker.deviceId)}
                    onChange={() => handleMovementSwitch(tracker.deviceId)}
                  />
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default SettingsMenu;