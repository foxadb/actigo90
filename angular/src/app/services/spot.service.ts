import Spot from '../models/spot.model';

import { HttpClient, HttpHeaders, HttpParams, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';


@Injectable()
export class SpotService {

  private spotUrl = `${environment.apiUrl}/spot`;

  constructor(private http: HttpClient) { }

  // Requests options
  public options(page?: number, limit?: number): any {
    let headers = new HttpHeaders();
    headers = headers.set('Content-Type', 'application/json');

    let params = new HttpParams();
    params = params.set('page', String(page));
    params = params.set('limit', String(limit));

    const options = {
      headers: headers,
      params: params
    };

    return options;
  }

  // Get Spots from API
  public getSpots(page: number, limit: number): Observable<Array<Spot>> {
    const options = this.options(page, limit);

    return this.http.get(this.spotUrl, options)
      .map(res => {
        const spots: Array<Spot> = [];
        res['data'].docs.forEach(spot => {
          spots.push(new Spot(spot));
        });
        return spots;
      })
      .catch(err => this.handleError(err));
  }

  // Get a Spot from API by ID
  public getSpot(id: string): Observable<Spot> {
    return this.http.get(`${this.spotUrl}/${id}`)
      .map(res => {
        return new Spot(res['data']);
      })
      .catch(err => this.handleError(err));
  }

  // Get Spots of a Stock from API by ID
  public getStockSpots(id: string, page: number, limit: number): Observable<Array<Spot>> {
    const options = this.options(page, limit);

    return this.http.get(`${this.spotUrl}/stock/${id}`, options)
      .map(res => {
        const spots: Array<Spot> = [];
        res['data'].docs.forEach(spot => {
          spots.push(new Spot(spot));
        });
        return spots;
      })
      .catch(err => this.handleError(err));
  }

  // Create a new Spot to API
  public createSpot(spot: any): Observable<boolean> {
    return this.http.post(this.spotUrl, spot, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Update a Spot to API
  public updateSpot(spot: any): Observable<boolean> {
    return this.http.put(this.spotUrl, spot, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete a Spot from API
  public deleteSpot(id: any): Observable<boolean> {
    return this.http.delete(`${this.spotUrl}/${id}`, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete all Spots of a Stock from API
  public deleteStockSpots(id: any): Observable<boolean> {
    return this.http.delete(`${this.spotUrl}/stock/${id}`, this.options())
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
