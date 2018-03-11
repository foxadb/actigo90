const express = require('express');
const router = express.Router();

const StockController = require('../controllers/stock.controller');

router.get('/', StockController.getStocks);
router.get('/:id', StockController.getStock);
router.post('/', StockController.createStock);
router.put('/:id', StockController.updateStock);
router.delete('/:id', StockController.deleteStock);

// Export the Router
module.exports = router;