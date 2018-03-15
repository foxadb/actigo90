var express = require('express');
var path = require('path');
//var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var mongoose = require('mongoose');
var cors = require('cors');

var index = require('./routes/index.route');
var api = require('./routes/api.route');

var app = express();

// MongoDB
//mongoose.Promise = bluebird;
var mongodbUrl = 'mongodb://localhost:27017/peps';
mongoose.connect(mongodbUrl).then(
    res => console.log(`Successfully connected to the MongoDB Database at: ${mongodbUrl}\n`),
    err => console.log(`Error Connecting to the MongoDB Database at: ${mongodbUrl}\n`)
);

// Enable CORS
var corsWhitelist = [
    undefined, // Remove it to disallow local API request
    'http://localhost:4200',  // Angular Client
    'http://localhost:49152'  // E2E Test Client
];
var corsOptions = {
    origin: function (origin, callback) {
        if (corsWhitelist.indexOf(origin) !== -1) {
            callback(null, true);
        } else {
            callback(new Error('Not allowed by CORS'));
        }
    },
    optionsSuccessStatus: 200
};
app.use(cors(corsOptions));

// Config Init
require('./config/db-init');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// API routes
app.use('/', index);
app.use('/api', api);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

// error handler
app.use(function (err, req, res, next) {
    // set locals, only providing error in development
    res.locals.message = err.message;
    res.locals.error = req.app.get('env') === 'development' ? err : {};

    // render the error page
    res.status(err.status || 500);
    res.render('error');
});

module.exports = app;
