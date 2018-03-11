var mongoose = require('mongoose');
var mongoosePaginate = require('mongoose-paginate');

var SpotSchema = new mongoose.Schema({
    stock: { type: mongoose.Schema.Types.ObjectId, ref: 'Stock', required: true },
    date: { type: Date, required: true },
    price: { type: Number, default: 0 }
}, { timestamps: true });

// Avoid spot duplication
SpotSchema.index({ stock: 1, date: 1}, { unique: true });

SpotSchema.methods.toDto = function () {
    return {
        id: this._id,
        stock: this.stock,
        date: this.date,
        price: this.price
    };
};

SpotSchema.plugin(mongoosePaginate);
const Spot = mongoose.model('Spot', SpotSchema);

module.exports = Spot;