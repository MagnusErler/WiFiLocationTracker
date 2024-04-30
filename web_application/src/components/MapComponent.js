import React, { useEffect, useState } from "react";
import { MapContainer, TileLayer, Marker, Popup, Polyline } from "react-leaflet";
import MarkerClusterGroup from "react-leaflet-cluster";
import { Icon, divIcon } from "leaflet";

const MapComponent = ({ center, allMarkers, useCustomClusterIcon, trackerInformation, selectedMapTile }) => {
  // State for storing calculated lines
  const [lines, setLines] = useState([]);

  useEffect(() => {
    const calculatedLines = {};

    // Group locations by pingId
    for (const marker of allMarkers) {
      if (!calculatedLines[marker.deviceId]) {
        calculatedLines[marker.deviceId] = [];
      }
      calculatedLines[marker.deviceId].push(marker);
    }
    setLines(calculatedLines);

  }, [allMarkers]);

  // Icon for individual markers
  const customIcon = new Icon({
    iconUrl: require("../img/markerIcon.png"),
    iconSize: [38, 38]
  });

  // Function to create custom cluster icon
  const createCustomClusterIcon = cluster => {
    return new divIcon({
      html: `<span class="cluster-icon">${cluster.getChildCount()}</span>`,
      className: "custom-marker-cluster"
    });
  };

  // Function to calculate line coordinates between markers
  const makeLineCoordinates = markerGroup => {
    const lineCoordinates = [];
    for (let i = 1; i < markerGroup.length; i++) {
      if (markerGroup[i].id === markerGroup[i - 1].id) {
        lineCoordinates.push(markerGroup[i - 1].geocode, markerGroup[i].geocode);
      }
    }
    return lineCoordinates;
  };

  return (
    <MapContainer center={center} zoom={7}>
      {/* Render selected map tile */}
      {selectedMapTile === "OpenStreetMap" && (
        <TileLayer
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
      )}
      {selectedMapTile === "Stadia_AlidadeSatellite" && (
        <TileLayer
          attribution='&copy; CNES, Distribution Airbus DS, © Airbus DS, © PlanetObserver (Contains Copernicus Data) | &copy; <a href="https://www.stadiamaps.com/" target="_blank">Stadia Maps</a> &copy; <a href="https://openmaptiles.org/" target="_blank">OpenMapTiles</a> &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://tiles.stadiamaps.com/tiles/alidade_satellite/{z}/{x}/{y}{r}.{ext}"
          minZoom={0}
          maxZoom={20}
          ext="jpg"
        />
      )}
      {selectedMapTile === "Stadia_StamenWatercolor" && (
        <TileLayer
          attribution='&copy; <a href="https://www.stadiamaps.com/" target="_blank">Stadia Maps</a> &copy; <a href="https://www.stamen.com/" target="_blank">Stamen Design</a> &copy; <a href="https://openmaptiles.org/" target="_blank">OpenMapTiles</a> &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://tiles.stadiamaps.com/tiles/stamen_watercolor/{z}/{x}/{y}.{ext}"
          minZoom={1}
          maxZoom={16}
          ext="jpg"
        />
      )}
      {selectedMapTile === "CartoDB_Voyager" && (
        <TileLayer
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://carto.com/attributions">CARTO</a>'
          url="https://{s}.basemaps.cartocdn.com/rastertiles/voyager/{z}/{x}/{y}{r}.png"
          subdomains="abcd"
          maxZoom={20}
        />
      )}
      
      {/* Render lines */}
      {Object.values(lines).map((markerGroup, index) => (
        <Polyline key={index} positions={makeLineCoordinates(markerGroup)} color="blue" />
      ))}
      <MarkerClusterGroup
        chunkedLoading
        iconCreateFunction={useCustomClusterIcon ? createCustomClusterIcon : null}
      >
        {allMarkers.map((marker, index) => {
          // Find device name from trackerInformation
          const deviceInfo = trackerInformation.find(info => info.deviceId.toUpperCase() === marker.deviceId.toUpperCase());
          const deviceName = deviceInfo ? deviceInfo.name : "Unknown";

          return (
            <Marker key={index} position={marker.geocode} icon={customIcon}>
              <Popup>{"Name: " + deviceName + ", seen: " + marker.timeStamp.slice(0, -3)}</Popup>
            </Marker>
          );
        })}
      </MarkerClusterGroup>
    </MapContainer>
  );
};

export default MapComponent;
//TODO: See if arrows can be implemented: https://imfeld.dev/writing/leaflet_shapes