import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { JwtHelperService } from '@auth0/angular-jwt';
import { Observable } from 'rxjs/Observable';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';
import 'rxjs/add/observable/throw';

import { environment } from '../../environments/environment';

@Injectable()
export class AuthenticationService {

  private loginUrl = `${environment.apiUrl}/user/login`;
  private jwtHelper = new JwtHelperService();

  constructor(private http: HttpClient) { }

  public isAuthenticated(): boolean {
    // Get user token
    const token = this.getToken();

    if (token) {
      // Check whether the token is expired and return
      return !this.jwtHelper.isTokenExpired(token);
    } else {
      return false;
    }
  }

  public getToken(): string {
    const user = localStorage.getItem('currentUser');
    if (user) {
      return JSON.parse(user).token;
    } else {
      return undefined;
    }
  }

  public getTokenPayload(): any {
    const token = this.getToken();
    if (token) {
      return this.jwtHelper.decodeToken(token);
    } else {
      return undefined;
    }
  }

  public getUsername(): string {
    const payload = this.getTokenPayload();
    if (payload) {
      return payload.username;
    } else {
      return undefined;
    }
  }

  public getRole(): string {
    const payload = this.getTokenPayload();
    if (payload) {
      return payload.role;
    } else {
      return undefined;
    }
  }

  public login(username: string, password: string): Observable<boolean> {
    const body = {
      username: username,
      password: password
    };
    return this.http.post(this.loginUrl, body)
      .map(response => {
        // login successful if there's a jwt token in the response
        const token = response['token'];

        if (token) {
          // store username and jwt token in local storage to keep user logged in between page refreshes
          localStorage.setItem('currentUser', JSON.stringify({ username: username, token: token }));

          // return true to indicate successful login
          return true;
        } else {
          // return false to indicate failed login
          return false;
        }
      })
      .catch((error: any) => {
        if (error.status === 401) {
          return Observable.throw('Unauthorized');
        }
      });
  }

  public logout(): void {
    // clear token remove user from local storage to log user out
    localStorage.removeItem('currentUser');
  }
}
