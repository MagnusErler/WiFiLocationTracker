import React from "react";
import "./SettingsMenu.css";

const SettingsMenu = ({ isOpen, handleClose }) => {
  return (
    <div className={`modal ${isOpen ? "show" : ""}`}>
      <div className="modal-content">
        <span className="close" onClick={handleClose}>&times;</span>
        <h1>Settings</h1>
        <div className="checkboxes">
          {[...Array(10)].map((_, index) => (
            <div key={index} className="checkbox">
              <input type="checkbox" id={`checkbox-${index}`} />
              <label htmlFor={`checkbox-${index}`}>{`Checkbox ${index + 1}`}</label>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default SettingsMenu;