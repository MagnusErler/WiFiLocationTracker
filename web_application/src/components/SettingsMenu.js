import React, { useState, useEffect } from "react";
import "./SettingsMenu.css";

const SettingsMenu = ({ isOpen, handleClose, markers, handleCheckboxChange }) => {
  // Extracting unique IDs from markers
  const uniqueIds = [...new Set(markers.map(marker => marker.id))];
  const [checkedIds, setCheckedIds] = useState(uniqueIds);

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
        <h1>Settings</h1>
        <div className="checkboxes">
          {uniqueIds.map(id => (
            <div key={id} className="checkbox">
              <input
                type="checkbox"
                id={`checkbox-${id}`}
                checked={checkedIds.includes(id)}
                onChange={() => handleCheckbox(id)}
              />
              <label htmlFor={`checkbox-${id}`}>{`Checkbox for ID ${id}`}</label>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default SettingsMenu;

//TODO: Implement checkboxes for showing markers and polylines on the map