'use strict';

module.exports = function(sequelize, DataTypes){
    let Node = sequelize.define('Node', {
        id: {type: DataTypes.STRING, primaryKey: true},
        description: DataTypes.STRING
    });

    //Node.hasMany(Models.Action, {as: 'actions'})

    return Node;
};
