const express = require('express');
const router = express.Router();

const PriceController = require('../controllers/price.controller');
const UserController = require('../controllers/user.controller');

router.get('/', UserController.userOnly, PriceController.getPrices);
router.get('/:id', UserController.userOnly, PriceController.getPrice);
router.post('/', UserController.userOnly, PriceController.createPrice);
router.put('/:id', UserController.userOnly, PriceController.updatePrice);
router.delete('/:id', UserController.userOnly, PriceController.deletePrice);

// Export the Router
module.exports = router;