var express = require('express');
var bodyParser = require('body-parser');
var app = express();
var port = process.env.PORT || 8000;
var Models = require('./app/models');

app.use(bodyParser());

var routes = require('./app/routes/nodeRoutes');
routes(app); //register the route

Models.sequelize.sync().then(() => {
    app.listen(port);
    console.log('Call Automation RESTful API server started on: ' + port);
})

