class Delta {

    _id: string;
    stock: string;
    date: Date;
    delta: number;

    constructor(json: any) {
        this._id = json._id;
        this.stock = json.stock;
        this.date = json.date;
        this.delta = json.delta;
    }

}

export default Delta;
