'use strict';

var Models = require('../models');

exports.list = function(req, res) {
  Models.Node.findAll().then(function(nodes){
    res.json(nodes);
  });
};

exports.create = function(req, res) {
  Models.Node.build(req.body)
    .save()
    .then(function(new_node){
        console.log(new_node);
        res.json(new_node);
    });
};

exports.view = function(req, res) {
    Node.findById(req.params.nodeId, function(err, node) {
        if (err){
            res.send(err);
        }
        res.json(node);
    });
};
