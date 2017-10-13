'use strict';
module.exports = function(app) {
  const node = require('../controllers/node');
  const action = require('../controllers/action');

  // todoList Routes
  app.route('/nodes')
    .get(node.list)
    .post(node.create);

  app.route('/nodes/:nodeId')
    .get(node.view)
    .post(node.update)
    .delete(node.destroy);

  app.route('/nodes/:nodeId/action/:actionIndex')
    .get(action.forNode);

  app.route('/nodes/:nodeId/finish')
    .post(node.finish);

  app.route('/nodes/:nodeId/start')
    .post(node.start);
};
