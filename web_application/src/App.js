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
      popUp: convertTimestampToPopup(1712781085),
      name: "Hilti rundsav"
    },
    {
      id: 1,
      geocode: [56.85, 10.1522],
      popUp: convertTimestampToPopup(1712761085),
      name: "Hilti rundsav"
    },
    {
      id: 1,
      geocode: [56.855, 10.14],
      popUp: convertTimestampToPopup(1712721085),
      name: "Hilti rundsav"
    },
    {
      id: 2,
      geocode: [56.845, 10.1328],
      popUp: convertTimestampToPopup(1712681085),
      name: "MaiCut flamingoskærer"
    },
    {
      id: 2,
      geocode: [56.838, 10.129],
      popUp: convertTimestampToPopup(1712781150),
      name: "MaiCut flamingoskærer"
    },
    {
      id: 3,
      geocode: [55.352129024259426, 10.371932315878896],
      popUp: convertTimestampToPopup(1712800065),
      name: "SoRoTo tvangsblander 40 liter"
    },
    {
      id: 3,
      geocode: [55.39036773193939, 10.003890323691396],
      popUp: convertTimestampToPopup(1712805065),
      name: "SoRoTo tvangsblander 40 liter"
    },
    {
      id: 3,
      geocode: [55.51186134119964, 9.76631097798827],
      popUp: convertTimestampToPopup(1712810065),
      name: "SoRoTo tvangsblander 40 liter"
    },
    {
      id: 3,
      geocode: [55.48385755904464, 9.471053409628896],
      popUp: convertTimestampToPopup(1712815065),
      name: "SoRoTo tvangsblander 40 liter"
    },
    {
      id: 4,
      geocode: [55.21336109122791, 11.76730468116085],
      popUp: convertTimestampToPopup(1712800065),
      name: "Milwaukee Inspektionskamera M18 SIC36-0"
    },
    {
      id: 4,
      geocode: [54.97306836214099, 12.002836349452343],
      popUp: convertTimestampToPopup(1712805065),
      name: "Milwaukee Inspektionskamera M18 SIC36-0"
    },
    {
      id: 4,
      geocode: [54.576789525964145, 11.929138297811294],
      popUp: convertTimestampToPopup(1712810065),
      name: "Milwaukee Inspektionskamera M18 SIC36-0"
    },
    {
      id: 4,
      geocode: [54.08320371108278, 12.134133963429825],
      popUp: convertTimestampToPopup(1712815065),
      name: "Milwaukee Inspektionskamera M18 SIC36-0"
    },
    {
      id: 4,
      geocode: [53.853835486983506, 12.444120362979492],
      popUp: convertTimestampToPopup(1712820065),
      name: "Milwaukee Inspektionskamera M18 SIC36-0"
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