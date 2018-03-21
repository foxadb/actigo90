import { Injectable } from '@angular/core';
import { Http, Headers, Response } from '@angular/http';
import { JwtHelperService } from '@auth0/angular-jwt';
import { Observable } from 'rxjs/Observable';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';
import 'rxjs/add/observable/throw';

import { environment } from '../../environments/environment';

@Injectable()
export class AuthenticationService {

  private loginUrl = `${environment.apiUrl}/user/login`;

  constructor(
    private jwtHelper: JwtHelperService,
    private http: Http
  ) { }

  public isAuthenticated(): boolean {
    // Get the token
    const token = JSON.parse(localStorage.getItem('currentUser')).token;

    // Check whether the token is expired and return
    return !this.jwtHelper.isTokenExpired(token);
  }

  public getToken(): string {
    return JSON.parse(localStorage.getItem('currentUser')).token;
  }

  public getTokenPayload(): any {
    return this.jwtHelper.decodeToken(this.getToken());
  }

  public getUsername(): string {
    return this.getTokenPayload().username;
  }

  public getRole(): string {
    return this.getTokenPayload().role;
  }

  public login(username: string, password: string): Observable<boolean> {
    const body = { username: username, password: password };
    return this.http.post(this.loginUrl, body)
      .map((response: Response) => {
        // login successful if there's a jwt token in the response
        const token = response.json() && response.json().token;

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
