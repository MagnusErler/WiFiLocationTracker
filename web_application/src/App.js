import "./styles.css";
import "leaflet/dist/leaflet.css";

import { Icon, divIcon } from "leaflet";
import { MapContainer, TileLayer, Marker, Popup } from "react-leaflet";
import MarkerClusterGroup from "react-leaflet-cluster";

export default function App() {
  const center = [56.234538, 10.231792]; // Denmark coordinates

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

  const customIcon = new Icon({
    //iconUrl: "https://png.pngtree.com/png-vector/20220502/ourmid/pngtree-3d-location-icon-design-symbol-png-transparent-background-png-image_4562236.png",
    iconUrl: require("./img/markerIcon.png"),
    iconSize: [38, 38]
  })

    // // custom cluster icon
    // const createCustomClusterIcon = function (cluster) {
    //   return new divIcon({
    //     html: `<span class="cluster-icon">${cluster.getChildCount()}</span>`,
    //     className: "custom-marker-cluster",
    //   });
    // };

  return (
    <MapContainer center={center} zoom={7}>
      <TileLayer
        attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
        url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
      />

      <MarkerClusterGroup
        chunkedLoading
        // iconCreateFunction={createCustomClusterIcon}
      >
        {markers.map(marker => (
          <Marker position={marker.geocode} icon={customIcon}>
            <Popup>{marker.popUp}</Popup>
          </Marker>
        ))
        }
      </MarkerClusterGroup>
    </MapContainer>
  );
}
