import React from "react";
import "./ConfirmationDialog.css";

const ConfirmationDialog = ({ message, onConfirm, onCancel }) => {
  return (
    <div className="confirmation-dialog">
      <p>{message}</p>
      <div className="buttons">
        <button className="button confirm-button" onClick={onConfirm}>Confirm</button>
        <button className="button cancel-button" onClick={onCancel}>Cancel</button>
      </div>
    </div>
  );
};

export default ConfirmationDialog;