import { Injectable } from '@angular/core';
import { Subject } from 'rxjs/Subject';

@Injectable()
export class FinancialDataService {

  private download = new Subject<any>();
  public downloadObservable = this.download.asObservable();

  private delete = new Subject<boolean>();
  public deleteObservable = this.delete.asObservable();

  constructor() { }

  public downloadStocksDataNotify(data: any): void {
    this.download.next(data);
  }

  public deleteStocksDataNotify(data: boolean): void {
    this.delete.next(data);
  }

}
