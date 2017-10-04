'use strict';

module.exports = (sequelize, DataTypes) => {
    let Action = sequelize.define('Action', {
        type: {type: DataTypes.ENUM('CALL', 'TEXT'), notNull: true, defaultValue: 'CALL'},
        phone_number: {type: DataTypes.STRING, notNull: true},
        amount: {type: DataTypes.INTEGER, notNull: true, defaultValue: 1},
        duration: {type: DataTypes.INTEGER},
        content: {type: DataTypes.STRING},
    });

    //Action.belongsTo(Models.Node)

    return Action;
};
