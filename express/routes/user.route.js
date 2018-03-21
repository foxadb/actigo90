const express = require('express');
const router = express.Router();

const UserController = require('../controllers/user.controller');

router.get('/', UserController.adminOnly, UserController.getUsers);
router.get('/:id', UserController.adminOnly, UserController.getUser);
router.post('/register', UserController.adminOnly, UserController.registerUser);
router.post('/login', UserController.loginUser);
router.put('/:id', UserController.adminOnly, UserController.updateUser);
router.delete('/:id', UserController.adminOnly, UserController.deleteUser);

// Export the Router
module.exports = router;