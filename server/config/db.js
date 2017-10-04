const Sequelize = require('sequelize');
const Settings = require('settings');

const dbSettings = Settings[Settings.env].db;
const sequelize = new Sequelize(
    dbSettings.database,
    dbSettings.user,
    dbSettings.password,
    dbSettings
);
