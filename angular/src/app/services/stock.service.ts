import Stock from '../models/stock.model';

import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class StockService {

  private stockUrl = `${environment.apiUrl}/stock`;

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

  // Get Stocks from API
  public getStocks(): Observable<Array<Stock>> {
    return this.http.get(this.stockUrl)
      .map(res => {
        const stocks: Array<Stock> = [];
        res['data'].docs.forEach(stock => {
          stocks.push(new Stock(stock));
        });
        return stocks;
      })
      .catch(err => this.handleError(err));
  }

  // Get a Stock from API by ID
  public getStock(id: string): Observable<Stock> {
    return this.http.get(`${this.stockUrl}/${id}`)
      .map(res => {
        return new Stock(res['data']);
      })
      .catch(err => this.handleError(err));
  }

  // Create a new Stock to API
  public createStock(stock: any): Observable<boolean> {
    return this.http.post(this.stockUrl, stock, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Update a Stock to API
  public updateStock(stock: any): Observable<boolean> {
    return this.http.put(this.stockUrl, stock, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete a Stock from API
  public deleteStock(id: any): Observable<boolean> {
    return this.http.delete(`${this.stockUrl}/${id}`, this.options())
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
