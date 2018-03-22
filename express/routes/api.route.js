const express = require('express');
const router = express.Router();

const user = require('./user.route');
const spot = require('./spot.route');
const stock = require('./stock.route');
const delta = require('./delta.route');
const price = require('./price.route');
const pricer = require('./pricer.route');
const yahoofinance = require('./yahoo-finance.route');

router.use('/user', user);
router.use('/spot', spot);
router.use('/stock', stock);
router.use('/delta', delta);
router.use('/price', price);
router.use('/pricer', pricer);
router.use('/yahoo-finance', yahoofinance);

module.exports = router;