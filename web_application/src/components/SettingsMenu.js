import React, { useState, useEffect } from "react";
import "./ModalMenu.css";
import "./SettingsMenu.css";
import Switch from '@mui/material/Switch';
import axios from "axios";

const SettingsMenu = ({ isOpen, handleClose, trackerInformation, handleShowLocationSwitch, handleShowMovement, handleTrackerInformationUpdate, markers }) => {
  const [showCurrentLocationIds, setShowCurrentLocationIds] = useState(trackerInformation.map(tracker => tracker.deviceId));
  const [showMovementIds, setShowMovementIds] = useState([]);
  const [originalNames, setOriginalNames] = useState([]);

  useEffect(() => {
    setOriginalNames(trackerInformation.map(tracker => tracker.name));
  }, [isOpen]);

  const hasCorrespondingPing = (deviceId) => {
    return markers.some(marker => marker.deviceId === deviceId);
  };

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

  const handleNameChange = (id, newName) => {
    const updatedTrackerInformation = trackerInformation.map(tracker => {
      if (tracker.deviceId === id) {
        return { ...tracker, name: newName };
      }
      return tracker;
    });
    // Update tracker information with the new name
    handleTrackerInformationUpdate(updatedTrackerInformation);
  };

  const handleCloseAndUpdateDevices = async () => {
    const devicesToUpdate = trackerInformation.filter((tracker, index) => tracker.name !== originalNames[index]);
  
    if (devicesToUpdate.length === 0) {
      console.log("No devices to update.");
      handleClose();
      return;
    }
  
    const token = process.env.REACT_APP_TTN_API_TOKEN;
    const appID = process.env.REACT_APP_APP_ID;
  
    if (!token) {
      console.error("API token ID is not available.");
      return;
    }
    
    if (!appID) {
      console.error("Application ID is not available.");
      return;
    }
  
    try {
      for (const tracker of devicesToUpdate) {
        console.log(`Updating device ${tracker.deviceId} with original name ${originalNames[trackerInformation.findIndex(trackerInfo => trackerInfo.deviceId === tracker.deviceId)]} to new name ${tracker.name}...`);
  
        const response = await axios.put(`/api/updateDeviceNameOnTTN`, {
          deviceId: tracker.deviceId,
          name: tracker.name,
          Token: token,
          AppID: appID
        });
  
        console.log(`Device ${tracker.deviceId} updated successfully`);
      }
  
      setOriginalNames(trackerInformation.map(tracker => tracker.name));
      handleClose();
    } catch (error) {
      console.error("Error updating devices:", error);
    }
  };

  return (
    <div className={`modal ${isOpen ? "show" : ""}`}>
      <div className="modal-content settings-menu-size">
        <span className="close close-placement-settings-menu" onClick={handleCloseAndUpdateDevices}>&times;</span>
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
                <td className="name-column">
                  {/* Editable name field */}
                  <input
                    type="text"
                    className="editable-input"
                    value={tracker.name}
                    onChange={(e) => handleNameChange(tracker.deviceId, e.target.value)}
                  />
                </td>
                <td className="battery-status-column">{tracker.batteryStatus}</td>
                <td className="temperature-status-column">{tracker.temperature}</td>
                <td className="update-interval-column">{tracker.updateInterval}</td>
                <td className="view-location-column">
                  <Switch
                    color="warning"
                    checked={showCurrentLocationIds.includes(tracker.deviceId)}
                    disabled={!hasCorrespondingPing(tracker.deviceId)}
                    onChange={() => handleLocationSwitch(tracker.deviceId)}
                  />
                </td>
                <td className="view-movement-column">
                  <Switch
                    color="warning"
                    checked={showMovementIds.includes(tracker.deviceId)}
                    disabled={!hasCorrespondingPing(tracker.deviceId)}
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