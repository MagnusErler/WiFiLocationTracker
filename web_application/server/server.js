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
  timestamp:  {
    type: DataTypes.INTEGER,
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
    const { deviceID, geocode, timestamp } = req.body;
    console.log(deviceID);
    console.log(geocode);
    console.log(timestamp);
    const geolocationSolve = await GeolocationSolve.create({ deviceID, geocode, timestamp });
    res.json(geolocationSolve);
  } catch (error) {
    console.error('Error querying database:', error.message);
  }
});

// Start server
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});