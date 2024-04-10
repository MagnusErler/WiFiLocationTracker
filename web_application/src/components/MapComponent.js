import React from "react";
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

  // Extracting coordinates from markers
  const coordinates = markers.map(marker => marker.geocode);

  return (
    <MapContainer center={center} zoom={7}>
      <TileLayer
        attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
        url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
      />
      <Polyline positions={coordinates} color="blue" />
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
//TODO: Make lines only between markers with the same ID
//TODO: See if arrows can be implemented: https://imfeld.dev/writing/leaflet_shapes