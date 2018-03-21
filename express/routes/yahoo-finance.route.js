const express = require('express');
const router = express.Router();

const YahooFinanceController = require('../controllers/yahoo-finance.controller');
const UserController = require('../controllers/user.controller');

router.post('/', UserController.userOnly, YahooFinanceController.downloadHistoricalSpots);

// Export the Router
module.exports = router;