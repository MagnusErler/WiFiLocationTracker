import { useState, useEffect } from "react";
import axios from "axios";

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
        deviceId: "0016c001f0003fc5",
        geocode: [56.86, 10.1522],
        timeStamp: convertUnixSecondsToReadableDate(1712781085),
      },
      {
        pingId: 2,
        deviceId: "0016c001f0003fc5",
        geocode: [56.85, 10.1522],
        timeStamp: convertUnixSecondsToReadableDate(1712761085),
      },
      {
        pingId: 3,
        deviceId: "0016c001f0003fc5",
        geocode: [56.855, 10.14],
        timeStamp: convertUnixSecondsToReadableDate(1712721085),
      },
      {
        pingId: 4,
        deviceId: "0016c001f0003ff5",
        geocode: [56.845, 10.1328],
        timeStamp: convertUnixSecondsToReadableDate(1712681085),
      },
      {
        pingId: 5,
        deviceId: "0016c001f0003ff5",
        geocode: [56.838, 10.129],
        timeStamp: convertUnixSecondsToReadableDate(1712781150),
      },
      {
        pingId: 6,
        deviceId: "0016c001f0005f5d",
        geocode: [55.352129024259426, 10.371932315878896],
        timeStamp: convertUnixSecondsToReadableDate(1712800065),
      },
      {
        pingId: 7,
        deviceId: "0016c001f0005f5d",
        geocode: [55.39036773193939, 10.003890323691396],
        timeStamp: convertUnixSecondsToReadableDate(1712805065),
      },
      {
        pingId: 8,
        deviceId: "0016c001f0005f5d",
        geocode: [55.51186134119964, 9.76631097798827],
        timeStamp: convertUnixSecondsToReadableDate(1712810065),
      },
      {
        pingId: 9,
        deviceId: "0016c001f0005f5d",
        geocode: [55.48385755904464, 9.471053409628896],
        timeStamp: convertUnixSecondsToReadableDate(1712815065),
      },
      {
        pingId: 10,
        deviceId: "0016c001f0005f84",
        geocode: [55.21336109122791, 11.76730468116085],
        timeStamp: convertUnixSecondsToReadableDate(1712800065),
      },
      {
        pingId: 11,
        deviceId: "0016c001f0005f84",
        geocode: [54.97306836214099, 12.002836349452343],
        timeStamp: convertUnixSecondsToReadableDate(1712805065),
      },
      {
        pingId: 12,
        deviceId: "0016c001f0005f84",
        geocode: [54.576789525964145, 11.929138297811294],
        timeStamp: convertUnixSecondsToReadableDate(1712810065),
      },
      {
        pingId: 13,
        deviceId: "0016c001f0005f84",
        geocode: [54.08320371108278, 12.134133963429825],
        timeStamp: convertUnixSecondsToReadableDate(1712815065),
      },
      {
        pingId: 14,
        deviceId: "0016c001f0005f84",
        geocode: [53.853835486983506, 12.444120362979492],
        timeStamp: convertUnixSecondsToReadableDate(1712820065),
      }
  ]);

  // TODO: fetch tracker information from API
  const [trackerInformation, setTrackerInformation] = useState([
  ]);

  useEffect(() => {
    fetchTrackerInformation();
  }, []);

  const center = [56.234538, 10.231792]; // Denmark coordinates
  const [showCurrentLocation, setShowCurrentLocation] = useState(trackerInformation.map(tracker => tracker.deviceId));
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [isAddDeviceMenuOpen, setAddDeviceMenuOpen] = useState(false);
  const [showMovement, setShowMovement] = useState([]);

  const handleOpenSettingsMenu = () => {
    setSettingsOpen(true);
  };

  const handleCloseSettingsMenu = () => {
    setSettingsOpen(false);
  };

  const handleShowLocationSwitch = (ids) => {
    setShowCurrentLocation(ids);
  };

  const handleShowMovementSwitch = (ids) => {
    setShowMovement(ids);
  };

  const handleOpenAddDeviceMenu = () => {
    setAddDeviceMenuOpen(true);
  };

  const handleCloseAddDeviceMenu = () => {
    setAddDeviceMenuOpen(false);
  };

  // Function to handle successful addition of device and refetch tracker information
  const handleAddDeviceSuccess = async () => {
    try {
      // Fetch tracker information again
      await fetchTrackerInformation();
    } catch (error) {
      console.error("Error fetching tracker information:", error);
      // Handle error fetching tracker information
    }
  };

  const getDeviceInfo = async () => {
    try {
      const response = await axios.get("/api/getDeviceInfoFromJoinserver");
      console.log(response.data);
      // Process device information as needed
    } catch (error) {
      console.error("Failed to fetch device information:", error);
      alert("Failed to fetch device information. Check console for details.");
    }
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

  const handleTrackerInformationUpdate = (updatedTrackerInformation) => {
    setTrackerInformation(updatedTrackerInformation);
  };

  const fetchTrackerInformation = async () => {
    console.log("Fetching tracker information...");
    try {
      const token = process.env.REACT_APP_TTN_API_TOKEN;
      const appID = process.env.REACT_APP_APP_ID;
      if (!token) {
        throw new Error("API token ID is not available.");
      }
      if (!appID) {
        throw new Error("Application ID is not available.");
      }
      const response = await fetch(`https://eu1.cloud.thethings.network/api/v3/applications/${appID}/devices?field_mask=name,description`, {
        method: "GET",
        headers: {
          Authorization: `Bearer ${token}`,
        },
      });

      if (response.ok) {
        const data = await response.json();
        console.log(data);
        
        if (data && data.end_devices) {
          setTrackerInformation(prevState => {
            // Iterate over the fetched data
            data.end_devices.forEach(newDevice => {
              const deviceIdWithoutPrefix = newDevice.ids.device_id.replace("eui-", "");
              // Check if the device ID already exists in the current state
              const existingIndex = prevState.findIndex(existingDevice => existingDevice.deviceId === deviceIdWithoutPrefix);
              if (existingIndex !== -1) {
                // Update existing entry
                prevState[existingIndex] = {
                  ...prevState[existingIndex],
                  name: newDevice.name || "Unknown",
                  batteryStatus: "-",
                  temperature: "-",
                  updateInterval: "-"
                };
              } else {
                // Add new entry
                prevState.push({
                  deviceId: deviceIdWithoutPrefix,
                  name: newDevice.name || "Unknown",
                  batteryStatus: "-",
                  temperature: "-",
                  updateInterval: "-"
                });
              }
            });
            return [...prevState]; // Return a new array to trigger state update
          });
        }
      } else {
        throw new Error(`Failed to fetch tracker information: ${response.status}, ${response.statusText}`);
      }
    } catch (error) {
      alert("Failed to fetch tracker information. Either your API key or your application key is wrong");
    }
  };

  return (
    <div className="map-container">
      <button
        className="menu-button"
        onClick={handleOpenSettingsMenu}
      >
        <ListIcon />
      </button>
      <button
        className="add-button"
        onClick={handleOpenAddDeviceMenu}
      >
        <AddIcon />
      </button>
      <MapComponent
        center={center}
        allMarkers={filteredAllMarkers}
        useCustomClusterIcon={false}
      />
      <SettingsMenu 
        isOpen={settingsOpen} 
        handleClose={handleCloseSettingsMenu} 
        trackerInformation={trackerInformation} 
        handleShowLocationSwitch={handleShowLocationSwitch} 
        handleShowMovement={handleShowMovementSwitch}
        handleTrackerInformationUpdate={handleTrackerInformationUpdate}
        markers={markers}
      />
      <AddDeviceMenu 
        isOpen={isAddDeviceMenuOpen} 
        handleClose={handleCloseAddDeviceMenu}
        onDeviceAdded={handleAddDeviceSuccess} 
        />
    </div>
  );
}