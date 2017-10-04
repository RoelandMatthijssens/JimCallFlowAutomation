'use strict';
module.exports = function(app) {
  var node = require('../controllers/node');

  // todoList Routes
  app.route('/nodes')
    .get(node.list)
    .post(node.create);
};
