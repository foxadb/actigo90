const express = require('express');
const router = express.Router();

const DeltaController = require('../controllers/delta.controller');
const UserController = require('../controllers/user.controller');

router.get('/', UserController.userOnly, DeltaController.getDeltas);
router.get('/:id', UserController.userOnly, DeltaController.getDelta);
router.get('/stock/:id', UserController.userOnly, DeltaController.getStockDeltas);
router.post('/', UserController.userOnly, DeltaController.createDelta);
router.put('/:id', UserController.userOnly, DeltaController.updateDelta);
router.delete('/:id', UserController.userOnly, DeltaController.deleteDelta);

// Export the Router
module.exports = router;