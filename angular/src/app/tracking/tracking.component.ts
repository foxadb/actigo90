import { Component, OnInit, ViewChild } from '@angular/core';
import { BaseChartDirective } from 'ng2-charts/ng2-charts';

import Price from '../models/price.model';

import { PriceService } from '../services/price.service';

@Component({
  selector: 'app-tracking',
  templateUrl: './tracking.component.html',
  styleUrls: ['./tracking.component.scss']
})
export class TrackingComponent implements OnInit {

  public trackingDate: Date;
  public trackingError: number;

  public prices: Array<Price>;

  @ViewChild('baseChart') chart: BaseChartDirective;

  public actigoPrices: Array<Number> = [];
  public hedgingPrices: Array<Number> = [];
  public chartData: Array<any> = [
    { data: this.actigoPrices, label: 'Actigo 90' },
    { data: this.hedgingPrices, label: 'Hedging Portfolio' }
  ];
  public chartLabels: Array<any> = [];

  public chartOptions: any = {
    responsive: true,
    elements: {
      point: {
        radius: 0
      }
    }
  };
  public chartLegend = true;
  public chartType = 'line';

  constructor(private priceService: PriceService) { }

  public ngOnInit(): void {
    // Price subscription
    this.priceService.getPrices(1, 5000).subscribe(
      prices => {
        this.prices = prices;
      },
      error => console.error('Error: ', error),
      () => {
        // Sort spot by date
        this.prices.sort((a, b) =>
          new Date(a.date).getTime() - new Date(b.date).getTime());

        // Fill chart data
        this.prices.forEach(price => {
          this.chartLabels.push(new Date(price.date).toLocaleDateString());
          this.actigoPrices.push(price.actigo);
          this.hedgingPrices.push(price.hedging);
        });

        // Refresh chart
        this.chart.ngOnInit();
      }
    );
  }

  public computeTrackingError(): void {
    // Find the current price
    const currentPrice = this.prices.find(price => {
      const date = new Date(price.date).setHours(0, 0, 0);
      const trackingDate = new Date(this.trackingDate).setHours(0, 0, 0);
      return date === trackingDate;
    });

    // Compute the tracking error
    if (currentPrice) {
      this.trackingError = Math.abs(currentPrice.hedging - currentPrice.actigo);

      // Convert into percent
      this.trackingError *= 100;
      this.trackingError = Math.round(this.trackingError * 100) / 100;
    }
  }

  public chartClicked(event: any): void {
    console.log(event);
  }

  public chartHovered(event: any): void {
    console.log(event);
  }

}
