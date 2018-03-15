import { Component, OnInit } from '@angular/core';

import Delta from '../models/delta.model';

@Component({
  selector: 'app-pricing',
  templateUrl: './pricing.component.html',
  styleUrls: ['./pricing.component.scss']
})
export class PricingComponent implements OnInit {

  public price: number;
  public pnl: number;
  public deltas: Array<Delta>;

  constructor() { }

  ngOnInit() {
  }

  public rebalance(): void {
    console.log('Rebalancing');
  }

}
