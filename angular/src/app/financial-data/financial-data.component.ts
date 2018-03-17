import { Component, OnInit } from '@angular/core';

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

  constructor() { }

  ngOnInit() {
  }

}
