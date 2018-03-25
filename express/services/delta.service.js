const Delta = require('../models/delta.model');

exports.getDeltas = async function (query, page, limit) {
    // Options setup for the mongoose paginate
    var options = { page, limit };

    try {
        // Pagination
        let deltas = await Delta.paginate(query, options);
        
        // Return the result
        return deltas;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.getDelta = async function (id) {
    try {
        let delta = await Delta.findById(id);

        if (delta) {
            return delta;
        } else {
            throw Error;
        }
    } catch (error) {
        throw Error('Delta not found');
    }

};

exports.createDelta = async function (delta) {
    let newDelta = new Delta({
        stock: delta.stock,
        date: delta.date,
        delta: delta.delta
    });

    try {
        // Saving the Delta 
        let savedDelta = await newDelta.save();

        // Return the result
        return savedDelta;
    } catch (error) {
        throw Error('Invalid parameters');
    }
};

exports.updateDelta = async function (delta) {
    try {
        // Find the old Delta Object by the Id
        var oldDelta = await Delta.findById(delta.id);
    } catch (error) {
        throw Error('Delta not found');
    }

    // If no old Delta Object exists return false
    if (!oldDelta) {
        return false;
    }

    // Edit the Delta Object
    oldDelta.date = delta.date != null ? delta.date : oldDelta.date;
    oldDelta.delta = delta.delta != null ? delta.delta : oldDelta.delta;

    try {
        // Saving the Delta 
        var savedDelta = await oldDelta.save();
        
        // Return the result
        return savedDelta;
    } catch (e) {
        throw Error('Invalid parameters');
    }
};

exports.deleteDelta = async function (id) {
    try {
        let deleted = await Delta.remove({ _id: id });
        
        // Check if deletion passed correctly
        if (deleted.n === 0) {
            throw Error('Delta could not be deleted');
        }
        
        // Return the result
        return deleted;
    } catch (error) {
        throw Error('Error occured while deleting the delta');
    }
};

exports.deleteAllDeltas = async function () {
    try {
        let deleted = await Delta.remove({});
        return deleted;
    } catch (error) {
        throw Error('Error occured while deleting all deltas');
    }
};