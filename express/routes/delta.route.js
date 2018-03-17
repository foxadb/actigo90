const express = require('express');
const router = express.Router();

const DeltaController = require('../controllers/delta.controller');

router.get('/', DeltaController.getDeltas);
router.get('/:id', DeltaController.getDelta);
router.get('/stock/:id', DeltaController.getStockDeltas);
router.post('/', DeltaController.createDelta);
router.put('/:id', DeltaController.updateDelta);
router.delete('/:id', DeltaController.deleteDelta);

// Export the Router
module.exports = router;