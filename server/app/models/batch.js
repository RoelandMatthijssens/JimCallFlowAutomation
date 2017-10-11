"use strict";

module.exports = function(sequelize, DataTypes) {
  var Batch = sequelize.define("Batch", {
    id: {type: DataTypes.STRING, primaryKey: true},
  });

  Batch.associate = function(models) {
    Batch.hasMany(models.Action, {foreignKey: 'BatchId'});
    Batch.hasMany(models.Node, {foreignKey: 'BatchId'});
  }

  return Batch;
};
