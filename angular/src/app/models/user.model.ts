class User {

    _id: string;
    username: string;
    role: string;

    constructor(json: any) {
        this._id = json._id;
        this.username = json.username;
        this.role = json.role;
    }

    public isAdmin(): boolean {
        return this.role === 'admin';
    }

}

export default User;
