import User from '../models/user.model';

import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { AuthenticationService } from './authentication.service';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class UserService {

  private userUrl = `${environment.apiUrl}/user`;

  constructor(
    private http: HttpClient,
    private auth: AuthenticationService
  ) { }


  // Requests options
  public options(): any {
    let headers = new HttpHeaders();
    headers = headers.set('Authorization', 'Bearer ' + this.auth.getToken());
    headers = headers.set('Content-Type', 'application/json');

    const options = {
      headers: headers
    };

    return options;
  }

  // Get Users from API
  public getUsers(): Observable<Array<User>> {
    return this.http.get(this.userUrl, this.options())
      .map(res => {
        const users: Array<User> = [];
        res['data'].forEach(user => {
          users.push(new User(user));
        });
        return users;
      })
      .catch(err => this.handleError(err));
  }

  // Get a User from API by ID
  public getUser(id: string): Observable<User> {
    return this.http.get(`${this.userUrl}/${id}`, this.options())
      .map(res => {
        return new User(res['data']);
      })
      .catch(err => this.handleError(err));
  }

  // Create a new User to API
  public createUser(user: any): Observable<boolean> {
    return this.http.post(`${this.userUrl}/register`, user, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Update a User to API
  public updateUser(user: any): Observable<boolean> {
    return this.http.put(`${this.userUrl}/${user._id}`, user, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete a User from API
  public deleteUser(id: any): Observable<boolean> {
    return this.http.delete(`${this.userUrl}/${id}`, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Error handling method
  private handleError(error: any): Promise<any> {
    console.error('An error occured', error);
    return Promise.reject(error.message || error);
  }

}
