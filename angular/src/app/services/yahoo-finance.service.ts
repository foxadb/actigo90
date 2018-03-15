import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class YahooFinanceService {

  private yahooFinanceUrl = `${environment.apiUrl}/yahoo-finance`;

  constructor(private http: HttpClient) { }

  // Requests options
  public options(): any {
    let headers = new HttpHeaders();
    headers = headers.set('Content-Type', 'application/json');

    const options = {
      headers: headers
    };

    return options;
  }

  // Download Stock data into API
  public downloadStockData(json: any): Observable<boolean> {
    return this.http.post(this.yahooFinanceUrl, json, this.options())
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
