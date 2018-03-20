const express = require('express');
const router = express.Router();

const UserController = require('../controllers/user.controller');

router.get('/', UserController.getUsers);
router.get('/:id', UserController.getUser);
router.post('/register', UserController.registerUser);
router.post('/login', UserController.loginUser);
router.put('/', UserController.updateUser);
router.delete('/:id', UserController.deleteUser);

// Export the Router
module.exports = router;