const express = require('express');
const router = express.Router();

const spot = require('./spot.route');
const stock = require('./stock.route');
const delta = require('./delta.route');
const pricer = require('./pricer.route');
const yahoofinance = require('./yahoo-finance.route');

router.use('/spot', spot);
router.use('/stock', stock);
router.use('/delta', delta);
router.use('/pricer', pricer);
router.use('/yahoo-finance', yahoofinance);

module.exports = router;