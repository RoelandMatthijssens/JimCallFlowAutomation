'use strict';
module.exports = function(app) {
  var node = require('../controllers/node');
  var action = require('../controllers/action');

  // todoList Routes
  app.route('/nodes')
    .get(node.list)
    .post(node.create);

  app.route('/nodes/:nodeId')
    .get(node.view);
    //.post(node.update);

  app.route('/nodes/:nodeId/action/:actionIndex')
    .get(action.for_node);
};
