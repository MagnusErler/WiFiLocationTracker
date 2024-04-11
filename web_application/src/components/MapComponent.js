import React, { useEffect, useState } from "react";
import { MapContainer, TileLayer, Marker, Popup, Polyline } from "react-leaflet";
import MarkerClusterGroup from "react-leaflet-cluster";
import { Icon, divIcon } from "leaflet";

const MapComponent = ({ center, markers, useCustomClusterIcon }) => {
  const customIcon = new Icon({
    iconUrl: require("../img/markerIcon.png"),
    iconSize: [38, 38]
  });

  const createCustomClusterIcon = function (cluster) {
    return new divIcon({
      html: `<span class="cluster-icon">${cluster.getChildCount()}</span>`,
      className: "custom-marker-cluster"
    });
  };

  const makeLineCoordinates = (markerGroup) => {
    const lineCoordinates = [];
    for (let i = 1; i < markerGroup.length; i++) {
      if (markerGroup[i].id === markerGroup[i - 1].id) {
        lineCoordinates.push(markerGroup[i - 1].geocode, markerGroup[i].geocode);
      }
    }
    return lineCoordinates;
  };

  const [lines, setLines] = useState([]);

  useEffect(() => {
    const calculatedLines = {};
    for (const marker of markers) {
      if (!calculatedLines[marker.id]) {
        calculatedLines[marker.id] = [];
      }
      calculatedLines[marker.id].push(marker);
    }
    setLines(calculatedLines);
  }, [markers]);

  return (
    <MapContainer center={center} zoom={7}>
      <TileLayer
        attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
        url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
      />
      {/* Render lines */}
      {Object.values(lines).map((markerGroup, index) => (
        <Polyline key={index} positions={makeLineCoordinates(markerGroup)} color="blue" />
      ))}
      <MarkerClusterGroup
        chunkedLoading
        iconCreateFunction={useCustomClusterIcon ? createCustomClusterIcon : null}
      >
        {markers.map((marker, index) => (
          <Marker key={index} position={marker.geocode} icon={customIcon}>
            <Popup>{marker.popUp}</Popup>
          </Marker>
        ))}
      </MarkerClusterGroup>
    </MapContainer>
  );
};

export default MapComponent;
//TODO: See if arrows can be implemented: https://imfeld.dev/writing/leaflet_shapes