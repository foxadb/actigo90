const express = require('express');
const router = express.Router();

const PricerController = require('../controllers/pricer.controller');

// Map GET controller functions
router.get('/', PricerController.getPricing);

// Export the Router
module.exports = router;