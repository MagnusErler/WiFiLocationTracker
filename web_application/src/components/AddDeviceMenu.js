import React, { useState } from "react";
import axios from "axios";
import "./ModalMenu.css"
import "./AddDeviceMenu.css";

const AddDeviceMenu = ({ isOpen, handleClose, onDeviceAdded }) => {
  const [devEUI, setDevEUI] = useState("");
  const [joinEUI, setJoinEUI] = useState("00-16-C0-01-FF-FE-00-01");
  const [pin, setPin] = useState("");
  const [errorMessage, setErrorMessage] = useState("");
  const [name, setName] = useState("");

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

  const handleChangeName = (e) => {
    setName(e.target.value);
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

  const handleSubmit = async () => {
    try {
      if (!name.trim()) {
        setErrorMessage("Name is required.");
        return;
      }
      // Validate input fields
      if (devEUI.length !== 23) {
        setErrorMessage("DevEUI must be 16 characters long and contain only hexadecimal characters.");
        return;
      }
      if (joinEUI.length !== 23) {
        console.log("joinEUI", joinEUI);
        setErrorMessage("JoinEUI must be 16 characters long and contain only hexadecimal characters.");
        return;
      }
      if (pin.length !== 8) {
        setErrorMessage("PIN must be 8 characters long and contain only hexadecimal characters.");
        return;
      }

      // Remove "-" from devEUI and joinEUI
      const cleanDevEUI = devEUI.replace(/-/g, '');
      const cleanJoinEUI = joinEUI.replace(/-/g, '');
  
      // Prepare request data for each endpoint
      const requestDataClaim = [{ DevEUI: cleanDevEUI, Pin: pin }];
      const requestDataTTNID = {
        Token: process.env.REACT_APP_TTN_API_TOKEN,
        AppID: process.env.REACT_APP_APP_ID,
        deviceID: "eui-" + cleanDevEUI.toLowerCase(),
        devEUI: cleanDevEUI,
        joinEUI: cleanJoinEUI
      };
      const requestDataTTNNS = {
        Token: process.env.REACT_APP_TTN_API_TOKEN,
        AppID: process.env.REACT_APP_APP_ID,
        deviceID: "eui-" + cleanDevEUI.toLowerCase(),
        devEUI: cleanDevEUI,
        joinEUI: cleanJoinEUI
      };
      const requestDataTTNAS = {
        Token: process.env.REACT_APP_TTN_API_TOKEN,
        AppID: process.env.REACT_APP_APP_ID,
        deviceID: "eui-" + cleanDevEUI.toLowerCase(),
        devEUI: cleanDevEUI,
        joinEUI: cleanJoinEUI
      };
      const requestDataUpdateNameOnTTN = {
        Token: process.env.REACT_APP_TTN_API_TOKEN,
        AppID: process.env.REACT_APP_APP_ID,
        name: name,
        deviceId: cleanDevEUI
      };
  
      // Call each endpoint sequentially
      await axios.post("/api/claimDevicesOnJoinServer", requestDataClaim);
      await axios.post("/api/createDeviceOnTTNIDServer", requestDataTTNID);
      await axios.put("/api/createDeviceOnTTNNS", requestDataTTNNS);
      await axios.put("/api/createDeviceOnTTNAS", requestDataTTNAS);
      await axios.put("/api/updateDeviceNameOnTTN", requestDataUpdateNameOnTTN);
  
      // All requests succeeded
      console.log("All devices added successfully.");
      window.alert("Device has been added successfully.");
      handleClose();
      setName("");
      setDevEUI("");
      setJoinEUI("00-16-C0-01-FF-FE-00-01");
      setPin("");
      setErrorMessage("");
      onDeviceAdded();
    } catch (error) {
      // Handle errors
      console.error("Error adding devices:", error);
      setErrorMessage(error.response?.data?.message || "An error occurred while adding the device. Please try again");
    }
  };

  return (
      <div className={`modal ${isOpen ? "show" : ""}`}>
        <div className="modal-content add-device-menu-size">
          <div className="top-bar">
            <h2>Add Device</h2>
            <span className="close close-placement-add-device-menu" onClick={handleClose}>&times;</span>
          </div>
          <div className="main-content">
            {errorMessage && <p className="error-message">{errorMessage}</p>}
            <div className="input-group">
            <label htmlFor="name">Name:</label>
            <input type="text" id="name" value={name} onChange={handleChangeName} />
            </div>
            <div className="input-group">
            <label htmlFor="devEUI">Dev/chipEUI:</label>
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