import React, { useState, useEffect, useRef } from "react";
import "./ModalMenu.css";
import "./SettingsMenu.css";
import axios from "axios";
import Switch from '@mui/material/Switch';
import UpdateIcon from '@mui/icons-material/Update';
import FmdGoodIcon from '@mui/icons-material/FmdGood';
import TimelineIcon from '@mui/icons-material/Timeline';
import DeleteForeverIcon from '@mui/icons-material/DeleteForever';
import BatteryUnknownIcon from '@mui/icons-material/BatteryUnknown';
import DeviceThermostatIcon from '@mui/icons-material/DeviceThermostat';
import ConfirmationDialog from "./ConfirmationDialog";

const SettingsMenu = React.forwardRef(({ isOpen, handleClose, trackerInformation, handleShowLocationSwitch, handleShowMovement, handleTrackerInformationUpdate, markers, dataLoaded }, ref) => {
  const [showCurrentLocationIds, setShowCurrentLocationIds] = useState([]);
  const [showMovementIds, setShowMovementIds] = useState([]);
  const [originalNames, setOriginalNames] = useState([]);
  const [deviceToDelete, setDeviceToDelete] = useState(null);
  const [confirmationOpen, setConfirmationOpen] = useState(false);
  const [isDeleting, setIsDeleting] = useState(false);
  const menuRef = useRef(null);

  useEffect(() => {
    setOriginalNames(trackerInformation.map(tracker => tracker.name));
  }, [isOpen]);

  useEffect(() => {
    if (dataLoaded) {
      setShowCurrentLocationIds(trackerInformation.map(tracker => tracker.deviceId));
    }
  }, [dataLoaded, trackerInformation]);

  const hasCorrespondingPing = (deviceId) => {
    return markers.some(marker => marker.deviceId.toUpperCase() === deviceId.toUpperCase());
  };

  const handleLocationSwitch = (id) => {
    const newShowCurrentLocationIds = showCurrentLocationIds.includes(id)
      ? showCurrentLocationIds.filter((checkedId) => checkedId !== id)
      : [...showCurrentLocationIds, id];
  
    setShowCurrentLocationIds(newShowCurrentLocationIds);
    handleShowLocationSwitch(newShowCurrentLocationIds);
  
    // If current location is being disabled, disable movement as well
    if (!newShowCurrentLocationIds.includes(id)) {
      const newShowMovementIds = showMovementIds.filter((checkedId) => checkedId !== id);
      setShowMovementIds(newShowMovementIds);
      handleShowMovement(newShowMovementIds);
    }
  };
  
  //TODO: Make sure that only corresponding switches is co-dependent
  const handleMovementSwitch = (id) => {
    const newShowMovementIds = showMovementIds.includes(id)
      ? showMovementIds.filter((checkedId) => checkedId !== id)
      : [...showMovementIds, id];
  
    setShowMovementIds(newShowMovementIds);
    handleShowMovement(newShowMovementIds);
  
    // If movement is being enabled, enable current location as well
    if (newShowMovementIds.includes(id)) {
      const newShowCurrentLocationIds = showCurrentLocationIds.includes(id)
        ? showCurrentLocationIds
        : [...showCurrentLocationIds, id];
      setShowCurrentLocationIds(newShowCurrentLocationIds);
      handleShowLocationSwitch(newShowCurrentLocationIds);
    }
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
  
    const token = process.env.REACT_APP_TTN_API_KEY;
    const appID = process.env.REACT_APP_TTN_APP_ID;
  
    if (!token) {
      console.error("API KEY is not available.");
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

  const deleteDevice = async (deviceID) => {
    try {
      setIsDeleting(true);
      const TTNtoken = process.env.REACT_APP_TTN_API_KEY;
      const LoraCloudtoken = process.env.REACT_APP_LORACLOUD_API_KEY;
      const appID = process.env.REACT_APP_TTN_APP_ID;
      if (!token) {
        console.error("API token ID is not available.");
        return;
      }
      
      if (!appID) {
        console.error("Application ID is not available.");
        return;
      }
      console.log(`Deleting device with ID ${deviceID}...`);

      // Delete device on LoRaCloud
      await Promise.all([
        axios.delete(`/api/unclaimDeviceOnJoinServer`, { data: [{ "DevEUI": deviceID }] }),
        axios.delete(`/api/unclaimDeviceFromModemServices`, { 
          data: {
            Token: LoraCloudtoken,
            deveuis: [deviceID]
          }
        }),
      ]);

      // Delete device on TTN network and application servers
      await Promise.all([
        axios.delete(`/api/deleteDeviceOnTTNNS`, { data: { Token: TTNtoken, AppID: appID, DeviceID: "eui-" + deviceID } }),
        axios.delete(`/api/deleteDeviceOnTTNAS`, { data: { Token: TTNtoken, AppID: appID, DeviceID: "eui-" + deviceID } })
      ]);

      // After successfully deleting on other servers, call deleteDeviceOnTTN
      await axios.delete(`/api/deleteDeviceOnTTN`, {
        data: { Token: TTNtoken, AppID: appID, DeviceID: "eui-" + deviceID }
      });
  
      // Update the UI accordingly (remove the deleted device from state)
      const updatedTrackerInformation = trackerInformation.filter(tracker => tracker.deviceId !== deviceID);
      handleTrackerInformationUpdate(updatedTrackerInformation);
  
      // Optionally, provide feedback to the user
      console.log(`Device with ID ${deviceID} deleted successfully.`);
      setIsDeleting(false);
    } catch (error) {
      setIsDeleting(false);
      console.error("Error deleting device:", error);
      // Optionally, handle errors or provide feedback to the user
    }
  };

  const handleDeleteConfirmation = async () => {
    if (deviceToDelete) {
      await deleteDevice(deviceToDelete);
      setDeviceToDelete(null);
      setConfirmationOpen(false); // Close the confirmation dialog
    }
  };

  const toggleConfirmationDialog = (deviceId) => {
    setDeviceToDelete(deviceId);
    setConfirmationOpen(true); // Open the confirmation dialog
  };

  useEffect(() => {
    const handleClickOutside = (event) => {
      if (!confirmationOpen && menuRef.current && !menuRef.current.contains(event.target)) {
        handleClose(); // Close the menu if clicked outside and confirmation dialog is not open
      }
    };

    document.addEventListener("mousedown", handleClickOutside);
    return () => {
      document.removeEventListener("mousedown", handleClickOutside);
    };
  }, [handleClose, confirmationOpen]);

  return (
    <div ref={ref} className={`modal ${isOpen ? "show" : ""}`}>
      <div ref={menuRef} className="modal-content settings-menu-size">
        <span className="close close-placement-settings-menu" onClick={handleCloseAndUpdateDevices}>&times;</span>
        <table>
          {/* Table headers */}
          <thead>
            <tr>
              <th className="id-column">ID</th>
              <th className="name-column">Name</th>
              <th className="battery-status-column"><BatteryUnknownIcon /></th>
              <th className="temperature-status-column"><DeviceThermostatIcon /></th>
              <th className="update-interval-column"><UpdateIcon /></th>
              <th className="view-location-column"><FmdGoodIcon /></th>
              <th className="view-movement-column"><TimelineIcon /></th>
              <th className="delete-device-column"><DeleteForeverIcon /></th>
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
                <td className="delete-device-column">
                  <DeleteForeverIcon className="delete-icon" onClick={() => toggleConfirmationDialog(tracker.deviceId)} />
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
      {confirmationOpen && (
        <ConfirmationDialog
          message="Are you sure you want to delete this device?"
          onConfirm={handleDeleteConfirmation}
          onCancel={() => setConfirmationOpen(false)}
          confirmText={isDeleting ? "Removing..." : "Confirm"}
          confirmDisabled={isDeleting}
        />
      )}
    </div>
  );
});

export default SettingsMenu;