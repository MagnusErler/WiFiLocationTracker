const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const { Sequelize, Model, DataTypes } = require('sequelize');

const app = express();
const PORT = process.env.PORT || 3001;

app.use(cors());

// Create Sequelize instance
const sequelize = new Sequelize({
  dialect: 'sqlite',
  storage: './geolocationdata.sqlite'
});

// Define GeolocationSolve model
class GeolocationSolve extends Model {}
GeolocationSolve.init({
  deviceID: {
    type: DataTypes.STRING,
    allowNull: false,
  },
  geocode: {
    type: DataTypes.JSON,
    allowNull: false,
  },
  source: {
    type: DataTypes.STRING,
    allowNull: false,
  },
}, { sequelize, modelName: 'geolocationSolve' });

// Sync model with database
sequelize.sync();

// Middleware for parsing request body
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('/GeolocationSolves', async (req, res) => {
  try {
    const geolocationSolves = await GeolocationSolve.findAll();
    res.json(geolocationSolves);
  } catch (error) {
    console.error('Error querying database:', error.message);
  }
});

app.get('/GeolocationSolves/:deviceID', async (req, res) => {
  try {
    const geolocationSolvesWithDeviceID = await GeolocationSolve.findAll({where: {deviceID: req.params.deviceID}});
    res.json(geolocationSolvesWithDeviceID);
  } catch (error) {
    console.error('Error querying database:', error.message);
  }
});

app.post('/GeolocationSolves', async (req, res) => {
  try {
    console.log(req.body);
    const { end_device_ids, location_solved } = req.body;
    
    // Extract device ID and geolocation data
    const deviceID = end_device_ids.dev_eui;
    const { location } = location_solved;
    const { latitude, longitude, accuracy, source } = location;
    
    // Format geocode
    const geocode = [latitude, longitude];
      
    // Save geolocation solve to the database
    const geolocationSolve = await GeolocationSolve.create({ deviceID, geocode, accuracy, source});
    
    res.json(geolocationSolve);
    console.log('Geolocation solve added to database:', geolocationSolve.toJSON());
  } catch (error) {
    console.error('Error querying database:', error.message);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Start server
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});

//TODO: Implement websocket server for real-time updates