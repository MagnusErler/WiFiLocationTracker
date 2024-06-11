const { Model, DataTypes } = require('sequelize');
const sequelize = require('../database'); // Adjust the path to your database configuration file

class GeolocationSolve extends Model {}

GeolocationSolve.init({
    deviceID: { type: DataTypes.STRING, allowNull: false },
    geocode: { type: DataTypes.JSON, allowNull: false },
    source: { type: DataTypes.STRING, allowNull: false },
    accuracy: { type: DataTypes.FLOAT, allowNull: false }
}, {
    sequelize,
    modelName: 'GeolocationSolve'
});

module.exports = GeolocationSolve;