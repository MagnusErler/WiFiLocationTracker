import { useState } from "react";

import "./styles.css";
import "leaflet/dist/leaflet.css";

import MapComponent from "./components/MapComponent";
import SettingsMenu from "./components/SettingsMenu";

import ListIcon from '@mui/icons-material/List';

function convertUnixSecondsToReadableDate(timestamp) {
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
  const [showCurrentLocation, setShowCurrentLocation] = useState([]);
  const [showMovement, setShowMovement] = useState([]);

  // TODO: fetch tracker information from API
  const trackerInformation = [
    {
      id: 1,
      name: "Hilti rundsav",
      description: "Hilti rundsav",
      batteryStatus: "100%",
      temperature: "20°C",
      updateInterval: "5 min"
    },
    {
      id: 2,
      name: "MaiCut flamingoskærer",
      description: "MaiCut flamingoskærer",
      batteryStatus: "100%",
      temperature: "24°C",
      updateInterval: "2.5 timer"
    },
    {
      id: 3,
      name: "SoRoTo tvangsblander 40 liter",
      description: "SoRoTo tvangsblander 40 liter",
      batteryStatus: "100%",
      temperature: "-6°C",
      updateInterval: "12 timer"
    },
    {
      id: 4,
      name: "Milwaukee Inspektionskamera M18 SIC36-0",
      description: "Milwaukee Inspektionskamera M18 SIC36-0",
      batteryStatus: "100%",
      temperature: "12°C",
      updateInterval: "6 timer"
    }
  ];

  // TODO: fetch markers from API
  const markers = [
    {
      id: 1,
      geocode: [56.86, 10.1522],
      timeStamp: convertUnixSecondsToReadableDate(1712781085),
    },
    {
      id: 1,
      geocode: [56.85, 10.1522],
      timeStamp: convertUnixSecondsToReadableDate(1712761085),
    },
    {
      id: 1,
      geocode: [56.855, 10.14],
      timeStamp: convertUnixSecondsToReadableDate(1712721085),
    },
    {
      id: 2,
      geocode: [56.845, 10.1328],
      timeStamp: convertUnixSecondsToReadableDate(1712681085),
    },
    {
      id: 2,
      geocode: [56.838, 10.129],
      timeStamp: convertUnixSecondsToReadableDate(1712781150),
    },
    {
      id: 3,
      geocode: [55.352129024259426, 10.371932315878896],
      timeStamp: convertUnixSecondsToReadableDate(1712800065),
    },
    {
      id: 3,
      geocode: [55.39036773193939, 10.003890323691396],
      timeStamp: convertUnixSecondsToReadableDate(1712805065),
    },
    {
      id: 3,
      geocode: [55.51186134119964, 9.76631097798827],
      timeStamp: convertUnixSecondsToReadableDate(1712810065),
    },
    {
      id: 3,
      geocode: [55.48385755904464, 9.471053409628896],
      timeStamp: convertUnixSecondsToReadableDate(1712815065),
    },
    {
      id: 4,
      geocode: [55.21336109122791, 11.76730468116085],
      timeStamp: convertUnixSecondsToReadableDate(1712800065),
    },
    {
      id: 4,
      geocode: [54.97306836214099, 12.002836349452343],
      timeStamp: convertUnixSecondsToReadableDate(1712805065),
    },
    {
      id: 4,
      geocode: [54.576789525964145, 11.929138297811294],
      timeStamp: convertUnixSecondsToReadableDate(1712810065),
    },
    {
      id: 4,
      geocode: [54.08320371108278, 12.134133963429825],
      timeStamp: convertUnixSecondsToReadableDate(1712815065),
    },
    {
      id: 4,
      geocode: [53.853835486983506, 12.444120362979492],
      timeStamp: convertUnixSecondsToReadableDate(1712820065),
    }
  ];

  const useCustomClusterIcon = false; // Set to true to use custom cluster icon

  const handleButtonClick = () => {
    setSettingsOpen(true);
  };

  const handleCloseSettings = () => {
    setSettingsOpen(false);
  };

  const handleShowLocationSwitch = (ids) => {
    setShowCurrentLocation(ids);
  };

  const handleShowMovementSwitch = (ids) => {
    setShowMovement(ids);
  };

  // Function to filter the markers so that only the newest marker for each ID is included
  const filterNewestMarkers = (markers) => {
    const newestMarkers = {};
    markers.forEach(marker => {
      if (!newestMarkers[marker.id] || marker.timeStamp > newestMarkers[marker.id].timeStamp) {
        newestMarkers[marker.id] = marker;
      }
    });
    return Object.values(newestMarkers);
  };

  // Filter markers based on checked IDs
  // const filteredMarkers = showCurrentLocation.length === 0 ? [] : markers.filter(marker => showCurrentLocation.includes(marker.id));
  const filteredMarkers = showCurrentLocation.length === 0 ? [] : filterNewestMarkers(markers.filter(marker => showCurrentLocation.includes(marker.id)));

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
      <SettingsMenu isOpen={settingsOpen} handleClose={handleCloseSettings} trackerInformation={trackerInformation} handleShowLocationSwitch={handleShowLocationSwitch} handleShowMovement={handleShowMovementSwitch}/>
    </div>
  );
}