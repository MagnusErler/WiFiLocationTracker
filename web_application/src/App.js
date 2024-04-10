import { useState } from "react";

import "./styles.css";
import "leaflet/dist/leaflet.css";

import MapComponent from "./components/MapComponent";
import SettingsMenu from "./components/SettingsMenu";

import ListIcon from '@mui/icons-material/List';


export default function App() {
  const center = [56.234538, 10.231792]; // Denmark coordinates
  const [settingsOpen, setSettingsOpen] = useState(false);

  // markers TODO: fetch markers from API
  const markers = [
    {
      geocode: [56.86, 10.1522],
      popUp: "Hello, I am pop up 1"
    },
    {
      geocode: [56.85, 10.1522],
      popUp: "Hello, I am pop up 2"
    },
    {
      geocode: [56.855, 10.14],
      popUp: "Hello, I am pop up 3"
    }
  ];

  const useCustomClusterIcon = false; // Set to true to use custom cluster icon

  const handleButtonClick = () => {
    setSettingsOpen(true);
  };

  const handleCloseSettings = () => {
    setSettingsOpen(false);
  };

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
        markers={markers}
        useCustomClusterIcon={useCustomClusterIcon}
      />
      <SettingsMenu isOpen={settingsOpen} handleClose={handleCloseSettings} />
    </div>
  );
}