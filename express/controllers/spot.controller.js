const SpotService = require('../services/spot.service');
const StockService = require('../services/stock.service');

exports.getSpots = async function (req, res) {
    // Check the existence of the query parameters, If the exists doesn't exists assign a default value
    let page = req.query.page ? +req.query.page : 1;
    let limit = req.query.limit ? +req.query.limit : 30;

    try {
        let spots = await SpotService.getSpots({}, page, limit);
        return res.status(200).json({ status: 200, data: spots, message: 'Successfully spots received' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.getSpot = async function (req, res) {
    // Spot ID
    let id = req.params.id;

    try {
        let spot = await SpotService.getSpot(id);
        return res.status(200).json({ status: 200, data: spot, message: 'Successfully spot received' });
    } catch (error) {
        return res.status(404).json({ status: 404, message: error.message });
    }
};

exports.getStockSpots = async function (req, res) {
    // Stock ID
    let id = req.params.id;

    // Check the existence of the query parameters, If the exists doesn't exists assign a default value
    let page = req.query.page ? +req.query.page : 1;
    let limit = req.query.limit ? +req.query.limit : 30;

    try {
        let spots = await SpotService.getSpots({ stock: id }, page, limit);
        return res.status(200).json({ status: 200, data: spots, message: 'Successfully spots received' });
    } catch (error) {
        return res.status(404).json({ status: 404, message: error.message });
    }
};

exports.createSpot = async function (req, res) {
    let spot = {
        stock: req.body.stock,
        date: req.body.date,
        price: req.body.price
    };

    try {
        let createdSpot = await SpotService.createSpot(spot);

        // Add the new spot to its stock
        await StockService.addSpot(createdSpot.stock, createdSpot._id);

        // Return success result
        return res.status(201).json({ status: 201, data: createdSpot, message: 'Successfully created spot' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.updateSpot = async function (req, res) {
    // Spot ID
    let id = req.params.id;

    // Id is necessary for the update
    if (!id) {
        return res.status(400).json({ status: 400, message: 'Id must be present' });
    }

    let spot = {
        id,
        date: req.body.date,
        price: req.body.price
    };

    try {
        // Update the spot
        let updatedSpot = await SpotService.updateSpot(spot);

        // Return success result
        return res.status(200).json({ status: 200, data: updatedSpot, message: 'Successfully updated spot' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};

exports.deleteSpot = async function (req, res) {
    // Spot ID
    let spotId = req.params.id;

    try {
        let spot = await SpotService.getSpot(spotId);

        if (spot) {
            // Remove the spot from its stock
            StockService.removeSpot(spot.stock, spotId);
        } else {
            throw Error('Spot does not exist');
        }

        // Delete the spot
        await SpotService.deleteSpot(spotId);

        // Return the result
        return res.status(204).json({ status: 204, message: 'Successfully spot deleted' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};

exports.deleteStockSpots = async function (req, res) {
    // Stock ID
    let stockId = req.params.id;

    try {
        let stock = await StockService.getStock(stockId);
        let spots = stock.spots;

        // Delete all stock spots
        for (let i = 0; i < spots.length; ++i) {
            SpotService.deleteSpot(spots[i]);
        }

        // Delete the spot reference in the stock
        StockService.removeAllSpots(stockId);

        // Return the result
        return res.status(204).json({ status: 204, message: 'Successfully spot deleted' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};