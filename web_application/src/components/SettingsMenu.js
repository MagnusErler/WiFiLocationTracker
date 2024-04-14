import React, { useState, useEffect } from "react";
import "./SettingsMenu.css";
import Checkbox from '@mui/material/Checkbox';
import Switch from '@mui/material/Switch';

const SettingsMenu = ({ isOpen, handleClose, trackerInformation, handleCheckboxChange }) => {
  const [checkedIds, setCheckedIds] = useState([]);

  useEffect(() => {
    handleCheckboxChange(checkedIds);
  }, [checkedIds, handleCheckboxChange]);

  const handleCheckbox = (id) => {
    const newCheckedIds = checkedIds.includes(id)
      ? checkedIds.filter((checkedId) => checkedId !== id)
      : [...checkedIds, id];
    setCheckedIds(newCheckedIds);
    handleCheckboxChange(newCheckedIds);
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
              <th className="view-location-column">View location</th>
              <th className="view-travel-column">View travel</th>
            </tr>
          </thead>
          <tbody>
            {trackerInformation.map((tracker, index) => (
              <tr key={tracker.id} style={{ backgroundColor: index % 2 === 0 ? "#ffffff" : "#f9f9f9" }}>
                <td className="id-column">{tracker.id}</td>
                <td className="name-column">{tracker.name}</td>
                <td className="battery-status-column">{tracker.batteryStatus}</td>
                <td className="view-location-column">
                  <Switch 
                    color="warning" 
                    checked={checkedIds.includes(tracker.id)}
                    onChange={() => handleCheckbox(tracker.id)}
                  />
                </td>
                <td className="view-travel-column">
                  <Switch 
                    color="warning" 
                    checked={checkedIds.includes(tracker.id)}
                    onChange={() => handleCheckbox(tracker.id)}
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

//TODO: Implement checkboxes for showing markers and polylines on the map