const StockService = require('../services/stock.service');
const SpotService = require('../services/spot.service');

exports.getStocks = async function (req, res) {
    // Check the existence of the query parameters, If the exists doesn't exists assign a default value
    var page = req.query.page ? +req.query.page : 1;
    var limit = req.query.limit ? +req.query.limit : 10;

    if (limit > 50) {
        return res.status(403).json({ status: 403, message: 'Limit can not be higher than 50' });
    }

    try {
        var stocks = await StockService.getStocks({}, page, limit);
        return res.status(200).json({ status: 200, data: stocks, message: 'Successfully stocks received' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.getStock = async function (req, res) {
    // Stock ID
    var id = req.params.id;

    try {
        let stock = await StockService.getStock(id);
        return res.status(200).json({ status: 200, data: stock, message: 'Successfully stock received' });
    } catch (error) {
        return res.status(404).json({ status: 404, message: error.message });
    }
};

exports.createStock = async function (req, res) {
    var stock = {
        symbol: req.body.symbol,
        name: req.body.name,
        currency: req.body.currency
    };

    try {
        var createdStock = await StockService.createStock(stock);
        return res.status(201).json({ status: 201, data: createdStock, message: 'Successfully created stock' });
    } catch (error) {
        return res.status(400).json({ status: 400, message: error.message });
    }
};

exports.updateStock = async function (req, res) {
    // Stock ID
    let id = req.params.id;

    // Id is necessary for the update
    if (!id) {
        return res.status(400).json({ status: 400, message: 'Id must be present' });
    }

    let stock = {
        id,
        symbol: req.body.symbol,
        name: req.body.name,
        currency: req.body.currency
    };

    try {
        let updatedStock = await StockService.updateStock(stock);
        return res.status(200).json({ status: 200, data: updatedStock, message: 'Successfully updated stock' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};

exports.deleteStock = async function (req, res) {
    // Stock ID
    let stockId = req.params.id;

    try {
        let stock = await StockService.getStock(stockId);
        let spots = stock.spots;

        // Delete all stock spots
        for (let i = 0; i < spots.length; ++i) {
            SpotService.deleteSpot(spots[i]);
        }

        // Delete the stock
        await StockService.deleteStock(stockId);

        // Return result
        return res.status(204).json({ status: 204, message: 'Successfully stock deleted' });
    } catch (error) {
        return res.status(403).json({ status: 403, message: error.message });
    }
};