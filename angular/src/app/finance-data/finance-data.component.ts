import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-finance-data',
  templateUrl: './finance-data.component.html',
  styleUrls: ['./finance-data.component.scss']
})
export class FinanceDataComponent implements OnInit {

  public euroStoxx50 = '^STOXX50E';
  public snp500 = '^GSPC';
  public snp200 = '^AXJO';

  constructor() { }

  ngOnInit() {
  }

}
