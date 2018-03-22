import Delta from '../models/delta.model';

import { Component, OnInit } from '@angular/core';

import { PricerService } from '../services/pricer.service';

@Component({
  selector: 'app-pricing',
  templateUrl: './pricing.component.html',
  styleUrls: ['./pricing.component.scss']
})
export class PricingComponent implements OnInit {

  public price: number;
  public pnl: number;

  public pricingDate: Date;
  public pricing: any;
  public pricingSpinner = false;

  public deltas: Array<Delta>;

  constructor(private pricerService: PricerService) { }

  ngOnInit() {
  }

  public rebalance(): void {
    console.log('Rebalancing');
  }

  public actigoDelta(): void {
    const body = {
      date: new Date(this.pricingDate).getTime() / 1000
    };

    // Loading spinner
    this.pricingSpinner = true;

    // Compute Actigo Delta
    this.pricerService.actigoDelta(body).subscribe(
      res => {
        this.pricing = res;
        this.pricingSpinner = false;
      },
      err => console.error('Error', err)
    );
  }

}
