const { Model, DataTypes } = require('sequelize');
const sequelize = require('../database'); // Adjust the path to your database configuration file

class KeepAliveGatewayLocation extends Model {}

KeepAliveGatewayLocation.init({
    deviceID: { type: DataTypes.STRING, allowNull: false },
    gatewayID: { type: DataTypes.STRING, allowNull: false },
    geocode: { type: DataTypes.JSON, allowNull: false },
}, {
    sequelize,
    modelName: 'KeepAliveGatewayLocation'
});

module.exports = KeepAliveGatewayLocation;