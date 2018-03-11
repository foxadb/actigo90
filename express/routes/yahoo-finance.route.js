const express = require('express');
const router = express.Router();

const YahooFinanceController = require('../controllers/yahoo-finance.controller');

router.post('/', YahooFinanceController.downloadHistoricalSpots);

// Export the Router
module.exports = router;