import Delta from '../models/delta.model';

import { HttpClient, HttpHeaders, HttpParams, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { Injectable } from '@angular/core';

import { AuthenticationService } from './authentication.service';

import { environment } from '../../environments/environment';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';

@Injectable()
export class DeltaService {

  private deltaUrl = `${environment.apiUrl}/delta`;

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

  // Get Deltas from API
  public getDeltas(page: number, limit: number): Observable<Array<Delta>> {
    const options = this.options(page, limit);

    return this.http.get(this.deltaUrl, options)
      .map(res => {
        const deltas: Array<Delta> = [];
        res['data'].docs.forEach(delta => {
          deltas.push(new Delta(delta));
        });
        return deltas;
      })
      .catch(err => this.handleError(err));
  }

  // Get a Delta from API by ID
  public getDelta(id: string): Observable<Delta> {
    return this.http.get(`${this.deltaUrl}/${id}`, this.options())
      .map(res => {
        return new Delta(res['data']);
      })
      .catch(err => this.handleError(err));
  }

  // Create a new Delta to API
  public createDelta(delta: any): Observable<boolean> {
    return this.http.post(this.deltaUrl, delta, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Update a Delta to API
  public updateDelta(delta: any): Observable<boolean> {
    return this.http.put(`${this.deltaUrl}/${delta._id}`, delta, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete a Delta from API
  public deleteDelta(id: any): Observable<boolean> {
    return this.http.delete(`${this.deltaUrl}/${id}`, this.options())
      .map(res => {
        return true;
      })
      .catch(err => this.handleError(err));
  }

  // Delete all Deltas from API
  public deleteAllDeltas(): Observable<boolean> {
    return this.http.delete(this.deltaUrl, this.options())
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
