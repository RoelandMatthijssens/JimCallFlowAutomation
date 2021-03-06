"use strict";

module.exports = function(sequelize, DataTypes) {
  const Action = sequelize.define("Action", {
    index: {type: DataTypes.INTEGER},
    type: {type: DataTypes.ENUM('CALL', 'TEXT'), notNull: true, defaultValue: 'CALL'},
    phoneNumber: {type: DataTypes.STRING, notNull: true},
    amount: {type: DataTypes.INTEGER, notNull: true, defaultValue: 1},
    duration: {type: DataTypes.INTEGER},
    content: {type: DataTypes.STRING}
  });

  Action.associate = function(models) {
    Action.belongsTo(models.Batch, {
      onDelete: "CASCADE",
      foreignKey: {
        allowNull: false
      }
    });
  }

  return Action;
};
