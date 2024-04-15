import React, { useState, useEffect } from "react";
import "./SettingsMenu.css";
import Switch from '@mui/material/Switch';

const SettingsMenu = ({ isOpen, handleClose, trackerInformation, handleShowLocationSwitch, handleShowMovement }) => {
  const [showCurrentLocationIds, setShowCurrentLocationIds] = useState(trackerInformation.map(tracker => tracker.id));
  const [showMovementIds, setShowMovementIds] = useState([]);

  useEffect(() => {
    handleShowLocationSwitch(showCurrentLocationIds);
    handleShowMovement(showMovementIds);
  }, [showCurrentLocationIds, showMovementIds, handleShowLocationSwitch, handleShowMovement]);

  const handleLocationCheckbox = (id) => {
    const newShowCurrentLocationIds = showCurrentLocationIds.includes(id)
      ? showCurrentLocationIds.filter((checkedId) => checkedId !== id)
      : [...showCurrentLocationIds, id];
    setShowCurrentLocationIds(newShowCurrentLocationIds);
  
    // Disable showMovementIds for the ID being toggled
    setShowMovementIds(prevIds => prevIds.filter(prevId => prevId !== id));
  
    handleShowLocationSwitch(newShowCurrentLocationIds, showMovementIds);
  };
  
  
  const handleMovementCheckbox = (id) => {
    const newShowMovementIds = showMovementIds.includes(id)
      ? showMovementIds.filter((checkedId) => checkedId !== id)
      : [...showMovementIds, id];
    setShowMovementIds(newShowMovementIds);
  
    // Enable showCurrentLocationIds only for the ID being toggled
    if (!showCurrentLocationIds.includes(id) && newShowMovementIds.includes(id)) {
      setShowCurrentLocationIds(prevIds => [...prevIds, id]);
    }
  
    handleShowMovement(showCurrentLocationIds, newShowMovementIds);
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
              <tr key={tracker.id} style={{ backgroundColor: index % 2 === 0 ? "#ffffff" : "#f9f9f9" }}>
                <td className="id-column">{tracker.id}</td>
                <td className="name-column">{tracker.name}</td>
                <td className="battery-status-column">{tracker.batteryStatus}</td>
                <td className="temperature-status-column">{tracker.temperature}</td>
                <td className="update-interval-column">{tracker.updateInterval}</td>
                <td className="view-location-column">
                  <Switch
                    color="warning"
                    checked={showCurrentLocationIds.includes(tracker.id)}
                    onChange={() => handleLocationCheckbox(tracker.id)}
                  />
                </td>
                <td className="view-movement-column">
                  <Switch
                    color="warning"
                    checked={showMovementIds.includes(tracker.id)}
                    onChange={() => handleMovementCheckbox(tracker.id)}
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