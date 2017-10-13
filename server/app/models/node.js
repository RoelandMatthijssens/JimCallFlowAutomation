'use strict';

module.exports = function(sequelize, DataTypes) {
  const Node = sequelize.define("Node", {
    id: {type: DataTypes.STRING, primaryKey: true},
    description: DataTypes.STRING,
    state: {type: DataTypes.ENUM('STARTING', 'RUNNING', 'FINISHED'), notNull: true, defaultValue: 'FINISHED'}
  });

  Node.associate = function(models) {
    Node.belongsTo(models.Batch, {
      foreignKey: {
        allowNull: true
      }
    });
  }

  return Node;
};
