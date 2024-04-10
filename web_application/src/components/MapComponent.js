import React from "react";
import { MapContainer, TileLayer, Marker, Popup } from "react-leaflet";
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
  
    return (
      <MapContainer center={center} zoom={7}>
        <TileLayer
          attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
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