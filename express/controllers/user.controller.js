const UserService = require('../services/user.service');
const config = require('config');
const jwt = require('jsonwebtoken');

exports.getUsers = async function (req, res) {
    try {
        let users = await UserService.getUsers();
        return res.status(200).json({ status: 200, data: users, message: 'Successfully users received' });
    } catch (e) {
        return res.status(400).json({ status: 400, message: e.message });
    }
};

exports.getUser = async function (req, res) {
    // Tournament ID
    const id = req.params.id;

    try {
        let user = await UserService.getUser(id);
        return res.status(200).json({ status: 200, data: user, message: 'Successfully user received' });
    } catch (e) {
        return res.status(404).json({ status: 404, message: e.message });
    }
};

exports.registerUser = async function (req, res) {
    const user = {
        username: req.body.username,
        password: req.body.password,
        role: req.body.role
    };

    try {
        let createdUser = await UserService.registerUser(user);
        return res.status(201).json({ status: 201, data: createdUser, message: 'Successfully created user' });
    } catch (e) {
        return res.status(403).json({ status: 403, message: e.message });
    }
};

exports.loginUser = async function (req, res) {
    const user = {
        username: req.body.username,
        password: req.body.password
    };

    try {
        let loggedUser = await UserService.loginUser(user);

        if (!loggedUser) {
            return res.status(401).json({ status: 401, message: 'Authentication failed. Wrong username or password' });
        } else {
            // JSON Web Token
            let token = await jwt.sign(
                { _id: loggedUser._id, username: loggedUser.username, role: loggedUser.role },
                config.get('jwtsecret'), // private key
                { expiresIn: 7200 } // 2 hours
            );
            return res.status(200).json({ status: 200, token: token, message: 'Authentication successfull' });
        }
    } catch (e) {
        throw Error('Error while sign in');
    }
};

exports.updateUser = async function (req, res) {
    // Id is necessary for the update
    if (!req.body._id) {
        return res.status(400).json({ status: 400., message: 'Id must be present' });
    }

    const id = req.body._id;

    const user = {
        id,
        username: req.body.username,
        password: req.body.password,
        role: req.body.role
    };

    try {
        let updatedUser = await UserService.updateUser(user);
        return res.status(200).json({ status: 200, data: updatedUser, message: 'Successfully updated tournament' });
    } catch (e) {
        return res.status(400).json({ status: 400., message: e.message });
    }
};

exports.deleteUser = async function (req, res) {
    // User ID
    const userId = req.params.id;

    try {
        await UserService.deleteUser(userId);
        return res.status(204).json({ status: 204, message: 'Successfully deleted user' });
    } catch (e) {
        return res.status(403).json({ status: 403, message: e.message });
    }
};

exports.userOnly = async function (req, res, next) {
    if (req.user && (req.user.role == 'user' || req.user.role == 'admin')) {
        next();
    } else {
        return res.status(401).json({ status: 401, message: 'Unauthorized user' });
    }
};

exports.adminOnly = async function (req, res, next) {
    if (req.user && (req.user.role == 'admin')) {
        next();
    } else {
        return res.status(401).json({ status: 401, message: 'Unauthorized user' });
    }
};