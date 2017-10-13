const express = require('express');
const bodyParser = require('body-parser');
const app = express();
const port = process.env.PORT || 8000;
const models = require('./app/models');

app.use(bodyParser());

const nodeRoutes = require('./app/routes/nodes');
const actionRoutes = require('./app/routes/actions');
const batchRoutes = require('./app/routes/batches');
nodeRoutes(app);
actionRoutes(app);
batchRoutes(app);

models.sequelize.sync().then(() => {
    app.listen(port);
    console.log('Call Automation RESTful API server started on: ' + port);
})
