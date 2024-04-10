import { useState } from "react";

import "./styles.css";
import "leaflet/dist/leaflet.css";

import MapComponent from "./components/MapComponent";
import SettingsMenu from "./components/SettingsMenu";

import ListIcon from '@mui/icons-material/List';

function convertTimestampToPopup(timestamp) {
  const date = new Date(timestamp * 1000); // Convert to milliseconds
  const formattedDate = date.toLocaleString(); // Format as a human-readable string

  return formattedDate;
}

function sortMarkers(markers) {
  return markers.sort((a, b) => {
    const timestampA = new Date(a.popUp).getTime() / 1000;
    const timestampB = new Date(b.popUp).getTime() / 1000;

    return timestampA - timestampB;
  });
}

export default function App() {
  const center = [56.234538, 10.231792]; // Denmark coordinates
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [checkedIds, setCheckedIds] = useState([]);

  // markers TODO: fetch markers from API
  const markers = [
    {
      id: 1,
      geocode: [56.86, 10.1522],
      popUp: convertTimestampToPopup(1712781085)
    },
    {
      id: 1,
      geocode: [56.85, 10.1522],
      popUp: convertTimestampToPopup(1712761085)
    },
    {
      id: 1,
      geocode: [56.855, 10.14],
      popUp: convertTimestampToPopup(1712721085)
    },
    {
      id: 2,
      geocode: [56.845, 10.1328],
      popUp: convertTimestampToPopup(1712681085)
    },
    {
      id: 2,
      geocode: [56.838, 10.129],
      popUp: convertTimestampToPopup(1712781150)
    }
  ];

  const useCustomClusterIcon = false; // Set to true to use custom cluster icon

  const handleButtonClick = () => {
    setSettingsOpen(true);
  };

  const handleCloseSettings = () => {
    setSettingsOpen(false);
  };

  const handleCheckboxChange = (ids) => {
    setCheckedIds(ids);
  };

  // Filter markers based on checked IDs
  const filteredMarkers = checkedIds.length === 0 ? [] : markers.filter(marker => checkedIds.includes(marker.id));

  return (
    <div className="map-container">
      <button
        className="map-button"
        onClick={handleButtonClick}
      >
        <ListIcon />
      </button>
      <MapComponent
        center={center}
        markers={sortMarkers(filteredMarkers)}
        useCustomClusterIcon={useCustomClusterIcon}
      />
      <SettingsMenu isOpen={settingsOpen} handleClose={handleCloseSettings} markers={markers} handleCheckboxChange={handleCheckboxChange}/>
    </div>
  );
}