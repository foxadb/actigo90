import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class PricerService {

  private pricerUrl = `${environment.apiUrl}/pricer`;

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

  // Get a Actigo Delta from API by date
  public actigoDelta(body: any): Observable<any> {
    return this.http.post(this.pricerUrl, body, this.options())
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
