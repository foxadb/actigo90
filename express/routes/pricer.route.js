const express = require('express');
const router = express.Router();

const PricerController = require('../controllers/pricer.controller');

router.post('/', PricerController.actigoDelta);

// Export the Router
module.exports = router;