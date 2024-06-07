import React from "react";
import Dialog from '@mui/material/Dialog';
import DialogTitle from '@mui/material/DialogTitle';
import "./ConfirmationDialog.css";

const ConfigureDeviceDialog = ({ deviceId, handleClose }) => {
  return (
    <Dialog open={true} onClose={handleClose}>
      <DialogTitle>Configure Device {deviceId}</DialogTitle>
      <div style={{ padding: "20px" }}>
        {/* Your configuration content here */}
        <button onClick={handleClose}>Send uplink and close</button>
      </div>
    </Dialog>
  );
};

export default ConfigureDeviceDialog;

