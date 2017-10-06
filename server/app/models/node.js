'use strict';

module.exports = function(sequelize, DataTypes) {
  var Node = sequelize.define("Node", {
    id: {type: DataTypes.STRING, primaryKey: true},
    description: DataTypes.STRING
  });

  Node.associate = function(models) {
    // Using additional options like CASCADE etc for demonstration
    // Can also simply do Task.belongsTo(models.User);
    Node.hasMany(models.Action, {
      as: 'actions'
    });
  }

  return Node;
};
