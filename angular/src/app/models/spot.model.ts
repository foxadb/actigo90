class Spot {

    _id: string;
    stock: string;
    date: Date;
    price: number;

    constructor(json: any) {
        this._id = json._id;
        this.stock = json.stock;
        this.date = json.date;
        this.price = json.price;
    }

}

export default Spot;
