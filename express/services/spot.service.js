const Spot = require('../models/spot.model');

exports.getSpots = async function (query, page, limit) {
    // Options setup for the mongoose paginate
    var options = { page, limit };

    try {
        let spots = await Spot.paginate(query, options);
        return spots;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.getSpot = async function (id) {
    try {
        let spot = await Spot.findById(id);

        if (spot) {
            return spot;
        } else {
            throw Error;
        }
    } catch (error) {
        throw Error('Spot not found');
    }

};

exports.createSpot = async function (spot) {
    let newSpot = new Spot({
        stock: spot.stock,
        date: spot.date,
        price: spot.price
    });

    try {
        // Saving the Spot 
        let savedSpot = await newSpot.save();

        // Return the result
        return savedSpot;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.updateSpot = async function (spot) {
    try {
        // Find the old Spot Object by the Id
        var oldSpot = await Spot.findById(spot.id);
    } catch (error) {
        throw Error('Spot not found');
    }

    // If no old Spot Object exists return false
    if (!oldSpot) {
        return false;
    }

    // Edit the Spot Object
    oldSpot.date = spot.date != null ? spot.date : oldSpot.date;
    oldSpot.price = spot.price != null ? spot.price : oldSpot.price;

    try {
        // Saving the Spot 
        var savedSpot = await oldSpot.save();
        
        // Return the result
        return savedSpot;
    } catch (e) {
        throw Error('Invalid parameters');
    }
};

exports.deleteSpot = async function (id) {
    try {
        let deleted = await Spot.remove({ _id: id });
        
        // Check if deletion passed correctly
        if (deleted.n === 0) {
            throw Error('Spot could not be deleted');
        }
        
        // Return the result
        return deleted;
    } catch (error) {
        throw Error('Error occured while deleting the spot');
    }
};