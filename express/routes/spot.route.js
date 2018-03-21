const express = require('express');
const router = express.Router();

const SpotController = require('../controllers/spot.controller');
const UserController = require('../controllers/user.controller');

router.get('/', UserController.userOnly, SpotController.getSpots);
router.get('/:id', UserController.userOnly, SpotController.getSpot);
router.get('/stock/:id', UserController.userOnly, SpotController.getStockSpots);
router.post('/', UserController.userOnly, SpotController.createSpot);
router.put('/:id', UserController.userOnly, SpotController.updateSpot);
router.delete('/:id', UserController.userOnly, SpotController.deleteSpot);
router.delete('/stock/:id', UserController.userOnly, SpotController.deleteStockSpots);

// Export the Router
module.exports = router;