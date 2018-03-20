const mongoose = require('mongoose');
const mongoosePaginate = require('mongoose-paginate');
const bcrypt = require('bcrypt');

const UserSchema = new mongoose.Schema({
    username: { type: String, required: true, unique: true },
    password: { type: String, required: true },
    role: { type: String, enum: ['user', 'admin'], required: true }
}, { timestamps: true });

UserSchema.methods.hashPassword = async function () {
    try {
        // Generating salt
        let salt = await bcrypt.genSalt(10);
        
        // Hashing the user password
        this.password = await bcrypt.hash(this.password, salt);
    } catch (e) {
        throw Error('Error while creating user');
    }
};

UserSchema.methods.comparePassword = async function (password) {
    try {
        var res = await bcrypt.compare(password, this.password);
        return res;
    } catch (e) {
        throw Error('Wrong password');
    }
};

UserSchema.plugin(mongoosePaginate);
const User = mongoose.model('User', UserSchema);

module.exports = User;