'use strict';

var Models = require('../models');

exports.list = function(req, res) {
  Models.Action.findAll().then(function(actions){
    res.json(actions);
  });
};

exports.create = function(req, res) {
  Models.Action.build(req.body)
    .save()
    .then(function(new_action){
        console.log(new_action);
        res.json(new_action);
    });
};

exports.for_node = function(req, res) {
    Models.Action.findOne({
        attributes: ['type', 'phone_number', 'amount', 'duration', 'content'],
        where: {
            NodeId: req.params.nodeId,
            index: req.params.actionIndex
        }
    }).then(function(action, err) {
        if (err){
            res.json(err);
        };
        if(action){
            if(action['type'] == 'CALL'){
                action['type'] = 1;
            } else {
                action['type'] = 2;
            }
            res.json(action);
        } else {
            res.json();
        }
    });
};
