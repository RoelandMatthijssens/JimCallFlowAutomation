'use strict';
module.exports = function(app) {
  var action = require('../controllers/action');

  // todoList Routes
  app.route('/actions')
    .get(action.list)
    .post(action.create);
};
