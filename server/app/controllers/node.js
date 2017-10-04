'use strict';

const Models = require('../models');

exports.list = function(req, res) {
  Models.Node.findAll().then(nodes => {
    res.json(nodes);
  });
};

exports.create = function(req, res) {
  Models.Node.findAll().then(function(nodes){
    res.json(nodes);
  });
};

exports.create = function(req, res) {
  console.log(req);
  Models.Node.build(req.body)
    .save()
    .then(function(new_node){
        console.log(new_node);
        res.json(new_node);
    });
};
