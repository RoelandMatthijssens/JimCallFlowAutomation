"use strict";

var fs = require("fs");
var path = require("path");
var Sequelize = require("sequelize");
var env = process.env.NODE_ENV || "development";
var settings = require(path.join(__dirname, '..',  '..', 'config', 'settings.js'));
var dbSettings = settings[settings.env].db;
if (process.env.DATABASE_URL) {
  var sequelize = new Sequelize(process.env.DATABASE_URL,dbSettings);
} else {
  var sequelize = new Sequelize(dbSettings.database, dbSettings.username, dbSettings.password, dbSettings);
}
var db = {};

fs
  .readdirSync(__dirname)
  .filter(function(file) {
    return (file.indexOf(".") !== 0) && (file !== "index.js");
  })
  .forEach(function(file) {
    var model = sequelize.import(path.join(__dirname, file));
    db[model.name] = model;
  });

Object.keys(db).forEach(function(modelName) {
  if ("associate" in db[modelName]) {
    db[modelName].associate(db);
  }
});

db.sequelize = sequelize;
db.Sequelize = Sequelize;

module.exports = db;
