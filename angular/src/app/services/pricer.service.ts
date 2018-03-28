import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { AuthenticationService } from './authentication.service';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class PricerService {

  private pricerUrl = `${environment.apiUrl}/pricer`;

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

  // Rebalancing
  public rebalance(body: any): Observable<any> {
    return this.http.post(`${this.pricerUrl}/rebalance`, body, this.options())
      .map(res => {
        return res['data'];
      })
      .catch(err => this.handleError(err));
  }

  // Actigo Delta from API by date
  public actigoDelta(body: any): Observable<any> {
    return this.http.post(`${this.pricerUrl}/delta`, body, this.options())
      .map(res => {
        return res['data'];
      })
      .catch(err => this.handleError(err));
  }

  // Portfolio value from API by date
  public hedging(body: any): Observable<any> {
    return this.http.post(`${this.pricerUrl}/hedging`, body, this.options())
      .map(res => {
        return res['data'];
      })
      .catch(err => this.handleError(err));
  }

  // Error handling method
  private handleError(error: any): Promise<any> {
    console.error('An error occured', error);
    return Promise.reject(error.message || error);
  }

}
