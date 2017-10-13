'use strict';
module.exports = function(app) {
  const batch = require('../controllers/batch');

  // todoList Routes
  app.route('/batches')
    .get(batch.list)
    .post(batch.create);

  app.route('/batches/:batchId')
    .get(batch.view)
    .post(batch.update)
    .delete(batch.destroy);
};
