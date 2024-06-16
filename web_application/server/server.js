const express = require('express');
const bodyParser = require('body-parser');
const geolocationRoutes = require('./routes/geolocationRoutes');
const trackerManagementRoutes = require('./routes/trackerManagementRoutes');
const trackerInformationRoutes = require('./routes/trackerInformationRoutes');
const sequelize = require('./database');
const GeolocationSolve = require('./models/geolocationSolve');
const TrackerInformation = require('./models/trackerInformation');
const KeepAliveGatewayLocation = require('./models/keepAliveGatewayLocation');

const app = express();

app.use(bodyParser.json());
app.use('/api', geolocationRoutes);
app.use('/api', trackerManagementRoutes);
app.use('/api', trackerInformationRoutes);

const PORT = process.env.PORT || 3001;

// Sync the database
sequelize.sync().then(() => {
    app.listen(PORT, () => {
        console.log(`Server is running on port ${PORT}`);
    });
}).catch(error => {
    console.error('Unable to connect to the database:', error);
});