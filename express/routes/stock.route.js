const express = require('express');
const router = express.Router();

const StockController = require('../controllers/stock.controller');
const UserController = require('../controllers/user.controller');

router.get('/', UserController.userOnly, StockController.getStocks);
router.get('/:id', UserController.userOnly, StockController.getStock);
router.post('/', UserController.userOnly, StockController.createStock);
router.put('/:id', UserController.userOnly, StockController.updateStock);
router.delete('/:id', UserController.userOnly, StockController.deleteStock);

// Export the Router
module.exports = router;