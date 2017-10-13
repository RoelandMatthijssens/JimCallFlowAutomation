'use strict';

const Models = require('../models');

exports.list = function(req, res) {
  Models.Node.findAll().then(function(nodes){
    res.json(nodes);
  });
};

exports.create = function(req, res) {
  Models.Node.build(req.body)
    .save()
    .then(function(newNode){
        res.json(newNode);
    });
};

exports.update = function(req, res){
    Models.Node.findById(req.params.nodeId).then(function(node, err) {
        node.updateAttributes(req.body);
        res.json(node);
    });
};

exports.view = function(req, res) {
    Models.Node.findOne({
        attributes: ['id', 'description', 'state'],
        where: {
            id: req.params.nodeId
        }
    }).then(function(node, err) {
        if (err){
            res.send(err);
        }
        res.json(node);
    });
};

exports.finish = function(req, res){
    Models.Node.findById(req.params.nodeId).then(function(node, err) {
        node.updateAttributes({state: 'FINISHED'});
        res.json(node);
    });
};

exports.start = function(req, res){
    Models.Node.findById(req.params.nodeId).then(function(node, err) {
        node.updateAttributes({state: 'STARTING'});
        res.json(node);
    });
};
