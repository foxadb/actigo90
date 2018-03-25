const Price = require('../models/price.model');

exports.getPrices = async function (query, page, limit) {
    // Options setup for the mongoose paginate
    var options = { page, limit };

    try {
        // Pagination
        let prices = await Price.paginate(query, options);

        // Return the result
        return prices;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.getPrice = async function (id) {
    try {
        let price = await Price.findById(id);

        if (price) {
            return price;
        } else {
            throw Error;
        }
    } catch (error) {
        throw Error('Price not found');
    }

};

exports.createPrice = async function (price) {
    let newPrice = new Price({
        date: price.date,
        actigo: price.actigo,
        hedging: price.hedging
    });

    try {
        // Saving the Price 
        let savedPrice = await newPrice.save();

        // Return the result
        return savedPrice;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.updatePrice = async function (price) {
    try {
        // Find the old Price Object by the Id
        var oldPrice = await Price.findById(price.id);
    } catch (error) {
        throw Error('Price not found');
    }

    // If no old Price Object exists return false
    if (!oldPrice) {
        return false;
    }

    // Edit the Price Object
    oldPrice.date = price.date != null ? price.date : oldPrice.date;
    oldPrice.actigo = price.actigo != null ? price.actigo : oldPrice.actigo;
    oldPrice.hedging = price.hedging != null ? price.hedging : oldPrice.hedging;

    try {
        // Saving the Price 
        var savedPrice = await oldPrice.save();

        // Return the result
        return savedPrice;
    } catch (e) {
        throw Error('Invalid parameters');
    }
};

exports.deletePrice = async function (id) {
    try {
        let deleted = await Price.remove({ _id: id });

        // Check if deletion passed correctly
        if (deleted.n === 0) {
            throw Error('Price could not be deleted');
        }

        // Return the result
        return deleted;
    } catch (error) {
        throw Error('Error occured while deleting the price');
    }
};

exports.deleteAllPrices = async function () {
    try {
        let deleted = await Price.remove({});
        return deleted;
    } catch (error) {
        throw Error('Error occured while deleting all prices');
    }
};