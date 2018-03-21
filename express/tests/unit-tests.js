const config = require('config');
const mongoose = require('mongoose');

const app = require('../app');

// Wait until app is started
before(done => app.on('appStarted', done));

function importTest(name, path) {
    describe(name, function () {
        require(path);
    });
}

describe('API Unit tests', function () {

    importTest('User', './unit-tests/user.route.test');
    importTest('Stock', './unit-tests/stock.route.test');
    importTest('Spot', './unit-tests/spot.route.test');
    importTest('Delta', './unit-tests/delta.route.test');
    importTest('Yahoo Finance', './unit-tests/yahoo-finance.route.test');

    after(function (done) {
        const mongodbUrl = 'mongodb://'
            + config.get('mongodb.host') + ':'
            + config.get('mongodb.port') + '/'
            + config.get('mongodb.name');
        mongoose.connect(mongodbUrl, function () {
            // Drop test database
            mongoose.connection.db.dropDatabase(function () {
                console.log('Test database dropped');
                done();
            });
        });
    });

});