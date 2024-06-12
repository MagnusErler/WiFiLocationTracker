const { Model, DataTypes } = require('sequelize');
const sequelize = require('../database');

class TrackerInformation extends Model {}

TrackerInformation.init({
    deviceID: { type: DataTypes.STRING, allowNull: false, primaryKey: true},
    temperature: { type: DataTypes.INTEGER, allowNull: false },
    updateInterval: { type: DataTypes.INTEGER, allowNull: false },
    batteryStatus: { type: DataTypes.INTEGER, allowNull: false },
    wifiStatus: { type: DataTypes.BOOLEAN, allowNull: false },
    gnssStats: { type: DataTypes.BOOLEAN, allowNull: false },
    loraWANClass: { type: DataTypes.INTEGER, allowNull: false }
}, {
    sequelize,
    modelName: 'TrackerInformation'
});

module.exports = TrackerInformation;