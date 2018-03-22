import { Component, OnInit } from '@angular/core';

import { FinancialDataService } from '../services/financial-data.service';

@Component({
  selector: 'app-financial-data',
  templateUrl: './financial-data.component.html',
  styleUrls: ['./financial-data.component.scss']
})
export class FinancialDataComponent implements OnInit {

  public euroStoxx50 = '^STOXX50E';
  public snp500 = '^GSPC';
  public snp200 = '^AXJO';
  public eurUsd = 'EURUSD=X';
  public eurAud = 'EURAUD=X';

  public startDate: Date;
  public endDate = Date.now();

  constructor(private financialDataService: FinancialDataService) { }

  ngOnInit() {
  }

  public downloadStocksData(): void {
    this.financialDataService.downloadStocksDataNotify({
      startDate: this.startDate,
      endDate: this.endDate
    });
  }

  public deleteStocksData(): void {
    this.financialDataService.deleteStocksDataNotify(true);
  }

}
