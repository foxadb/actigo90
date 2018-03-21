const express = require('express');
const path = require('path');
const favicon = require('serve-favicon');
const logger = require('morgan');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');
const config = require('config');
const mongoose = require('mongoose');
const jwt = require('jsonwebtoken');
const cors = require('cors');

const app = express();

// MongoDB
const mongodbUrl = 'mongodb://'
    + config.get('mongodb.host') + ':'
    + config.get('mongodb.port') + '/'
    + config.get('mongodb.name');

const dbInit = require('./config/db-init');

mongoose.connect(mongodbUrl).then(
    res => {
        console.log(`Successfully connected to the MongoDB Database at: ${mongodbUrl}`);

        // Initialize Database
        dbInit.adminInit(done => {
            console.log('Default admin credentials:\n', done);
            dbInit.stockInit(done => {
                // App started signal
                app.emit('appStarted');
            });
        });
    },
    err => {
        console.log(`Error Connecting to the MongoDB Database at: ${mongodbUrl}`);
    }
);

// API
const index = require('./routes/index.route');
const api = require('./routes/api.route');

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

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// uncomment after placing your favicon in /public
app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// Authentication with JSON Web Token
app.use(function (req, res, next) {
    if (req.headers && req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
        jwt.verify(req.headers.authorization.split(' ')[1], config.get('jwtsecret'), function (err, decode) {
            if (err) {
                req.user = undefined;
            } else {
                req.user = decode;
            }
            next();
        });
    } else {
        req.user = undefined;
        next();
    }
});

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
