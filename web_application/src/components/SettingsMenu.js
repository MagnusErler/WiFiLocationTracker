import React, { useState, useEffect } from "react";
import "./SettingsMenu.css";
import Switch from '@mui/material/Switch';

const SettingsMenu = ({ isOpen, handleClose, trackerInformation, handleShowLocationSwitch, handleShowMovement, handleTrackerInformationUpdate }) => {
  const [showCurrentLocationIds, setShowCurrentLocationIds] = useState(trackerInformation.map(tracker => tracker.deviceId));
  const [showMovementIds, setShowMovementIds] = useState([]);
  const [originalNames, setOriginalNames] = useState([]);

  useEffect(() => {
    setOriginalNames(trackerInformation.map(tracker => tracker.name));
  }, [isOpen]);

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
    // compare the original names with the updated names and print out the ones that have changed
    const devicesToUpdate = trackerInformation.filter((tracker, index) => tracker.name !== originalNames[index]);
    console.log("Devices to update:", devicesToUpdate);

    // If there are devices to update, make API calls to update each device's name
    if (devicesToUpdate.length === 0) {
      console.log("No devices to update.");
      handleClose();
      return;
    }

    // Make API calls to update each device's name
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
    devicesToUpdate.forEach(async (tracker) => {
      try {
        console.log(`Updating device ${tracker.deviceId} with original name ${originalNames[trackerInformation.findIndex(trackerInfo => trackerInfo.deviceId === tracker.deviceId)]} to new name ${tracker.name}...`);
        const response = await fetch(`https://eu1.cloud.thethings.network/api/v3/applications/${appID}/devices/eui-${tracker.deviceId}`, {
          method: 'PUT',
          headers: {
            'Content-Type': 'application/json',
            Authorization: `Bearer ${token}`,
          },
          body: JSON.stringify({
            end_device: {
              name: tracker.name
            },
            field_mask: {
              paths: ['name']
            }
          })
        });

        if (!response.ok) {
          throw new Error(`Failed to update device ${tracker.deviceId}. ${response.status}, ${response.statusText}`);
        }

        console.log(`Device ${tracker.deviceId} updated successfully`);
      } catch (error) {
        console.error(`Error updating device ${tracker.deviceId}:`, error);
      }
    });




    setOriginalNames(trackerInformation.map(tracker => tracker.name));













    // // Iterate over trackerInformation to find devices with changed names
    // const devicesToUpdate = trackerInformation.filter(tracker => tracker.name !== originalNames);
    
    // // Make API calls to update each device's name
    // devicesToUpdate.forEach(async (tracker) => {
    //   try {
    //     const token = process.env.REACT_APP_TTN_API_TOKEN;
    //     if (!token) {
    //       console.error("API token is not available.");
    //       return;
    //     }
    //     console.log(`Updating device ${tracker.deviceId}...`);
    //     console.log(`New name: ${tracker.name}`);
    //     console.log(`Original name: ${tracker.originalName}`);
    //     console.log(`Application ID: ${tracker.applicationId}`);
    //     const response = await fetch(`/api/v3/applications/${tracker.applicationId}/devices/${tracker.deviceId}`, {
    //       method: 'PUT',
    //       headers: {
    //         'Content-Type': 'application/json',
    //         Authorization: `Bearer ${token}`,
    //       },
    //       body: JSON.stringify({
    //         end_device: {
    //           name: tracker.name
    //         },
    //         field_mask: {
    //           paths: ['name']
    //         }
    //       })
    //     });

    //     if (!response.ok) {
    //       throw new Error(`Failed to update device ${tracker.deviceId}.`);
    //     }

    //     console.log(`Device ${tracker.deviceId} updated successfully`);
    //   } catch (error) {
    //     console.error(`Error updating device ${tracker.deviceId}:`, error);
    //   }
    // });

    // Close the menu
    handleClose();
  };

  return (
    <div className={`modal ${isOpen ? "show" : ""}`}>
      <div className="modal-content">
        <span className="close" onClick={handleCloseAndUpdateDevices}>&times;</span>
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