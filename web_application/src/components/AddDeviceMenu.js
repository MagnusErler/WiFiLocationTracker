import React, { useState } from "react";
import "./ModalMenu.css"
import "./AddDeviceMenu.css";

const AddDeviceMenu = ({ isOpen, handleClose }) => {
  const [devEUI, setDevEUI] = useState("");
  const [joinEUI, setJoinEUI] = useState("00-16-C0-01-FF-FE-00-01");
  const [pin, setPin] = useState("");
  const [errorMessage, setErrorMessage] = useState("");

  const validateHex = (value) => {
    const hexRegex = /^[0-9A-Fa-f]+$/;
    return hexRegex.test(value);
  };

  const handleChangeDevEUI = (e) => {
    const value = e.target.value.toUpperCase().replace(/[^0-9A-F]/g, "");
    let formattedValue = value.replace(/-/g, ''); // Remove existing hyphens
    if (formattedValue.length <= 16) {
      formattedValue = formattedValue
        .match(/.{1,2}/g) // Split the value into groups of 2 characters
        ?.join("-") || ''; // Join the groups with hyphens if formattedValue is not null or undefined
  
      // If the last character entered is a hyphen, remove it
      if (value.endsWith('-')) {
        formattedValue = formattedValue.slice(0, -1);
      }
  
      setDevEUI(formattedValue);
    }
  };

  const handleChangeJoinEUI = (e) => {
    const value = e.target.value.toUpperCase().replace(/[^0-9A-F]/g, "");
    let formattedValue = value.replace(/-/g, ''); // Remove existing hyphens
    if (formattedValue.length <= 16) {
      formattedValue = formattedValue
        .match(/.{1,2}/g) // Split the value into groups of 2 characters
        ?.join("-"); // Join the groups with hyphens if formattedValue is not null
  
      // If the last character entered is a hyphen, remove it
      if (value.endsWith('-')) {
        formattedValue = formattedValue.slice(0, -1);
      }
  
      setJoinEUI(formattedValue);
    }
  };

  const handleChangePin = (e) => {
    const value = e.target.value.toUpperCase().replace(/[^0-9A-F]/g, "");
    if (value.length <= 8) {
      setPin(value);
    }
  };

  const handleSubmit = () => {
    if (devEUI.length !== 19) {
      setErrorMessage("DevEUI must be 16 characters long and contain only hexadecimal characters.");
      return;
    }
    if (pin.length !== 8) {
      setErrorMessage("PIN must be 8 characters long and contain only hexadecimal characters.");
      return;
    }
    // Add your logic here for handling form submission
  };

  return (
    <div className={`modal ${isOpen ? "show" : ""}`}>
      <div className="modal-content add-device-menu-size">
        <span className="close close-placement-add-device-menu" onClick={handleClose}>&times;</span>
        <h2>Add Device</h2>
        <div className="main-content">
            {errorMessage && <p className="error-message">{errorMessage}</p>}
            <div className="input-group">
            <label htmlFor="devEUI">DevEUI:</label>
            <input type="text" id="devEUI" value={devEUI} onChange={handleChangeDevEUI} />
            </div>
            <div className="input-group">
            <label htmlFor="joinEUI">JoinEUI:</label>
            <input type="text" id="joinEUI" value={joinEUI} onChange={handleChangeJoinEUI} />
            </div>
            <div className="input-group">
            <label htmlFor="pin">PIN:</label>
            <input type="text" id="pin" value={pin} onChange={handleChangePin} />
            </div>
            <button onClick={handleSubmit}>Add</button>
        </div>
      </div>
    </div>
  );
};

export default AddDeviceMenu;