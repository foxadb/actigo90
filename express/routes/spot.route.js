const express = require('express');
const router = express.Router();

const SpotController = require('../controllers/spot.controller');

router.get('/', SpotController.getSpots);
router.get('/:id', SpotController.getSpot);
router.get('/stock/:id', SpotController.getStockSpots);
router.post('/', SpotController.createSpot);
router.put('/:id', SpotController.updateSpot);
router.delete('/:id', SpotController.deleteSpot);
router.delete('/stock/:id', SpotController.deleteStockSpots);

// Export the Router
module.exports = router;