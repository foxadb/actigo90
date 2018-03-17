const mongoose = require('mongoose');
const mongoosePaginate = require('mongoose-paginate');

const DeltaSchema = new mongoose.Schema({
    stock: { type: mongoose.Schema.Types.ObjectId, ref: 'Stock', required: true },
    date: { type: Date, required: true },
    delta: { type: Number, default: 0 }
}, { timestamps: true });

// Avoid spot duplication
DeltaSchema.index({ stock: 1, date: 1}, { unique: true });

DeltaSchema.methods.toDto = function () {
    return {
        id: this._id,
        stock: this.stock,
        date: this.date,
        delta: this.delta
    };
};

DeltaSchema.plugin(mongoosePaginate);
const Delta = mongoose.model('Delta', DeltaSchema);

module.exports = Delta;