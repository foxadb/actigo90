const app = require('../app');

// Wait until app is started
before(done => app.on('appStarted', done));

// Unit tests
require('./unit-tests/stock.route.test');
require('./unit-tests/spot.route.test');
require('./unit-tests/delta.route.test');