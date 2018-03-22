const PriceService = require('../services/price.service');

exports.getPrices = async function (req, res) {
    // Check the existence of the query parameters, If the exists doesn't exists assign a default value
    let page = req.query.page ? +req.query.page : 1;
    let limit = req.query.limit ? +req.query.limit : 30;

    try {
        let prices = await PriceService.getPrices({}, page, limit);
        return res.status(200).json({ status: 200, data: prices, message: 'Successfully prices received' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.getPrice = async function (req, res) {
    // Price ID
    let id = req.params.id;

    try {
        let price = await PriceService.getPrice(id);
        return res.status(200).json({ status: 200, data: price, message: 'Successfully price received' });
    } catch (error) {
        return res.status(404).json({ status: 404, message: error.message });
    }
};

exports.createPrice = async function (req, res) {
    let price = {
        date: req.body.date,
        actigo: req.body.actigo,
        hedging: req.body.hedging
    };

    try {
        let createdPrice = await PriceService.createPrice(price);

        // Return success result
        return res.status(201).json({ status: 201, data: createdPrice, message: 'Successfully created price' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.updatePrice = async function (req, res) {
    // Price ID
    let id = req.params.id;

    // Id is necessary for the update
    if (!id) {
        return res.status(400).json({ status: 400, message: 'Id must be present' });
    }

    let price = {
        id,
        date: req.body.date,
        actigo: req.body.actigo,
        hedging: req.body.hedging
    };

    try {
        // Update the price
        let updatedPrice = await PriceService.updatePrice(price);

        // Return success result
        return res.status(200).json({ status: 200, data: updatedPrice, message: 'Successfully updated price' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};

exports.deletePrice = async function (req, res) {
    // Price ID
    let id = req.params.id;

    try {
        // Delete the price
        await PriceService.deletePrice(id);

        // Return the result
        return res.status(204).json({ status: 204, message: 'Successfully price deleted' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};