import Price from '../models/price.model';

import { HttpClient, HttpHeaders, HttpParams, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { AuthenticationService } from './authentication.service';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class PriceService {

  private priceUrl = `${environment.apiUrl}/price`;

  constructor(
    private http: HttpClient,
    private auth: AuthenticationService
  ) { }

  // Requests options
  public options(page?: number, limit?: number): any {
    let headers = new HttpHeaders();
    headers = headers.set('Authorization', 'Bearer ' + this.auth.getToken());
    headers = headers.set('Content-Type', 'application/json');

    const options = {
      headers: headers,
      params: undefined
    };

    if (page && limit) {
      let params = new HttpParams();
      params = params.set('page', String(page));
      params = params.set('limit', String(limit));
      options.params = params;
    }

    return options;
  }

  // Get Prices from API
  public getPrices(page: number, limit: number): Observable<Array<Price>> {
    const options = this.options(page, limit);

    return this.http.get(this.priceUrl, options)
      .map(res => {
        const prices: Array<Price> = [];
        res['data'].docs.forEach(price => {
          prices.push(new Price(price));
        });
        return prices;
      })
      .catch(err => this.handleError(err));
  }

  // Get a Price from API by ID
  public getPrice(id: string): Observable<Price> {
    return this.http.get(`${this.priceUrl}/${id}`, this.options())
      .map(res => {
        return new Price(res['data']);
      })
      .catch(err => this.handleError(err));
  }

  // Create a new Price to API
  public createPrice(price: any): Observable<boolean> {
    return this.http.post(this.priceUrl, price, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Update a Price to API
  public updatePrice(price: any): Observable<boolean> {
    return this.http.put(this.priceUrl, price, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete a Price from API
  public deletePrice(id: any): Observable<boolean> {
    return this.http.delete(`${this.priceUrl}/${id}`, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete all Prices from API
  public deleteAllPrices(): Observable<boolean> {
    return this.http.delete(this.priceUrl, this.options())
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
