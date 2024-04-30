import React, { useState, useEffect, useRef } from "react";
import "./ModalMenu.css";
import "./MapTileMenu.css";
import Radio from '@mui/material/Radio';
import terrain from "./images/terrain.png";
import terrainDetailed from "./images/terrainDetailed.png";
import satellite from "./images/satellite.png";
import watercolour from "./images/watercolour.png";

const MapTileMenu = React.forwardRef(({ isOpen, handleClose, handleTileChange }, ref) => {
    const menuRef = useRef(null);
    const [selectedTile, setSelectedTile] = useState(null);

    useEffect(() => {
      const handleClickOutside = (event) => {
        if (menuRef.current && !menuRef.current.contains(event.target)) {
          handleClose(); // Close the menu if clicked outside
        }
      };

      document.addEventListener("mousedown", handleClickOutside);
      return () => {
        document.removeEventListener("mousedown", handleClickOutside);
      };
    }, [handleClose]);

    const handleChange = (tile) => {
      setSelectedTile(tile);
      handleTileChange(tile);
    };

    return (
        <div ref={ref} className={`modal ${isOpen ? "show" : ""}`}>
          <div ref={menuRef} className="modal-content map-tile-menu-size">
            {/* Add your map tile options here */}
            <div className="image-options">
              <div className="map-tile-option" onClick={() => handleChange("CartoDB_Voyager")}>
                <img src={terrain} alt="Terrain" />
                <span>Terrain</span>
              </div>
              <div className="map-tile-option" onClick={() => handleChange("OpenStreetMap")}>
                <img src={terrainDetailed} alt="Detailed Terrain" />
                <span>Detailed<br></br>Terrain</span>
              </div>
              <div className="map-tile-option" onClick={() => handleChange("Stadia_AlidadeSatellite")}>
                <img src={satellite} alt="Satellite" />
                <span>Satellite</span>
              </div>
              <div className="map-tile-option" onClick={() => handleChange("Stadia_StamenWatercolor")}>
                <img src={watercolour} alt="Watercolour" />
                <span>Water-<br></br>colour</span>
              </div>
            </div>
          </div>
        </div>
    );
});

export default MapTileMenu;