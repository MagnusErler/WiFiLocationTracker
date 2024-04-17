import { useState } from "react";

import "./styles.css";
import "leaflet/dist/leaflet.css";

import MapComponent from "./components/MapComponent";
import SettingsMenu from "./components/SettingsMenu";
import AddDeviceMenu from "./components/AddDeviceMenu";

import ListIcon from '@mui/icons-material/List';
import AddIcon from '@mui/icons-material/Add';

function convertUnixSecondsToReadableDate(timestamp) {
  const date = new Date(timestamp * 1000); // Convert to milliseconds
  const formattedDate = date.toLocaleString(); // Format as a human-readable string

  return formattedDate;
}

export default function App() {
  // TODO: fetch markers from API
  const [markers, setMarkers] = useState([
      {
        pingId: 1,
        deviceId: 1,
        geocode: [56.86, 10.1522],
        timeStamp: convertUnixSecondsToReadableDate(1712781085),
      },
      {
        pingId: 2,
        deviceId: 1,
        geocode: [56.85, 10.1522],
        timeStamp: convertUnixSecondsToReadableDate(1712761085),
      },
      {
        pingId: 3,
        deviceId: 1,
        geocode: [56.855, 10.14],
        timeStamp: convertUnixSecondsToReadableDate(1712721085),
      },
      {
        pingId: 4,
        deviceId: 2,
        geocode: [56.845, 10.1328],
        timeStamp: convertUnixSecondsToReadableDate(1712681085),
      },
      {
        pingId: 5,
        deviceId: 2,
        geocode: [56.838, 10.129],
        timeStamp: convertUnixSecondsToReadableDate(1712781150),
      },
      {
        pingId: 6,
        deviceId: 3,
        geocode: [55.352129024259426, 10.371932315878896],
        timeStamp: convertUnixSecondsToReadableDate(1712800065),
      },
      {
        pingId: 7,
        deviceId: 3,
        geocode: [55.39036773193939, 10.003890323691396],
        timeStamp: convertUnixSecondsToReadableDate(1712805065),
      },
      {
        pingId: 8,
        deviceId: 3,
        geocode: [55.51186134119964, 9.76631097798827],
        timeStamp: convertUnixSecondsToReadableDate(1712810065),
      },
      {
        pingId: 9,
        deviceId: 3,
        geocode: [55.48385755904464, 9.471053409628896],
        timeStamp: convertUnixSecondsToReadableDate(1712815065),
      },
      {
        pingId: 10,
        deviceId: 4,
        geocode: [55.21336109122791, 11.76730468116085],
        timeStamp: convertUnixSecondsToReadableDate(1712800065),
      },
      {
        pingId: 11,
        deviceId: 4,
        geocode: [54.97306836214099, 12.002836349452343],
        timeStamp: convertUnixSecondsToReadableDate(1712805065),
      },
      {
        pingId: 12,
        deviceId: 4,
        geocode: [54.576789525964145, 11.929138297811294],
        timeStamp: convertUnixSecondsToReadableDate(1712810065),
      },
      {
        pingId: 13,
        deviceId: 4,
        geocode: [54.08320371108278, 12.134133963429825],
        timeStamp: convertUnixSecondsToReadableDate(1712815065),
      },
      {
        pingId: 14,
        deviceId: 4,
        geocode: [53.853835486983506, 12.444120362979492],
        timeStamp: convertUnixSecondsToReadableDate(1712820065),
      }
  ]);

  // TODO: fetch tracker information from API
  const trackerInformation = [
    {
      deviceId: 1,
      name: "Hilti rundsav",
      batteryStatus: "100%",
      temperature: "20°C",
      updateInterval: "5 min"
    },
    {
      deviceId: 2,
      name: "MaiCut flamingoskærer",
      batteryStatus: "100%",
      temperature: "24°C",
      updateInterval: "2.5 timer"
    },
    {
      deviceId: 3,
      name: "SoRoTo tvangsblander 40 liter",
      batteryStatus: "100%",
      temperature: "-6°C",
      updateInterval: "12 timer"
    },
    {
      deviceId: 4,
      name: "Milwaukee Inspektionskamera M18 SIC36-0",
      batteryStatus: "100%",
      temperature: "12°C",
      updateInterval: "6 timer"
    }
  ];  

  const center = [56.234538, 10.231792]; // Denmark coordinates
  const [showCurrentLocation, setShowCurrentLocation] = useState(trackerInformation.map(tracker => tracker.deviceId));
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [showMovement, setShowMovement] = useState([]);

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
      if (!newestMarkers[marker.deviceId] || marker.timeStamp > newestMarkers[marker.deviceId].timeStamp) {
        newestMarkers[marker.deviceId] = marker;
      }
    });
    return Object.values(newestMarkers);
  };

  // Update filtered markers when toggling switches
  const filteredCurrentLocationMarkers = filterNewestMarkers(markers.filter(marker => showCurrentLocation.includes(marker.deviceId)));
  const filteredAllLocationMarkers = markers.filter(marker => showMovement.includes(marker.deviceId));

  // Combine all markers
  const allMarkers = [...filteredCurrentLocationMarkers, ...filteredAllLocationMarkers];

  // Function to filter out duplicates from all markers
  const filterUniqueMarkers = (markers) => {
    const uniqueMarkers = {};
    markers.forEach(marker => {
      if (!uniqueMarkers[marker.pingId]) {
        uniqueMarkers[marker.pingId] = marker;
      }
    });
    return Object.values(uniqueMarkers);
  };

  // Filter out duplicates
  const filteredAllMarkers = filterUniqueMarkers(allMarkers);

  return (
    <div className="map-container">
      <button
        className="menu-button"
        onClick={handleButtonClick}
      >
        <ListIcon />
      </button>
      {/* <button
        className="add-button"
        onClick={handleButtonClick}
      >
        <AddIcon />
      </button> */}
      <MapComponent
        center={center}
        allMarkers={filteredAllMarkers}
        useCustomClusterIcon={false}
      />
      <SettingsMenu isOpen={settingsOpen} handleClose={handleCloseSettings} trackerInformation={trackerInformation} handleShowLocationSwitch={handleShowLocationSwitch} handleShowMovement={handleShowMovementSwitch}/>
    </div>
  );
}

 // const sortedAllLocationMarkers = filteredAllLocationMarkers.sort((a, b) => {
  //   const timestampA = new Date(a.timeStamp).getTime() / 1000;
  //   const timestampB = new Date(b.timeStamp).getTime() / 1000;
  //   return timestampA - timestampB;
  // });