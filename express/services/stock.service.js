const Stock = require('../models/stock.model');

exports.getStocks = async function (query, page, limit) {
    // Options setup for the mongoose paginate
    var options = { page, limit };

    try {
        // Pagination
        let stocks = await Stock.paginate(query, options);
        
        // Return the result
        return stocks;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.getStock = async function (id) {
    try {
        let stock = await Stock.findById(id);

        if (stock) {
            return stock;
        } else {
            throw Error;
        }
    } catch (error) {
        throw Error('Stock not found');
    }

};

exports.createStock = async function (stock) {
    let newStock = new Stock({
        symbol: stock.symbol,
        name: stock.name,
        currency: stock.currency
    });

    try {
        // Saving the Stock 
        let savedStock = await newStock.save();
        
        // Return the result
        return savedStock;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.updateStock = async function (stock) {
    try {
        // Find the old Stock Object by the Id
        var oldStock = await Stock.findById(stock.id);
    } catch (error) {
        throw Error('Stock not found');
    }

    // If no old Stock Object exists return false
    if (!oldStock) {
        return false;
    }

    // Edit the Stock Object
    oldStock.symbol = stock.symbol != null ? stock.symbol : oldStock.symbol;
    oldStock.name = stock.name != null ? stock.name : oldStock.name;
    oldStock.currency = stock.currency != null ? stock.currency : oldStock.currency;

    try {
        // Saving the Stock
        var savedStock = await oldStock.save();
        
        // Return the result
        return savedStock;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.deleteStock = async function (id) {
    try {
        let deleted = await Stock.remove({ _id: id });
        
        // Check if deletion passed correctly
        if (deleted.n === 0) {
            throw Error('Stock could not be deleted');
        }
        
        // Return the result
        return deleted;
    } catch (error) {
        throw Error('Error occured while deleting the stock');
    }
};

exports.addSpot = async function (stockId, spotId) {
    try {
        // Find the stock
        let stock = await Stock.findById(stockId);

        // Push the new spot to the list
        stock.spots.push(spotId);

        // Save to the database
        let savedStock = await stock.save();
        return savedStock;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.removeSpot = async function (stockId, spotId) {
    try {
        // Find the stock
        let stock = await Stock.findById(stockId);

        // Remove the spot from the list
        let index = stock.spots.indexOf(spotId);
        if (index > -1) {
            stock.spots.splice(index, 1);
        }

        // Save to the database
        let savedStock = await stock.save();

        // Return the stock
        return savedStock;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.removeAllSpots = async function (stockId) {
    try {
        // Find the stock
        let stock = await Stock.findById(stockId);

        // Remove the spot list
        stock.spots = [];

        // Save to the database
        let savedStock = await stock.save();

        // Return the stock
        return savedStock;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};