import { useState, useEffect, useRef } from "react";
import axios from "axios";
import MapComponent from "./components/MapComponent";
import SettingsMenu from "./components/SettingsMenu";
import AddDeviceMenu from "./components/AddDeviceMenu";
import MapTileMenu from "./components/MapTileMenu";
import ListIcon from '@mui/icons-material/List';
import AddIcon from '@mui/icons-material/Add';
import LayersIcon from '@mui/icons-material/Layers';
import "./styles.css";
import "leaflet/dist/leaflet.css";

export default function App() {
  // State
  const [markers, setMarkers] = useState([]);
  const [trackerInformation, setTrackerInformation] = useState([]);
  const [showCurrentLocation, setShowCurrentLocation] = useState([]);
  const [showMovement, setShowMovement] = useState([]);
  const [selectedMapTile, setSelectedMapTile] = useState("CartoDB_Voyager");
  const [isSettingsMenuOpen, setSettingsMenuOpen] = useState(false);
  const [isAddDeviceMenuOpen, setAddDeviceMenuOpen] = useState(false);
  const [isMapTileMenuOpen, setMapTileMenuOpen] = useState(false);
  const [dataLoaded, setDataLoaded] = useState(false);

  // Refs
  const settingsMenuRef = useRef(null);
  const addDeviceMenuRef = useRef(null);
  const mapTileMenuRef = useRef(null);

  // Constants
  const center = [56.234538, 10.231792]; // Denmark coordinates
  const zoomLevel = 7;
  const drawMarkersAtZoomLevel = 16;

  // Effects
  useEffect(() => {
    console.log("Version 12/06/2023 - 12:00");
  }, []);

  useEffect(() => {
    const fetchData = async () => {
      await fetchTrackerInformation();
    };
    fetchData();
  }, []);

  useEffect(() => {
    if (markers.length > 0 && trackerInformation.length > 0 && !dataLoaded) {
      const allIds = trackerInformation.map(info => info.deviceId.toUpperCase());
      setShowCurrentLocation(allIds);
      setDataLoaded(true);
    }
  }, [markers, trackerInformation, dataLoaded]);

  useEffect(() => {
    const handleClickOutside = (event) => {
      if (settingsMenuRef.current && !settingsMenuRef.current.contains(event.target)) {
        setSettingsMenuOpen(false);
      }
      if (addDeviceMenuRef.current && !addDeviceMenuRef.current.contains(event.target)) {
        setAddDeviceMenuOpen(false);
      }
      if (mapTileMenuRef.current && !mapTileMenuRef.current.contains(event.target)) {
        setMapTileMenuOpen(false);
      }
    };

    document.addEventListener("mousedown", handleClickOutside);
    return () => {
      document.removeEventListener("mousedown", handleClickOutside);
    };
  }, []);

  // Handlers
  const handleOpenSettingsMenu = () => setSettingsMenuOpen(true);
  const handleCloseSettingsMenu = () => setSettingsMenuOpen(false);
  const handleOpenAddDeviceMenu = () => setAddDeviceMenuOpen(true);
  const handleCloseAddDeviceMenu = () => setAddDeviceMenuOpen(false);
  const handleOpenMapTileMenu = () => setMapTileMenuOpen(true);
  const handleCloseMapTileMenu = () => setMapTileMenuOpen(false);
  const handleShowLocationSwitch = (ids) => setShowCurrentLocation(ids.map(id => id.toUpperCase()));
  const handleShowMovementSwitch = (ids) => setShowMovement(ids.map(id => id.toUpperCase()));
  const handleTileChange = (tile) => {
    setSelectedMapTile(tile);
    console.log(`Map tile changed to: ${tile}`);
  };
  const handleTrackerInformationUpdate = (updatedTrackerInformation) => {
    setTrackerInformation(updatedTrackerInformation);
  };

  // Helper functions
  const filterNewestMarkers = (markers) => {  // Function to filter the markers so that only the newest marker for each ID is included
    const newestMarkers = {};
    markers.forEach(marker => {
      if (!newestMarkers[marker.deviceId] || marker.timeStamp > newestMarkers[marker.deviceId].timeStamp) {
        newestMarkers[marker.deviceId] = marker;
      }
    });
    return Object.values(newestMarkers);
  };

  const filterUniqueMarkers = (markers) => { // Function to filter out duplicates from all markers
    const uniqueMarkers = {};
    markers.forEach(marker => {
      if (!uniqueMarkers[marker.pingId]) {
        uniqueMarkers[marker.pingId] = marker;
      }
    });
    return Object.values(uniqueMarkers);
  };

  // Fetch functions
  const fetchMarkers = async () => {
    console.log("Fetching markers...");
    try {
      // Initialize an array to store the markers
      const updatedMarkers = [];
  
      // Create an array of promises for fetching markers for each device
      const fetchMarkerPromises = trackerInformation.map(async (device) => {
        const deviceId = device.deviceId.toUpperCase();
        try {
          const response = await axios.get(`/api/geolocationSolves/${deviceId}`);
          
          // Map pings to the desired format
          const mappedMarkers = response.data.map((ping) => ({
            pingId: ping.id,
            deviceId: ping.deviceID,
            geocode: ping.geocode,
            timeStamp: new Date(ping.updatedAt).toLocaleString(), // Convert to human-readable format
            accuracy: ping.accuracy,
          }));
          
          // Add the mapped markers to the updatedMarkers array
          updatedMarkers.push(...mappedMarkers);
          console.log(`Markers for device ${deviceId}:`, response.data);
        } catch (error) {
          // Handle 404 error specifically
          if (error.response && error.response.status === 404) {
            console.log(`No entries found for device ${deviceId}`);
          } else {
            // Handle other errors
            console.error(`Failed to fetch markers for device ${deviceId}:`, error);
          }
        }
      });
  
      // Wait for all fetchMarkerPromises to complete
      await Promise.all(fetchMarkerPromises);
  
      // Update the markers state with the updatedMarkers array
      console.log("Markers fetched successfully:", updatedMarkers);
      setMarkers(updatedMarkers);
    } catch (error) {
      console.error("Failed to fetch markers:", error);
      alert("Failed to fetch markers. Check console for details.");
    }
  };

  const fetchTrackerInformation = async () => {
    console.log("Fetching tracker information...");
    try {
      const token = process.env.REACT_APP_TTN_API_KEY;
      const appID = process.env.REACT_APP_TTN_APP_ID;
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
          const devicePromises = data.end_devices.map(async newDevice => {
            const deviceIdWithoutPrefix = newDevice.ids.device_id.replace("eui-", "");
            console.log(`Fetching data for device ${deviceIdWithoutPrefix.toUpperCase()}...`);
            try {
              // Make a call to an API endpoint here
              const apiResponse = await axios.get(`/api/trackerInformation/${deviceIdWithoutPrefix.toUpperCase()}`);
      
              // Process the API response as needed
              console.log(apiResponse.data);
      
              // Return the processed data or anything you need
              return apiResponse.data;
            } catch (error) {
              console.error(`Failed to fetch data for device ${deviceIdWithoutPrefix.toUpperCase()}:`, error);
              // Return null or any default value if the call fails
              return null;
            }
          });
      
          // Wait for all device promises to resolve
          const deviceData = await Promise.all(devicePromises);

          console.log("Device data:")
          console.log(deviceData);

          setTrackerInformation(prevState => {
            // Iterate over the fetched data
            data.end_devices.forEach(async newDevice => {
              const deviceIdWithoutPrefix = newDevice.ids.device_id.replace("eui-", "");
          
              // Find the corresponding device data fetched from /api/trackerInformation
              const deviceInfo = deviceData.find(device => device && device.deviceID == deviceIdWithoutPrefix.toUpperCase());
          
              // Check if the device ID already exists in the current state
              const existingIndex = prevState.findIndex(existingDevice => existingDevice.deviceId === deviceIdWithoutPrefix);
              if (existingIndex !== -1) {
                // Update existing entry
                prevState[existingIndex] = {
                  ...prevState[existingIndex],
                  name: newDevice.name || "Unknown",
                  batteryStatus: deviceInfo ? parseFloat(deviceInfo.batteryStatus).toFixed(2) + "\xa0V" : "-", 
                  temperature: deviceInfo ? deviceInfo.temperature + "\xa0°C" : "-",
                  updateInterval: deviceInfo ? deviceInfo.updateInterval : "-",
                  wifiStatus: deviceInfo ? deviceInfo.wifiStatus : "1",
                  gnssStatus: deviceInfo ? deviceInfo.gnssStatus : "1",
                  loraWANClass: deviceInfo ? deviceInfo.loraWANClass : "1"
                };
              } else {
                // Add new entry
                prevState.push({
                  deviceId: deviceIdWithoutPrefix,
                  name: newDevice.name || "Unknown",
                  batteryStatus: deviceInfo ? parseFloat(deviceInfo.batteryStatus).toFixed(2) + "\xa0V" : "-", 
                  temperature: deviceInfo ? deviceInfo.temperature + "\xa0°C" : "-",
                  updateInterval: deviceInfo ? deviceInfo.updateInterval : "-",
                  wifiStatus: deviceInfo ? deviceInfo.wifiStatus : "1",
                  gnssStatus: deviceInfo ? deviceInfo.gnssStatus : "1",
                  loraWANClass: deviceInfo ? deviceInfo.loraWANClass : "1"
                });
              }
            });
            return [...prevState]; // Return a new array to trigger state update
          });
  
          // Call fetchMarkers here after fetchTrackerInformation is completed
          fetchMarkers();
        }
      } else {
        throw new Error(`Failed to fetch tracker information: ${response.status}, ${response.statusText}`);
      }
    } catch (error) {
      alert("Failed to fetch tracker information. Either your API key or your application key is wrong");
    }
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

  const scheduleDownlink = async (updatedTracker) => {
    console.log("Scheduling downlink for device", updatedTracker.deviceId);

    // Convert tracker information to bytes
    const byte12temp = timeFromMinutesToHex(updatedTracker.updateInterval);
    const byte3temp = convertTrackerInformationToByte(updatedTracker);

    // Split bytes into 8-bit chunks
    const byte1 = byte12temp.slice(0, 2);
    const byte2 = byte12temp.slice(2, 4);    
    const byte3 = byte3temp.toString(16).toUpperCase().padStart(2, '0');
    
    try {
      const token = process.env.REACT_APP_TTN_API_KEY;
      const appID = process.env.REACT_APP_TTN_APP_ID;
      if (!token) {
        throw new Error("API token ID is not available.");
      }
      if (!appID) {
        throw new Error("Application ID is not available.");
      }
  
      const payload = {
        downlinks: [
          {
            decoded_payload: {
              bytes: [byte1,byte2,byte3]
            },
            f_port: 1,
            priority: "NORMAL"
          }
        ]
      };
      
      const response = await fetch(`https://eu1.cloud.thethings.network/api/v3/as/applications/${appID}/webhooks/schedule-downlink/devices/eui-${updatedTracker.deviceId}/down/push`, {
        method: "POST",
        headers: {
          Authorization: `Bearer ${token}`,
          "Content-Type": "application/json"
        },
        body: JSON.stringify(payload)
      });
  
      if (response.ok) {
        console.log(`Downlink scheduled successfully for device ${updatedTracker.deviceId}.`);
        await postTrackerInformation(updatedTracker); // Call postTrackerInformation on success
      } else {
        throw new Error(`Failed to schedule downlink for device ${updatedTracker.deviceId}: ${response.status} - ${response.statusText}`);
      }
    } catch (error) {
      console.error("Error scheduling downlink:", error.message);
      // Handle error as needed
    }
  };

  const postTrackerInformation = async (updatedTracker) => {
    try {
      const response = await fetch(`/api/trackerInformation/${updatedTracker.deviceId}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(updatedTracker)
      });
  
      if (response.ok) {
        console.log('Tracker information updated successfully.');
      } else {
        throw new Error(`Failed to update tracker information: ${response.status} - ${response.statusText}`);
      }
    } catch (error) {
      console.error("Error posting tracker information:", error.message);
      // Handle error as needed
    }
  };

  // Derived data and helper functions
  const filteredCurrentLocationMarkers = filterNewestMarkers(markers.filter(marker => showCurrentLocation.includes(marker.deviceId))); // Update filtered markers when toggling switches
  const filteredAllLocationMarkers = markers.filter(marker => showMovement.includes(marker.deviceId)); // Update filtered markers when toggling switches
  const allMarkers = [...filteredCurrentLocationMarkers, ...filteredAllLocationMarkers]; // Combine all markers
  const filteredAllMarkers = filterUniqueMarkers(allMarkers); // Filter out duplicates

  const convertTrackerInformationToByte = (trackerInfo) => {
    const CLASS_A = 0b000000;
    const CLASS_B = 0b000001;
    const CLASS_C = 0b000010;
    const WIFI_STATUS_BIT = 0b000100;
    const GNSS_STATUS_BIT = 0b001000;

    let classBits = 0;
    let wifiStatusBit = trackerInfo.wifiStatus ? WIFI_STATUS_BIT : 0;
    let gnssStatusBit = trackerInfo.gnssStatus ? GNSS_STATUS_BIT : 0;

    console.log(`LoRaWAN Class: ${trackerInfo.loraWANClass}`);
    console.log(`Wifi Status: ${trackerInfo.wifiStatus}`);
    console.log(`GNSS Status: ${trackerInfo.gnssStatus}`);

    switch (parseInt(trackerInfo.loraWANClass)) {
        case 1:
            console.log("Class A");
            classBits = CLASS_A;
            break;
        case 2:
            console.log("Class B");
            classBits = CLASS_B;
            break;
        case 3:
            console.log("Class C");
            classBits = CLASS_C;
            break;
        default:
            console.log("Invalid LoRaWAN Class");
            break;
    }

    const result = classBits | wifiStatusBit | gnssStatusBit;
    return result;
  };

  const timeFromMinutesToHex = (minutes) => {
    const minutesAsNumber = parseInt(minutes, 10);

    // Ensure the number is within the unsigned 16-bit range
    if (minutesAsNumber < 0 || minutesAsNumber > 65535) {
        throw new RangeError("Value out of range for unsigned 16-bit integer");
    }

    // Convert to hexadecimal and pad the result to ensure it's 4 digits
    const minutesAsHex = minutesAsNumber.toString(16).toUpperCase().padStart(4, '0');

    return minutesAsHex;
  }

  // Function to convert decimal to binary and pad with leading zeros (for print debugging)
  const decimalToBinary = (decimal) => {
    return decimal.toString(2).padStart(8, '0');
  };

  return (
    <div className="map-container">
      <button className="menu-button" onClick={handleOpenSettingsMenu}>
        <ListIcon />
      </button>
      <button className="add-button" onClick={handleOpenAddDeviceMenu}>
        <AddIcon />
      </button>
      <button className="map-tile-button" onClick={handleOpenMapTileMenu}>
        <LayersIcon />
      </button>
      <MapComponent
        center={center}
        zoomLevel={zoomLevel}
        drawMarkersAtZoomLevel={drawMarkersAtZoomLevel}
        allMarkers={filteredAllMarkers}
        useCustomClusterIcon={false}
        trackerInformation={trackerInformation}
        selectedMapTile={selectedMapTile}
      />
      <SettingsMenu 
        isOpen={isSettingsMenuOpen} 
        handleClose={handleCloseSettingsMenu} 
        trackerInformation={trackerInformation} 
        handleShowLocationSwitch={handleShowLocationSwitch} 
        handleShowMovement={handleShowMovementSwitch}
        handleTrackerInformationUpdate={handleTrackerInformationUpdate}
        scheduleDownlink={scheduleDownlink}
        markers={markers}
        ref={settingsMenuRef}
        dataLoaded={dataLoaded}
      />
      <AddDeviceMenu 
        isOpen={isAddDeviceMenuOpen} 
        handleClose={handleCloseAddDeviceMenu}
        onDeviceAdded={handleAddDeviceSuccess}
        ref={addDeviceMenuRef} 
        />
      <MapTileMenu
        isOpen={isMapTileMenuOpen}
        handleClose={handleCloseMapTileMenu}
        handleTileChange={handleTileChange}
        ref={mapTileMenuRef}
      />
    </div>
  );
}